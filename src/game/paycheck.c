#include <stdbool.h>

#include "engine/graphic.h"
#include "engine/sfx.h"

#include "game/game.h"
#include "game/texture_index.h"
#include "game/save_data.h"
#include "game/paycheck.h"

#define PAYCHECK_SCREEN_CNT 3

#define PAYCHECK_TIMER_FADE_IN_END    2.f
#define PAYCHECK_TIMER_FADE_OUT_START 17.f
#define PAYCHECK_TIMER_RETURN         19.f

static bool paycheck_is_loaded = false;

static float paycheck_timer;
static struct graphic paycheck_screens[PAYCHECK_SCREEN_CNT];

static void paycheck_load(void)
{
        static const char *screen_paths[PAYCHECK_SCREEN_CNT] = {
                TX_PAYCHECK1, TX_PAYCHECK2, TX_PINK_SLIP
        };

        paycheck_timer = 0.f;
        graphics_load(paycheck_screens, PAYCHECK_SCREEN_CNT,
                      screen_paths, true);
        mixer_ch_set_vol(SFX_CH_AMBIENCE, 0.8f, 0.8f);
        wav64_play(&sfx_music_box, SFX_CH_AMBIENCE);

        paycheck_is_loaded = true;
}

static void paycheck_unload(void)
{
        graphics_unload(paycheck_screens, PAYCHECK_SCREEN_CNT);

        paycheck_is_loaded = false;
}

void paycheck_draw(void)
{
        float alpha;

        if (!paycheck_is_loaded)
                paycheck_load();

        alpha = 1.f;
        if (paycheck_timer <= PAYCHECK_TIMER_FADE_IN_END)
                alpha = paycheck_timer * 0.5f;

        if (paycheck_timer >= PAYCHECK_TIMER_FADE_OUT_START)
                alpha = 1.0f -
                        ((paycheck_timer - PAYCHECK_TIMER_FADE_OUT_START) *
                         0.5f);

        rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xFF));
        rdpq_fill_rectangle(0, 0, 320, 240);

        rdpq_set_mode_standard();
        rdpq_mode_alphacompare(true);
        rdpq_set_fog_color(RGBA32(0xFF, 0xFF, 0xFF, alpha * 0xFF));
             rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY_CONST);

        /*
         * FIXME: With the way the new save system is implemented,
         * there's a bug where, upon completing night 20/20/20/20, you
         * get the same paycheck as night 6. However, you still get the
         * 3rd star, and it's marked on your save file.
         */
        graphic_draw(paycheck_screens + SAVE_NIGHT_NUM(save_data) - 6,
                     0, 0, 0, 0, GFX_FLIP_NONE);
}

enum scene paycheck_update(struct update_params uparms)
{
        paycheck_timer += uparms.dt;

        if ((uparms.pressed.a | uparms.pressed.b | uparms.pressed.start) &&
            (paycheck_timer < PAYCHECK_TIMER_FADE_OUT_START &&
             paycheck_timer > PAYCHECK_TIMER_FADE_IN_END))
                paycheck_timer = PAYCHECK_TIMER_FADE_OUT_START;

        if (paycheck_timer >= PAYCHECK_TIMER_RETURN) {
                if (paycheck_is_loaded)
                        paycheck_unload();

                sfx_stop_all_channels();
                return SCENE_TITLE_SCREEN;
        }

        return SCENE_PAYCHECK;
}
