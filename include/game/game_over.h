#pragma once

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

void game_over_draw(void);
enum scene game_over_update(struct update_params uparms);
