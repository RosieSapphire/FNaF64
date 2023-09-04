#include "engine/object.h"
#include "engine/sfx.h"

#include "game/office.h"
#include "game/doors.h"
#include "game/camera.h"
#include "game/game.h"
#include "game/bonnie.h"
#include "game/chica.h"
#include "game/settings.h"
#include "game/buttons.h"

typedef struct {
	int state, index;
} button_state_t;

button_state_t left_states[] = {
	{0,                                      0},
	{BUTTON_LEFT_DOOR,                       1},
	{BUTTON_LEFT_LIGHT,                      2},
	{BUTTON_LEFT_LIGHT | BUTTON_LEFT_DOOR,   3},
};

button_state_t right_states[] = {
	{0,                                      4},
	{BUTTON_RIGHT_DOOR,                      5},
	{BUTTON_RIGHT_LIGHT,                     6},
	{BUTTON_RIGHT_LIGHT | BUTTON_RIGHT_DOOR, 7},
};

int button_state = 0;

static int button_state_get_index(button_state_t *states, bool left_door)
{
	int bitmask = left_door ? (BUTTON_LEFT_DOOR | BUTTON_LEFT_LIGHT) : 
		(BUTTON_RIGHT_DOOR | BUTTON_RIGHT_LIGHT);
	for(int i = 0; i < 4; i++) {
		if(states[i].state == (button_state & bitmask))
			return states[i].index;
	}

	return 0;
}

object_t button[8];
static const char *button_paths[8] = {
	"rom:/button_left_none.ci8.sprite",
	"rom:/button_left_door.ci8.sprite",
	"rom:/button_left_light.ci8.sprite",
	"rom:/button_left_door_light.ci8.sprite",
	"rom:/button_right_none.ci8.sprite",
	"rom:/button_right_door.ci8.sprite",
	"rom:/button_right_light.ci8.sprite",
	"rom:/button_right_door_light.ci8.sprite",
};

void buttons_load(void)
{
	button_state = 0;
	objects_load(button, 8, button_paths);
}

void buttons_unload(void)
{
	objects_unload(button, 8);
}

void buttons_draw(void)
{
	rdpq_set_mode_copy(true);
	object_draw(button[button_state_get_index(left_states, 1)],
			48 + office_turn, 390, 42, 127);
	object_draw(button[button_state_get_index(right_states, 0)],
			1546 + office_turn, 400, 49, 127);
}

static void button_left_update(update_parms_t uparms)
{
	bool left_door_can_interact =
		(int)door_timers[0] == 0 || (int)door_timers[0] == 14;
	if(office_turn <= -68) {
		if(settings_flags & SET_LIGHT_HOLD_BIT)
			button_state &= ~(BUTTON_LEFT_LIGHT);
		return;
	}

	if(bonnie_cam == YOURE_FUCKED) {
		if(uparms.down.c->B || uparms.down.c->A)
			wav64_play(&error_sfx, SFXC_BLIP);
		return;
	}

	if(uparms.down.c->B && left_door_can_interact) {
		button_state ^= BUTTON_LEFT_DOOR;
		wav64_play(&door_sfx, SFXC_DOOR);
	}

	if(settings_flags & SET_LIGHT_HOLD_BIT) {
		button_state &= ~(BUTTON_LEFT_LIGHT);
		button_state |= BUTTON_LEFT_LIGHT * uparms.held.c->A;
		return;
	}

	if(uparms.down.c->A) {
		button_state ^= BUTTON_LEFT_LIGHT;
		button_state &= ~BUTTON_RIGHT_LIGHT;
	}
}

static void button_right_update(update_parms_t uparms)
{
	bool right_door_can_interact =
		(int)door_timers[1] == 0 || (int)door_timers[1] == 14;
	if(office_turn >= -554) {
		if(settings_flags & SET_LIGHT_HOLD_BIT)
			button_state &= ~(BUTTON_RIGHT_LIGHT);
		return;
	}

	if(chica_cam == YOURE_FUCKED) {
		if(uparms.down.c->B || uparms.down.c->A)
			wav64_play(&error_sfx, SFXC_BLIP);
		return;
	}

	if(uparms.down.c->B && right_door_can_interact) {
		button_state ^= BUTTON_RIGHT_DOOR;
		wav64_play(&door_sfx, SFXC_DOOR);
	}

	if(settings_flags & SET_LIGHT_HOLD_BIT) {
		button_state &= ~(BUTTON_RIGHT_LIGHT);
		button_state |= BUTTON_RIGHT_LIGHT * uparms.held.c->A;
		return;
	}

	if(uparms.down.c->A) {
		button_state ^= BUTTON_RIGHT_LIGHT;
		button_state &= ~BUTTON_LEFT_LIGHT;
	}
}

void buttons_update(update_parms_t uparms)
{
	if(camera_is_visible)
		return;

	button_left_update(uparms);
	button_right_update(uparms);
}
