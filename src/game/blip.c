#include "config.h"

#include "engine/sfx.h"
#include "engine/util.h"

#include "game/blip.h"

#define FRAMES 9

static float timer = 0.0f;
static rspq_block_t *blocks[FRAMES];

void blip_load(void)
{
	rdpq_set_mode_fill(RGBA16(0xFF, 0xFF, 0xFF, 0xFF));

	rspq_block_begin();
	rdpq_fill_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	blocks[0] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 52, DISPLAY_WIDTH, 52 + 93);
	rdpq_fill_rectangle(0, 146, DISPLAY_WIDTH, 146 + 27);
	blocks[1] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 33, DISPLAY_WIDTH, 33 + 93);
	rdpq_fill_rectangle(0, 190, DISPLAY_WIDTH, 190 + 28);
	blocks[2] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 40, DISPLAY_WIDTH, 40 + 27);
	rdpq_fill_rectangle(0, 80, DISPLAY_WIDTH, 80 + 93);
	blocks[3] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 0, DISPLAY_WIDTH, 40);
	rdpq_fill_rectangle(0, 166, DISPLAY_WIDTH, 166 + 28);
	blocks[4] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 86, DISPLAY_WIDTH, 86 + 28);
	rdpq_fill_rectangle(0, 127, DISPLAY_WIDTH, 127 + 93);
	blocks[5] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 176, DISPLAY_WIDTH, 176 + 28);
	blocks[6] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 34, DISPLAY_WIDTH, 34 + 28);
	blocks[7] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 144, DISPLAY_WIDTH, 144 + 28);
	blocks[8] = rspq_block_end();
}

void blip_draw(void)
{
	int frame = (int)timer;
	if (frame >= FRAMES)
		return;
	rdpq_set_mode_fill(RGBA16(0xFF, 0xFF, 0xFF, 0xFF));
	rspq_block_run(blocks[frame]);
}

void blip_update(double dt)
{
	timer += speed_fps(70) * dt;
	timer = clampf(timer, 0, FRAMES);
}

void blip_trigger(bool is_visual)
{
	wav64_play(&blip_sfx, SFXC_BLIP);

	if (is_visual)
		timer = 0;
}
