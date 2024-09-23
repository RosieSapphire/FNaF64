#ifndef _GAME_SAVE_DATA_H_
#define _GAME_SAVE_DATA_H_

#include <stdint.h>

#define SAVE_NIGHT_NUM_BITMASK 0b00001111
#define SAVE_NIGHT_5_BEATEN_BIT 0b00100000
#define SAVE_NIGHT_6_BEATEN_BIT 0b01000000
#define SAVE_MODE_20_BEATEN_BIT 0b10000000
#define SAVE_BEAT_EVERYTHING 0b11100101
#define SAVE_NIGHT_NUM (save_data & SAVE_NIGHT_NUM_BITMASK)
extern uint8_t save_data;

int save_data_read(void);

#endif /* _GAME_SAVE_DATA_H_ */
