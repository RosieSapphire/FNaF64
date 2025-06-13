#pragma once

#include <stdbool.h>

#define FREDDY_SCARE_FRAME_CNT 14
#define FREDDY_BIT 1

extern int freddy_ai_level;
extern int freddy_cam_last;
extern int freddy_cam;

void freddy_load(void);
void freddy_unload(void);
void freddy_draw_debug(void);
void freddy_update(const int button_state, const float dt);
