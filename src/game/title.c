/* TODO: POSSIBLE REFACTOR! */

#include <stdlib.h>

#include "config.h"

#include "engine/graphic.h"
#include "engine/util.h"
#include "engine/sfx.h"

#include "game/static.h"
#include "game/blip_flash.h"
#include "game/ui.h"
#include "game/game.h"
#include "game/settings.h"
#include "game/texture_index.h"
#include "game/save_data.h"
#include "game/title.h"

#define TITLE_FACE_STATE_TIMER_MAX 0.08f
#define TITLE_FACE_ALPHA_TIMER_MAX 0.30f
#define TITLE_SCANLINE_TIMER_MAX   20.f
#define TITLE_SCANLINE_HEIGHT      10
#define TITLE_GFX_FREDDY_FACE_CNT  4

#define TITLE_GFX_BLIP_LOOP_DL_CNT    8

#define TITLE_NEW_GAME_TIMER_ONLY_PAPER 2.f
#define TITLE_NEW_GAME_TIMER_FADE_OUT        7.f
#define TITLE_NEW_GAME_TIMER_NEXT_SCENE      9.f

#define TITLE_DELETE_TIMER_COMPLETE 1.f

enum {
        TITLE_OPT_NEW_GAME,
        TITLE_OPT_CONTINUE,
        TITLE_OPT_NIGHT_6,
        TITLE_OPT_CUSTOM_NIGHT,
        TITLE_OPT_CNT
};

enum {
        TITLE_FLAG_IS_LOADED_SHIFT,
        TITLE_FLAG_EEP_FAIL_CONF_SHIFT,
        TITLE_FLAG_NEW_GAME_START_SHIFT,
        TITLE_FLAG_SETT_MENU_OPEN_SHIFT,
        TITLE_FLAG_SAVE_FILE_DEL_SHIFT
};

enum {
        TITLE_FLAG_IS_LOADED      = (1 << TITLE_FLAG_IS_LOADED_SHIFT),
        TITLE_FLAG_EEP_FAIL_CONF  = (1 << TITLE_FLAG_EEP_FAIL_CONF_SHIFT),
        TITLE_FLAG_NEW_GAME_START = (1 << TITLE_FLAG_NEW_GAME_START_SHIFT),
        TITLE_FLAG_SETT_MENU_OPEN = (1 << TITLE_FLAG_SETT_MENU_OPEN_SHIFT),
        TITLE_FLAG_SAVE_FILE_DEL  = (1 << TITLE_FLAG_SAVE_FILE_DEL_SHIFT)
};

static const char *title_gfx_freddy_face_paths[TITLE_GFX_FREDDY_FACE_CNT] = {
        TX_FRED_FACE0, TX_FRED_FACE1, TX_FRED_FACE2, TX_FRED_FACE3,
};

static struct graphic title_gfx_freddy_face[TITLE_GFX_FREDDY_FACE_CNT];
static struct graphic title_gfx_night_text;
static struct graphic title_gfx_night_atlas;
static struct graphic title_gfx_star;
static struct graphic title_gfx_selector;
static struct graphic title_gfx_fnaf_text;
static struct graphic title_gfx_opt_text;
static struct graphic title_gfx_settings_text;
static struct graphic title_gfx_settings_opt_text;
static struct graphic title_gfx_bind_btn_text;
static struct graphic title_gfx_settings_descs;
static struct graphic title_gfx_eeprom_err;
static struct graphic title_gfx_newspaper;
static rspq_block_t **title_gfx_blip_loop_dls;

static float          title_face_state_timer;
static int            title_face_state;
static float          title_face_alpha_timer;
static uint8_t        title_face_alpha;
static float          title_scanline_timer;
static float          title_blip_loop_timer;
static uint8_t        title_blip_loop_frame;
static uint8_t        title_blip_loop_alpha;
static uint8_t        title_blip_loop_invisible;
static int            title_opt_cur;
static int            title_opt_cur_setting;
static int            title_opt_available;
static float          title_new_game_timer;
static float          title_save_file_delete_timer;
static uint8_t        title_flags;

static void title_load(void)
{
        /* Graphics */
        graphics_load(title_gfx_freddy_face, TITLE_GFX_FREDDY_FACE_CNT,
                      title_gfx_freddy_face_paths, true);
        graphic_load(       &title_gfx_night_text,        TX_NIGHT_TEXT, true);
        graphic_load(      &title_gfx_night_atlas,   TX_NIGHT_NUM_ATLAS, true);
        graphic_load(             &title_gfx_star,              TX_STAR, true);
        graphic_load(         &title_gfx_selector,          TX_SELECTOR, true);
        graphic_load(        &title_gfx_fnaf_text,        TX_TITLE_TEXT, true);
        graphic_load(         &title_gfx_opt_text,     TX_TITLE_OPTIONS, true);
        graphic_load(    &title_gfx_settings_text,     TX_SETTINGS_TEXT, true);
        graphic_load(&title_gfx_settings_opt_text,     TX_SETTINGS_OPTS, true);
        graphic_load(    &title_gfx_bind_btn_text, TX_BIND_BUTTONS_TEXT, true);
        graphic_load(   &title_gfx_settings_descs,    TX_SETTINGS_DESCS, true);
        graphic_load(       &title_gfx_eeprom_err,      TX_EEPROM_ERROR, true);
        graphic_load(        &title_gfx_newspaper,         TX_NEWSPAPER, true);

        blip_flash_trigger(true);
        wav64_play(&sfx_static, SFX_CH_STATIC);
        mixer_ch_set_vol(SFX_CH_AMBIENCE, 0.8f, 0.8f);
        wav64_play(&sfx_title_music, SFX_CH_AMBIENCE);

        /* Generate the blip loop animation at runtime. */
        {
                const struct blip_frame_info inf[TITLE_GFX_BLIP_LOOP_DL_CNT] = {
                        { 1, { {  61,  73 }, {  -1,  -1 }, {  -1,  -1 } } },
                        { 1, { { 156, 165 }, {  -1,  -1 }, {  -1,  -1 } } },
                        { 3, { {  12,  21 }, {  34,  36 }, { 156, 180 } } },
                        { 2, { {  69, 107 }, { 117, 121 }, {  -1,  -1 } } },
                        { 3, { {   6,  11 }, {  65,  69 }, { 189, 196 } } },
                        { 1, { { 142, 146 }, {  -1,  -1 }, {  -1,  -1 } } },
                        { 2, { {  64,  79 }, {  90,  93 }, {  -1,  -1 } } },
                        { 1, { { 144, 173 }, {  -1,  -1 }, {  -1,  -1 } } }
                };

                title_gfx_blip_loop_dls =
                        blip_create_from_info(inf, TITLE_GFX_BLIP_LOOP_DL_CNT);
        }

        /* Variables */
        title_face_state_timer       = 0.f;
        title_face_state             = rand() % 100;
        title_face_alpha_timer       = 0.f;
        title_face_alpha             = 0xFF - (rand() % 250);
        title_scanline_timer         = 0.f;
        title_blip_loop_timer        = 0.f;
        title_blip_loop_frame        = 0;
        title_blip_loop_alpha        = 0xFF - ((rand() % 100) + 100);
        title_blip_loop_invisible    = rand() % 3;
        title_opt_cur                = TITLE_OPT_NEW_GAME;
        title_opt_cur_setting        = 0;
        title_opt_available          = 2;
        title_new_game_timer         = 0.f;
        title_save_file_delete_timer = 0.f;
        title_flags                  = TITLE_FLAG_IS_LOADED;
}

static void title_unload(void)
{
        graphic_unload(       &title_gfx_eeprom_err);
        graphic_unload(   &title_gfx_settings_descs);
        graphic_unload(    &title_gfx_bind_btn_text);
        graphic_unload(&title_gfx_settings_opt_text);
        graphic_unload(    &title_gfx_settings_text);
        graphic_unload(        &title_gfx_newspaper);
        graphic_unload(      &title_gfx_night_atlas);
        graphic_unload(       &title_gfx_night_text);
        graphic_unload(         &title_gfx_opt_text);
        graphic_unload(        &title_gfx_fnaf_text);
        graphic_unload(         &title_gfx_selector);
        graphic_unload(             &title_gfx_star);
        graphics_unload(title_gfx_freddy_face, TITLE_GFX_FREDDY_FACE_CNT);
        blip_destroy(&title_gfx_blip_loop_dls, TITLE_GFX_BLIP_LOOP_DL_CNT);

        title_flags &= ~(TITLE_FLAG_IS_LOADED);
}

void title_draw(void)
{
        if (!(title_flags & TITLE_FLAG_IS_LOADED))
                title_load();

        /*
         * When fading to the newspaper from a new game, it fades the paper
         * in while still drawing all the title stuff behind it. After 2
         * seconds have passed, it encompasses the entire screen and the
         * rest of the title is no longer required and just wastes
         * rendering time.
         */
        if (title_new_game_timer >= TITLE_NEW_GAME_TIMER_ONLY_PAPER)
                goto title_draw_newspaper;

        /* Draw Freddy's face on the screen. */
        {
                int face_cur;

                face_cur = (title_face_state < TITLE_GFX_FREDDY_FACE_CNT) *
                        title_face_state;
                rdpq_set_mode_copy(false);
                graphic_draw(title_gfx_freddy_face + face_cur,
                             0, 0, 0, 0, GFX_FLIP_NONE);
        }

        /* Make Freddy face darker randomly. */
        rdpq_set_mode_standard();
        rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
        rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY_CONST);
        rdpq_set_prim_color(color_from_packed16(0));
        rdpq_set_fog_color(color_from_packed32(title_face_alpha));
        rdpq_fill_rectangle(0, 0, 320, 240);

        static_draw(true);

        /* Draw Scanline */
        {
                int y_pos;

                rdpq_set_mode_standard();
                rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
                rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY_CONST);
                rdpq_set_prim_color(color_from_packed16(0xFFFF));
                rdpq_set_fog_color(RGBA32(0xFF, 0xFF, 0xFF, 75));
                y_pos = (title_scanline_timer / TITLE_SCANLINE_TIMER_MAX) *
                        (240 + TITLE_SCANLINE_HEIGHT);
                rdpq_fill_rectangle(0, y_pos - TITLE_SCANLINE_HEIGHT,
                                    320, y_pos);
        }

        /* Draw Blip Loop */
        if (!title_blip_loop_invisible) {
                rdpq_set_mode_standard();
                rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
                rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY_CONST);
                rdpq_set_prim_color(color_from_packed16(0xFFFE));
                rdpq_set_fog_color(color_from_packed32(title_blip_loop_alpha));
                rspq_block_run(title_gfx_blip_loop_dls[title_blip_loop_frame]);
        }

        /* Draw settings menu if it's open and skip the rest. */
        if (title_flags & TITLE_FLAG_SETT_MENU_OPEN) {
                int i;
                const int sett_opt_cur_y_pos[SETTING_COUNT] = {
                        259, 311, 371, 424, 479, 534, 589
                };

                rdpq_set_mode_standard();
                rdpq_mode_alphacompare(true);
                graphic_draw(&title_gfx_settings_text, 64,
                             64, 0, 0, GFX_FLIP_NONE);
                graphic_draw(&title_gfx_settings_opt_text,
                             128, 256, 0, 0, GFX_FLIP_NONE);

                graphic_draw(&title_gfx_selector, 40,
                             sett_opt_cur_y_pos[title_opt_cur_setting],
                             0, 0, GFX_FLIP_NONE);
                graphic_draw_index_y(&title_gfx_settings_descs, 132, 162, 20,
                                     title_opt_cur_setting, GFX_FLIP_NONE);

                /*
                 * Draw the little boxes indicating
                 * if a setting is on or off.
                 */
                rdpq_set_mode_fill(RGBA32(0xff, 0xff, 0xff, 0xff));
                for (i = 0; i < SETTING_COUNT; ++i) {
                        int x, y;

                        if (!(settings_flags & (1 << i)))
                                continue;

                        x = 280 + 6;
                        y = (sett_opt_cur_y_pos[i] / 3) + 6;
                        rdpq_fill_rectangle(x, y, x + 6, y + 6);
                }

                return;
        }

        /* Draw the normal title menu. */
        {
                int star_cnt, i;

                star_cnt = ((save_data & SAVE_NIGHT_5_BEATEN_BIT) >>
                        SAVE_NIGHT_5_BEATEN_BIT_SHIFT) +
                        ((save_data & SAVE_NIGHT_6_BEATEN_BIT) >>
                        SAVE_NIGHT_6_BEATEN_BIT_SHIFT) +
                        ((save_data & SAVE_MODE_20_BEATEN_BIT) >>
                        SAVE_MODE_20_BEATEN_BIT_SHIFT);
                title_opt_available = CLAMP(star_cnt, 0, 2) + 2;

                rdpq_set_mode_standard();
                rdpq_mode_alphacompare(1);
                for (i = 0; i < star_cnt; ++i)
                        graphic_draw(&title_gfx_star, 93 + 77 * i,
                                     350, 28, 27, GFX_FLIP_NONE);
        }

        graphic_draw_index_y(&title_gfx_opt_text, 118, 420,
                             22 * title_opt_available, 0, GFX_FLIP_NONE);
        graphic_draw(&title_gfx_fnaf_text, 70, 68, 0, 0, GFX_FLIP_NONE);
        graphic_draw(&title_gfx_bind_btn_text, 558, 45, 0, 0, GFX_FLIP_NONE);
        graphic_draw(&title_gfx_selector, 40, 429 + title_opt_cur * 66,
                     0, 0, GFX_FLIP_NONE);

        /*
         * Draw night number ONLY if we're hovering over Continue, and
         * make sure that it can only go up to 5 (as per the original game).
         */
        if (title_opt_cur == TITLE_OPT_CONTINUE) {
                graphic_draw(&title_gfx_night_text, 444, 509,
                             0, 0, GFX_FLIP_NONE);
                graphic_draw_index_y(&title_gfx_night_atlas, 512, 509, 6,
                                     CLAMP(SAVE_NIGHT_NUM(save_data), 1, 5),
                                     GFX_FLIP_NONE);
        }

        blip_flash_draw();

        /* Draw EEPROM failed error screen. */
        if (save_data_eeprom_failed &&
            !(title_flags & TITLE_FLAG_EEP_FAIL_CONF)) {
                rdpq_set_mode_standard();
                rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
                rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
                rdpq_set_prim_color(RGBA32(0x0, 0x0, 0x0, 0xD8));
                rdpq_fill_rectangle(0, 0, 320, 240);

                rdpq_set_mode_standard();
                rdpq_mode_alphacompare(true);
                graphic_draw(&title_gfx_eeprom_err, 96, 168,
                             0, 0, GFX_FLIP_NONE);
        }

        if (title_flags & TITLE_FLAG_NEW_GAME_START) {
                float newspaper_alpha;

title_draw_newspaper:
                if (title_new_game_timer <= TITLE_NEW_GAME_TIMER_ONLY_PAPER) {
                        newspaper_alpha = title_new_game_timer * 0.5f;
                } else if (title_new_game_timer >=
                           TITLE_NEW_GAME_TIMER_FADE_OUT) {
                        newspaper_alpha = 1.0f - ((title_new_game_timer -
                                TITLE_NEW_GAME_TIMER_FADE_OUT) * 0.5f);
                        rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xFF));
                        rdpq_fill_rectangle(0, 0, 320, 240);
                } else {
                        newspaper_alpha = 1.f;
                }

                rdpq_set_mode_standard();
                rdpq_mode_combiner(RDPQ_COMBINER_TEX_FLAT);
                rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY_CONST);
                rdpq_set_prim_color(color_from_packed32(0xFFFFFFFF));
                rdpq_set_fog_color(color_from_packed32(newspaper_alpha * 0xFF));
                graphic_draw(&title_gfx_newspaper, 0, 0, 0, 0, GFX_FLIP_NONE);
        }
}

enum scene title_update(const struct update_params uparms)
{
        /* Face State */
        {
                bool state_tick;

                title_face_state_timer =
                        wrapf(title_face_state_timer + uparms.dt,
                              TITLE_FACE_STATE_TIMER_MAX, &state_tick);
                if (state_tick) {
                        title_face_state = rand() % 100;
                        title_blip_loop_alpha = 0xFF - ((rand() % 100) + 100);
                }
        }

        /* Face Transparency */
        {
                bool alpha_tick;

                title_face_alpha_timer =
                        wrapf(title_face_alpha_timer + uparms.dt,
                              TITLE_FACE_ALPHA_TIMER_MAX, &alpha_tick);
                if (alpha_tick) {
                        title_face_alpha = 0xFF - (rand() % 250);
                        title_blip_loop_invisible = rand() % 3;
                }
        }

        /* Scanline */
        title_scanline_timer = wrapf(title_scanline_timer + uparms.dt,
                                     TITLE_SCANLINE_TIMER_MAX, NULL);

        /* Blip Loop */
        {
                bool blip_loop_tick;

                title_blip_loop_timer = wrapf(title_blip_loop_timer +
                                              uparms.dt * speed_fps(10), 1.f,
                                              &blip_loop_tick);
                if (blip_loop_tick) {
                        ++title_blip_loop_frame;
                        if (title_blip_loop_frame >= TITLE_GFX_BLIP_LOOP_DL_CNT)
                                title_blip_loop_frame = 0;
                }
        }

        /* Check if EEPROM failed. */
        if (save_data_eeprom_failed &&
            !(title_flags & TITLE_FLAG_EEP_FAIL_CONF)) {
                title_flags |= (uparms.pressed.start <<
                                TITLE_FLAG_EEP_FAIL_CONF_SHIFT);
                return SCENE_TITLE_SCREEN;
        }

        /* If new game started, do the fade in. */
        if (title_flags & TITLE_FLAG_NEW_GAME_START) {
                bool skip_newspaper;

                title_new_game_timer += uparms.dt;

                /* Check for skipping newspaper for new game. */
                skip_newspaper =
                        (uparms.pressed.a | uparms.pressed.start) &&
                        title_new_game_timer < TITLE_NEW_GAME_TIMER_FADE_OUT &&
                        title_new_game_timer > TITLE_NEW_GAME_TIMER_ONLY_PAPER;

                if (skip_newspaper)
                        title_new_game_timer = TITLE_NEW_GAME_TIMER_FADE_OUT;

                if (title_new_game_timer >= TITLE_NEW_GAME_TIMER_NEXT_SCENE) {
                        title_unload();
                        sfx_stop_all_channels();
                        return SCENE_WHICH_NIGHT;
                }

                return SCENE_TITLE_SCREEN;
        }

        /* Deleting your save file */
        if (uparms.held.z) {
                if (!(title_flags & TITLE_FLAG_SAVE_FILE_DEL))
                        title_save_file_delete_timer += uparms.dt;
        } else {
                title_save_file_delete_timer = 0.0f;
                title_flags &= ~(TITLE_FLAG_SAVE_FILE_DEL);
        }

        if (title_save_file_delete_timer >= TITLE_DELETE_TIMER_COMPLETE) {
                blip_flash_trigger(false);
                save_data                     = 0x1;
                title_save_file_delete_timer  = 0.0f;
                title_flags                  |= TITLE_FLAG_SAVE_FILE_DEL;
                title_opt_cur                 = TITLE_OPT_NEW_GAME;
                if (!save_data_eeprom_failed)
                        eepfs_write("fnaf.dat", &save_data, sizeof(save_data));

#ifdef TITLE_DEBUG_ENABLED
                debugf("Wiped save file to night %d\n", save_data);
#endif
        }

        /* Operating the settings menu only. */
        title_flags ^= (uparms.pressed.r << TITLE_FLAG_SETT_MENU_OPEN_SHIFT);
        if (title_flags & TITLE_FLAG_SETT_MENU_OPEN) {
                if (uparms.pressed.l)
                        wav64_play(&sfx_boop, SFX_CH_BLIP);

                if (uparms.pressed.d_down || uparms.pressed.c_down) {
                        blip_flash_trigger(false);
                        title_opt_cur_setting++;
                }

                if (title_opt_cur_setting >= SETTING_COUNT)
                        title_opt_cur_setting = 0;

                if (uparms.pressed.d_up || uparms.pressed.c_up) {
                        blip_flash_trigger(false);
                        title_opt_cur_setting--;
                }

                if (title_opt_cur_setting < 0)
                        title_opt_cur_setting = SETTING_COUNT - 1;

                settings_flags ^= (uparms.pressed.a << title_opt_cur_setting);

                return SCENE_TITLE_SCREEN;
        }

        /* Switching menu options. */
        if (uparms.pressed.d_up || uparms.pressed.c_up) {
                title_opt_cur--;
                blip_flash_trigger(false);
                if (title_opt_cur < 0)
                        title_opt_cur = title_opt_available - 1;
        }

        if (uparms.pressed.d_down || uparms.pressed.c_down) {
                title_opt_cur++;
                blip_flash_trigger(false);
                if (title_opt_cur >= title_opt_available)
                        title_opt_cur = 0;
        }

        /* Starting conditions. */
        if (uparms.pressed.start || uparms.pressed.a) {
                uint8_t save_beaten_bits_tmp = save_data & SAVE_BEATEN_BITMASK;

                switch(title_opt_cur) {
                case TITLE_OPT_NEW_GAME:
                        /* Just reset the nights, not the stars */
                        save_data &= SAVE_NIGHT_5_BEATEN_BIT |
                                     SAVE_NIGHT_6_BEATEN_BIT |
                                     SAVE_MODE_20_BEATEN_BIT;
                        save_data |= 1;

                        if (!save_data_eeprom_failed)
                                eepfs_write("fnaf.dat", &save_data, 1);

                        title_flags |= TITLE_FLAG_NEW_GAME_START;
                        return SCENE_TITLE_SCREEN;

                case TITLE_OPT_CONTINUE:
                        title_unload();
                        save_data = MIN(SAVE_NIGHT_NUM(save_data), 5);
                        save_data |= save_beaten_bits_tmp;
                        sfx_stop_all_channels();
                        return SCENE_WHICH_NIGHT;

                case TITLE_OPT_NIGHT_6:
                        title_unload();
                        save_data = 6;
                        save_data |= save_beaten_bits_tmp;
                        sfx_stop_all_channels();
                        return SCENE_WHICH_NIGHT;

                case TITLE_OPT_CUSTOM_NIGHT:
                        title_unload();
                        save_data = 7;
                        save_data |= save_beaten_bits_tmp;
                        return SCENE_CUSTOM_NIGHT;
                }
        }

        return SCENE_TITLE_SCREEN;
}
