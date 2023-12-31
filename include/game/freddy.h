#ifndef GAME_FREDDY_H
#define GAME_FREDDY_H

#include <stdbool.h>

#define FREDDY_SCARE_FRAMES 14
#define FREDDY_BIT 1

extern int freddy_ai_level;
extern int freddy_cam_last;
extern int freddy_cam;
extern bool freddy_is_jumpscaring;
extern float freddy_scare_timer;

void freddy_load(void);
void freddy_unload(void);
void freddy_draw_debug(void);
void freddy_update(double dt);

#endif /* GAME_FREDDY_H */
