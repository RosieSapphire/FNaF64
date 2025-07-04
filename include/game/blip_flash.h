#pragma once

#include <stdbool.h>

void blip_flash_create(void);
void blip_flash_draw(void);
void blip_flash_update(const float dt);
void blip_flash_trigger(const bool is_visual);
void blip_flash_destroy(void);
