#include "engine/graphic.h"
#include "engine/util.h"

#include "game/camera.h"
#include "game/buttons.h"
#include "game/game.h"
#include "game/settings.h"
#include "game/texture_index.h"
#include "game/save_data.h"
#include "game/game.h"
#include "game/ui.h"

static struct graphic night_text;
static struct graphic am;
static struct graphic hour_atlas;
static struct graphic night_atlas;
static struct graphic power_left_text;
static struct graphic usage_text;
static struct graphic usage_atlas;

void ui_load(void)
{
	graphic_load(&night_text, TX_NIGHT_TEXT);
	graphic_load(&am, TX_AM_SMALL);
	graphic_load(&hour_atlas, TX_HOUR_ATLAS);
	graphic_load(&night_atlas, TX_NIGHT_NUM_ATLAS);
	graphic_load(&power_left_text, TX_POWER_LEFT_TEXT);
	graphic_load(&usage_text, TX_USAGE_TEXT);
	graphic_load(&usage_atlas, TX_USAGE_ATLAS);
}

void ui_unload(void)
{
	graphic_unload(&night_text);
	graphic_unload(&am);
	graphic_unload(&hour_atlas);
	graphic_unload(&night_atlas);
	graphic_unload(&power_left_text);
	graphic_unload(&usage_text);
	graphic_unload(&usage_atlas);
}

void ui_draw(void)
{
        int i;

	rdpq_set_mode_standard();
	rdpq_mode_alphacompare(true);
	graphic_draw(am, 879, 30, 0, 0, GFX_FLIP_NONE);
	graphic_draw(night_text, 828, 71, 0, 0, GFX_FLIP_NONE);
        /* TODO: Replace with CLAMP */
	int hour = (int)CLAMP(game_night_timer / HOUR_LEN_SECONDS, 0, 6);

	if (hour > 0)
		graphic_draw_index_y(hour_atlas, 843, 30,
                                     9, hour, GFX_FLIP_NONE);
	else {
		graphic_draw_index_y(hour_atlas, 843 - 26,
                                     30, 9, 1, GFX_FLIP_NONE);
		graphic_draw_index_y(hour_atlas, 843, 30, 9, 2, GFX_FLIP_NONE);
	}

	graphic_draw_index_y(night_atlas, 903, 71, 6,
                             SAVE_NIGHT_NUM(save_data), GFX_FLIP_NONE);
	graphic_draw(power_left_text, 106, 638, 68, 7, GFX_FLIP_NONE);
	graphic_draw(usage_text, 74, 674, 36, 7, GFX_FLIP_NONE);

	for (i = 0; i < game_power_usage; ++i) {
		int ind = CLAMP(i - 1, 0, 69);
		graphic_draw_index_x(usage_atlas, 120 + i * 21,
                                     657, 6, ind, GFX_FLIP_NONE);
	}

	graphic_draw_index_y(night_atlas, 200, 632, 6,
                             (game_power_left % 100) / 10, GFX_FLIP_NONE);
	
	if (game_power_left > 100) {
		graphic_draw_index_y(night_atlas, 185, 632, 6,
                                     game_power_left / 100, GFX_FLIP_NONE);
        }
}

void ui_update(double dt)
{
	game_power_timer += dt;
	bool game_power_tick;
	game_power_timer = wrapf(game_power_timer, 1, &game_power_tick);
	if (game_power_tick)
		game_power_left -= game_power_usage;
	game_power_left = CLAMP(game_power_left, 0, 999);

	game_power_usage = 1;
	game_power_usage += camera_is_visible;
	game_power_usage += (button_state & BUTTON_LEFT_DOOR) > 0;
	game_power_usage += (button_state & BUTTON_RIGHT_DOOR) > 0;
	game_power_usage += (button_state & BUTTON_LEFT_LIGHT) > 0;
	game_power_usage += (button_state & BUTTON_RIGHT_LIGHT) > 0;
}
