#pragma once

#define GAME_DEBUG_ENABLED

#include <libdragon.h>

#include "engine/scene.h"
#include "engine/update.h"

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
