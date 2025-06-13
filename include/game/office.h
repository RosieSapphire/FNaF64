#pragma once

#define OFFICE_TURN_MIN -640
#define OFFICE_TURN_SPEED 12 

#include <libdragon.h>

#include "engine/update.h"

void office_update(const int button_state, struct update_params uparms);
