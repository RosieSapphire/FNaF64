#include <stdlib.h>

#include "engine/util.h"
#include "engine/sfx.h"

#include "game/camera.h"
#include "game/buttons.h"
#include "game/texture_index.h"
#include "game/game.h"
#include "game/foxy.h"

int foxy_ai_level = 0;
static float move_timer;
static float stun_timer;
static float no_check_timer;
static bool use_run_timer;
int foxy_progress;
float foxy_run_timer;
static int num_door_pounds;
struct graphic foxy_run[FOXY_RUN_FRAMES];

static float fox_song_timer = 0.0f;

static const char *foxy_run_paths[FOXY_RUN_FRAMES] = {
	TX_CAM_2B_FOX00, TX_CAM_2B_FOX01, TX_CAM_2B_FOX02,
	TX_CAM_2B_FOX03, TX_CAM_2B_FOX04, TX_CAM_2B_FOX05,
	TX_CAM_2B_FOX06, TX_CAM_2B_FOX07, TX_CAM_2B_FOX08,
	TX_CAM_2B_FOX09, TX_CAM_2B_FOX10, TX_CAM_2B_FOX11,
	TX_CAM_2B_FOX12,
};

#define MOVE_INTERVAL 5.01f

void foxy_load(void)
{
	move_timer = 0.0f;
	foxy_progress = 0;
	stun_timer = 0.0f;
	no_check_timer = 0.0f;
        game_jumpscare_flags &= ~(JUMPSCARE_FLAG_FOXY);
	game_scare_timer_foxy = 0.0f;
	foxy_run_timer = 0.0f;
	use_run_timer = false;
	num_door_pounds = 0;
	graphics_load(foxy_run, FOXY_RUN_FRAMES, foxy_run_paths);
}

void foxy_unload(void)
{
	graphics_unload(foxy_run, FOXY_RUN_FRAMES);
}

static void _foxy_update_stun_timer(double dt)
{
	if (camera_is_visible) {
		stun_timer = 50 + (rand() % 1000);
		return;
	}
 
	stun_timer -= dt * 60;
	stun_timer = CLAMP(stun_timer, 0, 1005);
}

static bool _foxy_update_move_timer(double dt)
{
	move_timer += dt;
	bool move_tick;
	move_timer = wrapf(move_timer, MOVE_INTERVAL, &move_tick);
	return move_tick;
}

static void _foxy_trigger_sfx_jumpscare(void)
{
	camera_is_using = false;
        game_jumpscare_flags |= JUMPSCARE_FLAG_FOXY;
	wav64_play(&sfx_jumpscare, SFX_CH_JUMPSCARE);
}

static void _foxy_trigger_reset(void)
{
	wav64_play(&sfx_banging, SFX_CH_JUMPSCARE);
	foxy_progress = rand() & 1;
	no_check_timer = 0;
	foxy_run_timer = 0;
	move_timer = 0;
	use_run_timer = false;
	num_door_pounds++;

	int power_deduct = 10 + (50 * num_door_pounds);
	if (power_deduct >= game_power_left) {
		game_power_left = 0;
		return;
	}

	game_power_left -= power_deduct;
}

void foxy_update(const int button_state, const float dt)
{
	/* Checking for humming */
	bool is_looking_at = camera_is_visible && cam_selected == CAM_1C;
	float foxsong_vol = 0.05f + 0.1f * is_looking_at;
	mixer_ch_set_vol(SFX_CH_FOXSONG, foxsong_vol, foxsong_vol);

	fox_song_timer += dt;
	bool fox_song_play;
	fox_song_timer = wrapf(fox_song_timer, 4.0f, &fox_song_play);
	if (fox_song_play && (rand() % 30) == 1)
		wav64_play(&sfx_foxy_hum, SFX_CH_FOXSONG);

	/* Handle jumpscaring */
        if (game_jumpscare_flags & JUMPSCARE_FLAG_FOXY) {
		game_scare_timer_foxy += speed_fps(25) * dt;
		game_scare_timer_foxy = CLAMP(game_scare_timer_foxy,
				0, FOXY_SCARE_FRAME_CNT - 1);
		return;
	}

	if (cam_selected == CAM_2A && camera_is_visible &&
	   foxy_progress == 3 && !use_run_timer) {
		use_run_timer = true;
		wav64_play(&sfx_foxy_running, SFX_CH_JUMPSCARE);
	}

	if (use_run_timer) {
		foxy_run_timer += dt * 60;
		foxy_run_timer = CLAMP(foxy_run_timer, 0, 100);
		if (foxy_run_timer == 100) {
			if (button_state & GAME_DOOR_BTN_LEFT_DOOR) {
				_foxy_trigger_reset();
                        } else {
				_foxy_trigger_sfx_jumpscare();
                        }
		}
	}

	if (foxy_progress == 3 &&
            !(game_jumpscare_flags & JUMPSCARE_FLAG_FOXY)) {
		no_check_timer += dt * 60;
        }

        if (no_check_timer >= 1500 &&
            !(game_jumpscare_flags & JUMPSCARE_FLAG_FOXY)) {
		if (button_state & GAME_DOOR_BTN_LEFT_DOOR) {
			_foxy_trigger_reset();
                } else {
			_foxy_trigger_sfx_jumpscare();
                }
	}

	_foxy_update_stun_timer(dt);
	if (!_foxy_update_move_timer(dt) || stun_timer > 0 || foxy_progress >= 3)
		return;

	if ((1 + (rand() % 20)) > foxy_ai_level)
		return;

	foxy_progress++;
}
