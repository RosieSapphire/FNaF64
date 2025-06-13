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
#include "game/blip.h"
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

#define GAME_DOOR_ANIM_POS_X 72
#define GAME_DOOR_ANIM_POS_Y -1
#define GAME_DOOR_BUTTON_GFX_CNT 8
#define GAME_FAN_FRAME_CNT 3

#define GAME_OFFICE_VIEW_CNT 5

struct door_btn {
	int state, index;
};

struct door_btn game_door_btn_states_left[] = {
	{ 0,                                                  0 },
	{ GAME_DOOR_BTN_LEFT_DOOR,                            1 },
	{ GAME_DOOR_BTN_LEFT_LIGHT,                           2 },
	{ GAME_DOOR_BTN_LEFT_LIGHT | GAME_DOOR_BTN_LEFT_DOOR, 3 },
};

struct door_btn game_door_btn_states_right[] = {
	{ 0,                                                    4 },
	{ GAME_DOOR_BTN_RIGHT_DOOR,                             5 },
	{ GAME_DOOR_BTN_RIGHT_LIGHT,                            6 },
	{ GAME_DOOR_BTN_RIGHT_LIGHT | GAME_DOOR_BTN_RIGHT_DOOR, 7 },
};

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
	TX_BUTTON_LEFT00,
	TX_BUTTON_LEFT10,
	TX_BUTTON_LEFT01,
	TX_BUTTON_LEFT11,
	TX_BUTTON_RIGHT00,
	TX_BUTTON_RIGHT10,
	TX_BUTTON_RIGHT01,
	TX_BUTTON_RIGHT11,
};

static bool game_is_loaded = false;

static float game_sfx_jumpscare_exit_timer = 40;
static float game_circus_timer;
static int   game_night_skip_correct;
static float game_ticks_since_load;
static float game_time_since_load;
/*
static float game_office_turn;
static float game_office_turn_lerp;
static float game_scare_timer_foxy;
static float game_scare_timer_freddy;
static float game_office_flicker_rand_timer;
static int   game_office_flicker_rand;
static float game_door_anim_timers[GAME_DOOR_CNT];
*/
float game_office_turn;
float game_office_turn_lerp;
float game_scare_timer_foxy;
float game_scare_timer_freddy;
float game_office_flicker_rand_timer;
int   game_office_flicker_rand;
float game_door_anim_timers[GAME_DOOR_CNT];

static int   game_door_btn_states_cur = 0;

float        game_night_timer;
int          game_power_usage;
int          game_power_left;
float        game_power_timer;
uint8_t      game_jumpscare_flags;

static struct graphic game_gfx_office_views[GAME_OFFICE_VIEW_CNT];
static struct graphic game_gfx_foxy_scare[FOXY_SCARE_FRAME_CNT];
static struct graphic game_freddy_scare[FREDDY_SCARE_FRAME_CNT];
static struct graphic game_gfx_door_btns[GAME_DOOR_BUTTON_GFX_CNT];

/* DOOR BUTTON START */

void buttons_load(void)
{
	game_door_btn_states_cur = 0;
	graphics_load(game_gfx_door_btns, GAME_DOOR_BUTTON_GFX_CNT,
                      game_gfx_door_btn_paths);
}

void buttons_unload(void)
{
	graphics_unload(game_gfx_door_btns, GAME_DOOR_BUTTON_GFX_CNT);
}

static void button_left_update(struct update_params uparms)
{
	bool left_door_can_interact =
		(int)game_door_anim_timers[0] == 0 || (int)game_door_anim_timers[0] == 14;
	if (game_office_turn <= -68) {
		if (settings_flags & SET_LIGHT_HOLD_BIT)
			game_door_btn_states_cur &= ~(GAME_DOOR_BTN_LEFT_LIGHT);
		return;
	}

	if (bonnie_cam == YOURE_FUCKED) {
		if (uparms.pressed.b || uparms.pressed.a)
			wav64_play(&sfx_error, SFX_CH_BLIP);
		return;
	}

	if (uparms.pressed.b && left_door_can_interact) {
		game_door_btn_states_cur ^= GAME_DOOR_BTN_LEFT_DOOR;
		wav64_play(&sfx_door, SFX_CH_DOOR);
	}

	if (settings_flags & SET_LIGHT_HOLD_BIT) {
		game_door_btn_states_cur &= ~(GAME_DOOR_BTN_LEFT_LIGHT);
		game_door_btn_states_cur |= GAME_DOOR_BTN_LEFT_LIGHT * uparms.held.a;
		return;
	}

	if (uparms.pressed.a) {
		game_door_btn_states_cur ^= GAME_DOOR_BTN_LEFT_LIGHT;
		game_door_btn_states_cur &= ~GAME_DOOR_BTN_RIGHT_LIGHT;
	}
}

static void button_right_update(struct update_params uparms)
{
	bool right_door_can_interact =
		(int)game_door_anim_timers[1] == 0 || (int)game_door_anim_timers[1] == 14;
	if (game_office_turn >= -554) {
		if (settings_flags & SET_LIGHT_HOLD_BIT)
			game_door_btn_states_cur &= ~(GAME_DOOR_BTN_RIGHT_LIGHT);
		return;
	}

	if (chica_cam == YOURE_FUCKED) {
		if (uparms.pressed.b || uparms.pressed.a)
			wav64_play(&sfx_error, SFX_CH_BLIP);
		return;
	}

	if (uparms.pressed.b && right_door_can_interact) {
		game_door_btn_states_cur ^= GAME_DOOR_BTN_RIGHT_DOOR;
		wav64_play(&sfx_door, SFX_CH_DOOR);
	}

	if (settings_flags & SET_LIGHT_HOLD_BIT) {
		game_door_btn_states_cur &= ~(GAME_DOOR_BTN_RIGHT_LIGHT);
		game_door_btn_states_cur |= GAME_DOOR_BTN_RIGHT_LIGHT * uparms.held.a;
		return;
	}

	if (uparms.pressed.a) {
		game_door_btn_states_cur ^= GAME_DOOR_BTN_RIGHT_LIGHT;
		game_door_btn_states_cur &= ~GAME_DOOR_BTN_LEFT_LIGHT;
	}
}

void buttons_update(struct update_params uparms)
{
	if (camera_is_visible)
		return;

	button_left_update(uparms);
	button_right_update(uparms);
}

/* DOOR BUTTON END */

/* FAN START */
static float game_fan_anim_timer;
static struct graphic game_gfx_fan_anim[GAME_FAN_FRAME_CNT];
static const char *game_gfx_fan_anim_paths[GAME_FAN_FRAME_CNT] = {
	TX_FAN0, TX_FAN1, TX_FAN2,
};

/* FAN END */

/* DOORS START */
struct graphic game_gfx_anim_doors[GAME_DOOR_ANIM_FRAME_CNT];
const char *game_gfx_anim_doors_paths[GAME_DOOR_ANIM_FRAME_CNT] = {
	TX_DOOR_ANIM00, TX_DOOR_ANIM01, TX_DOOR_ANIM02,
	TX_DOOR_ANIM03, TX_DOOR_ANIM04, TX_DOOR_ANIM05,
	TX_DOOR_ANIM06, TX_DOOR_ANIM07, TX_DOOR_ANIM08,
	TX_DOOR_ANIM09, TX_DOOR_ANIM10, TX_DOOR_ANIM11,
	TX_DOOR_ANIM12, TX_DOOR_ANIM13, TX_DOOR_ANIM14,
};

/* DOORS END */

static void game_load(void)
{
        int night_cur;
        wav64_t *sfx_phone_calls[5] = {
                &sfx_phone_call_1,
                &sfx_phone_call_2,
                &sfx_phone_call_3,
                &sfx_phone_call_4,
                &sfx_phone_call_5
        };

	game_ticks_since_load = get_ticks();
	game_night_skip_correct = 0;

        /* Office */
	graphics_load(game_gfx_office_views, GAME_OFFICE_VIEW_CNT,
                      game_gfx_office_view_paths);
	graphics_load(game_gfx_foxy_scare, FOXY_SCARE_FRAME_CNT,
                      game_gfx_foxy_scare_paths);
	wav64_play(&sfx_light, SFX_CH_LIGHT);

	game_office_turn = OFFICE_TURN_MIN >> 1;
	game_office_turn_lerp = game_office_turn;
	game_office_flicker_rand_timer = 0.f;
	game_office_flicker_rand = 0;

        /* Fan */
	game_fan_anim_timer = 0.f;
	graphics_load(game_gfx_fan_anim, GAME_FAN_FRAME_CNT, game_gfx_fan_anim_paths);
	mixer_ch_set_vol(SFX_CH_FAN, 0.25f, 0.25f);
	wav64_play(&sfx_fan, SFX_CH_FAN);

        /* Doors */
	graphics_load(game_gfx_anim_doors, GAME_DOOR_ANIM_FRAME_CNT, game_gfx_anim_doors_paths);
	game_door_anim_timers[0] = 0;
	game_door_anim_timers[1] = 0;

	buttons_load();
	camera_load();
	ui_load();
	bonnie_load();
	chica_load();
	foxy_load();
	freddy_load();
	golden_freddy_load();
	hallucinations_load();
	
        night_cur = SAVE_NIGHT_NUM(save_data);
        if (night_cur <= 5) {
	        wav64_play(sfx_phone_calls[night_cur - 1], SFX_CH_PHONECALL);
        }

	game_night_timer = 0.0f;
	game_power_usage = 1;
	game_power_left = 999;
	game_power_timer = 0.0f;
        game_jumpscare_flags = 0;
	game_is_loaded = true;

	game_ticks_since_load = get_ticks() - game_ticks_since_load;
	game_time_since_load = (float)game_ticks_since_load / (float)TICKS_PER_SECOND;
}

static void game_unload(void)
{
	hallucinations_unload();
	golden_freddy_unload();
	freddy_unload();
	foxy_unload();
	chica_unload();
	bonnie_unload();
	ui_unload();
	camera_unload();
	buttons_unload();

        /* Doors */
	graphics_unload(game_gfx_anim_doors, GAME_DOOR_ANIM_FRAME_CNT);

        /* Fan */
	graphics_unload(game_gfx_fan_anim, GAME_FAN_FRAME_CNT);

        /* Office */
	graphics_unload(game_gfx_office_views, GAME_OFFICE_VIEW_CNT);
	graphics_unload(game_gfx_foxy_scare, FOXY_SCARE_FRAME_CNT);

	game_is_loaded = false;
}

void game_draw(void)
{
        if (!game_is_loaded) {
	        game_load();
        }

	if (golden_freddy_state == GOLDEN_FREDDY_STATE_JUMPSCARING) {
		golden_freddy_draw_scare();
		return;
	}

	if (!camera_is_visible) {
                int i, office_view_cur;

		perspective_begin();

                /* Office */
	        rdpq_set_mode_copy(false);

	        if (game_jumpscare_flags & JUMPSCARE_FLAG_FOXY) {
	        	graphic_draw(game_gfx_foxy_scare[(int)game_scare_timer_foxy],
	        		     game_office_turn, 0, 0, 0, GFX_FLIP_NONE);
                } else {
                        /* FIXME: Freddy's room jumpscare's fucking broken. */
                        for (i = 0; i < FREDDY_SCARE_FRAME_CNT; ++i) {
                                if ((int)game_scare_timer_freddy == i) {
                                        continue;
                                }

                                graphic_unload(game_freddy_scare + i);
                        }

                        if (game_jumpscare_flags & JUMPSCARE_FLAG_FREDDY) {
                                int frame = (int)game_scare_timer_freddy;
                                graphic_load(game_freddy_scare + frame,
                                             game_freddy_scare_paths[frame]);
                                graphic_draw(game_freddy_scare[
                                             (int)game_scare_timer_freddy],
                                             game_office_turn, 0, 0,
                                             0, GFX_FLIP_NONE);
                        } else {
                                /* Determine view to show for office */
                                office_view_cur = 0;
                                if (game_door_btn_states_cur & GAME_DOOR_BTN_LEFT_LIGHT) {
                                        if (bonnie_cam == AT_DOOR) {
                                                office_view_cur = 3;
                                        } else {
                                                office_view_cur = 1;
                                        }
                                } else if (game_door_btn_states_cur & GAME_DOOR_BTN_RIGHT_LIGHT) {
                                        if (chica_cam == AT_DOOR) {
                                                office_view_cur = 4;
                                        } else {
                                                office_view_cur = 2;
                                        }
                                }

                                /*
                                 * If we are trying to display a view for the
                                 * light being on for either side, but the
                                 * random flicker value says nah, then it
                                 * does a nah.
                                 */
                                if (game_office_flicker_rand <= 1) {
                                        office_view_cur = 0;
                                }

                                graphic_draw(game_gfx_office_views[office_view_cur],
                                             game_office_turn, 0, 0, 0,
                                             GFX_FLIP_NONE);
                        }
                }

                /*
                 * TODO: Put this into a check statement
                 * to make it much clearer.
                 */
		golden_freddy_draw_in_room(game_office_turn);

                if (!game_jumpscare_flags) {
                        int door_frame_left, door_frame_right;

                        /* Fan */
	                rdpq_set_mode_copy(false);
	                graphic_draw(game_gfx_fan_anim[(int)game_fan_anim_timer],
                                     868 + game_office_turn, 400, 88, 97, 0);

                        /* Door Left */
	                rdpq_set_mode_copy(true);
                        door_frame_left =
                                (int)(game_door_anim_timers[GAME_DOOR_LEFT]);
	                graphic_draw(game_gfx_anim_doors[door_frame_left],
                                     GAME_DOOR_ANIM_POS_X + game_office_turn,
                                     GAME_DOOR_ANIM_POS_Y,
                                     0, 0, GFX_FLIP_NONE);

                        /* Door Right */
	                rdpq_set_mode_copy(true);
	                rdpq_set_mode_standard();
	                rdpq_mode_alphacompare(true);
                        door_frame_right =
                                (int)(game_door_anim_timers[GAME_DOOR_RIGHT]);
	                graphic_draw(game_gfx_anim_doors[door_frame_right],
	                	     1270 + game_office_turn, -2,
                                     0, 0, GFX_FLIP_X);

                        /* Door Buttons */
                        /* TODO: Make vars more descriptive */
                        int left_index, right_index, bitmask_left, bitmask_right;

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
	                graphic_draw(game_gfx_door_btns[left_index], 48 + game_office_turn,
                                     390, 42, 127, 0);
	                graphic_draw(game_gfx_door_btns[right_index], 1546 + game_office_turn,
                                     400, 49, 127, 0);
                }

		if (game_jumpscare_flags & JUMPSCARE_FLAG_BONNIE) {
			bonnie_draw_scare();
                }

		if (game_jumpscare_flags & JUMPSCARE_FLAG_CHICA) {
			chica_draw_scare();
                }

		perspective_end();
	} else {
		camera_view_draw();
		static_draw(true);
		camera_ui_draw();
		blip_draw();
	}

	if (settings_flags & SET_ROBOT_CHEAT_BIT) {
		bonnie_draw_debug();
		chica_draw_debug();
		freddy_draw_debug();
	}

	if (settings_flags & SET_SUBTITLES_BIT &&
	    mixer_ch_playing(SFX_CH_PHONECALL)) {
		subtitles_draw(game_night_timer, SAVE_NIGHT_NUM(save_data),
                               game_time_since_load);
	}

	ui_draw();
	camera_flip_draw();
	hallucinations_draw();
}

static void _game_handle_cheat_code(joypad_buttons_t down)
{
        int i, cheat_inputs[6], cheat_indis[10];

        cheat_inputs[0] = down.c_up;
        cheat_inputs[1] = down.c_down;
        cheat_inputs[2] = down.c_left;
        cheat_inputs[3] = down.c_right;
        cheat_inputs[4] = down.b;
        cheat_inputs[5] = down.a;

        cheat_indis[0] = 0;
        cheat_indis[1] = 0;
        cheat_indis[2] = 1;
        cheat_indis[3] = 1;
        cheat_indis[4] = 2;
        cheat_indis[5] = 3;
        cheat_indis[6] = 2;
        cheat_indis[7] = 3;
        cheat_indis[8] = 4;
        cheat_indis[9] = 5;

	for (i = 0; i < 6; ++i) {
		if (!cheat_inputs[i]) {
			continue;
                }

		if (i != cheat_indis[game_night_skip_correct]) {
			game_night_skip_correct = 0;
			break;
		}

		++game_night_skip_correct;
	}

	if (game_night_skip_correct == 10) {
		game_night_timer = 6 * HOUR_LEN_SECONDS;
        }
}

static void _game_update_random_events(float dt)
{
	/* Circus music */
	bool play_circus_music;
	game_circus_timer = wrapf(game_circus_timer + dt, 5, &play_circus_music);
	if (play_circus_music && (rand() % 30) == 1) {
		mixer_ch_set_vol(SFX_CH_AMBIENCE, 0.05f, 0.05f);
		wav64_play(&sfx_circus_music, SFX_CH_AMBIENCE);
	}
}

enum scene game_update(struct update_params uparms)
{
	golden_freddy_update(uparms.dt);
	hallucinations_update(uparms.dt);

        /*
         * You are actively being jumpscared by
         * Golden Freddy, and are powerless to do anything.
         */
	if (golden_freddy_state == GOLDEN_FREDDY_STATE_JUMPSCARING) {
		return SCENE_MAIN_GAME;
	}

	if (!camera_is_visible && uparms.pressed.c_up &&
           SAVE_NIGHT_NUM(save_data) <= 5) {
		mixer_ch_stop(SFX_CH_PHONECALL);
        }

	_game_handle_cheat_code(uparms.pressed);
	_game_update_random_events(uparms.dt);

	game_night_timer += uparms.dt;
	if (game_night_timer >= 6 * HOUR_LEN_SECONDS) {
		sfx_stop_all_channels();
                game_unload();
		return SCENE_NIGHT_END;
	}

	static int hour_last = 0;
	int hour = (game_night_timer / (float)HOUR_LEN_SECONDS);
	if (hour == 2 && hour_last != 2) {
		bonnie_ai_level++;
	}

	if ((hour == 3 && hour_last != 3) || (hour == 4 && hour_last != 4)) {
		bonnie_ai_level++;
		chica_ai_level++;
		foxy_ai_level++;
	}

	bonnie_ai_level = CLAMP(bonnie_ai_level, 0, 20);
	chica_ai_level = CLAMP(chica_ai_level, 0, 20);
	foxy_ai_level = CLAMP(foxy_ai_level, 0, 20);
	freddy_ai_level = CLAMP(freddy_ai_level, 0, 20);

	hour_last = hour;

	if (game_power_left <= 0) {
		sfx_stop_all_channels();
                game_unload();
		return SCENE_POWER_DOWN;
	}

	office_update(game_door_btn_states_cur, uparms);

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

	buttons_update(uparms);
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
	}
	
	if (uparms.pressed.start) {
		sfx_stop_all_channels();
                game_unload();
		return SCENE_TITLE_SCREEN;
	}

#ifdef GAME_DEBUG_ENABLED
        /*
	if (game_jumpscare_flags & JUMPSCARE_FLAG_BONNIE) {
                debugf("BONNIE JUMPSCARE!\n");
        }

	if (game_jumpscare_flags & JUMPSCARE_FLAG_CHICA) {
                debugf("CHICA JUMPSCARE!\n");
        }

	if (game_jumpscare_flags & JUMPSCARE_FLAG_FREDDY) {
                debugf("FREDDY JUMPSCARE!\n");
        }

	if (game_jumpscare_flags & JUMPSCARE_FLAG_FOXY) {
                debugf("FOXY JUMPSCARE!\n");
        }
        debugf("B:%d C:%d Fo:%d Fr:%d (0x%X), %x, %x, %x, %x\n",
               (game_jumpscare_flags & JUMPSCARE_FLAG_BONNIE) <<
                        JUMPSCARE_FLAG_BONNIE_SHIFT,
               (game_jumpscare_flags & JUMPSCARE_FLAG_CHICA) <<
                        JUMPSCARE_FLAG_CHICA_SHIFT,
               (game_jumpscare_flags & JUMPSCARE_FLAG_FOXY) <<
                        JUMPSCARE_FLAG_FOXY_SHIFT,
               (game_jumpscare_flags & JUMPSCARE_FLAG_FREDDY) <<
                        JUMPSCARE_FLAG_FREDDY_SHIFT,
               game_jumpscare_flags,
               JUMPSCARE_FLAG_BONNIE,
               JUMPSCARE_FLAG_CHICA,
               JUMPSCARE_FLAG_FOXY,
               JUMPSCARE_FLAG_FREDDY
               );
        */
#endif

	return SCENE_MAIN_GAME;
}
