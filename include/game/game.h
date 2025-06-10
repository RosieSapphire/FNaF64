#pragma once

#define GAME_DEBUG_ENABLED

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

extern float game_night_timer;
extern int game_power_usage;
extern int game_power_left;
extern float game_power_timer;

void game_draw(void);
enum scene game_update(struct update_params uparms);
