#include <stdlib.h>

#include "engine/graphic.h"
#include "engine/util.h"
#include "engine/sfx.h"

#include "game/game.h"
#include "game/buttons.h"
#include "game/camera.h"
#include "game/bonnie.h"
#include "game/chica.h"
#include "game/foxy.h"
#include "game/freddy.h"
#include "game/settings.h"
#include "game/texture_index.h"
#include "game/office.h"

void office_update(const int button_state, struct update_params uparms)
{
        if (game_jumpscare_flags & JUMPSCARE_FLAG_MASK) {
		camera_is_using = false;
        }

	if (!camera_is_visible) {
		if (settings_flags & SET_SMOOTH_TURN_BIT) {
	                const int stick_clamped = icutoff(uparms.sticks.stick_x, 10);
	                const float turn_amount = stick_clamped * OFFICE_TURN_SPEED;
	                game_office_turn_lerp -= turn_amount * uparms.dt;
	                game_office_turn_lerp = CLAMP(game_office_turn_lerp, OFFICE_TURN_MIN, 0);

	                if (fabsf(game_office_turn_lerp - game_office_turn) >= 0.001f) {
	                        game_office_turn = lerpf(game_office_turn, game_office_turn_lerp, uparms.dt * 16);
	                }

	                game_office_turn = game_office_turn_lerp;
                } else {
	                const int stick_clamped = icutoff(uparms.sticks.stick_x, 10);
	                const float turn_amount = stick_clamped * OFFICE_TURN_SPEED;
	                game_office_turn -= turn_amount * uparms.dt;
	                game_office_turn = CLAMP(game_office_turn, OFFICE_TURN_MIN, 0);
                }
	}

	if (button_state & (GAME_DOOR_BTN_LEFT_LIGHT | GAME_DOOR_BTN_RIGHT_LIGHT) &&
			game_office_flicker_rand > 1)
		mixer_ch_set_vol(SFX_CH_LIGHT, 1, 1);
	else
		mixer_ch_set_vol(SFX_CH_LIGHT, 0, 0);

	static bool bonnie_scared_last = false;
	if ((button_state & GAME_DOOR_BTN_LEFT_LIGHT) && bonnie_cam == AT_DOOR) {
		bonnie_scared = true;
        }
	if (bonnie_scared && !bonnie_scared_last) {
		mixer_ch_set_vol(SFX_CH_AMBIENCE, 0.8f, 0.8f);
		wav64_play(&sfx_window_scare, SFX_CH_AMBIENCE);

	}
	bonnie_scared_last = bonnie_scared;

	static bool chica_scared_last = false;
	if ((button_state & GAME_DOOR_BTN_RIGHT_LIGHT) && chica_cam == AT_DOOR)
		chica_scared = true;
	if (chica_scared && !chica_scared_last)
		wav64_play(&sfx_window_scare, SFX_CH_AMBIENCE);
	chica_scared_last = chica_scared;

	bool do_rand;
	game_office_flicker_rand_timer =
		wrapf(game_office_flicker_rand_timer + uparms.dt * 60, 1.0 / 60, &do_rand);
	if (do_rand) {
		game_office_flicker_rand = rand() % 10;
	}

	if (camera_is_visible)
		return;

	if (fabsf(game_office_turn + 193) < 32 &&
			(uparms.pressed.a || uparms.pressed.b))
		wav64_play(&sfx_boop, SFX_CH_BLIP);
}
