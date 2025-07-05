#pragma once

#include <libdragon.h>

#include "engine/update.h"

#include "game/scene.h"

void game_over_draw(void);
enum scene game_over_update(struct update_params uparms);
