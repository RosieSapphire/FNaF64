#include <stdlib.h>

#include "engine/debug.h"
#include "engine/util.h"
#include "engine/sfx.h"

#include "game/camera.h"
#include "game/buttons.h"
#include "game/ui.h"
#include "game/texture_index.h"
#include "game/foxy.h"

int foxy_ai_level = 0;
static float move_timer;
static float stun_timer;
static float no_check_timer;
static bool use_run_timer;
int foxy_progress;
bool foxy_is_scaring;
float foxy_scare_timer;
float foxy_run_timer;
static int num_door_pounds;
object_t foxy_run[FOXY_RUN_FRAMES];

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
	foxy_is_scaring = false;
	foxy_scare_timer = 0.0f;
	foxy_run_timer = 0.0f;
	use_run_timer = false;
	num_door_pounds = 0;
	objects_load(foxy_run, FOXY_RUN_FRAMES, foxy_run_paths);
}

void foxy_unload(void)
{
	objects_unload(foxy_run, FOXY_RUN_FRAMES);
}

static void _foxy_update_stun_timer(double dt)
{
	if(camera_is_visible) {
		stun_timer = 50 + (rand() % 1000);
		return;
	}
 
	stun_timer -= dt * 60;
	stun_timer = clampf(stun_timer, 0, 1005);
}

static bool _foxy_update_move_timer(double dt)
{
	move_timer += dt;
	bool move_tick;
	move_timer = wrapf(move_timer, MOVE_INTERVAL, &move_tick);
	return move_tick;
}

static void _foxy_trigger_jumpscare(void)
{
	camera_is_using = false;
	foxy_is_scaring = true;
	wav64_play(&jumpscare_sfx, SFXC_JUMPSCARE);
}

static void _foxy_trigger_reset(void)
{
	wav64_play(&banging_sfx, SFXC_JUMPSCARE);
	foxy_progress = rand() & 1;
	no_check_timer = 0;
	foxy_run_timer = 0;
	move_timer = 0;
	use_run_timer = false;
	num_door_pounds++;

	int power_deduct = 10 + (50 * num_door_pounds);
	if(power_deduct >= power_left) {
		power_left = 0;
		return;
	}

	power_left -= power_deduct;
}

void foxy_update(double dt)
{
	/* Checking for humming */
	bool is_looking_at = camera_is_visible && cam_selected == CAM_1C;
	float foxsong_vol = 0.05f + 0.1f * is_looking_at;
	mixer_ch_set_vol(SFXC_FOXSONG, foxsong_vol, foxsong_vol);

	fox_song_timer += dt;
	bool fox_song_play;
	fox_song_timer = wrapf(fox_song_timer, 4.0f, &fox_song_play);
	if(fox_song_play && (rand() % 30) == 1)
		wav64_play(&foxy_hum, SFXC_FOXSONG);

	/* Handle jumpscaring */
	if(foxy_is_scaring) {
		foxy_scare_timer += speed_fps(25) * dt;
		foxy_scare_timer = clampf(foxy_scare_timer,
				0, FOXY_SCARE_FRAMES - 1);
		return;
	}

	if(cam_selected == CAM_2A && camera_is_visible
			&& foxy_progress == 3 && !use_run_timer) {
		use_run_timer = true;
		wav64_play(&foxy_running, SFXC_JUMPSCARE);
	}

	if(use_run_timer) {
		foxy_run_timer += dt * 60;
		foxy_run_timer = clampf(foxy_run_timer, 0, 100);
		if(foxy_run_timer == 100) {
			if(button_state & BUTTON_LEFT_DOOR)
				_foxy_trigger_reset();
			else
				_foxy_trigger_jumpscare();
		}
	}

	if(foxy_progress == 3 && !foxy_is_scaring)
		no_check_timer += dt * 60;

	if(no_check_timer >= 1500 && !foxy_is_scaring) {
		if(button_state & BUTTON_LEFT_DOOR)
			_foxy_trigger_reset();
		else
			_foxy_trigger_jumpscare();
	}

	_foxy_update_stun_timer(dt);
	if(!_foxy_update_move_timer(dt) || stun_timer > 0 || foxy_progress >= 3)
		return;

	if((1 + (rand() % 20)) > foxy_ai_level)
		return;

	foxy_progress++;
}
