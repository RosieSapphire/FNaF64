#ifndef GAME_TITLE_H
#define GAME_TITLE_H

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

void title_draw(void);
enum scene title_update(update_parms_t uparms);

#endif /* GAME_TITLE_H */
