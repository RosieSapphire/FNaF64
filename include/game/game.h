#pragma once

#define GAME_DEBUG_ENABLED

#include <libdragon.h>

#include "engine/update.h"

#include "game/scene.h"

#define GAME_DOOR_CNT 2

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

enum {
        GAME_JUMPSCARE_FLAG_BONNIE_SHIFT,
        GAME_JUMPSCARE_FLAG_CHICA_SHIFT,
        GAME_JUMPSCARE_FLAG_FOXY_SHIFT,
        GAME_JUMPSCARE_FLAG_FREDDY_SHIFT
};

enum {
        GAME_JUMPSCARE_FLAG_BONNIE = (1 << GAME_JUMPSCARE_FLAG_BONNIE_SHIFT),
        GAME_JUMPSCARE_FLAG_CHICA  = (1 << GAME_JUMPSCARE_FLAG_CHICA_SHIFT),
        GAME_JUMPSCARE_FLAG_FOXY   = (1 << GAME_JUMPSCARE_FLAG_FOXY_SHIFT),
        GAME_JUMPSCARE_FLAG_FREDDY = (1 << GAME_JUMPSCARE_FLAG_FREDDY_SHIFT),
        GAME_JUMPSCARE_FLAG_MASK   = (GAME_JUMPSCARE_FLAG_BONNIE |
                                      GAME_JUMPSCARE_FLAG_CHICA |
                                      GAME_JUMPSCARE_FLAG_FOXY |
                                      GAME_JUMPSCARE_FLAG_FREDDY)
};

enum {
        GAME_GOLDFRED_STATE_ATTEMPT,
        GAME_GOLDFRED_STATE_IN_CAM2B,
        GAME_GOLDFRED_STATE_READY_TO_ATTACK,
        GAME_GOLDFRED_STATE_IN_OFFICE,
        GAME_GOLDFRED_STATE_JUMPSCARING,
        GAME_GOLDFRED_STATE_NEUTRALIZED,
        GAME_GOLDFRED_STATE_COUNT
};

extern float   game_office_turn;
extern float   game_office_turn_lerp;
extern float   game_scare_timer_foxy;
extern float   game_scare_timer_freddy;
extern float   game_office_flicker_rand_timer;
extern int     game_office_flicker_rand;
extern float   game_door_anim_timers[GAME_DOOR_CNT];
extern float   game_pause_timer_accum;
extern uint8_t game_hour_cur;
extern uint8_t game_hour_prev;
extern int     game_power_usage;
extern int     game_power_left;
extern float   game_power_timer;
extern uint8_t game_jumpscare_flags;
extern bool    game_shotgun_is_unlocked;
extern float   game_shotgun_reload_timer;
extern bool    game_won_by_murder;
extern uint8_t game_goldfred_state;

void       game_draw(void);
void       game_hour_update(const struct update_params uparms);
enum scene game_update(struct update_params uparms);
