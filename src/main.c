#include <stdlib.h>
#include <time.h>
#include <libdragon.h>

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
	srand(TICKS_READ());
	display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3,
			GAMMA_NONE, ANTIALIAS_RESAMPLE);

	rdpq_init();

	dfs_init(DFS_DEFAULT_LOCATION);

	// rdpq_debug_start();
	debug_init_isviewer();
	debug_init_usblog();

	audio_init(32000, 4);
	mixer_init(SFXC_COUNT);

	timer_init();
	controller_init();
	subtitles_load();

	/*
	eepfs_entry_t save_entry = {"/fnaf.sav", 1};
	int eepfs_result = eepfs_init(&save_entry, 1);
	assertf(eepfs_result == EEPFS_ESUCCESS, "Failed to init eeprom.\n");
	uint8_t save_dat;
	eepfs_read("/fnaf.sav", &save_dat, 1);
	char buf[8];
	snprintf(buf, 8, "%d", save_dat & 0xFF);
	debugf("%s\n", buf);
	*/
}

int main(void)
{
	n64_init();
	perspective_init();
	sfx_load();
	blip_load();
	static_load();

	enum scene scene = SCENE_TITLE_SCREEN;
	long ticks_last = get_ticks();

	while(1) {
		long ticks_now = get_ticks();
		long tick_delta = TICKS_DISTANCE(ticks_last, ticks_now);
		ticks_last = ticks_now;

		controller_scan();
		const update_parms_t uparms = {
			.dt = (float)tick_delta / (float)TICKS_PER_SECOND,
			.held = get_keys_held(),
			.down = get_keys_down(),
		};

		void (*draw_funcs[SCENE_COUNT])(void) = {
			title_draw, which_night_draw, game_draw,
			night_end_draw, paycheck_draw, game_over_draw,
			power_down_draw, custom_night_draw,
		};

		enum scene (*update_funcs[SCENE_COUNT])(update_parms_t) = {
			title_update, which_night_update, game_update,
			night_end_update, paycheck_update, game_over_update,
			power_down_update, custom_night_update,
		};

		surface_t *dsp = display_get();
		rdpq_attach(dsp, NULL);
		(*draw_funcs[scene])();
		rdpq_detach_show();

		static enum scene scene_last = SCENE_MAIN_GAME;
		scene_last = scene;
		scene = (*update_funcs[scene])(uparms);
		blip_update(uparms.dt);
		static_update(uparms.dt);

		if(scene_last != scene)
			rspq_wait();

		if(!audio_can_write())
			continue;

		short *audio_buf = audio_write_begin();
		mixer_poll(audio_buf, audio_get_buffer_length());
		audio_write_end();
	}

	subtitles_unload();

	return 0;
}
