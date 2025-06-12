#pragma once

// #define TITLE_DEBUG_ENABLED

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

void title_draw(void);
enum scene title_update(const struct update_params uparms);
