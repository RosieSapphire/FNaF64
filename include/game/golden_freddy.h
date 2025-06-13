#pragma once

enum {
        GOLDEN_FREDDY_STATE_ATTEMPT,
        GOLDEN_FREDDY_STATE_IN_CAM2B,
        GOLDEN_FREDDY_STATE_READY_TO_ATTACK,
        GOLDEN_FREDDY_STATE_IN_OFFICE,
        GOLDEN_FREDDY_STATE_JUMPSCARING,
        GOLDEN_FREDDY_STATE_NEUTRALIZED,
        GOLDEN_FREDDY_STATE_CRASH_GAME,
        GOLDEN_FREDDY_STATE_CNT
};

extern int golden_freddy_state;

void golden_freddy_load(void);
void golden_freddy_draw_in_room(const float office_turn);
void golden_freddy_draw_scare(void);
void golden_freddy_update(double dt);
void golden_freddy_unload(void);
