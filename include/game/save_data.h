#pragma once

#include <stdint.h>
#include <stdbool.h>

enum {
        SAVE_NIGHT_NUM_BITMASK  = 0xF,
        SAVE_NIGHT_5_BEATEN_BIT = (1 << 5),
        SAVE_NIGHT_6_BEATEN_BIT = (1 << 6),
        SAVE_MODE_20_BEATEN_BIT = (1 << 7),
        SAVE_BEATEN_BITMASK = SAVE_NIGHT_5_BEATEN_BIT |
                              SAVE_NIGHT_6_BEATEN_BIT |
                              SAVE_MODE_20_BEATEN_BIT
};

#define SAVE_NIGHT_NUM(SAVE_DATA) (SAVE_DATA & SAVE_NIGHT_NUM_BITMASK)

extern uint8_t save_data;
extern bool save_data_eeprom_failed;

bool save_data_init(void);
void save_data_free(void);
