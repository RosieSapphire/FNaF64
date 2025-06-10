#pragma once

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

void power_down_draw(void);
enum scene power_down_update(struct update_params uparms);
