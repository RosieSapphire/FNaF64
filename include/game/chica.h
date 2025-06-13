#pragma once

#include <stdbool.h>

#define CHICA_BIT 0x2

extern int chica_ai_level;
extern int chica_cam_last;
extern int chica_cam;
extern float chica_blackout_timer;
extern bool chica_scared;

void chica_load(void);
void chica_unload(void);
void chica_draw_scare(void);
void chica_draw_debug(void);
void chica_update(int *button_state_ptr, const float dt);
