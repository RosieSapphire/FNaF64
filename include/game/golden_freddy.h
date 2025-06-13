#pragma once

extern int golden_freddy_progress;

void golden_freddy_load(void);
void golden_freddy_draw_in_room(const float office_turn);
void golden_freddy_draw_scare(void);
void golden_freddy_update(double dt);
void golden_freddy_unload(void);
