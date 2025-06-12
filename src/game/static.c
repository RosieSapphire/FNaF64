#include <stdlib.h>

#include "engine/graphic.h"
#include "engine/util.h"

#include "game/texture_index.h"
#include "game/static.h"

#define STATIC_FRAME_CNT 8

static struct graphic static_frames[STATIC_FRAME_CNT];
static const char *static_frame_paths[STATIC_FRAME_CNT] = {
	TX_STATIC0,
        TX_STATIC1,
        TX_STATIC2,
	TX_STATIC3,
        TX_STATIC4,
        TX_STATIC5,
	TX_STATIC6,
        TX_STATIC7
};

static float static_timer = 0.f;
static int static_frame = 0;

void static_load(void)
{
	graphics_load(static_frames, STATIC_FRAME_CNT, static_frame_paths);
}

void static_draw(const bool as_overlay)
{
	rdpq_set_mode_standard();
	if (as_overlay) {
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
        }
	graphic_draw(static_frames[static_frame], 0, 0, 0, 0, GFX_FLIP_NONE);
}

void static_update(const float dt)
{
        bool tick_frame;
        int frame_last;

	static_timer += dt * speed_fps(100);
	static_timer = wrapf(static_timer, 1.0f, &tick_frame);
	if (!tick_frame) {
		return;
        }

        frame_last = static_frame;
	do {
		static_frame = rand() & 7;
	} while (frame_last == static_frame);
}

void static_unload(void)
{
	graphics_unload(static_frames, STATIC_FRAME_CNT);
}
