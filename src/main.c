#include "engine/graphic.h"
#include "engine/scene.h"
#include "engine/sfx.h"
#include "engine/perspective.h"
#include "engine/update.h"

#include "game/static.h"
#include "game/subtitles.h"
#include "game/blip.h"
#include "game/which_night.h"
#include "game/game.h"
#include "game/title.h"
#include "game/night_end.h"
#include "game/game_over.h"
#include "game/powerdown.h"
#include "game/custom_night.h"
#include "game/paycheck.h"
#include "game/save_data.h"

enum {
        RET_GOOD,
        RET_EEPROM_FAILED
};

static void (*draw_funcs[SCENE_COUNT])(void) = {
        title_draw,
        which_night_draw,
        game_draw,
        night_end_draw,
        paycheck_draw,
        game_over_draw,
        power_down_draw,
        custom_night_draw
};

static enum scene (*update_funcs[SCENE_COUNT])(struct update_params) = {
        title_update,
        which_night_update,
        game_update,
        night_end_update,
        paycheck_update,
        game_over_update,
        power_down_update,
        custom_night_update
};

int main(void)
{
        int dfs_handle;
        enum scene scene;

        /* N64 Init */
        srand(TICKS_READ());
        display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3,
                     GAMMA_NONE, FILTERS_RESAMPLE);
        rdpq_init();
        dfs_handle = dfs_init(DFS_DEFAULT_LOCATION);
        audio_init(32000, 4);
        mixer_init(SFX_CH_CNT);
        timer_init();
        joypad_init();

#ifdef DEBUG_ENABLED
        debug_init_isviewer();
        debug_init_usblog();
        rdpq_debug_start();
#endif

        /* Game Init */
        save_data_init();
        subtitles_load();
        perspective_init();
        sfx_load();
        static_load();
        blip_create();

        scene = SCENE_TITLE_SCREEN;

        while (1) {
                struct update_params uparams;
                static enum scene scene_last;
                short *audio_buf;

                /* Rendering */
                rdpq_attach(display_get(), NULL);
                draw_funcs[scene]();
                rdpq_detach_show();

                /* Updating */
                /*
                 * TODO: Change the update section of code to only
                 * happen at a designated tickrate of 60.
                 */
                joypad_poll();
                uparams.dt = display_get_delta_time();
                uparams.held = joypad_get_buttons_held(JOYPAD_PORT_1),
                        uparams.pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1),
                        uparams.sticks = joypad_get_inputs(JOYPAD_PORT_1),

                        scene_last = scene;
                scene = update_funcs[scene](uparams);
                blip_update(uparams.dt);
                static_update(uparams.dt);

                if (scene_last ^ scene) {
                        rspq_wait();
                }

                /* Audio */
                if (!audio_can_write()) {
                        continue;
                }

                audio_buf = audio_write_begin();
                mixer_poll(audio_buf, audio_get_buffer_length());
                audio_write_end();
        }

        /* N64 Terminate */
        joypad_close();
        timer_close();
        mixer_close();
        audio_close();
        dfs_close(dfs_handle);
#ifdef DEBUG_ENABLED
        rdpq_debug_stop();
#endif
        rdpq_close();
        display_close();

        /* Game Terminate */
        blip_destroy();
        static_unload();
        sfx_unload();
        perspective_free();
        subtitles_unload();
        save_data_free();

        return RET_GOOD;
}
