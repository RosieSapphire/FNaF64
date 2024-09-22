#include <stdlib.h>
#include <time.h>
#include <libdragon.h>

#include "debug_view.h"

#include "engine/object.h"
#include "engine/scene.h"
#include "engine/sfx.h"
#include "engine/perspective.h"
#include "engine/update.h"

#include "game/static.h"
#include "game/subtitles.h"
#include "game/blip.h"
#include "game/which_night.h"
#include "game/game.h"
#include "game/title.h"
#include "game/night_end.h"
#include "game/game_over.h"
#include "game/powerdown.h"
#include "game/custom_night.h"
#include "game/paycheck.h"

static void n64_init(void)
{
	debug_init_isviewer();
	debug_init_usblog();
	srand(TICKS_READ());
	display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE,
		     ANTIALIAS_RESAMPLE);

	rdpq_init();

	dfs_init(DFS_DEFAULT_LOCATION);
	asset_init_compression(1);
	asset_init_compression(2);
	debug_view_init();

	audio_init(AUDIO_FREQUENCY, AUDIO_BUFFER_COUNT);
	mixer_init(SFXC_COUNT);

	timer_init();
	joypad_init();
	subtitles_load();

	eepfs_entry_t save_entry = { "fnaf.dat", sizeof(save_data) };
	int eepfs_result = eepfs_init(&save_entry, sizeof(save_data));
	eeprom_failed = eepfs_result != EEPFS_ESUCCESS;
	if (eeprom_failed)
		return;

	if (!eepfs_verify_signature()) {
		debugf("EEPFS verification failed. Wiping save filesystem.\n");
		eepfs_wipe();
		save_data = 1;
		eepfs_write("fnaf.dat", &save_data, sizeof(save_data));
	}

	int read_result = eepfs_read("fnaf.dat", &save_data, sizeof(save_data));
	if (read_result != EEPFS_ESUCCESS)
		debugf("Failed to read from EEPFS.\n");
	else
		debugf("Sucessfully loaded save: %u\n", save_data);
}

int main(void)
{
	n64_init();
	perspective_init();
	sfx_load();
	blip_load();
	static_load();

	enum scene scene = SCENE_TITLE_SCREEN;

	while (1) {
		joypad_poll();
		const update_parms_t uparms = {
			.dt = display_get_delta_time(),
			.held = joypad_get_buttons_held(JOYPAD_PORT_1),
			.pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1),
			.sticks = joypad_get_inputs(JOYPAD_PORT_1),
		};

		void (*draw_funcs[SCENE_COUNT])(void) = {
			title_draw,	 which_night_draw,  game_draw,
			night_end_draw,	 paycheck_draw,	    game_over_draw,
			power_down_draw, custom_night_draw,
		};

		enum scene (*update_funcs[SCENE_COUNT])(update_parms_t) = {
			title_update,	   which_night_update,
			game_update,	   night_end_update,
			paycheck_update,   game_over_update,
			power_down_update, custom_night_update,
		};

		surface_t *dsp = display_get();
		rdpq_attach(dsp, NULL);
		(*draw_funcs[scene])();
		debug_view_values_draw();
		rdpq_detach_show();

		static enum scene scene_last = SCENE_MAIN_GAME;
		scene_last = scene;
		scene = (*update_funcs[scene])(uparms);
		blip_update(uparms.dt);
		static_update(uparms.dt);

		if (scene_last != scene)
			rspq_wait();

		if (!audio_can_write())
			continue;

		short *audio_buf = audio_write_begin();
		mixer_poll(audio_buf, audio_get_buffer_length());
		audio_write_end();
	}

	subtitles_unload();
	debug_view_terminate();

	return 0;
}
