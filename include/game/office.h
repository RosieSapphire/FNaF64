#pragma once

#define OFFICE_TURN_MIN -640
#define OFFICE_TURN_SPEED 12 

#include <libdragon.h>

#include "engine/update.h"

extern float office_turn;

void office_load(void);
void office_unload(void);
void office_draw(void);
void office_update(struct update_params uparms);
