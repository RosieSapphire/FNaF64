#pragma once

#include <libdragon.h>

#include "engine/update.h"

enum {
        BUTTON_LEFT_DOOR   = (1 << 0),
        BUTTON_LEFT_LIGHT  = (1 << 1),
        BUTTON_RIGHT_DOOR  = (1 << 2),
        BUTTON_RIGHT_LIGHT = (1 << 3)
};

extern int button_state;

void buttons_load(void);
void buttons_unload(void);
void buttons_draw(void);
void buttons_update(update_parms_t uparms);
