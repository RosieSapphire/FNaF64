#include <stdlib.h>

#include "engine/object.h"
#include "engine/util.h"

#include "game/texture_index.h"
#include "game/static.h"

#define FRAMES 8

static object_t frames[FRAMES];
static const char *frame_paths[FRAMES] = {
	TX_STATIC0, TX_STATIC1, TX_STATIC2, TX_STATIC3,
	TX_STATIC4, TX_STATIC5, TX_STATIC6, TX_STATIC7,
};

static float timer = 0.0f;
int which_frame = 0;

void static_load(void)
{
	objects_load(frames, FRAMES, frame_paths);
}

void static_draw(bool as_overlay)
{
	rdpq_set_mode_standard();
	if (as_overlay)
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);

	object_draw(frames[which_frame], 0, 0, 0, 0);
}

void static_update(double dt)
{
	timer += dt * speed_fps(100);
	bool tick_frame;
	timer = wrapf(timer, 1.0f, &tick_frame);
	if (!tick_frame)
		return;

	int frame_last = which_frame;
	do {
		which_frame = rand() & 7;
	} while (frame_last == which_frame);
}
