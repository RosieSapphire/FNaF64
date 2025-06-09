#include <stdlib.h>
#include <stdbool.h>

#include "engine/perspective.h"
#include "engine/sfx.h"
#include "engine/util.h"

#include "game/office.h"
#include "game/fan.h"
#include "game/buttons.h"
#include "game/doors.h"
#include "game/camera.h"
#include "game/static.h"
#include "game/blip.h"
#include "game/ui.h"
#include "game/subtitles.h"
#include "game/hallucinations.h"
#include "game/freddy.h"
#include "game/bonnie.h"
#include "game/chica.h"
#include "game/foxy.h"
#include "game/golden_freddy.h"
#include "game/settings.h"
#include "game/save_data.h"
#include "game/game.h"

static bool is_loaded = false;

float night_timer;
static float jumpscare_exit_timer = 40;
int night_skip_correct;
float ticks_since_load;
float time_since_load;

static float circus_timer;

int settings_flags = 0;// SET_SUBTITLES_BIT;

static void _game_load(void)
{
	if(is_loaded)
		return;

	ticks_since_load = get_ticks();

	night_skip_correct = 0;
	office_load();
	fan_load();
	doors_load();
	buttons_load();
	camera_load();
	ui_load();
	bonnie_load();
	chica_load();
	foxy_load();
	freddy_load();
	golden_freddy_load();
	hallucinations_load();
	
	switch(SAVE_NIGHT_NUM(save_data)) {
	case 1:
		wav64_play(&phonecall1, SFX_CH_PHONECALL);
		break;

	case 2:
		wav64_play(&phonecall2, SFX_CH_PHONECALL);
		break;

	case 3:
		wav64_play(&phonecall3, SFX_CH_PHONECALL);
		break;

	case 4:
		wav64_play(&phonecall4, SFX_CH_PHONECALL);
		break;
		
	case 5:
		wav64_play(&phonecall5, SFX_CH_PHONECALL);
		break;
	}

	night_timer = 0.0f;
	is_loaded = true;

	// debug_add("Time Since Load", &time_since_load, DV_FLOAT);
	// debug_add("Time", &night_timer, DV_FLOAT);

	ticks_since_load = get_ticks() - ticks_since_load;
	time_since_load = (float)ticks_since_load / (float)TICKS_PER_SECOND;
}

static void _game_unload(void)
{
	if(!is_loaded)
		return;
	hallucinations_unload();
	golden_freddy_unload();
	freddy_unload();
	foxy_unload();
	chica_unload();
	bonnie_unload();
	ui_unload();
	camera_unload();
	buttons_unload();
	doors_unload();
	fan_unload();
	office_unload();

	is_loaded = false;
}

void game_draw(void)
{
	_game_load();

	if(golden_freddy_progress == 4) {
		golden_freddy_draw_scare();
		return;
	}

	if(!camera_is_visible) {
		perspective_begin();
		office_draw();
		fan_draw();
		golden_freddy_draw_in_room();
		doors_draw();
		buttons_draw();

		if(bonnie_is_jumpscaring)
			bonnie_draw_scare();

		if(chica_is_jumpscaring)
			chica_draw_scare();

		perspective_end();
	} else {
		camera_view_draw();
		static_draw(true);
		camera_ui_draw();
		blip_draw();
	}

	if(settings_flags & SET_ROBOT_CHEAT_BIT) {
		bonnie_draw_debug();
		chica_draw_debug();
		freddy_draw_debug();
	}

	if(settings_flags & SET_SUBTITLES_BIT &&
			mixer_ch_playing(SFX_CH_PHONECALL)) {
		subtitles_draw(night_timer, SAVE_NIGHT_NUM(save_data), time_since_load);
	}

	ui_draw();
	camera_flip_draw();
	hallucinations_draw();
}

static void _game_handle_cheat_code(joypad_buttons_t down)
{
	int cheat_inputs[6] = {
		down.c_up,    down.c_down, down.c_left,
		down.c_right, down.b,      down.a,
	};

	int cheat_indis[10] = {0, 0, 1, 1, 2, 3, 2, 3, 4, 5};

	for(int i = 0; i < 6; i++) {
		if(!cheat_inputs[i])
			continue;

		if(i != cheat_indis[night_skip_correct]) {
			night_skip_correct = 0;
			break;
		}

		night_skip_correct++;
	}

	if(night_skip_correct == 10)
		night_timer = 6 * HOUR_LEN_SECONDS;
}

static void _game_update_random_events(float dt)
{
	/* Circus music */
	bool play_circus_music;
	circus_timer = wrapf(circus_timer + dt, 5, &play_circus_music);
	if(play_circus_music && (rand() % 30) == 1) {
		mixer_ch_set_vol(SFX_CH_AMBIENCE, 0.05f, 0.05f);
		wav64_play(&circus_music, SFX_CH_AMBIENCE);
	}
}

enum scene game_update(update_parms_t uparms)
{
	golden_freddy_update(uparms.dt);
	hallucinations_update(uparms.dt);

	if(golden_freddy_progress == 4) {
		golden_freddy_update(uparms.dt);
		return SCENE_MAIN_GAME;
	}

	if(golden_freddy_progress == 6) {
		uint32_t *sorry = NULL;
		*sorry = 69;
	}

	if(!camera_is_visible && uparms.pressed.c_up &&
           SAVE_NIGHT_NUM(save_data) <= 5) {
		mixer_ch_stop(SFX_CH_PHONECALL);
        }

	_game_handle_cheat_code(uparms.pressed);
	_game_update_random_events(uparms.dt);

	night_timer += uparms.dt;
	if(night_timer >= 6 * HOUR_LEN_SECONDS) {
		sfx_stop_all_channels();
                /* TODO: Maybe replace this with an `rspq_wait()`. */
		rdpq_call_deferred((void (*)(void *))_game_unload, NULL);
		return SCENE_NIGHT_END;
	}

	static int hour_last = 0;
	int hour = (night_timer / (float)HOUR_LEN_SECONDS);
	if(hour == 2 && hour_last != 2) {
		bonnie_ai_level++;
	}

	if((hour == 3 && hour_last != 3) || (hour == 4 && hour_last != 4)) {
		bonnie_ai_level++;
		chica_ai_level++;
		foxy_ai_level++;
	}

	bonnie_ai_level = clampf(bonnie_ai_level, 0, 20);
	chica_ai_level = clampf(chica_ai_level, 0, 20);
	foxy_ai_level = clampf(foxy_ai_level, 0, 20);
	freddy_ai_level = clampf(freddy_ai_level, 0, 20);

	hour_last = hour;

	if(ui_power_left <= 0) {
		sfx_stop_all_channels();
		rdpq_call_deferred((void (*)(void *))_game_unload, NULL);
		return SCENE_POWER_DOWN;
	}

	office_update(uparms);
	fan_update(uparms.dt);
	doors_update(uparms.dt);
	buttons_update(uparms);
	camera_update(uparms);
	ui_update(uparms.dt);
	bonnie_update(uparms.dt);
	chica_update(uparms.dt);
	foxy_update(uparms.dt);
	freddy_update(uparms.dt);
	if(bonnie_is_jumpscaring || chica_is_jumpscaring ||
			foxy_is_scaring || freddy_is_jumpscaring) {
		jumpscare_exit_timer -= uparms.dt * 60;
		if(jumpscare_exit_timer <= 0) {
			sfx_stop_all_channels();
			rdpq_call_deferred((void (*)(void *))_game_unload,
					NULL);
			jumpscare_exit_timer = 40;
			return SCENE_GAME_OVER;
		}
	}
	
	if(uparms.pressed.start) {
		sfx_stop_all_channels();
                /* TODO: Replace with `rspq_wait()`. */
		rdpq_call_deferred((void (*)(void *))_game_unload, NULL);
		return SCENE_TITLE_SCREEN;
	}

	return SCENE_MAIN_GAME;
}
