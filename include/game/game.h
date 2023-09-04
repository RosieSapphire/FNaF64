#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

extern float night_timer;
extern uint8_t night_num;

#define NIGHT_5_BEATEN_BIT 1
#define NIGHT_6_BEATEN_BIT 2
#define MODE_20_BEATEN_BIT 4
extern int night_beat_flags;

void game_draw(void);
enum scene game_update(update_parms_t uparms);

#endif /* GAME_GAME_H */
