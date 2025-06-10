#pragma once

#define TITLE_DEBUG_ENABLED

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

void title_draw(void);
enum scene title_update(struct update_params uparms);
