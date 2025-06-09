#pragma once

#include <stdbool.h>

#define BONNIE_BIT 0x4

extern int bonnie_ai_level;
extern int bonnie_cam_last;
extern int bonnie_cam;
extern float bonnie_blackout_timer;
extern bool bonnie_scared;
extern bool bonnie_is_jumpscaring;

void bonnie_load(void);
void bonnie_unload(void);
void bonnie_draw_scare(void);
void bonnie_draw_debug(void);
void bonnie_update(double dt);
