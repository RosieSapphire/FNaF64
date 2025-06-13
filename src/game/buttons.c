/*
#include "engine/graphic.h"
#include "engine/sfx.h"

#include "game/office.h"
#include "game/doors.h"
#include "game/camera.h"
#include "game/game.h"
#include "game/bonnie.h"
#include "game/chica.h"
#include "game/settings.h"
#include "game/texture_index.h"
#include "game/buttons.h"

struct button_state {
	int state, index;
};

struct button_state left_states[] = {
	{ 0,                                      0 },
	{ GAME_DOOR_BTN_LEFT_DOOR,                       1 },
	{ GAME_DOOR_BTN_LEFT_LIGHT,                      2 },
	{ GAME_DOOR_BTN_LEFT_LIGHT | GAME_DOOR_BTN_LEFT_DOOR,   3 },
};

struct button_state right_states[] = {
	{ 0,                                      4 },
	{ GAME_DOOR_BTN_RIGHT_DOOR,                      5 },
	{ GAME_DOOR_BTN_RIGHT_LIGHT,                     6 },
	{ GAME_DOOR_BTN_RIGHT_LIGHT | GAME_DOOR_BTN_RIGHT_DOOR, 7 },
};

int button_state = 0;

static int button_state_get_index(struct button_state *states, bool left_door)
{
        int i, bitmask;

	bitmask = left_door ? (GAME_DOOR_BTN_LEFT_DOOR | GAME_DOOR_BTN_LEFT_LIGHT) : 
                              (GAME_DOOR_BTN_RIGHT_DOOR | GAME_DOOR_BTN_RIGHT_LIGHT);
	for (i = 0; i < 4; ++i) {
		if (states[i].state == (button_state & bitmask))
			return states[i].index;
	}

	return 0;
}

struct graphic button[8];
static const char *button_paths[8] = {
	TX_BUTTON_LEFT00,
	TX_BUTTON_LEFT10,
	TX_BUTTON_LEFT01,
	TX_BUTTON_LEFT11,
	TX_BUTTON_RIGHT00,
	TX_BUTTON_RIGHT10,
	TX_BUTTON_RIGHT01,
	TX_BUTTON_RIGHT11,
};

void buttons_load(void)
{
	button_state = 0;
	graphics_load(button, 8, button_paths);
}

void buttons_unload(void)
{
	graphics_unload(button, 8);
}

void buttons_draw(void)
{
	rdpq_set_mode_copy(true);
	graphic_draw(button[button_state_get_index(left_states, 1)],
			48 + office_turn, 390, 42, 127, 0);
	graphic_draw(button[button_state_get_index(right_states, 0)],
			1546 + office_turn, 400, 49, 127, 0);
}

static void button_left_update(struct update_params uparms)
{
	bool left_door_can_interact =
		(int)door_timers[0] == 0 || (int)door_timers[0] == 14;
	if (office_turn <= -68) {
		if (settings_flags & SET_LIGHT_HOLD_BIT)
			button_state &= ~(GAME_DOOR_BTN_LEFT_LIGHT);
		return;
	}

	if (bonnie_cam == YOURE_FUCKED) {
		if (uparms.pressed.b || uparms.pressed.a)
			wav64_play(&sfx_error, SFX_CH_BLIP);
		return;
	}

	if (uparms.pressed.b && left_door_can_interact) {
		button_state ^= GAME_DOOR_BTN_LEFT_DOOR;
		wav64_play(&sfx_door, SFX_CH_DOOR);
	}

	if (settings_flags & SET_LIGHT_HOLD_BIT) {
		button_state &= ~(GAME_DOOR_BTN_LEFT_LIGHT);
		button_state |= GAME_DOOR_BTN_LEFT_LIGHT * uparms.held.a;
		return;
	}

	if (uparms.pressed.a) {
		button_state ^= GAME_DOOR_BTN_LEFT_LIGHT;
		button_state &= ~GAME_DOOR_BTN_RIGHT_LIGHT;
	}
}

static void button_right_update(struct update_params uparms)
{
	bool right_door_can_interact =
		(int)door_timers[1] == 0 || (int)door_timers[1] == 14;
	if (office_turn >= -554) {
		if (settings_flags & SET_LIGHT_HOLD_BIT)
			button_state &= ~(GAME_DOOR_BTN_RIGHT_LIGHT);
		return;
	}

	if (chica_cam == YOURE_FUCKED) {
		if (uparms.pressed.b || uparms.pressed.a)
			wav64_play(&sfx_error, SFX_CH_BLIP);
		return;
	}

	if (uparms.pressed.b && right_door_can_interact) {
		button_state ^= GAME_DOOR_BTN_RIGHT_DOOR;
		wav64_play(&sfx_door, SFX_CH_DOOR);
	}

	if (settings_flags & SET_LIGHT_HOLD_BIT) {
		button_state &= ~(GAME_DOOR_BTN_RIGHT_LIGHT);
		button_state |= GAME_DOOR_BTN_RIGHT_LIGHT * uparms.held.a;
		return;
	}

	if (uparms.pressed.a) {
		button_state ^= GAME_DOOR_BTN_RIGHT_LIGHT;
		button_state &= ~GAME_DOOR_BTN_LEFT_LIGHT;
	}
}

void buttons_update(struct update_params uparms)
{
	if (camera_is_visible)
		return;

	button_left_update(uparms);
	button_right_update(uparms);
}
*/
