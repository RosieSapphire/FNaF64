#include <stdlib.h>

#include "engine/object.h"
#include "engine/util.h"
#include "engine/sfx.h"

#include "game/static.h"
#include "game/blip.h"
#include "game/ui.h"
#include "game/game.h"
#include "game/settings.h"
#include "game/texture_index.h"
#include "game/title.h"

static float face_timer = 0.0f;
static int face_state = 0;
static int selected = 0;
static int selected_setting = 0;
static int available = 2;
static bool is_loaded = false;
static bool settings_triggered = false;
static int unlock_all_correct = false;

static bool new_game_init;
static float new_game_timer;

static const char *freddy_face_paths[4] = {
	TX_FRED_FACE0,
	TX_FRED_FACE1,
	TX_FRED_FACE2,
	TX_FRED_FACE3,
};

static object_t freddy_face[4];
static object_t night_text;
static object_t night_atlas;

static object_t star;
static object_t selector;
static object_t title_text;
static object_t option_text;
static object_t settings_text;
static object_t settings_option_text;

static object_t bind_buttons_text;
static object_t setting_descs;
static object_t eeprom_error;
static object_t newspaper;

static bool eeprom_fail_notice = false;

static void _title_load(void)
{
	if (is_loaded)
		return;
	selected = 0;

	settings_triggered = false;
	objects_load(freddy_face, 4, freddy_face_paths);
	object_load(&night_text, TX_NIGHT_TEXT);
	object_load(&night_atlas, TX_NIGHT_NUM_ATLAS);
	object_load(&star, TX_STAR);
	object_load(&selector, TX_SELECTOR);
	object_load(&title_text, TX_TITLE_TEXT);
	object_load(&option_text, TX_TITLE_OPTIONS);
	object_load(&settings_text, TX_SETTINGS_TEXT);
	object_load(&settings_option_text, TX_SETTINGS_OPTS);

	object_load(&bind_buttons_text, TX_BIND_BUTTONS_TEXT);
	object_load(&setting_descs, TX_SETTINGS_DESCS);
	object_load(&eeprom_error, TX_EEPROM_ERROR);
	object_load(&newspaper, TX_NEWSPAPER);

	blip_trigger(true);
	wav64_play(&static_sfx, SFXC_STATIC);
	mixer_ch_set_vol(SFXC_AMBIENCE, 0.8f, 0.8f);
	wav64_play(&title_music, SFXC_AMBIENCE);
	new_game_init = false;
	new_game_timer = 0.0f;

	is_loaded = true;
}

static void _title_unload(void)
{
	if (!is_loaded)
		return;
	object_unload(&eeprom_error);
	object_unload(&setting_descs);
	object_unload(&bind_buttons_text);
	object_unload(&settings_option_text);
	object_unload(&settings_text);
	object_unload(&newspaper);
	object_unload(&night_atlas);
	object_unload(&night_text);
	object_unload(&option_text);
	object_unload(&title_text);
	object_unload(&selector);
	object_unload(&star);
	objects_unload(freddy_face, 4);
	is_loaded = false;
}

static void _title_draw_settings(void)
{
	if (!settings_triggered)
		return;

	rdpq_set_mode_standard();
	rdpq_mode_alphacompare(true);
	object_draw(settings_text, 64, 64, 0, 0);
	object_draw(settings_option_text, 128, 256, 0, 0);

	int selector_pos[SETTING_COUNT] = { 259, 311, 371, 424, 479, 534, 589 };
	object_draw(selector, 40, selector_pos[selected_setting], 0, 0);
	object_draw_index_y(setting_descs, 132, 162, 20, selected_setting);

	rdpq_set_mode_fill(RGBA32(0xff, 0xff, 0xff, 0xff));
	for (int i = 0; i < SETTING_COUNT; i++) {
		bool is_using = settings_flags & (1 << i);
		if (!is_using)
			continue;

		int x0 = 280 + 6;
		int y0 = (selector_pos[i] / 3) + 6;
		int x1 = x0 + 6;
		int y1 = y0 + 6;

		rdpq_fill_rectangle(x0, y0, x1, y1);
	}
}

void title_draw(void)
{
	_title_load();

	if (new_game_timer >= 2.0f)
		goto draw_newspaper;

	rdpq_set_mode_copy(false);
	int face = (face_state < 4) * face_state;
	object_draw(freddy_face[face], 0, 0, 0, 0);
	static_draw(true);

	if (settings_triggered) {
		_title_draw_settings();
		return;
	}

	rdpq_set_mode_standard();
	rdpq_mode_alphacompare(true);

	int star_count = 0;
	star_count += (save_data & NIGHT_5_BEATEN_BIT) > 0;
	star_count += (save_data & NIGHT_6_BEATEN_BIT) > 0;
	star_count += (save_data & MODE_20_BEATEN_BIT) > 0;
	available = clampf(star_count, 0, 2) + 2;
	for (int i = 0; i < star_count; i++)
		object_draw(star, 93 + 77 * i, 350, 28, 27);

	object_draw_index_y(option_text, 118, 420, 22 * available, 0);
	object_draw(title_text, 70, 68, 0, 0);
	object_draw(bind_buttons_text, 558, 45, 0, 0);
	object_draw(selector, 40, 429 + selected * 66, 0, 0);

	if (selected == 1) {
		int clamped = clampf(NIGHT_NUM, 1, 5);
		object_draw(night_text, 444, 509, 0, 0);
		object_draw_index_y(night_atlas, 512, 509, 6, clamped);
	}

	blip_draw();

	if (eeprom_failed && !eeprom_fail_notice) {
		rdpq_set_mode_standard();
		rdpq_set_prim_color(RGBA32(0x0, 0x0, 0x0, 0xD8));
		rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
		rdpq_fill_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

		rdpq_set_mode_standard();
		rdpq_mode_alphacompare(true);
		object_draw(eeprom_error, 96, 168, 0, 0);
	}

	if (!new_game_init)
		return;

draw_newspaper:
	float alpha = 1.0f;
	if (new_game_timer <= 2.0f)
		alpha = new_game_timer * 0.5f;

	if (new_game_timer >= 7.0f) {
		alpha = 1.0f - ((new_game_timer - 7.0f) * 0.5f);
		rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xFF));
		rdpq_fill_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	}

	rdpq_set_mode_standard();
	rdpq_mode_alphacompare(true);
	rdpq_set_fog_color(RGBA32(0xFF, 0xFF, 0xFF, alpha * 255));
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY_CONST);
	object_draw(newspaper, 0, 0, 0, 0);
}

static void _title_update_settings(joypad_buttons_t down)
{
	if (down.l)
		wav64_play(&boop_sfx, SFXC_BLIP);

	if (down.d_down || down.c_down) {
		blip_trigger(false);
		selected_setting++;
	}

	if (selected_setting >= SETTING_COUNT)
		selected_setting = 0;

	if (down.d_up || down.c_up) {
		blip_trigger(false);
		selected_setting--;
	}

	if (selected_setting < 0)
		selected_setting = SETTING_COUNT - 1;

	settings_flags ^= (down.a << selected_setting);
}

static void _title_update_deleting(update_parms_t uparms)
{
	static float delete_timer = 0.0f;
	static bool already_deleted = false;
	if (uparms.held.z) {
		if (!already_deleted)
			delete_timer += uparms.dt;
	} else {
		delete_timer = 0.0f;
		already_deleted = false;
	}

	if (delete_timer < 1.0f)
		return;

	blip_trigger(false);
	save_data = 1;
	delete_timer = 0.0f;
	already_deleted = true;
	selected = 0;
	if (!eeprom_failed)
		eepfs_write("fnaf.dat", &save_data, sizeof(save_data));

	debugf("Wiped save file to night %d\n", save_data);
}

static void _title_handle_cheat_code(joypad_buttons_t down)
{
	int cheat_inputs[1] = { down.l };

	int cheat_indis[5] = { 0, 0, 0, 0, 0 };

	for (int i = 0; i < 1; i++) {
		if (!cheat_inputs[i])
			continue;

		if (i != cheat_indis[unlock_all_correct]) {
			unlock_all_correct = 0;
			break;
		}

		unlock_all_correct++;
	}

	if (unlock_all_correct == 5)
		save_data = 7;
}

enum scene title_update(update_parms_t uparms)
{
	_title_handle_cheat_code(uparms.pressed);

	face_timer += uparms.dt * 60 * 0.08f;
	bool tick;
	face_timer = wrapf(face_timer, 1, &tick);
	if (tick)
		face_state = rand() % 100;

	if (eeprom_failed && !eeprom_fail_notice) {
		eeprom_fail_notice = uparms.pressed.start;
		return SCENE_TITLE_SCREEN;
	}

	if (new_game_init) {
		new_game_timer += uparms.dt;

		/* Check for skipping */
		const bool a_b_or_start_down =
			(uparms.pressed.a + uparms.pressed.a +
			 uparms.pressed.start) > 0;
		const bool can_skip = new_game_timer < 7.0f &&
				      new_game_timer > 2.0f;
		if (a_b_or_start_down && can_skip)
			new_game_timer = 7.0f;

		if (new_game_timer >= 9.0f) {
			rdpq_call_deferred((void (*)(void *))_title_unload,
					   NULL);
			sfx_stop_all();
			return SCENE_WHICH_NIGHT;
		}

		return SCENE_TITLE_SCREEN;
	}

	_title_update_deleting(uparms);

	settings_triggered ^= uparms.pressed.r;
	if (settings_triggered) {
		_title_update_settings(uparms.pressed);
		return SCENE_TITLE_SCREEN;
	}

	/* inputs */
	if (uparms.pressed.d_up || uparms.pressed.c_up) {
		selected--;
		blip_trigger(false);
		if (selected < 0)
			selected = available - 1;
	}

	if (uparms.pressed.d_down || uparms.pressed.c_down) {
		selected++;
		blip_trigger(false);
		if (selected >= available)
			selected = 0;
	}

	if (uparms.pressed.start || uparms.pressed.a) {
		uint8_t tmp = save_data &
			      (NIGHT_5_BEATEN_BIT | NIGHT_6_BEATEN_BIT |
			       MODE_20_BEATEN_BIT);
		switch (selected) {
		case 0:
			/* Just reset the nights, not the stars */
			save_data &= NIGHT_5_BEATEN_BIT | NIGHT_6_BEATEN_BIT |
				     MODE_20_BEATEN_BIT;
			save_data |= 1;

			if (!eeprom_failed)
				eepfs_write("fnaf.dat", &save_data, 1);
			debugf("Reset night to %d with %d%d%d\n", NIGHT_NUM,
			       save_data & NIGHT_5_BEATEN_BIT,
			       save_data & NIGHT_6_BEATEN_BIT,
			       save_data & MODE_20_BEATEN_BIT);
			new_game_init = true;
			return SCENE_TITLE_SCREEN;

		case 1:
			rdpq_call_deferred((void (*)(void *))_title_unload,
					   NULL);
			save_data = NIGHT_NUM > 5 ? 5 : NIGHT_NUM;
			save_data |= tmp;
			sfx_stop_all();
			return SCENE_WHICH_NIGHT;

		case 2:
			rdpq_call_deferred((void (*)(void *))_title_unload,
					   NULL);
			save_data = 6;
			save_data |= tmp;
			sfx_stop_all();
			return SCENE_WHICH_NIGHT;

		case 3:
			rdpq_call_deferred((void (*)(void *))_title_unload,
					   NULL);
			save_data = 7;
			save_data |= tmp;
			return SCENE_CUSTOM_NIGHT;
		}
	}

	return SCENE_TITLE_SCREEN;
}
