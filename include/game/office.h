#ifndef GAME_OFFICE_H
#define GAME_OFFICE_H

#define ROOM_TURN_MIN -640
#define ROOM_TURN_SPEED 12

#include <libdragon.h>

#include "engine/update.h"

extern float office_turn;

void office_load(void);
void office_unload(void);
void office_draw(void);
void office_update(update_parms_t uparms);

#endif /* GAME_OFFICE_H */
