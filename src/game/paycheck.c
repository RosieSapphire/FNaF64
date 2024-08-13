#include <stdbool.h>

#include "engine/object.h"
#include "engine/sfx.h"

#include "game/game.h"
#include "game/texture_index.h"
#include "game/paycheck.h"

static bool is_loaded = false;

static float timer;
static object_t screens[3];

static void _paycheck_load(void)
{
	if (is_loaded)
		return;

	timer = 0.0f;
	object_load(screens + 0, TX_PAYCHECK1);
	object_load(screens + 1, TX_PAYCHECK2);
	object_load(screens + 2, TX_PINK_SLIP);
	mixer_ch_set_vol(SFXC_AMBIENCE, 0.8f, 0.8f);
	wav64_play(&musicbox_sfx, SFXC_AMBIENCE);

	is_loaded = true;
}

static void _paycheck_unload(void)
{
	if (!is_loaded)
		return;

	objects_unload(screens, 3);

	is_loaded = false;
}

void paycheck_draw(void)
{
	_paycheck_load();

	float alpha = 1.0f;
	if (timer <= 2.0f)
		alpha = timer * 0.5f;

	if (timer >= 17.0f)
		alpha = 1.0f - ((timer - 17.0f) * 0.5f);

	rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xFF));
	rdpq_fill_rectangle(0, 0, 320, 240);

	rdpq_set_mode_standard();
	rdpq_mode_alphacompare(true);
	rdpq_set_fog_color(RGBA32(0xFF, 0xFF, 0xFF, alpha * 255));
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY_CONST);
	object_draw(screens[NIGHT_NUM - 6], 0, 0, 0, 0);
}

enum scene paycheck_update(update_parms_t uparms)
{
	timer += uparms.dt;

	const bool a_b_or_start_down =
		(uparms.pressed.a + uparms.pressed.b + uparms.pressed.start);
	const bool can_skip = timer < 7.0f && timer > 2.0f;
	if (a_b_or_start_down && can_skip)
		timer = 17.0f;

	if (timer >= 19.0f) {
		rdpq_call_deferred((void (*)(void *))_paycheck_unload, NULL);
		sfx_stop_all();
		return SCENE_TITLE_SCREEN;
	}

	return SCENE_PAYCHECK;
}
