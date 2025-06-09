#pragma once

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

extern float night_timer;
extern bool eeprom_failed;

#define NIGHT_NUM_BITMASK  0b00001111
#define NIGHT_5_BEATEN_BIT 0b00100000
#define NIGHT_6_BEATEN_BIT 0b01000000
#define MODE_20_BEATEN_BIT 0b10000000 
#define NIGHT_NUM          (save_data & NIGHT_NUM_BITMASK)
extern uint8_t save_data;

void game_draw(void);
enum scene game_update(update_parms_t uparms);
