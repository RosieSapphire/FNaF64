#pragma once

#include <stdbool.h>

void blip_create(void);
void blip_draw(void);
void blip_update(const float dt);
void blip_trigger(const bool is_visual);
void blip_destroy(void);
