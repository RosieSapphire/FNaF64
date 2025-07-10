#pragma once

#include <stdbool.h>

#define GOLDEN_FREDDY_TIMER_START_SCARING 5.f

extern bool golden_freddy_is_scaring;

void golden_freddy_scare_load(void);
void golden_freddy_scare_draw(void);
void golden_freddy_scare_update(const float dt);
void golden_freddy_scare_unload(void);
