#pragma once

#include <libdragon.h>

#include "engine/update.h"

#define BUTTON_LEFT_DOOR   1
#define BUTTON_LEFT_LIGHT  2
#define BUTTON_RIGHT_DOOR  4
#define BUTTON_RIGHT_LIGHT 8

extern int button_state;

void buttons_load(void);
void buttons_unload(void);
void buttons_draw(void);
void buttons_update(update_parms_t uparms);
