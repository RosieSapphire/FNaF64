#include <stdlib.h>

#include "engine/object.h"
#include "engine/util.h"
#include "engine/sfx.h"

#include "game/texture_index.h"
#include "game/hallucinations.h"

#define NUM_HALLUC 4

static object_t visuals[NUM_HALLUC];
static const char *visual_paths[NUM_HALLUC] = {
	TX_HALLUCINATION0, TX_HALLUCINATION1,
	TX_HALLUCINATION2, TX_HALLUCINATION3,
};

static bool is_running;
static int visible_val;
static int which_visual;

static float random_occur_timer;
static float visible_timer;
static float timeout;

void hallucinations_load(void)
{
	objects_load(visuals, NUM_HALLUC, visual_paths);
	is_running = false;
	visible_val = 0;
	which_visual = 0;
	random_occur_timer = 0.0f;
	visible_timer = 0.0f;
	timeout = 0.0f;

	/*
	debug_add("GF Visible", &visible_val, DV_INT);
	debug_add("GF Visible Timer", &visible_timer, DV_FLOAT);
	debug_add("GF Which", &which_visual, DV_INT);
	debug_add("GF Random Timer", &random_occur_timer, DV_FLOAT);
	debug_add("GF Timeout", &timeout, DV_FLOAT);
	*/
}

void hallucinations_draw(void)
{
	if(!is_running || visible_val != 1)
		return;

	object_draw(visuals[which_visual], 0, 0, 0, 0);
}

void hallucinations_update(double dt)
{
	if(is_running) {
		bool change_visible;
		visible_timer = wrapf(visible_timer + dt,
				1.0f / 60.0f, &change_visible);
		if(change_visible) {
			visible_val = rand() % 10;
			which_visual = rand() % 3;
		}

		timeout += dt * 60;
		if(timeout >= 150) {
			is_running = false;
			timeout = 0;
			mixer_ch_set_vol(SFXC_ROBOTVOICE, 0, 0);
			return;
		}

		mixer_ch_set_vol(SFXC_ROBOTVOICE, 1, 1);

		return;
	}

	bool try_hal;
	random_occur_timer = wrapf(random_occur_timer + dt, 1.0f, &try_hal);
	if(try_hal && (rand() % 1000) == 1)
		hallucinations_trigger();
}

void hallucinations_trigger(void)
{
	is_running = true;
}

void hallucinations_unload(void)
{
	objects_unload(visuals, NUM_HALLUC);
}
