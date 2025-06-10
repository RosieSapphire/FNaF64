#include "engine/object.h"
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

int main(void)
{
        /* N64 Init */
        int dfs_handle;

	srand(TICKS_READ());
	display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3,
		     GAMMA_NONE, ANTIALIAS_RESAMPLE);
	rdpq_init();
	dfs_handle = dfs_init(DFS_DEFAULT_LOCATION);
	audio_init(32000, 4);
	mixer_init(SFX_CH_CNT);
	timer_init();
	joypad_init();

        debug_init_isviewer();
        debug_init_usblog();
        rdpq_debug_start();

        /* Game Init */
        save_data_init();
	subtitles_load();
	perspective_init();
	sfx_load();
	static_load();
	blip_create();

	enum scene scene = SCENE_TITLE_SCREEN;
	long ticks_last = get_ticks();

	while(1) {
		long ticks_now = get_ticks();
		long tick_delta = TICKS_DISTANCE(ticks_last, ticks_now);
		ticks_last = ticks_now;

		joypad_poll();
		const update_parms_t uparms = {
			.dt = (float)tick_delta / (float)TICKS_PER_SECOND,
			.held = joypad_get_buttons_held(JOYPAD_PORT_1),
			.pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1),
			.sticks = joypad_get_inputs(JOYPAD_PORT_1),
		};

		void (*draw_funcs[SCENE_COUNT])(void) = {
			title_draw, which_night_draw, game_draw,
			night_end_draw, paycheck_draw, game_over_draw,
			power_down_draw, custom_night_draw,
		};

		enum scene (*update_funcs[SCENE_COUNT])(update_parms_t) = {
			title_update, which_night_update, game_update,
			night_end_update, paycheck_update, game_over_update,
			power_down_update, custom_night_update,
		};

		surface_t *dsp = display_get();
		rdpq_attach(dsp, NULL);
		(*draw_funcs[scene])();
		rdpq_detach_show();

		static enum scene scene_last = SCENE_MAIN_GAME;
		scene_last = scene;
		scene = (*update_funcs[scene])(uparms);
		blip_update(uparms.dt);
		static_update(uparms.dt);

		if(scene_last != scene)
			rspq_wait();

		if(!audio_can_write())
			continue;

		short *audio_buf = audio_write_begin();
		mixer_poll(audio_buf, audio_get_buffer_length());
		audio_write_end();
	}

        /* N64 Terminate */
	joypad_close();
	timer_close();
	mixer_close();
	audio_close();
	dfs_close(dfs_handle);
        rdpq_debug_stop();
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
