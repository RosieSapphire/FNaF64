#include "engine/object.h"
#include "engine/sfx.h"
#include "engine/util.h"

#include "game/freddy.h"
#include "game/bonnie.h"
#include "game/chica.h"
#include "game/foxy.h"
#include "game/custom_night.h"

object_t face_icons;
static object_t buttons;
static object_t nums;
static int ai_selected = 0;
static bool is_loaded = false;

static const int face_pos[4][2] = {
	{259, 150},
	{502, 150},
	{259, 405},
	{502, 405},
};

static void _custom_night_load(void)
{
	if(is_loaded)
		return;

	ai_selected = 0;
	object_load(&face_icons, "rom:/face_icons.ci8.sprite");
	object_load(&buttons, "rom:/ai_buttons.ci4.sprite");
	object_load(&nums, "rom:/ai_nums.ia4.sprite");

	is_loaded = true;
}

static void _custom_night_unload(void)
{
	if(!is_loaded)
		return;

	object_unload(&face_icons);
	object_unload(&buttons);
	object_unload(&nums);

	is_loaded = false;
}

static void _custom_night_ai_num_draw(int val, int x, int y)
{
	int ones = val % 10;
	int tens = val / 10;
	for(int i = 0; i < 1 + (tens > 0); i++)
		object_draw_index_x(nums, x - 36 * i, y, 12, i ? tens : ones);
}

void custom_night_draw(void)
{
	_custom_night_load();

	rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xFF));
	rdpq_fill_rectangle(0, 0, 320, 240);

	rdpq_set_mode_standard();
	for(int i = 0; i < 4; i++)
		object_draw_index_y(face_icons, face_pos[i][0],
				face_pos[i][1], 67, i);

	int bparms[8][4] = {
		{42, 225, 11, 0},
		{205, 225, 11, 1},
		{727, 225, 11, 0},
		{889, 225, 11, 1},
		{42, 479, 11, 0},
		{205, 479, 11, 1},
		{727, 479, 11, 0},
		{889, 479, 11, 1},
	};

	for(int i = 0; i < 8; i++)
		object_draw_index_x(buttons, bparms[i][0],
				bparms[i][1], bparms[i][2],
				bparms[i][3] + (2 * (ai_selected == (i >> 1))));

	rdpq_mode_alphacompare(true);
	_custom_night_ai_num_draw(freddy_ai_level, 145, 225 + 6);
	_custom_night_ai_num_draw(bonnie_ai_level, 829, 225 + 6);
	_custom_night_ai_num_draw(chica_ai_level, 145, 479 + 6);
	_custom_night_ai_num_draw(foxy_ai_level, 829, 479 + 6);
}

enum scene custom_night_update(update_parms_t uparms)
{
	ai_selected += uparms.down.c->right - uparms.down.c->left;
	ai_selected += uparms.down.c->down* 2 - uparms.down.c->up * 2;
	ai_selected &= 3;

	int *ais[4] = {
		&freddy_ai_level,
		&bonnie_ai_level,
		&chica_ai_level,
		&foxy_ai_level,
	};

	static float held_timer = 0.0f;
	static float held_tick = 0.0f;

	if(uparms.held.c->A || uparms.held.c->B)
		held_timer += uparms.dt;
	else
		held_timer = 0;

	if(held_timer >= 0.35f) {
		bool held_do;
		held_tick = wrapf(held_tick + uparms.dt, 0.06f, &held_do);
		**(ais + ai_selected) += held_do * uparms.held.c->A;
		**(ais + ai_selected) -= held_do * uparms.held.c->B;
	}

	**(ais + ai_selected) += uparms.down.c->A;
	**(ais + ai_selected) -= uparms.down.c->B;
	**(ais + ai_selected) = clampf(**(ais + ai_selected), 0, 20);
	
	if(uparms.down.c->start) {
		if(freddy_ai_level == 1 && bonnie_ai_level == 9 &&
				chica_ai_level == 8 && foxy_ai_level == 7)
			assertf(0, "Insert Golden Freddy jumpscare here.\n");

		sfx_stop_all();
		rdpq_call_deferred((void (*)(void *))_custom_night_unload,
				NULL);
		return SCENE_WHICH_NIGHT;
	}

	return SCENE_CUSTOM_NIGHT;
}
