#include "engine/graphic.h"
#include "engine/sfx.h"
#include "engine/util.h"

#include "game/freddy.h"
#include "game/bonnie.h"
#include "game/chica.h"
#include "game/foxy.h"
#include "game/golden_freddy.h"
#include "game/texture_index.h"
#include "game/custom_night.h"

#define CUSTOM_ROBOT_CNT 4
#define CUSTOM_HELD_TIMER_START_TICKING .35f
#define CUSTOM_TICK_TIMER_GO .06f

static struct graphic custom_face_icons;
static struct graphic custom_buttons;
static struct graphic custom_nums;
static        int     custom_ai_selected;
static        bool    custom_was_that_the_bite_of_87;
static        float   custom_held_timer;
static        float   custom_held_tick;
static        bool    custom_is_loaded;

static const int custom_face_pos[CUSTOM_ROBOT_CNT][2] = {
	{ 259, 150 }, { 502, 150 }, { 259, 405 }, { 502, 405 }
};

static void custom_night_load(void)
{
        /* Graphics */
	graphic_load(&custom_face_icons, TX_FACE_ATLAS);
	graphic_load(&custom_buttons,    TX_AI_BUTTONS);
	graphic_load(&custom_nums,       TX_AI_NUM_ATLAS);

        /* Variables */
        custom_ai_selected             = 0;
        custom_is_loaded               = false;
        custom_was_that_the_bite_of_87 = false;
        custom_held_timer              = 0.f;
        custom_held_tick               = 0.f;

	custom_is_loaded = true;
}

static void custom_night_unload(void)
{
	graphic_unload(&custom_face_icons);
	graphic_unload(&custom_buttons);
	graphic_unload(&custom_nums);

	custom_is_loaded = false;
}

void custom_night_draw(void)
{
	const int bparms[8][4] = {
		{  42, 225, 11, 0 },
		{ 205, 225, 11, 1 },
		{ 727, 225, 11, 0 },
		{ 889, 225, 11, 1 },
		{  42, 479, 11, 0 },
		{ 205, 479, 11, 1 },
		{ 727, 479, 11, 0 },
		{ 889, 479, 11, 1 },
	};

        const int ai_num_pos[CUSTOM_ROBOT_CNT][2] = {
                { 145, 231 }, { 829, 231 }, { 145, 485 }, { 829, 485 }
        };

        int i, robot_ais[CUSTOM_ROBOT_CNT] = {
                freddy_ai_level, bonnie_ai_level,
                chica_ai_level, foxy_ai_level
        };

        /* Golden Freddy jumpscaring takes precedence over everthing. */
        if (custom_was_that_the_bite_of_87) {
                golden_freddy_draw_scare();
                return;
        }
        
        if (!custom_is_loaded) {
	        custom_night_load();
        }

	rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xFF));
	rdpq_fill_rectangle(0, 0, 320, 240);

	rdpq_set_mode_standard();
	for (i = 0; i < CUSTOM_ROBOT_CNT; ++i) {
		graphic_draw_index_y(custom_face_icons, custom_face_pos[i][0],
				     custom_face_pos[i][1], 67, i,
                                     GFX_FLIP_NONE);
        }

	for (i = 0; i < CUSTOM_ROBOT_CNT << 1; ++i) {
		graphic_draw_index_x(custom_buttons, bparms[i][0],
				     bparms[i][1], bparms[i][2],
				     bparms[i][3] +
                                     (2 * (custom_ai_selected == (i >> 1))),
                                     GFX_FLIP_NONE);
        }

	rdpq_mode_alphacompare(true);

        for (i = 0; i < CUSTOM_ROBOT_CNT; ++i) {
                int x, y, val, ones, tens, j;

                x = ai_num_pos[i][0];
                y = ai_num_pos[i][1];
                val = robot_ais[i];
	        ones = val % 10;
	        tens = val / 10;
	        for (j = 0; j < 1 + (tens > 0); ++j) {
	        	graphic_draw_index_x(custom_nums, x - 36 * j, y,
                                             12, j ? tens : ones, 0);
                }
        }
}

enum scene custom_night_update(struct update_params uparms)
{
        bool press_left, press_right, press_up, press_down;
        int *ai_ptr_cur, *ai_ptrs[CUSTOM_ROBOT_CNT] = {
		&freddy_ai_level, &bonnie_ai_level,
		&chica_ai_level, &foxy_ai_level,
	};

        /* Keep updating Golden Freddy's jumpscare until the game crashes. */
        if (custom_was_that_the_bite_of_87) {
                golden_freddy_update(uparms.dt);
                return SCENE_CUSTOM_NIGHT;
        }

        press_left  = uparms.pressed.d_left  + uparms.pressed.c_left;
        press_right = uparms.pressed.d_right + uparms.pressed.c_right;
        press_up    = uparms.pressed.d_up    + uparms.pressed.c_up;
        press_down  = uparms.pressed.d_down  + uparms.pressed.c_down;

	custom_ai_selected += press_right - press_left;
	custom_ai_selected += press_down * 2 - press_up * 2;
	custom_ai_selected &= 3;

	if (uparms.held.a || uparms.held.b) {
		custom_held_timer += uparms.dt;
        } else {
		custom_held_timer = 0.f;
        }

        ai_ptr_cur = ai_ptrs[custom_ai_selected];
	if (custom_held_timer >= CUSTOM_HELD_TIMER_START_TICKING) {
		bool tick;

		custom_held_tick = wrapf(custom_held_tick + uparms.dt,
                                         CUSTOM_TICK_TIMER_GO, &tick);
		*ai_ptr_cur += (tick * uparms.held.a) - (tick * uparms.held.b);
	}

	*ai_ptr_cur =
                CLAMP(*ai_ptr_cur + uparms.pressed.a - uparms.pressed.b, 0, 20);
	
	if (uparms.pressed.start) {
		sfx_stop_all_channels();
                custom_night_unload();
		if (freddy_ai_level == 1 && bonnie_ai_level == 9 &&
		    chica_ai_level == 8 && foxy_ai_level == 7) {
                        custom_was_that_the_bite_of_87 = true;
                        wav64_play(&sfx_jumpscare_low, SFX_CH_JUMPSCARE);
                        golden_freddy_load();
                        golden_freddy_state = GOLDEN_FREDDY_STATE_JUMPSCARING;
                        return SCENE_CUSTOM_NIGHT;
                }
		return SCENE_WHICH_NIGHT;
	}

	return SCENE_CUSTOM_NIGHT;
}
