#ifndef GAME_GOLDEN_FREDDY_H
#define GAME_GOLDEN_FREDDY_H

extern int golden_freddy_progress;

void golden_freddy_load(void);
void golden_freddy_draw_in_room(void);
void golden_freddy_draw_scare(void);
void golden_freddy_update(double dt);
void golden_freddy_unload(void);

#endif /* GAME_GOLDEN_FREDDY_H */
