#pragma once

#include <stdint.h>

extern uint8_t settings_flags;

enum {
        SET_SMOOTH_TURN_BIT_SHIFT,
        SET_MANUAL_CAM_TURN_BIT_SHIFT,
        SET_SUBTITLES_BIT_SHIFT,
        SET_LIGHT_HOLD_BIT_SHIFT,
        SET_ROBOT_CHEAT_BIT_SHIFT,
        SET_GOOFY_MODE_BIT_SHIFT,
        SET_FAST_NIGHT_BIT_SHIFT
};

enum {
        SET_SMOOTH_TURN_BIT     = (1 << SET_SMOOTH_TURN_BIT_SHIFT),
        SET_MANUAL_CAM_TURN_BIT = (1 << SET_MANUAL_CAM_TURN_BIT_SHIFT),
        SET_SUBTITLES_BIT       = (1 << SET_SUBTITLES_BIT_SHIFT),
        SET_LIGHT_HOLD_BIT      = (1 << SET_LIGHT_HOLD_BIT_SHIFT),
        SET_ROBOT_CHEAT_BIT     = (1 << SET_ROBOT_CHEAT_BIT_SHIFT),
        SET_GOOFY_MODE_BIT      = (1 << SET_GOOFY_MODE_BIT_SHIFT),
        SET_FAST_NIGHT_BIT      = (1 << SET_FAST_NIGHT_BIT_SHIFT)
};

#define SETTING_COUNT 7
#define HOUR_LEN_SECONDS (90 >> ((settings_flags & SET_FAST_NIGHT_BIT) >> \
                                 SET_FAST_NIGHT_BIT_SHIFT))
