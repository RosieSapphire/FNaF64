#pragma once

#define GAME_DEBUG_ENABLED

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

#define GAME_DOOR_CNT 2

/* MOVE TO C START */
enum {
        GAME_DOOR_BTN_LEFT_DOOR_SHIFT,
        GAME_DOOR_BTN_LEFT_LIGHT_SHIFT,
        GAME_DOOR_BTN_RIGHT_DOOR_SHIFT,
        GAME_DOOR_BTN_RIGHT_LIGHT_SHIFT
};

enum {
        GAME_DOOR_BTN_LEFT_DOOR   = (1 << GAME_DOOR_BTN_LEFT_DOOR_SHIFT),
        GAME_DOOR_BTN_LEFT_LIGHT  = (1 << GAME_DOOR_BTN_LEFT_LIGHT_SHIFT),
        GAME_DOOR_BTN_RIGHT_DOOR  = (1 << GAME_DOOR_BTN_RIGHT_DOOR_SHIFT),
        GAME_DOOR_BTN_RIGHT_LIGHT = (1 << GAME_DOOR_BTN_RIGHT_LIGHT_SHIFT)
};

extern float game_office_turn;
extern float game_office_turn_lerp;
extern float game_scare_timer_foxy;
extern float game_scare_timer_freddy;
extern float game_office_flicker_rand_timer;
extern int   game_office_flicker_rand;
extern float game_door_anim_timers[GAME_DOOR_CNT];
/* MOVE TO C END */

#define GAME_DOOR_LEFT    0
#define GAME_DOOR_RIGHT   1
#define GAME_DOOR_ANIM_FRAME_CNT 15


enum {
        JUMPSCARE_FLAG_BONNIE_SHIFT,
        JUMPSCARE_FLAG_CHICA_SHIFT,
        JUMPSCARE_FLAG_FOXY_SHIFT,
        JUMPSCARE_FLAG_FREDDY_SHIFT
};

enum {
        JUMPSCARE_FLAG_BONNIE = (1 << JUMPSCARE_FLAG_BONNIE_SHIFT),
        JUMPSCARE_FLAG_CHICA  = (1 << JUMPSCARE_FLAG_CHICA_SHIFT),
        JUMPSCARE_FLAG_FOXY   = (1 << JUMPSCARE_FLAG_FOXY_SHIFT),
        JUMPSCARE_FLAG_FREDDY = (1 << JUMPSCARE_FLAG_FREDDY_SHIFT),
        JUMPSCARE_FLAG_MASK   = JUMPSCARE_FLAG_BONNIE | JUMPSCARE_FLAG_CHICA |
                                JUMPSCARE_FLAG_FOXY | JUMPSCARE_FLAG_FREDDY
};

extern float game_night_timer;
extern int game_power_usage;
extern int game_power_left;
extern float game_power_timer;
extern uint8_t game_jumpscare_flags;

void game_draw(void);
enum scene game_update(struct update_params uparms);
