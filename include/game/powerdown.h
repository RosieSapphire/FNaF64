#ifndef GAME_POWER_DOWN_H
#define GAME_POWER_DOWN_H

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

void power_down_draw(void);
enum scene power_down_update(update_parms_t uparms);

#endif /* GAME_POWER_DOWN_H */
