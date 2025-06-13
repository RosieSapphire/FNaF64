/*
 * FIXME (This goes for Chica too): When the animatronics are already inside
 * the room and you put up the camera, they should make a moaning noise,
 * and after I think about 30 seconds, they'll pull the camera down
 * automatically and you die. I almost think it's scary to lack this
 * functionality since it completely catches you by surprise if you didn't
 * already notice, but I'm going for near 100% accuracy, so this is something
 * I'll have to take care of at some point.
 */

#include <stdlib.h>

#include "engine/util.h"
#include "engine/graphic.h"
#include "engine/sfx.h"

#include "game/buttons.h"
#include "game/camera.h"
#include "game/texture_index.h"
#include "game/game.h"
#include "game/bonnie.h"

#define MOVE_TIMER 4.97f

int bonnie_ai_level = 0;
static int which_room;
static float which_room_timer;
static int which_spot;
float bonnie_blackout_timer;
bool bonnie_scared;
static float move_timer;
int bonnie_cam_last;
int bonnie_cam;
static float scare_timer;

static const float footstep_vol_lut[CAM_COUNT] = {
	0.1f, // 1A
	0.2f, // 1B
	0,    // 1C
	0.3f, // 2A
	0.4f, // 2B
	0.3f, // 3
	0,    // 4A
	0,    // 4B
	0,    // 5
	0.1f, // 6
	0,    // 7
};

static const int new_cam_lut[CAM_COUNT + 2][2] = {
	{       CAM_5,       CAM_1B}, // 1A
	{       CAM_5,       CAM_2A}, // 1B
	{          -1,           -1}, // 1C
	{       CAM_3,       CAM_2B}, // 2A
	{       CAM_3,      AT_DOOR}, // 2B
	{     AT_DOOR,       CAM_2A}, // 3
	{          -1,           -1}, // 4A
	{          -1,           -1}, // 4B
	{      CAM_1B,       CAM_2A}, // 5
	{          -1,           -1}, // 6
	{          -1,           -1}, // 7
	{YOURE_FUCKED, YOURE_FUCKED}, // At Door
	{YOURE_FUCKED, YOURE_FUCKED}, // You're Fucked
};

#define BONNIE_SCARE_FRAMES 11

struct graphic bonnie_scare[BONNIE_SCARE_FRAMES];
const char *bonnie_scare_paths[BONNIE_SCARE_FRAMES] = {
        TX_BONNIE_SCARE00, TX_BONNIE_SCARE01, TX_BONNIE_SCARE02,
        TX_BONNIE_SCARE03, TX_BONNIE_SCARE04, TX_BONNIE_SCARE05,
        TX_BONNIE_SCARE06, TX_BONNIE_SCARE07, TX_BONNIE_SCARE08,
        TX_BONNIE_SCARE09, TX_BONNIE_SCARE10,
};

void bonnie_load(void)
{
	which_room = 0;
	which_room_timer = 0.0f;
	which_spot = 0;
	bonnie_blackout_timer = 0.0f;
	bonnie_scared = 0;
	move_timer = 0.0f;
	bonnie_cam_last = 0;
	bonnie_cam = 0;
        game_jumpscare_flags &= ~(JUMPSCARE_FLAG_BONNIE);
	scare_timer = 0.0f;

	graphics_load(bonnie_scare, BONNIE_SCARE_FRAMES, bonnie_scare_paths);
}

void bonnie_unload(void)
{
	graphics_unload(bonnie_scare, BONNIE_SCARE_FRAMES);
}

void bonnie_draw_scare(void)
{
	graphic_draw(bonnie_scare[(int)scare_timer], 0, 0, 0, 0, 0);
}

void bonnie_draw_debug(void)
{
	int w = vcon(35);
	int h = w;
	int x = vcon(cam_button_pos[bonnie_cam][0] - 317);
	int y = vcon(cam_button_pos[bonnie_cam][1]);
	int x0 = x - (w >> 1);
	int y0 = y - (h >> 1);
	int x1 = x + (w >> 1);
	int y1 = y + (h >> 1);

	rdpq_set_mode_fill(RGBA32(0x3B, 0x7B, 0xA3, 0xFF));
	rdpq_fill_rectangle(x0, y0, x1, y1);
}

void bonnie_update(int *button_state_ptr, const float dt)
{
	if (game_jumpscare_flags & JUMPSCARE_FLAG_BONNIE) {
		scare_timer += dt * speed_fps(75);
		scare_timer = wrapf(scare_timer, BONNIE_SCARE_FRAMES, NULL);
		return;
	}

	bool cam_flip_down = (!camera_is_visible && camera_was_visible);
	if (bonnie_cam == YOURE_FUCKED && cam_flip_down) {
                game_jumpscare_flags |= JUMPSCARE_FLAG_BONNIE;
		wav64_play(&sfx_jumpscare, SFX_CH_JUMPSCARE);
		return;
	}

	which_room_timer += dt;
	bool switch_which;
	which_room_timer = wrapf(which_room_timer, 1, &switch_which);
	if (switch_which)
		which_room = rand() & 1;

	bonnie_blackout_timer -= dt * 60;
	bonnie_blackout_timer = CLAMP(bonnie_blackout_timer, 0, 10);

	move_timer += dt;
	bool try_move;
	move_timer = wrapf(move_timer, MOVE_TIMER, &try_move);
	if (!try_move)
		return;

	if ((1 + (rand() % 20)) > bonnie_ai_level)
		return;

	bonnie_cam_last = bonnie_cam;
	int cam_next = new_cam_lut[bonnie_cam][which_room];
	if (cam_next == YOURE_FUCKED && (*button_state_ptr & GAME_DOOR_BTN_LEFT_DOOR)) {
		cam_next = CAM_1B;
        }

	if (cam_next < AT_DOOR) {
		float foot_vol = footstep_vol_lut[cam_next];

		debugf("%f\n", foot_vol);
		mixer_ch_set_vol(SFX_CH_FOOTSTEPS, foot_vol, foot_vol);
	}

	wav64_play(&sfx_deep_step, SFX_CH_FOOTSTEPS);
	bonnie_cam = cam_next;
	which_spot = rand() & 1;

        /*
         * Black out the cameras if Bonnie moves while we're looking
         * either at where he was or has just moved to.
         */
        if ((bonnie_cam_last != bonnie_cam) &&
            ((bonnie_cam_last == cam_selected) ||
             (bonnie_cam == cam_selected))) {
        	bonnie_blackout_timer = 10;
	}

	if (bonnie_cam != AT_DOOR)
		bonnie_scared = false;

	if ((bonnie_cam == AT_DOOR && bonnie_cam_last != AT_DOOR) ||
	    (bonnie_cam != AT_DOOR && bonnie_cam_last == AT_DOOR)) {
		*button_state_ptr &= ~GAME_DOOR_BTN_LEFT_LIGHT;
        }
}
