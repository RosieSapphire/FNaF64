#include "engine/object.h"
#include "engine/util.h"

#include "game/blip.h"
#include "game/game.h"
#include "game/bonnie.h"
#include "game/chica.h"
#include "game/foxy.h"
#include "game/freddy.h"
#include "game/texture_index.h"
#include "game/which_night.h"

object_t which_night_atlas;
static float timer;
static bool is_loaded = false;

static const int night_ai_lut[6][4] = {
	{ 0, 0, 0, 0 }, { 3, 1, 1, 0 }, { 0, 5, 2, 1 },
	{ 2, 4, 6, 2 }, { 5, 7, 5, 3 }, { 10, 12, 6, 4 },
};

static void _which_night_load(void)
{
	if (is_loaded)
		return;
	timer = 0.0f;
	object_load(&which_night_atlas, TX_WHICH_NIGHT_ATLAS);
	is_loaded = true;
	blip_trigger(true);

	if (NIGHT_NUM == 7)
		return;

	bonnie_ai_level = night_ai_lut[NIGHT_NUM - 1][0];
	chica_ai_level = night_ai_lut[NIGHT_NUM - 1][1];
	foxy_ai_level = night_ai_lut[NIGHT_NUM - 1][2];
	freddy_ai_level = night_ai_lut[NIGHT_NUM - 1][3];
}

static void _which_night_unload(void)
{
	if (!is_loaded)
		return;
	object_unload(&which_night_atlas);
	is_loaded = false;
}

void which_night_draw(void)
{
	_which_night_load();

	rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xFF));
	rdpq_fill_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	rdpq_set_mode_standard();
	object_draw_index_y(which_night_atlas, 372, 270, 11, 0);
	object_draw_index_y(which_night_atlas, 373, 336, 11, NIGHT_NUM);

	float fade = clampf((timer - 70) / 60.0f, 0, 1);
	rdpq_set_prim_color(RGBA32(0x0, 0x0, 0x0, fade * 255));
	rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	rdpq_fill_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

	blip_draw();
}

enum scene which_night_update(update_parms_t uparms)
{
	timer += uparms.dt * 60;

	if (timer >= 130) {
		rdpq_call_deferred((void (*)(void *))_which_night_unload, NULL);
		return SCENE_MAIN_GAME;
	}

	return SCENE_WHICH_NIGHT;
}
