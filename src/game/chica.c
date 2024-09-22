#include <stdlib.h>

#include "engine/object.h"
#include "engine/sfx.h"
#include "engine/util.h"

#include "game/camera.h"
#include "game/buttons.h"
#include "game/texture_index.h"
#include "game/chica.h"

#define MOVE_TIMER 4.98f

int chica_ai_level = 0;
static int which_room;
static float which_room_timer;
static int which_spot;
float chica_blackout_timer;
bool chica_scared;
static float move_timer;
int chica_cam_last;
int chica_cam;
bool chica_is_jumpscaring;
static float scare_timer;
static float kitchen_noise_timer = 0.0f;

static const float footstep_vol_lut[CAM_COUNT] = {
	0.1f, // 1A
	0.1f, // 1B
	0, // 1C
	0, // 2A
	0, // 2B
	0, // 3
	0.3f, // 4A
	0.3f, // 4B
	0, // 5
	0.15f, // 6
	0.2f, // 7
};

static const int new_cam_lut[CAM_COUNT + 2][2] = {
	{ CAM_1B, CAM_1B }, // 1A
	{ CAM_7, CAM_6 }, // 1B
	{ -1, -1 }, // 1C
	{ -1, -1 }, // 2A
	{ -1, -1 }, // 2B
	{ -1, -1 }, // 3
	{ CAM_1B, CAM_4B }, // 4A
	{ CAM_4A, AT_DOOR }, // 4B
	{ -1, -1 }, // 5
	{ CAM_7, CAM_4A }, // 6
	{ CAM_6, CAM_4A }, // 7
	{ YOURE_FUCKED, YOURE_FUCKED }, // At Door
	{ YOURE_FUCKED, YOURE_FUCKED }, // You're Fucked
};

#define CHICA_SCARE_FRAMES 8

object_t chica_scare[CHICA_SCARE_FRAMES];
const char *chica_scare_paths[CHICA_SCARE_FRAMES] = {
	TX_CHICA_SCARE0, TX_CHICA_SCARE1, TX_CHICA_SCARE2, TX_CHICA_SCARE3,
	TX_CHICA_SCARE4, TX_CHICA_SCARE5, TX_CHICA_SCARE6, TX_CHICA_SCARE7,
};

void chica_load(void)
{
	which_room = 0;
	which_room_timer = 0.0f;
	which_spot = 0;
	chica_blackout_timer = 0.0f;
	chica_scared = 0;
	move_timer = 0.0f;
	chica_cam_last = CAM_1A;
	chica_cam = CAM_1A;
	chica_is_jumpscaring = false;
	scare_timer = 0.0f;

	objects_load(chica_scare, CHICA_SCARE_FRAMES, chica_scare_paths);
}

void chica_unload(void)
{
	objects_unload(chica_scare, CHICA_SCARE_FRAMES);
}

void chica_draw_scare(void)
{
	object_draw(chica_scare[(int)scare_timer], 0, 0, 0, 0);
}

void chica_draw_debug(void)
{
	int w = vcon(35);
	int h = w;
	int x = vcon(cam_button_pos[chica_cam][0] - 317);
	int y = vcon(cam_button_pos[chica_cam][1]);
	int x0 = x;
	int y0 = y;
	int x1 = x + w;
	int y1 = y + h;

	rdpq_set_mode_fill(RGBA32(0xDF, 0xCB, 0x00, 0xFF));
	rdpq_fill_rectangle(x0, y0, x1, y1);
}

static void _chica_update_kitchen_volume(void)
{
	if (CAM_6 != chica_cam) {
		mixer_ch_set_vol(SFXC_KITCHEN, 0, 0);
		return;
	}

	if (!camera_is_visible) {
		mixer_ch_set_vol(SFXC_KITCHEN, 0.1f, 0.1f);
		return;
	}

	if (cam_selected == chica_cam)
		mixer_ch_set_vol(SFXC_KITCHEN, 0.75f, 0.75f);
	else
		mixer_ch_set_vol(SFXC_KITCHEN, 0.2f, 0.2f);
}

void chica_update(double dt)
{
	if (chica_is_jumpscaring) {
		scare_timer += dt * speed_fps(50);
		scare_timer = wrapf(scare_timer, CHICA_SCARE_FRAMES, NULL);
		return;
	}

	bool cam_flip_down = (!camera_is_visible && camera_was_visible);
	if (chica_cam == YOURE_FUCKED && cam_flip_down) {
		chica_is_jumpscaring = true;
		wav64_play(&jumpscare_sfx, SFXC_JUMPSCARE);
		return;
	}

	_chica_update_kitchen_volume();

	kitchen_noise_timer += dt;
	bool kitchen_noise_tick;
	kitchen_noise_timer =
		wrapf(kitchen_noise_timer, 4, &kitchen_noise_tick);

	if (kitchen_noise_tick) {
		int kitchen_rand_val = rand() % 10;
		/* I'm sorry I have to do this, but for some reason, the way
		 * that Libdragon handles wav64_t, I have no choice */
		switch (kitchen_rand_val) {
		case 0:
			wav64_play(&kitchen_sfx0, SFXC_KITCHEN);
			break;

		case 1:
			wav64_play(&kitchen_sfx1, SFXC_KITCHEN);
			break;

		case 2:
			wav64_play(&kitchen_sfx2, SFXC_KITCHEN);
			break;

		case 3:
			wav64_play(&kitchen_sfx2, SFXC_KITCHEN);
			break;

		case 4:
			wav64_play(&kitchen_sfx3, SFXC_KITCHEN);
			break;
		}
	}

	which_room_timer += dt;
	bool switch_which;
	which_room_timer = wrapf(which_room_timer, 1, &switch_which);
	if (switch_which)
		which_room = rand() & 1;

	chica_blackout_timer -= dt * 60;
	chica_blackout_timer = clampf(chica_blackout_timer, 0, 10);

	move_timer += dt;
	bool try_move;
	move_timer = wrapf(move_timer, MOVE_TIMER, &try_move);
	if (!try_move)
		return;

	if ((1 + (rand() % 20)) > chica_ai_level)
		return;

	chica_cam_last = chica_cam;
	int cam_next = new_cam_lut[chica_cam][which_room];
	if (cam_next == YOURE_FUCKED && (button_state & BUTTON_RIGHT_DOOR)) {
		cam_next = CAM_4A;
		/* BULLSHIT FIX THIS SHOULDN'T HAVE TO BE HERE! */
		camera_states[CAM_4A] |= (CHICA_BIT | ROOM_SPOT_BIT);
		if (cam_selected == CAM_4A)
			chica_blackout_timer = 10;
	}

	if (cam_next < AT_DOOR) {
		float foot_vol = footstep_vol_lut[cam_next];
		mixer_ch_set_vol(SFXC_FOOTSTEPS, foot_vol, foot_vol);
	}

	wav64_play(&deepstep_sfx, SFXC_FOOTSTEPS);
	chica_cam = cam_next;
	// chica_cam = CAM_4B;
	which_spot = rand() & 1;

	/* I have no fucking clue why I have to do this */
	if (chica_cam_last < AT_DOOR) {
		camera_states[chica_cam_last] &= ~(CHICA_BIT | ROOM_SPOT_BIT);
		if (chica_cam < AT_DOOR)
			camera_states[chica_cam] |=
				CHICA_BIT | (ROOM_SPOT_BIT * which_spot);
		chica_blackout_timer = 10;
	}

	if (chica_cam != AT_DOOR)
		chica_scared = false;

	if ((chica_cam == AT_DOOR && chica_cam_last != AT_DOOR) ||
	    (chica_cam != AT_DOOR && chica_cam_last == AT_DOOR))
		button_state &= ~BUTTON_RIGHT_LIGHT;
}
