#ifndef _GAME_FREDDY_H_
#define _GAME_FREDDY_H_

#define FREDDY_SCARE_FRAMES 14
#define FREDDY_BIT 1

extern int freddy_ai_level;
extern int freddy_cam_last;
extern int freddy_cam;
extern bool freddy_is_jumpscaring;
extern float freddy_scare_timer;

void freddy_init(void);
void freddy_draw_debug(void);
void freddy_update(double dt);

#endif /* _GAME_FREDDY_H_ */
