/* Includes */
#include <stdlib.h>
#include <stdbool.h>

#include "engine/perspective.h"
#include "engine/sfx.h"
#include "engine/util.h"

#include "game/office.h"
#include "game/fan.h"
#include "game/buttons.h"
#include "game/doors.h"
#include "game/camera.h"
#include "game/static.h"
#include "game/blip_flash.h"
#include "game/ui.h"
#include "game/subtitles.h"
#include "game/hallucinations.h"
#include "game/freddy.h"
#include "game/bonnie.h"
#include "game/chica.h"
#include "game/foxy.h"
#include "game/golden_freddy.h"
#include "game/settings.h"
#include "game/save_data.h"
#include "game/texture_index.h"
#include "game/game.h"

/* Defines */
#define GAME_DOOR_LEFT_ANIM_POS_X      72
#define GAME_DOOR_LEFT_ANIM_POS_Y     -1
#define GAME_DOOR_RIGHT_ANIM_POS_X     1270
#define GAME_DOOR_RIGHT_ANIM_POS_Y    -2
#define GAME_DOOR_BUTTON_GFX_CNT       8
#define GAME_DOOR_LEFT                 0
#define GAME_DOOR_RIGHT                1
#define GAME_DOOR_ANIM_FRAME_CNT       15
#define GAME_DOOR_LEFT_INTERACT_DIST  -68
#define GAME_DOOR_RIGHT_INTERACT_DIST -554

#define GAME_FAN_POS_X 868
#define GAME_FAN_POS_Y 400
#define GAME_FAN_ORIG_X 88
#define GAME_FAN_ORIG_Y 97
#define GAME_FAN_FRAME_CNT 3

#define GAME_OFFICE_VIEW_CNT 5

#define GAME_SHOTGUN_ANIM_FRAME_CNT        10
#define GAME_SHOTGUN_RELOAD_TIMER          .8f
#define GAME_SHOTGUN_FRAME_PLAY_RELOAD_SFX 3
#define GAME_SHOTGUN_MAX_ABS_THETA         8.f
#define GAME_SHOTGUN_SCREEN_SHAKE_MAG_MAX  48
#define GAME_SHOTGUN_SCREEN_SHAKE_SPD      32.f

enum {
        GAME_SHOTGUN_BROKE_DOOR_LEFT_SHIFT,
        GAME_SHOTGUN_BROKE_DOOR_RIGHT_SHIFT
};

enum {
        GAME_SHOTGUN_BROKE_DOOR_LEFT =
                (1 << GAME_SHOTGUN_BROKE_DOOR_LEFT_SHIFT),
        GAME_SHOTGUN_BROKE_DOOR_RIGHT =
                (1 << GAME_SHOTGUN_BROKE_DOOR_RIGHT_SHIFT)
};

/* Graphics Paths */
static const char *game_gfx_office_view_paths[GAME_OFFICE_VIEW_CNT] = {
        TX_OFFICE_NORMAL, TX_OFFICE_LEFT_EMPTY, TX_OFFICE_RIGHT_EMPTY,
        TX_OFFICE_LEFT_BONNIE, TX_OFFICE_RIGHT_CHICA,
};

static const char *game_gfx_foxy_scare_paths[FOXY_SCARE_FRAME_CNT] = {
        TX_FOXY_SCARE0, TX_FOXY_SCARE1, TX_FOXY_SCARE2,
        TX_FOXY_SCARE3, TX_FOXY_SCARE4, TX_FOXY_SCARE5,
        TX_FOXY_SCARE6, TX_FOXY_SCARE7,
};

static const char *game_freddy_scare_paths[FREDDY_SCARE_FRAME_CNT] = {
        TX_FREDDY_SCARE_ROOM00, TX_FREDDY_SCARE_ROOM01, TX_FREDDY_SCARE_ROOM02,
        TX_FREDDY_SCARE_ROOM03, TX_FREDDY_SCARE_ROOM04, TX_FREDDY_SCARE_ROOM05,
        TX_FREDDY_SCARE_ROOM06, TX_FREDDY_SCARE_ROOM07, TX_FREDDY_SCARE_ROOM08,
        TX_FREDDY_SCARE_ROOM09, TX_FREDDY_SCARE_ROOM10, TX_FREDDY_SCARE_ROOM11,
        TX_FREDDY_SCARE_ROOM12, TX_FREDDY_SCARE_ROOM13,
};

static const char *game_gfx_door_btn_paths[GAME_DOOR_BUTTON_GFX_CNT] = {
        TX_BUTTON_LEFT00, TX_BUTTON_LEFT10, TX_BUTTON_LEFT01,
        TX_BUTTON_LEFT11, TX_BUTTON_RIGHT00, TX_BUTTON_RIGHT10,
        TX_BUTTON_RIGHT01, TX_BUTTON_RIGHT11,
};

static const char *game_gfx_shotgun_anim_paths[GAME_SHOTGUN_ANIM_FRAME_CNT] = {
        TX_SHOTGUN_00, TX_SHOTGUN_01, TX_SHOTGUN_02, TX_SHOTGUN_03,
        TX_SHOTGUN_04, TX_SHOTGUN_05, TX_SHOTGUN_06, TX_SHOTGUN_07,
        TX_SHOTGUN_08, TX_SHOTGUN_09
};

static const char *game_gfx_fan_anim_paths[GAME_FAN_FRAME_CNT] = {
        TX_FAN0, TX_FAN1, TX_FAN2,
};

static const char *game_gfx_anim_doors_paths[GAME_DOOR_ANIM_FRAME_CNT] = {
        TX_DOOR_ANIM00, TX_DOOR_ANIM01, TX_DOOR_ANIM02,
        TX_DOOR_ANIM03, TX_DOOR_ANIM04, TX_DOOR_ANIM05,
        TX_DOOR_ANIM06, TX_DOOR_ANIM07, TX_DOOR_ANIM08,
        TX_DOOR_ANIM09, TX_DOOR_ANIM10, TX_DOOR_ANIM11,
        TX_DOOR_ANIM12, TX_DOOR_ANIM13, TX_DOOR_ANIM14,
};

/* Graphics */
static struct graphic game_gfx_office_views[GAME_OFFICE_VIEW_CNT];
static struct graphic game_gfx_foxy_scare[FOXY_SCARE_FRAME_CNT];
static struct graphic game_freddy_scare[FREDDY_SCARE_FRAME_CNT];
static struct graphic game_gfx_door_btns[GAME_DOOR_BUTTON_GFX_CNT];
static struct graphic game_gfx_anim_doors[GAME_DOOR_ANIM_FRAME_CNT];
static struct graphic game_gfx_fan_anim[GAME_FAN_FRAME_CNT];
static struct graphic game_gfx_shotgun_anim[GAME_SHOTGUN_ANIM_FRAME_CNT];

/* TODO: Make graphics have a flag that specifies if it should be scaled. */
static sprite_t *game_gfx_exit_prompt;

/* Global Variables */
float   game_office_turn;
float   game_office_turn_lerp;
float   game_scare_timer_foxy;
float   game_scare_timer_freddy;
float   game_office_flicker_rand_timer;
int     game_office_flicker_rand;
float   game_door_anim_timers[GAME_DOOR_CNT];
float   game_pause_timer_accum;
uint8_t game_hour_cur;
uint8_t game_hour_prev;
int     game_power_usage;
int     game_power_left;
float   game_power_timer;
uint8_t game_jumpscare_flags;
bool    game_won_by_murder;
bool    game_shotgun_is_unlocked;
float   game_shotgun_reload_timer;

/* Local Variables */
static float   game_sfx_jumpscare_exit_timer;
static float   game_circus_timer;
static int     game_night_skip_correct;
static float   game_hour_timer;
static float   game_ticks_since_load;
static float   game_time_since_load;
static int     game_door_btn_states_cur;
static float   game_fan_anim_timer;
static bool    game_shotgun_play_sfx;
static float   game_shotgun_screen_shake_mag;
static uint8_t game_shotgun_broke_door_flags;
static float   game_shotgun_killed_all_timer;
static bool    game_show_exit_prompt;
static bool    game_is_loaded = false;

struct door_btn {
        int state;
        int index;
};

static struct door_btn game_door_btn_states_left[] = {
        { 0,                                                  0 },
        { GAME_DOOR_BTN_LEFT_DOOR,                            1 },
        { GAME_DOOR_BTN_LEFT_LIGHT,                           2 },
        { GAME_DOOR_BTN_LEFT_LIGHT | GAME_DOOR_BTN_LEFT_DOOR, 3 },
};

static struct door_btn game_door_btn_states_right[] = {
        { 0,                                                    4 },
        { GAME_DOOR_BTN_RIGHT_DOOR,                             5 },
        { GAME_DOOR_BTN_RIGHT_LIGHT,                            6 },
        { GAME_DOOR_BTN_RIGHT_LIGHT | GAME_DOOR_BTN_RIGHT_DOOR, 7 },
};

static void game_load(void)
{
        int night_cur;
        wav64_t *sfx_phone_calls[5] = {
                &sfx_phone_call_1, &sfx_phone_call_2, &sfx_phone_call_3,
                &sfx_phone_call_4, &sfx_phone_call_5
        };

        game_ticks_since_load   = get_ticks();
        game_night_skip_correct = 0;

        /* Office */
        graphics_load(game_gfx_office_views, GAME_OFFICE_VIEW_CNT,
                      game_gfx_office_view_paths);
        graphics_load(game_gfx_foxy_scare, FOXY_SCARE_FRAME_CNT,
                      game_gfx_foxy_scare_paths);
        wav64_play(&sfx_light, SFX_CH_LIGHT);

        game_office_turn               = OFFICE_TURN_MIN >> 1;
        game_office_turn_lerp          = game_office_turn;
        game_office_flicker_rand_timer = 0.f;
        game_office_flicker_rand       = 0;

        /* TODO: Make this really hard to obtain. :3 */
        game_shotgun_is_unlocked       = false;
        game_shotgun_reload_timer      = 0.f;
        game_shotgun_play_sfx          = false;
        game_shotgun_screen_shake_mag  = 0.f;
        game_shotgun_broke_door_flags  = 0;
        game_shotgun_killed_all_timer  = 0.f;

        /* Fan */
        game_fan_anim_timer = 0.f;
        graphics_load(game_gfx_fan_anim, GAME_FAN_FRAME_CNT,
                      game_gfx_fan_anim_paths);
        mixer_ch_set_vol(SFX_CH_FAN, 0.25f, 0.25f);
        wav64_play(&sfx_fan, SFX_CH_FAN);

        /* Doors */
        graphics_load(game_gfx_anim_doors, GAME_DOOR_ANIM_FRAME_CNT,
                      game_gfx_anim_doors_paths);
        game_door_anim_timers[GAME_DOOR_LEFT]  = 0;
        game_door_anim_timers[GAME_DOOR_RIGHT] = 0;

        /* Shotgun */
        if (game_shotgun_is_unlocked)
                graphics_load(game_gfx_shotgun_anim,
                              GAME_SHOTGUN_ANIM_FRAME_CNT,
                              game_gfx_shotgun_anim_paths);

        /* Door Buttons */
        game_door_btn_states_cur = 0;
        graphics_load(game_gfx_door_btns, GAME_DOOR_BUTTON_GFX_CNT,
                      game_gfx_door_btn_paths);

        /* TODO: Move all this stuff into here since it's only used here. */
        camera_load();
        ui_load();
        bonnie_load();
        chica_load();
        foxy_load();
        freddy_load();
        golden_freddy_load();
        hallucinations_load();

        /* Exit Prompt */
        game_gfx_exit_prompt = sprite_load(TX_EXIT_PROMPT);

        night_cur = SAVE_NIGHT_NUM(save_data);
        if (night_cur <= 5)
                wav64_play(sfx_phone_calls[night_cur - 1], SFX_CH_PHONECALL);

        game_hour_cur        = 0;
        game_hour_prev       = 0;
        game_hour_timer      = 0.f;
        game_power_usage     = 1;
        game_power_left      = 999;
        game_power_timer     = 0.0f;
        game_jumpscare_flags = 0;
        game_won_by_murder   = false;
        game_is_loaded       = true;

        game_ticks_since_load = get_ticks() - game_ticks_since_load;
        game_time_since_load  = (float)game_ticks_since_load /
                (float)TICKS_PER_SECOND;

        game_pause_timer_accum = 0.f;

        game_show_exit_prompt = false;
}

static void game_unload(void)
{
        /* Exit Prompt */
        sprite_free(game_gfx_exit_prompt);

        hallucinations_unload();
        golden_freddy_unload();
        freddy_unload();
        foxy_unload();
        chica_unload();
        bonnie_unload();
        ui_unload();
        camera_unload();

        /* Door Buttons */
        graphics_unload(game_gfx_door_btns, GAME_DOOR_BUTTON_GFX_CNT);

        /* Shotgun */
        if (game_shotgun_is_unlocked)
                graphics_unload(game_gfx_shotgun_anim,
                                GAME_SHOTGUN_ANIM_FRAME_CNT);

        /* Doors */
        graphics_unload(game_gfx_anim_doors, GAME_DOOR_ANIM_FRAME_CNT);

        /* Fan */
        graphics_unload(game_gfx_fan_anim, GAME_FAN_FRAME_CNT);

        /* Office */
        graphics_unload(game_gfx_office_views, GAME_OFFICE_VIEW_CNT);
        graphics_unload(game_gfx_foxy_scare, FOXY_SCARE_FRAME_CNT);

        game_is_loaded = false;
}

static void game_office_view_draw(const float shotgun_shake_offset)
{
        int i, office_view_cur;

        rdpq_set_mode_copy(false);
        
        if (game_jumpscare_flags & JUMPSCARE_FLAG_FOXY) {
                graphic_draw(game_gfx_foxy_scare[(int)game_scare_timer_foxy],
                             game_office_turn + shotgun_shake_offset,
                             0, 0, 0, GFX_FLIP_NONE);
                return;
        }

        /* FIXME: Freddy's room jumpscare's fucking broken. */
        for (i = 0; i < FREDDY_SCARE_FRAME_CNT; ++i) {
                if ((int)game_scare_timer_freddy == i)
                        continue;
        
                graphic_unload(game_freddy_scare + i);
        }
        
        if (game_jumpscare_flags & JUMPSCARE_FLAG_FREDDY) {
                int frame = (int)game_scare_timer_freddy;

                graphic_load(game_freddy_scare + frame,
                             game_freddy_scare_paths[frame]);
                graphic_draw(game_freddy_scare[
                             (int)game_scare_timer_freddy],
                             game_office_turn +
                             shotgun_shake_offset, 0, 0,
                             0, GFX_FLIP_NONE);
                return;
        }

        /* Determine view to show for office */
        /* TODO: Indexify these. */
        office_view_cur = 0;
        if (game_door_btn_states_cur & GAME_DOOR_BTN_LEFT_LIGHT) {
                if (bonnie_cam == AT_DOOR)
                        office_view_cur = 3;
                else
                        office_view_cur = 1;
        } else if (game_door_btn_states_cur & GAME_DOOR_BTN_RIGHT_LIGHT) {
                if (chica_cam == AT_DOOR)
                        office_view_cur = 4;
                else
                        office_view_cur = 2;
        }
        
        /*
         * If we are trying to display a view for the light being on for
         * either side, but the random flicker value says nah, then it
         * does a nah.
         */
        if (game_office_flicker_rand <= 1)
                office_view_cur = 0;
        
        graphic_draw(game_gfx_office_views[office_view_cur],
                     game_office_turn + shotgun_shake_offset,
                     0, 0, 0, GFX_FLIP_NONE);
}

static void game_office_elements_draw(const float shotgun_shake_offset)
{
        /* Fan */
        rdpq_set_mode_copy(false);
        graphic_draw(game_gfx_fan_anim[(int)game_fan_anim_timer],
                     GAME_FAN_POS_X + game_office_turn + shotgun_shake_offset,
                     GAME_FAN_POS_Y, GAME_FAN_ORIG_X,
                     GAME_FAN_ORIG_Y, GFX_FLIP_NONE);

        /* Door Left */
        {
                int frame;

                rdpq_set_mode_copy(true);
                frame = (int)(game_door_anim_timers[GAME_DOOR_LEFT]);
                graphic_draw(game_gfx_anim_doors[frame],
                             GAME_DOOR_LEFT_ANIM_POS_X + game_office_turn +
                             shotgun_shake_offset, GAME_DOOR_LEFT_ANIM_POS_Y,
                             0, 0, GFX_FLIP_NONE);
        }
        
        /* Door Right */
        {
                int frame;

                rdpq_set_mode_standard();
                rdpq_mode_alphacompare(1);
                frame = (int)(game_door_anim_timers[GAME_DOOR_RIGHT]);
                graphic_draw(game_gfx_anim_doors[frame],
                             GAME_DOOR_RIGHT_ANIM_POS_X + game_office_turn +
                             shotgun_shake_offset, GAME_DOOR_RIGHT_ANIM_POS_Y,
                             0, 0, GFX_FLIP_X);
        }
        
        /* Door Buttons */
        /* TODO: Make vars more descriptive */
        int left_index, right_index, bitmask_left, bitmask_right, i;
        
        left_index = 0;
        bitmask_left  = (GAME_DOOR_BTN_LEFT_DOOR | GAME_DOOR_BTN_LEFT_LIGHT);
        for (i = 0; i < 4; ++i) {
                if (game_door_btn_states_left[i].state ==
                        (game_door_btn_states_cur & bitmask_left)) {
                        left_index = game_door_btn_states_left[i].index;
                        break;
                }
        }
        
        right_index = 0;
        bitmask_right = (GAME_DOOR_BTN_RIGHT_DOOR | GAME_DOOR_BTN_RIGHT_LIGHT);
        for (i = 0; i < 4; ++i) {
                if (game_door_btn_states_right[i].state ==
                        (game_door_btn_states_cur & bitmask_right)) {
                        right_index = game_door_btn_states_right[i].index;
                        break;
                }
        }
        
        rdpq_set_mode_copy(true);
        graphic_draw(game_gfx_door_btns[left_index], 48 + game_office_turn + shotgun_shake_offset,
                     390, 42, 127, 0);
        graphic_draw(game_gfx_door_btns[right_index], 1546 + game_office_turn + shotgun_shake_offset,
                     400, 49, 127, 0);
}

void game_draw(void)
{
        float shotgun_shake_offset;

        if (!game_is_loaded)
                game_load();

        if (golden_freddy_state == GOLDEN_FREDDY_STATE_JUMPSCARING) {
                golden_freddy_draw_scare();
                return;
        }

        if (!camera_is_visible) {
                shotgun_shake_offset =
                        sinf((GAME_SHOTGUN_RELOAD_TIMER -
                             game_shotgun_reload_timer) * M_PI *
                             GAME_SHOTGUN_SCREEN_SHAKE_SPD) *
                        game_shotgun_screen_shake_mag;
                perspective_begin();

                /* Office */
                game_office_view_draw(shotgun_shake_offset);

                /*
                 * TODO: Put this into a check statement
                 * to make it much clearer.
                 */
                golden_freddy_draw_in_room(game_office_turn +
                                           shotgun_shake_offset);

                if (!game_jumpscare_flags)
                        game_office_elements_draw(shotgun_shake_offset);
                else if (game_jumpscare_flags & JUMPSCARE_FLAG_BONNIE)
                        bonnie_draw_scare();
                else if (game_jumpscare_flags & JUMPSCARE_FLAG_CHICA)
                        chica_draw_scare();

                perspective_end();
        } else {
                camera_view_draw();
                static_draw(true);
                camera_ui_draw();
                blip_flash_draw();
        }

        /* Shotgun */
        if (game_shotgun_is_unlocked && !camera_is_visible) {
                float shotty_n11, shotty_01, shotty_offset_x,
                shotty_offset_y, shotty_theta;
                int shotty_frame_cur;
                rdpq_blitparms_t params;

                shotty_n11       = -((game_office_turn + 320.f) / 320.f);
                shotty_01        = (shotty_n11 + 1.f) * .5f;
                shotty_offset_x  = shotty_n11 * 64.f;
                shotty_offset_y  = sinf(M_PI * shotty_01) * -8.f;
                shotty_theta     = TO_RADIANS(shotty_n11 *
                                              -GAME_SHOTGUN_MAX_ABS_THETA);
                shotty_frame_cur = (int)((game_shotgun_reload_timer /
                        GAME_SHOTGUN_RELOAD_TIMER) *
                        (GAME_SHOTGUN_ANIM_FRAME_CNT - 1));
                if (shotty_frame_cur > 0) {
                        shotty_frame_cur = (GAME_SHOTGUN_ANIM_FRAME_CNT - 1) -
                                shotty_frame_cur;
                }

                params.tile      = TILE0;
                params.s0        = 0;
                params.t0        = 0;
                params.width     = 0;
                params.height    = 0;
                params.flip_x    = false;
                params.flip_y    = false;
                params.cx        = 83;
                params.cy        = 77;
                params.scale_x   = 2.f;
                params.scale_y   = 2.f;
                params.theta     = shotty_theta;
                params.filtering = true;
                params.nx        = 0;
                params.ny        = 0;

                rdpq_set_mode_standard();
                rdpq_mode_alphacompare(1);
                rdpq_mode_filter(FILTER_MEDIAN);
                rdpq_sprite_blit(game_gfx_shotgun_anim[shotty_frame_cur].spr,
                                 160 + shotty_offset_x,
                                 240 + shotty_offset_y, &params);
        }

        if (settings_flags & SET_ROBOT_CHEAT_BIT) {
                bonnie_draw_debug();
                chica_draw_debug();
                freddy_draw_debug();
        }

        if (settings_flags & SET_SUBTITLES_BIT &&
            mixer_ch_playing(SFX_CH_PHONECALL)) {
                float night_timer;

                night_timer = game_hour_cur * HOUR_LEN_SECONDS +
                              game_hour_timer;
                subtitles_draw(night_timer, SAVE_NIGHT_NUM(save_data),
                               game_time_since_load);
        }

        ui_draw();
        camera_flip_draw();
        hallucinations_draw();

        if (game_show_exit_prompt) {
                rdpq_set_mode_standard();
                rdpq_set_prim_color(RGBA32(0x0, 0x0, 0x0, 0xA0));
                rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
                rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
                rdpq_fill_rectangle(0, 0, 320, 240);

                rdpq_set_mode_standard();
                rdpq_mode_alphacompare(1);
                rdpq_sprite_blit(game_gfx_exit_prompt, 78, 70, NULL);
        }
}

static bool game_check_cheat_code(joypad_buttons_t down)
{
        const int cheat_indis[10] = { 0, 0, 1, 1, 2, 3, 2, 3, 4, 5 };
        int cheat_inputs[6] = {
                down.c_up, down.c_down, down.c_left,
                down.c_right, down.b, down.a
        };

        int i;

        for (i = 0; i < 6; ++i) {
                if (!cheat_inputs[i])
                        continue;

                if (i != cheat_indis[game_night_skip_correct]) {
                        game_night_skip_correct = 0;
                        break;
                }

                ++game_night_skip_correct;
        }

        if (game_night_skip_correct == 10)
                return true;

        return false;
}

static void game_door_buttons_update_left(const struct update_params uparms)
{
        int anim_frame;

        if (game_office_turn <= GAME_DOOR_LEFT_INTERACT_DIST) {
                if (settings_flags & SET_LIGHT_HOLD_BIT)
                        game_door_btn_states_cur &= ~(GAME_DOOR_BTN_LEFT_LIGHT);
                return;
        }

        if (bonnie_cam == YOURE_FUCKED) {
                if (uparms.pressed.b || uparms.pressed.a)
                        wav64_play(&sfx_error, SFX_CH_BLIP);
                return;
        }

        anim_frame = game_door_anim_timers[GAME_DOOR_LEFT];
        if (uparms.pressed.b && (anim_frame == 0 || anim_frame == 14)) {
                if (game_shotgun_broke_door_flags &
                        GAME_SHOTGUN_BROKE_DOOR_LEFT) {
                        wav64_play(&sfx_error, SFX_CH_BLIP);
                } else {
                        game_door_btn_states_cur ^= GAME_DOOR_BTN_LEFT_DOOR;
                        wav64_play(&sfx_door, SFX_CH_DOOR);
                }
        }

        if (settings_flags & SET_LIGHT_HOLD_BIT) {
                game_door_btn_states_cur &= ~(GAME_DOOR_BTN_LEFT_LIGHT);
                game_door_btn_states_cur |=
                        uparms.held.a << GAME_DOOR_BTN_LEFT_LIGHT_SHIFT;
                return;
        }

        if (uparms.pressed.a) {
                game_door_btn_states_cur ^= GAME_DOOR_BTN_LEFT_LIGHT;
                game_door_btn_states_cur &= ~GAME_DOOR_BTN_RIGHT_LIGHT;
        }
}

static void game_door_buttons_update_right(const struct update_params uparms)
{
        int anim_frame;

        if (game_office_turn >= GAME_DOOR_RIGHT_INTERACT_DIST) {
                if (settings_flags & SET_LIGHT_HOLD_BIT)
                        game_door_btn_states_cur &=
                                ~(GAME_DOOR_BTN_RIGHT_LIGHT);
                return;
        }

        if (chica_cam == YOURE_FUCKED) {
                if (uparms.pressed.b || uparms.pressed.a)
                        wav64_play(&sfx_error, SFX_CH_BLIP);
                return;
        }

        anim_frame = game_door_anim_timers[GAME_DOOR_RIGHT];
        if (uparms.pressed.b && (anim_frame == 0 || anim_frame == 14)) {
                if (game_shotgun_broke_door_flags &
                        GAME_SHOTGUN_BROKE_DOOR_RIGHT) {
                        wav64_play(&sfx_error, SFX_CH_BLIP);
                } else {
                        game_door_btn_states_cur ^= GAME_DOOR_BTN_RIGHT_DOOR;
                        wav64_play(&sfx_door, SFX_CH_DOOR);
                }
        }

        if (settings_flags & SET_LIGHT_HOLD_BIT) {
                game_door_btn_states_cur &= ~(GAME_DOOR_BTN_RIGHT_LIGHT);
                game_door_btn_states_cur |=
                        (uparms.held.a << GAME_DOOR_BTN_RIGHT_LIGHT_SHIFT);
                return;
        }

        if (uparms.pressed.a) {
                game_door_btn_states_cur ^= GAME_DOOR_BTN_RIGHT_LIGHT;
                game_door_btn_states_cur &= ~GAME_DOOR_BTN_LEFT_LIGHT;
        }
}

static void game_shotgun_update(const struct update_params uparms)
{
        int reload_frame_old, reload_frame_cur;
        
        reload_frame_old = (int)((game_shotgun_reload_timer /
                                  GAME_SHOTGUN_RELOAD_TIMER) *
                                 (GAME_SHOTGUN_ANIM_FRAME_CNT - 1));
        game_shotgun_reload_timer = CLAMP(game_shotgun_reload_timer - uparms.dt,
                                          0.f, GAME_SHOTGUN_RELOAD_TIMER);
        game_shotgun_screen_shake_mag = game_shotgun_reload_timer /
                                        GAME_SHOTGUN_RELOAD_TIMER;
        game_shotgun_screen_shake_mag *= game_shotgun_screen_shake_mag *
                                         game_shotgun_screen_shake_mag;
        game_shotgun_screen_shake_mag *= GAME_SHOTGUN_SCREEN_SHAKE_MAG_MAX;
        reload_frame_cur = (int)((game_shotgun_reload_timer /
                                  GAME_SHOTGUN_RELOAD_TIMER) *
                                 (GAME_SHOTGUN_ANIM_FRAME_CNT - 1));
        
        if (!uparms.pressed.z || game_shotgun_reload_timer > 0.f ||
            camera_is_visible) {
                if (reload_frame_old > GAME_SHOTGUN_FRAME_PLAY_RELOAD_SFX &&
                        reload_frame_cur <= GAME_SHOTGUN_FRAME_PLAY_RELOAD_SFX) {
                        wav64_play(&sfx_shotgun_reload, SFX_CH_SHOTTY2);
                }
                return;
        }

        wav64_play(&sfx_shotgun_blast, SFX_CH_SHOTTY1);
        game_shotgun_reload_timer = GAME_SHOTGUN_RELOAD_TIMER;
        
        if (game_office_turn >= GAME_DOOR_LEFT_INTERACT_DIST) {
                if (game_door_btn_states_cur &
                        GAME_DOOR_BTN_LEFT_DOOR) {
                        game_door_btn_states_cur &=
                                ~(GAME_DOOR_BTN_LEFT_DOOR);
                        game_shotgun_broke_door_flags |=
                                GAME_SHOTGUN_BROKE_DOOR_LEFT;
                } else if (bonnie_cam == AT_DOOR) {
                        bonnie_ai_level = 0;
                        bonnie_cam = -1;
                } else if (game_jumpscare_flags &
                        JUMPSCARE_FLAG_FOXY) {
                        game_jumpscare_flags &=
                                ~(JUMPSCARE_FLAG_FOXY);
                        foxy_ai_level = 0;
                        foxy_progress = 0;
                        foxy_run_timer = 0.f;
                        foxy_no_check_timer = 0.f;
                        foxy_use_run_timer = false;
                        mixer_ch_stop(SFX_CH_JUMPSCARE);
                }
        } else if (game_office_turn <=
                GAME_DOOR_RIGHT_INTERACT_DIST) {
                if (game_door_btn_states_cur &
                        GAME_DOOR_BTN_RIGHT_DOOR) {
                        game_door_btn_states_cur &=
                                ~(GAME_DOOR_BTN_RIGHT_DOOR);
                        game_shotgun_broke_door_flags |=
                                GAME_SHOTGUN_BROKE_DOOR_RIGHT;
                } else if (freddy_cam == CAM_4B) {
                        freddy_ai_level = 0;
                        freddy_cam = -1;
                } else if (chica_cam == AT_DOOR) {
                        chica_ai_level = 0;
                        chica_cam = -1;
                }
        }
        
        /*
         * If we just killed all the animatronics,
         * start a small grace timer before ending the night.
         */
        if (freddy_ai_level == 0 && bonnie_ai_level == 0 &&
            chica_ai_level  == 0 && foxy_ai_level   == 0) {
                game_shotgun_killed_all_timer = 3.f;
                game_won_by_murder = true;
        }
}

void game_hour_update(const struct update_params uparms)
{
        bool tick;

        game_hour_timer = wrapf(game_hour_timer + uparms.dt,
                                HOUR_LEN_SECONDS, &tick);

        if (!tick)
                return;

        game_hour_prev = game_hour_cur++;

        /* Increment AI levels as the night progresses. */
        if (game_hour_cur == 2 && game_hour_prev != 2)
                bonnie_ai_level++;

        if ((game_hour_cur == 3 && game_hour_prev != 3) ||
            (game_hour_cur == 4 && game_hour_prev != 4)) {
                bonnie_ai_level++;
                chica_ai_level++;
                foxy_ai_level++;

        }

        /* But make sure they don't go above 20. */
        bonnie_ai_level = CLAMP(bonnie_ai_level, 0, 20);
        chica_ai_level  = CLAMP(chica_ai_level,  0, 20);
        foxy_ai_level   = CLAMP(foxy_ai_level,   0, 20);

        /*
         * Technically, I could ignore Freddy, since he
         * never gets an AI increment through the night,
         * but I'm pedantic as fuck, so it stays!
         */
        freddy_ai_level = CLAMP(freddy_ai_level, 0, 20);
}

enum scene game_update(struct update_params uparms)
{
        if (game_show_exit_prompt) {
                game_pause_timer_accum += uparms.dt;

                if (uparms.pressed.start) {
                        sfx_stop_all_channels();
                        game_unload();
                        return SCENE_TITLE_SCREEN;
                }

                if (uparms.pressed.b || uparms.pressed.a) {
                        game_show_exit_prompt = false;
                }

                return SCENE_MAIN_GAME;
        }

        golden_freddy_update(uparms.dt);
        hallucinations_update(uparms.dt);

        /*
         * If you kill all the animatronics with the shotgun, clearly there's
         * nothing else to do for the night. So there's a 3 second timer
         * after killing all of them before it booting you to the next night.
         */
        if (game_shotgun_killed_all_timer) {
                game_shotgun_killed_all_timer -= uparms.dt;
                if (game_shotgun_killed_all_timer <= 0.f)
                        game_hour_cur = 6;
        }

        /*
         * You are actively being jumpscared by Golden Freddy,
         * and are powerless to do absolutely anything. Damn.
         */
        if (golden_freddy_state == GOLDEN_FREDDY_STATE_JUMPSCARING)
                return SCENE_MAIN_GAME;

        /* TODO: Add an indication on which button to press to turn off call. */
        if (!camera_is_visible && uparms.pressed.c_up &&
            SAVE_NIGHT_NUM(save_data) <= 5) {
                mixer_ch_stop(SFX_CH_PHONECALL);
        }

        if (game_check_cheat_code(uparms.pressed))
                game_hour_cur = 6;

        /* Circus music */
        {
                bool try_play_circus_music;

                game_circus_timer = wrapf(game_circus_timer + uparms.dt, 5,
                                          &try_play_circus_music);
                if (try_play_circus_music && !(rand() % 30)) {
                        mixer_ch_set_vol(SFX_CH_AMBIENCE, 0.05f, 0.05f);
                        wav64_play(&sfx_circus_music, SFX_CH_AMBIENCE);
                }
        }

        game_hour_update(uparms);
        if (game_hour_cur >= 6) {
                sfx_stop_all_channels();
                game_unload();
                return SCENE_NIGHT_END;
        }

        if (game_power_left <= 0) {
                sfx_stop_all_channels();
                game_unload();
                return SCENE_POWER_DOWN;
        }

        office_update(game_door_btn_states_cur, uparms);

        /* Shotgun */
        if (game_shotgun_is_unlocked)
                game_shotgun_update(uparms);

        /* Fan */
        game_fan_anim_timer = wrapf(game_fan_anim_timer + uparms.dt * speed_fps(99), GAME_FAN_FRAME_CNT, NULL);

        /* Doors */
        const float door_speed = speed_fps(50);
        if (game_door_btn_states_cur & GAME_DOOR_BTN_LEFT_DOOR) {
                game_door_anim_timers[GAME_DOOR_LEFT] += uparms.dt * door_speed;
        } else {
                game_door_anim_timers[GAME_DOOR_LEFT] -= uparms.dt * door_speed;
        }

        if (game_door_btn_states_cur & GAME_DOOR_BTN_RIGHT_DOOR) {
                game_door_anim_timers[GAME_DOOR_RIGHT] += uparms.dt * door_speed;
        } else {
                game_door_anim_timers[GAME_DOOR_RIGHT] -= uparms.dt * door_speed;
        }

        game_door_anim_timers[GAME_DOOR_LEFT] = CLAMP(game_door_anim_timers[GAME_DOOR_LEFT], 0, GAME_DOOR_ANIM_FRAME_CNT - 1);
        game_door_anim_timers[GAME_DOOR_RIGHT] = CLAMP(game_door_anim_timers[GAME_DOOR_RIGHT], 0, GAME_DOOR_ANIM_FRAME_CNT - 1);

        if (!camera_is_visible) {
                game_door_buttons_update_left(uparms);
                game_door_buttons_update_right(uparms);
        }

        camera_update(&game_door_btn_states_cur, uparms);
        ui_update(game_door_btn_states_cur, uparms.dt);
        bonnie_update(&game_door_btn_states_cur, uparms.dt);
        chica_update(&game_door_btn_states_cur, uparms.dt);
        foxy_update(game_door_btn_states_cur, uparms.dt);
        freddy_update(game_door_btn_states_cur, uparms.dt);

        if (game_jumpscare_flags & JUMPSCARE_FLAG_MASK) {
                game_sfx_jumpscare_exit_timer -= uparms.dt * 60;
                if (game_sfx_jumpscare_exit_timer <= 0) {
                        sfx_stop_all_channels();
                        game_unload();
                        game_sfx_jumpscare_exit_timer = 40;
                        return SCENE_GAME_OVER;
                }
        } else {
                game_sfx_jumpscare_exit_timer = 40.f;
        }

        if (uparms.pressed.start && !game_show_exit_prompt) {
                game_show_exit_prompt = true;
        }

        return SCENE_MAIN_GAME;
}
