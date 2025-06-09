#include "engine/object.h"
#include "engine/util.h"

#include "game/camera.h"
#include "game/buttons.h"
#include "game/game.h"
#include "game/settings.h"
#include "game/texture_index.h"
#include "game/save_data.h"
#include "game/ui.h"

static object_t night_text;
static object_t am;
static object_t hour_atlas;
static object_t night_atlas;
static object_t ui_power_left_text;
static object_t usage_text;
static object_t usage_atlas;

int ui_power_usage;
int ui_power_left;
float ui_power_timer;

void ui_load(void)
{
	object_load(&night_text, TX_NIGHT_TEXT);
	object_load(&am, TX_AM_SMALL);
	object_load(&hour_atlas, TX_HOUR_ATLAS);
	object_load(&night_atlas, TX_NIGHT_NUM_ATLAS);
	object_load(&ui_power_left_text, TX_POWER_LEFT_TEXT);
	object_load(&usage_text, TX_USAGE_TEXT);
	object_load(&usage_atlas, TX_USAGE_ATLAS);

	ui_power_usage = 1;
	ui_power_left = 999;
	ui_power_timer = 0.0f;
}

void ui_unload(void)
{
	object_unload(&night_text);
	object_unload(&am);
	object_unload(&hour_atlas);
	object_unload(&night_atlas);
	object_unload(&ui_power_left_text);
	object_unload(&usage_text);
	object_unload(&usage_atlas);
}

void ui_draw(void)
{
	rdpq_set_mode_standard();
	rdpq_mode_alphacompare(true);
	object_draw(am, 879, 30, 0, 0);
	object_draw(night_text, 828, 71, 0, 0);
	int hour = (int)clampf(night_timer / HOUR_LEN_SECONDS, 0, 6);

	if(hour > 0)
		object_draw_index_y(hour_atlas, 843, 30, 9, hour);
	else {
		object_draw_index_y(hour_atlas, 843 - 26, 30, 9, 1);
		object_draw_index_y(hour_atlas, 843, 30, 9, 2);
	}

	object_draw_index_y(night_atlas, 903, 71, 6, SAVE_NIGHT_NUM(save_data));
	object_draw(ui_power_left_text, 106, 638, 68, 7);
	object_draw(usage_text, 74, 674, 36, 7);

	for(int i = 0; i < ui_power_usage; i++) {
		int ind = clampf(i - 1, 0, 69);
		object_draw_index_x(usage_atlas, 120 + i * 21, 657, 6, ind);
	}

	object_draw_index_y(night_atlas, 200, 632, 6, (ui_power_left % 100) / 10);
	
	if(ui_power_left > 100)
		object_draw_index_y(night_atlas, 185, 632, 6, ui_power_left / 100);
}

void ui_update(double dt)
{
	ui_power_timer += dt;
	bool ui_power_tick;
	ui_power_timer = wrapf(ui_power_timer, 1, &ui_power_tick);
	if(ui_power_tick)
		ui_power_left -= ui_power_usage;
	ui_power_left = clampf(ui_power_left, 0, 999);

	ui_power_usage = 1;
	ui_power_usage += camera_is_visible;
	ui_power_usage += (button_state & BUTTON_LEFT_DOOR) > 0;
	ui_power_usage += (button_state & BUTTON_RIGHT_DOOR) > 0;
	ui_power_usage += (button_state & BUTTON_LEFT_LIGHT) > 0;
	ui_power_usage += (button_state & BUTTON_RIGHT_LIGHT) > 0;
}
