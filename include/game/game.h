#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

extern float night_timer;
extern bool eeprom_failed;

void game_draw(void);
enum scene game_update(update_parms_t uparms);

#endif /* GAME_GAME_H */
