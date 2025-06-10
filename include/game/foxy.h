#pragma once

#include <stdbool.h>
#include "engine/object.h"

#define FOXY_SCARE_FRAMES 8

extern int foxy_ai_level;
extern int foxy_progress;
extern bool foxy_is_scaring;
extern float foxy_scare_timer;
extern float foxy_run_timer;

#define FOXY_RUN_FRAMES 13
extern struct object foxy_run[FOXY_RUN_FRAMES];

void foxy_load(void);
void foxy_unload(void);
void foxy_update(double dt);
