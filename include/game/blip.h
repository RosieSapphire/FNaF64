#ifndef GAME_BLIP_H
#define GAME_BLIP_H

#include <stdbool.h>

void blip_load(void);
void blip_draw(void);
void blip_update(double dt);
void blip_trigger(bool is_visual);

#endif /* GAME_BLIP_H */
