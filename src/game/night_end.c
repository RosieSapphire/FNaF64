#include "engine/sfx.h"
#include "engine/graphic.h"
#include "engine/util.h"

#include "game/game.h"
#include "game/freddy.h"
#include "game/bonnie.h"
#include "game/chica.h"
#include "game/foxy.h"
#include "game/settings.h"
#include "game/texture_index.h"
#include "game/save_data.h"
#include "game/night_end.h"

/* Uncomment this if you want to skip the 6AM transition. */
// #define NIGHT_END_SKIP

#define NIGHT_END_TIMER_PLAY_CHEER 6.2f

#ifdef NIGHT_END_SKIP
#define NIGHT_END_TIMER_EXIT 0.f
#else
#define NIGHT_END_TIMER_EXIT 11.5f
#endif

/*
 * TODO: Make it write the save data as soon as the night hits 6 AM instead
 * of waiting for the little animation to finish. That way if something
 * happens between the 5-6 AM transition, it won't affect your save file. :D
 */

static struct graphic night_end_gfx_am;
static struct graphic night_end_gfx_six;
static struct graphic night_end_gfx_five;
static struct graphic night_end_gfx_damn_shell;
static        float   night_end_timer;
static        bool    night_end_played_cheer;
static        bool    night_end_is_loaded = false;

static void night_end_load(void)
{
        int night_last;
        bool beat_20_mode_no_cheats;

        night_end_timer = 0.f;

        /* Give stars for beating nights 5 and 6, as well as all 20 mode. */
        beat_20_mode_no_cheats = freddy_ai_level == 20 &&
                bonnie_ai_level == 20 &&
                chica_ai_level == 20 &&
                foxy_ai_level == 20 &&
                !(settings_flags & SET_ROBOT_CHEAT_BIT) &&
                !(settings_flags & SET_FAST_NIGHT_BIT);

        save_data |=
                ((SAVE_NIGHT_NUM(save_data) == 5) <<
                SAVE_NIGHT_5_BEATEN_BIT_SHIFT) |
                ((SAVE_NIGHT_NUM(save_data) == 6) <<
                SAVE_NIGHT_6_BEATEN_BIT_SHIFT) |
                (((SAVE_NIGHT_NUM(save_data) == 7) & beat_20_mode_no_cheats) <<
                SAVE_MODE_20_BEATEN_BIT_SHIFT);


        night_last = (save_data++) & SAVE_NIGHT_NUM_BITMASK;
        debugf("Night Last: %d, Night Cur: %d\n",
               night_last, save_data & SAVE_NIGHT_NUM_BITMASK);
        mixer_ch_set_vol(SFX_CH_AMBIENCE, 0.8f, 0.8f);
        wav64_play(&sfx_chimes, SFX_CH_AMBIENCE);

        graphic_load(&night_end_gfx_am, TX_END_AM);
        graphic_load(&night_end_gfx_six, TX_END_SIX);
        graphic_load(&night_end_gfx_five, TX_END_FIVE);
        if (game_won_by_murder)
                graphic_load(&night_end_gfx_damn_shell, TX_END_DAMN);

        night_end_played_cheer = false;
        night_end_is_loaded    = true;
}

static void night_end_unload(void)
{
        if (game_won_by_murder)
                graphic_unload(&night_end_gfx_damn_shell);

        graphic_unload(&night_end_gfx_six);
        graphic_unload(&night_end_gfx_five);
        graphic_unload(&night_end_gfx_am);
        night_end_is_loaded = false;
}

void night_end_draw(void)
{
        float t;
        const int five_start = 298, five_end = 188,
                  six_start  = 408, six_end  = 298;

        if (!night_end_is_loaded) {
                night_end_load();
        }

        rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xFF));
        rdpq_fill_rectangle(0, 0, 320, 240);

        rdpq_set_mode_standard();

        t = CLAMP((night_end_timer - 1) * 0.2f, 0, 1);
        graphic_draw(game_won_by_murder ?
                     night_end_gfx_damn_shell : night_end_gfx_six,
                     390, lerpf(six_start, six_end, t),
                     -5, 0, GFX_FLIP_NONE);
        graphic_draw(night_end_gfx_five, 390, lerpf(five_start, five_end, t),
                     -5, 0, GFX_FLIP_NONE);
        graphic_draw(night_end_gfx_am, 486, 296, -5, 0, GFX_FLIP_NONE);

        rdpq_set_mode_fill(RGBA32(0x0, 0x0, 0x0, 0xFF));
        rdpq_fill_rectangle(vcon(339), vcon(168),
                            vcon(339) + vcon(400),
                            vcon(168) + vcon(120));
        rdpq_fill_rectangle(vcon(339), vcon(384),
                            vcon(339) + vcon(400),
                            vcon(384) + vcon(120));

        rdpq_set_mode_standard();
        rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
        rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
        {
                float fade;

                fade = (night_end_timer < 9) ?
                       CLAMP(1.0f - night_end_timer, 0, 1) :
                       CLAMP(night_end_timer - 10.5f, 0, 1);
                rdpq_set_prim_color(RGBA32(0x0, 0x0, 0x0, fade * 255));
        }
        rdpq_fill_rectangle(0, 0, 320, 240);
}

enum scene night_end_update(const struct update_params uparms)
{
        night_end_timer += uparms.dt;

        if (night_end_timer >= NIGHT_END_TIMER_PLAY_CHEER &&
                !night_end_played_cheer) {
                night_end_played_cheer = true;
                mixer_ch_set_vol(SFX_CH_FAN, 0.8f, 0.8f);
                wav64_play(game_won_by_murder ? &sfx_daaamn : &sfx_cheering,
                           SFX_CH_FAN);
        }

        if (night_end_timer < NIGHT_END_TIMER_EXIT)
                return SCENE_NIGHT_END;

        if (!save_data_eeprom_failed)
                eepfs_write("fnaf.dat", &save_data, 1);

        debugf("Saved night %d and %d%d%d to save file.\n",
               SAVE_NIGHT_NUM(save_data),
               ((save_data & SAVE_NIGHT_5_BEATEN_BIT) >>
               SAVE_NIGHT_5_BEATEN_BIT_SHIFT),
               ((save_data & SAVE_NIGHT_6_BEATEN_BIT) >>
               SAVE_NIGHT_6_BEATEN_BIT_SHIFT),
               ((save_data & SAVE_MODE_20_BEATEN_BIT) >>
               SAVE_MODE_20_BEATEN_BIT_SHIFT));

        if (night_end_is_loaded)
                night_end_unload();

        sfx_stop_all_channels();

        if (SAVE_NIGHT_NUM(save_data) < 6)
                return SCENE_WHICH_NIGHT;

        return SCENE_PAYCHECK;
}
