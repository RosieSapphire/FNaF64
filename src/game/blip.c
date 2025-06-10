#include "engine/sfx.h"
#include "engine/util.h"

#include "game/blip.h"

#define BLIP_FRAME_CNT 9

static float timer = 0.f;
static rspq_block_t *blocks[BLIP_FRAME_CNT];

void blip_create(void)
{
	rdpq_set_mode_fill(RGBA16(0xFF, 0xFF, 0xFF, 0xFF));

	rspq_block_begin();
	rdpq_fill_rectangle(0, 0, 320, 240);
	blocks[0] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 52, 320, 52 + 93);
	rdpq_fill_rectangle(0, 146, 320, 146 + 27);
	blocks[1] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 33, 320, 33 + 93);
	rdpq_fill_rectangle(0, 190, 320, 190 + 28);
	blocks[2] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 40, 320, 40 + 27);
	rdpq_fill_rectangle(0, 80, 320, 80 + 93);
	blocks[3] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 0, 320, 40);
	rdpq_fill_rectangle(0, 166, 320, 166 + 28);
	blocks[4] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 86, 320, 86 + 28);
	rdpq_fill_rectangle(0, 127, 320, 127 + 93);
	blocks[5] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 176, 320, 176 + 28);
	blocks[6] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 34, 320, 34 + 28);
	blocks[7] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 144, 320, 144 + 28);
	blocks[8] = rspq_block_end();
}

void blip_draw(void)
{
	int frame;

        frame = (int)timer;
	if(frame >= BLIP_FRAME_CNT) {
		return;
        }

	rdpq_set_mode_fill(RGBA16(0xFF, 0xFF, 0xFF, 0xFF));
	rspq_block_run(blocks[frame]);
}

void blip_update(const float dt)
{
	timer += speed_fps(70) * dt;
	timer = clampf(timer, 0, BLIP_FRAME_CNT);
}

void blip_trigger(const bool is_visual)
{
	wav64_play(&sfx_blip, SFX_CH_BLIP);

	if(is_visual) {
		timer = 0;
        }
}

void blip_destroy(void)
{
        int i;

        for (i = 0; i < BLIP_FRAME_CNT; ++i) {
                rspq_block_free(blocks[i]);
        }
}
