#pragma once

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

extern float night_timer;

void game_draw(void);
enum scene game_update(update_parms_t uparms);
