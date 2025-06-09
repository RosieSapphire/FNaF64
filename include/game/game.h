#pragma once

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

extern float game_night_timer;
extern int game_power_usage;
extern int game_power_left;
extern float game_power_timer;

void game_draw(void);
enum scene game_update(update_parms_t uparms);
