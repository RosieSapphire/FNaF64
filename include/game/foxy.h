#pragma once

#include <stdbool.h>
#include "engine/graphic.h"

#define FOXY_SCARE_FRAME_CNT 8
#define FOXY_RUN_FRAMES 13

extern int foxy_ai_level;
extern int foxy_progress;
extern float foxy_run_timer;

extern struct graphic foxy_run[FOXY_RUN_FRAMES];

void foxy_load(void);
void foxy_unload(void);
void foxy_update(const int button_state, const float dt);
