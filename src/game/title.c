#include <stdlib.h>

#include "engine/object.h"
#include "engine/util.h"
#include "engine/sfx.h"

#include "game/static.h"
#include "game/blip.h"
#include "game/ui.h"
#include "game/game.h"
#include "game/settings.h"
#include "game/title.h"

static float face_timer = 0.0f;
static int face_state = 0;
static int selected = 0;
static int selected_setting = 0;
static int available = 2;
static bool is_loaded = false;
static bool settings_triggered = false;

static object_t night_text;
static object_t night_atlas;

object_t freddy_face[4];
const char *freddy_face_paths[4] = {
	"rom:/title0.ci8.sprite",
	"rom:/title1.ci8.sprite",
	"rom:/title2.ci8.sprite",
	"rom:/title3.ci8.sprite",
};

object_t star;
object_t selector;
object_t title_text;
object_t option_text[4];
const char *option_text_paths[4] = {
	"rom:/new_game.ia4.sprite",
	"rom:/continue.ia4.sprite",
	"rom:/night_6.ia4.sprite",
	"rom:/custom_night.ia4.sprite",
};

object_t settings_text;
object_t settings_option_text;

object_t bind_buttons_text;
object_t setting_descs;

static void _title_load(void)
{
	if(is_loaded)
		return;
	selected = 0;

	settings_triggered = false;
	objects_load(freddy_face, 4, freddy_face_paths);
	object_load(&star, "rom:/star.ia4.sprite");
	object_load(&selector, "rom:/selector.ia4.sprite");
	object_load(&title_text, "rom:/title_text.ia4.sprite");
	objects_load(option_text, 4, option_text_paths);
	object_load(&night_text, "rom:/night.ia4.sprite");
	object_load(&night_atlas, "rom:/night_atlas.ia4.sprite");
	object_load(&settings_text, "rom:/settings_text.ia4.sprite");
	object_load(&settings_option_text, "rom:/settings_opts.ia4.sprite");
	object_load(&bind_buttons_text, "rom:/bind_buttons_text.ia4.sprite");
	object_load(&setting_descs, "rom:/setting_descs.ia4.sprite");

	blip_trigger(true);
	wav64_play(&static_sfx, SFXC_STATIC);
	mixer_ch_set_vol(SFXC_AMBIENCE, 0.8f, 0.8f);
	wav64_play(&title_music, SFXC_AMBIENCE);
	is_loaded = true;
}

static void _title_unload(void)
{
	if(!is_loaded)
		return;
	object_unload(&setting_descs);
	object_unload(&bind_buttons_text);
	object_unload(&settings_option_text);
	object_unload(&settings_text);
	object_unload(&night_atlas);
	object_unload(&night_text);
	objects_unload(option_text, 4);
	object_unload(&title_text);
	object_unload(&selector);
	object_unload(&star);
	objects_unload(freddy_face, 4);
	is_loaded = false;
}

static void _title_draw_settings(void)
{
	if(!settings_triggered)
		return;

	rdpq_set_mode_standard();
	rdpq_mode_alphacompare(true);
	object_draw(settings_text, 64, 64, 0, 0);
	object_draw(settings_option_text, 128, 256, 0, 0);

	int selector_pos[SETTING_COUNT] = {
		 259, 311, 371, 424, 479, 534, 589
	};
	object_draw(selector, 40, selector_pos[selected_setting], 0, 0);
	object_draw_index_y(setting_descs, 132, 162, 20, selected_setting);

	rdpq_set_mode_fill(RGBA32(0xff, 0xff, 0xff, 0xff));
	for(int i = 0; i < SETTING_COUNT; i++) {
		bool is_using = settings_flags & (1 << i);
		if(!is_using)
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

	rdpq_set_mode_copy(false);
	int face = (face_state < 4) * face_state;
	object_draw(freddy_face[face], 0, 0, 0, 0);
	static_draw(true);

	if(settings_triggered) {
		_title_draw_settings();
		return;
	}

	object_draw(title_text, 70, 68, 0, 0);
	for(int i = 0; i < available; i++)
		object_draw(option_text[i], 118, 420 + 58 * i, 0, 0);

	object_draw(bind_buttons_text, 558, 45, 0, 0);

	int star_count = 0;
	star_count += (night_beat_flags & NIGHT_5_BEATEN_BIT) > 0;
	star_count += (night_beat_flags & NIGHT_6_BEATEN_BIT) > 0;
	star_count += (night_beat_flags & MODE_20_BEATEN_BIT) > 0;
	available = clampf(star_count, 0, 2) + 2;
	for(int i = 0; i < star_count; i++)
		object_draw(star, 93 + 77 * i, 350, 28, 27);

	int selector_pos[4] = {
		426, 485, 546, 604
	};
	object_draw(selector, 40, selector_pos[selected], 0, 0);

	if(selected == 1) {
		int clamped = clampf(night_num, 1, 5);
		object_draw(night_text, 444, 509, 0, 0);
		object_draw_index_y(night_atlas, 512, 509, 6, clamped);
	}

	blip_draw();
}

static void _title_update_settings(struct controller_data down)
{
	if(down.c->L)
		wav64_play(&boop_sfx, SFXC_BLIP);

	if(down.c->down || down.c->C_down) {
		blip_trigger(false);
		selected_setting++;
	}

	if(selected_setting >= SETTING_COUNT)
		selected_setting = 0;

	if(down.c->up || down.c->C_up) {
		blip_trigger(false);
		selected_setting--;
	}

	if(selected_setting < 0)
		selected_setting = SETTING_COUNT - 1;

	settings_flags ^= (down.c->A << selected_setting);
}

enum scene title_update(update_parms_t uparms)
{
	static float delete_timer = 0.0f;
	static bool already_deleted = false;
	if(uparms.held.c->Z) {
		if(!already_deleted)
			delete_timer += uparms.dt;
	} else {
		delete_timer = 0.0f;
		already_deleted = false;
	}

	if(delete_timer >= 1.0f) {
		blip_trigger(false);
		night_num = 1;
		night_beat_flags = 0;
		delete_timer = 0.0f;
		already_deleted = true;
	}


	face_timer += uparms.dt * 60 * 0.08f;
	bool tick;
	face_timer = wrapf(face_timer, 1, &tick);
	if(tick)
		face_state = rand() % 100;

	settings_triggered ^= uparms.down.c->R;
	if(settings_triggered) {
		_title_update_settings(uparms.down);
		return SCENE_TITLE_SCREEN;
	}

	/* inputs */
	if(uparms.down.c->up || uparms.down.c->C_up) {
		selected--;
		blip_trigger(false);
		if(selected < 0)
			selected = available - 1;
	}

	if(uparms.down.c->down || uparms.down.c->C_down) {
		selected++;
		blip_trigger(false);
		if(selected >= available)
			selected = 0;
	}

	if(uparms.down.c->start || uparms.down.c->A) {
		rdpq_call_deferred((void (*)(void *))_title_unload, NULL);

		switch(selected) {
		case 0:
			night_num = 1;
			// eepfs_write("fnaf.sav", &night_num, 1);
			sfx_stop_all();
			return SCENE_WHICH_NIGHT;

		case 1:
			night_num = night_num > 5 ? 5 : night_num;
			sfx_stop_all();
			return SCENE_WHICH_NIGHT;

		case 2:
			night_num = 6;
			sfx_stop_all();
			return SCENE_WHICH_NIGHT;

		case 3:
			night_num = 7;
			return SCENE_CUSTOM_NIGHT;
		}
	}

	return SCENE_TITLE_SCREEN;
}
