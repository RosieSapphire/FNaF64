#pragma once

#include <libdragon.h>

struct update_params {
	float dt;
	joypad_buttons_t held, pressed;
	joypad_inputs_t sticks;
};
