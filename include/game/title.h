#pragma once

// #define TITLE_DEBUG_ENABLED

#include <libdragon.h>

#include "engine/update.h"

#include "game/scene.h"

void title_draw(void);
enum scene title_update(const struct update_params uparms);
