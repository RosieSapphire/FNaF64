#include "engine/object.h"
#include "engine/sfx.h"

#include "game/static.h"
#include "game/blip.h"
#include "game/texture_index.h"
#include "game/game_over.h"

static bool is_loaded = false;
object_t over_screen;
object_t over_text;
static float over_timer = 0.0f;

static void _game_over_load(void)
{
	if (is_loaded)
		return;
	object_load(&over_screen, TX_GAME_OVER_SCREEN);
	object_load(&over_text, TX_GAME_OVER_TEXT);
	over_timer = 0.0f;
	blip_trigger(true);

	sfx_stop_all_channels();
	mixer_ch_set_vol(SFX_CH_AMBIENCE, 0.5f, 0.5f);
	wav64_play(&sfx_static, SFX_CH_AMBIENCE);
	is_loaded = true;
}

static void _game_over_unload(void)
{
	if (!is_loaded)
		return;
	object_unload(&over_screen);
	object_unload(&over_text);
	over_timer = 0.0f;
	is_loaded = false;
}

void game_over_draw(void)
{
	_game_over_load();

	bool show_over_screen = over_timer >= 10;
	if (show_over_screen) {
		rdpq_set_mode_copy(false);
		object_draw(over_screen, 0, 0, 0, 0);
		rdpq_set_mode_standard();
		rdpq_mode_alphacompare(true);
		object_draw(over_text, 685, 636, 0, 0);
	}

	static_draw(show_over_screen);
	blip_draw();
}

enum scene game_over_update(update_parms_t uparms)
{
	over_timer += uparms.dt;

	bool show_over_screen = over_timer >= 10;
	if (show_over_screen) {
		sfx_stop_all_channels();
        }

	bool attempt_skip = uparms.pressed.a ||
		uparms.pressed.b || uparms.pressed.start;
	if (attempt_skip && show_over_screen) {
		rdpq_call_deferred((void(*)(void *))_game_over_unload, NULL);
		over_timer = 0.0f;
		return SCENE_TITLE_SCREEN;
	}

	if (over_timer < 20)
		return SCENE_GAME_OVER;

	rdpq_call_deferred((void(*)(void *))_game_over_unload, NULL);
	over_timer = 0.0f;
	return SCENE_TITLE_SCREEN;
}
