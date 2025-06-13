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

static struct graphic face_icons;
static struct graphic buttons;
static struct graphic nums;
static int            ai_selected             = 0;
static bool           is_loaded               = false;
static bool           was_that_the_bite_of_87 = false;

static const int face_pos[4][2] = {
	{259, 150},
	{502, 150},
	{259, 405},
	{502, 405},
};

static void _custom_night_load(void)
{
	if (is_loaded)
		return;

	ai_selected = 0;
	graphic_load(&face_icons, TX_FACE_ATLAS);
	graphic_load(&buttons, TX_AI_BUTTONS);
	graphic_load(&nums, TX_AI_NUM_ATLAS);

	is_loaded = true;
}

static void _custom_night_unload(void)
{
	if (!is_loaded)
		return;

	graphic_unload(&face_icons);
	graphic_unload(&buttons);
	graphic_unload(&nums);

	is_loaded = false;
}

static void _custom_night_ai_num_draw(int val, int x, int y)
{
        int ones, tens, i;

	ones = val % 10;
	tens = val / 10;
	for (i = 0; i < 1 + (tens > 0); ++i) {
		graphic_draw_index_x(nums, x - 36 * i, y,
                                     12, i ? tens : ones, 0);
        }
}

void custom_night_draw(void)
{
        int i;
        
	_custom_night_load();

        if (was_that_the_bite_of_87) {
                golden_freddy_draw_scare();
                return;
        }

	rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xFF));
	rdpq_fill_rectangle(0, 0, 320, 240);

	rdpq_set_mode_standard();
	for (i = 0; i < 4; ++i) {
		graphic_draw_index_y(face_icons, face_pos[i][0],
				     face_pos[i][1], 67, i, GFX_FLIP_NONE);
        }

	int bparms[8][4] = {
		{  42, 225, 11, 0 },
		{ 205, 225, 11, 1 },
		{ 727, 225, 11, 0 },
		{ 889, 225, 11, 1 },
		{  42, 479, 11, 0 },
		{ 205, 479, 11, 1 },
		{ 727, 479, 11, 0 },
		{ 889, 479, 11, 1 },
	};

	for (i = 0; i < 8; ++i)
		graphic_draw_index_x(buttons, bparms[i][0],
				     bparms[i][1], bparms[i][2],
				     bparms[i][3] +
                                     (2 * (ai_selected == (i >> 1))),
                                     GFX_FLIP_NONE);

	rdpq_mode_alphacompare(true);
	_custom_night_ai_num_draw(freddy_ai_level, 145, 225 + 6);
	_custom_night_ai_num_draw(bonnie_ai_level, 829, 225 + 6);
	_custom_night_ai_num_draw(chica_ai_level, 145, 479 + 6);
	_custom_night_ai_num_draw(foxy_ai_level, 829, 479 + 6);
}

enum scene custom_night_update(struct update_params uparms)
{
        bool press_left, press_right, press_up, press_down;

        if (was_that_the_bite_of_87) {
                golden_freddy_update(uparms.dt);
                if (golden_freddy_progress == 6) {
                        /* Crash the fucking game. */
                        uint8_t *crash;

                        crash = NULL;
                        *crash = 69;
                }
                return SCENE_CUSTOM_NIGHT;
        }

        press_left = uparms.pressed.d_left + uparms.pressed.c_left;
        press_right = uparms.pressed.d_right + uparms.pressed.c_right;
        press_up = uparms.pressed.d_up + uparms.pressed.c_up;
        press_down = uparms.pressed.d_down + uparms.pressed.c_down;

	ai_selected += press_right - press_left;
	ai_selected += press_down * 2 - press_up * 2;
	ai_selected &= 3;

	int *ais[4] = {
		&freddy_ai_level,
		&bonnie_ai_level,
		&chica_ai_level,
		&foxy_ai_level,
	};

	static float held_timer = 0.0f;
	static float held_tick = 0.0f;

	if (uparms.held.a || uparms.held.b)
		held_timer += uparms.dt;
	else
		held_timer = 0;

	if (held_timer >= 0.35f) {
		bool held_do;
		held_tick = wrapf(held_tick + uparms.dt, 0.06f, &held_do);
		**(ais + ai_selected) += held_do * uparms.held.a;
		**(ais + ai_selected) -= held_do * uparms.held.b;
	}

	**(ais + ai_selected) += uparms.pressed.a;
	**(ais + ai_selected) -= uparms.pressed.b;
	**(ais + ai_selected) = CLAMP(**(ais + ai_selected), 0, 20);
	
	if (uparms.pressed.start) {
		if (freddy_ai_level == 1 && bonnie_ai_level == 9 &&
		    chica_ai_level == 8 && foxy_ai_level == 7) {
                        was_that_the_bite_of_87 = true;
	                mixer_ch_set_vol(SFX_CH_JUMPSCARE, .8f, .8f);
                        wav64_play(&sfx_jumpscare_low, SFX_CH_JUMPSCARE);
                        golden_freddy_load();
                        golden_freddy_progress = 4;
                        return SCENE_CUSTOM_NIGHT;
                }
		sfx_stop_all_channels();
		rdpq_call_deferred((void (*)(void *))_custom_night_unload,
				NULL);
		return SCENE_WHICH_NIGHT;
	}

	return SCENE_CUSTOM_NIGHT;
}
