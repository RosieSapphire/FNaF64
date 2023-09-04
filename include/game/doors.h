#ifndef GAME_DOORS_H
#define GAME_DOORS_H

extern float door_timers[2];

void doors_load(void);
void doors_unload(void);
void doors_draw(void);
void doors_update(double dt);

#endif /* GAME_DOORS_H */
