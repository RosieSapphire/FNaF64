#ifndef GAME_UI_H
#define GAME_UI_H

#include "engine/object.h"

extern float night_timer;
extern int power_left;

void ui_load(void);
void ui_unload(void);
void ui_draw(void);
void ui_update(double dt);

#endif /* GAME_UI_H */
