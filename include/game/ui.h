#pragma once

#include "engine/object.h"

/* TODO: These might fit better in game.c, honestly. */
extern float ui_night_timer;
extern int ui_power_left;

void ui_load(void);
void ui_unload(void);
void ui_draw(void);
void ui_update(double dt);
