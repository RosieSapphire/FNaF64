#pragma once

#include <libdragon.h>

typedef struct {
	float dt;
	joypad_buttons_t held, pressed;
	joypad_inputs_t sticks;
} update_parms_t;
