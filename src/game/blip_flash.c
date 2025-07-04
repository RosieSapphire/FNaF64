#include "engine/sfx.h"
#include "engine/util.h"

#include "game/blip_flash.h"

#define BLIP_FLASH_FRAME_CNT 9

static float blip_flash_timer;
static rspq_block_t *blip_flash_dls[BLIP_FLASH_FRAME_CNT];

void blip_flash_create(void)
{
	rdpq_set_mode_fill(RGBA16(0xFF, 0xFF, 0xFF, 0xFF));

	rspq_block_begin();
	rdpq_fill_rectangle(0, 0, 320, 240);
	blip_flash_dls[0] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 52, 320, 52 + 93);
	rdpq_fill_rectangle(0, 146, 320, 146 + 27);
	blip_flash_dls[1] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 33, 320, 33 + 93);
	rdpq_fill_rectangle(0, 190, 320, 190 + 28);
	blip_flash_dls[2] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 40, 320, 40 + 27);
	rdpq_fill_rectangle(0, 80, 320, 80 + 93);
	blip_flash_dls[3] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 0, 320, 40);
	rdpq_fill_rectangle(0, 166, 320, 166 + 28);
	blip_flash_dls[4] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 86, 320, 86 + 28);
	rdpq_fill_rectangle(0, 127, 320, 127 + 93);
	blip_flash_dls[5] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 176, 320, 176 + 28);
	blip_flash_dls[6] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 34, 320, 34 + 28);
	blip_flash_dls[7] = rspq_block_end();

	rspq_block_begin();
	rdpq_fill_rectangle(0, 144, 320, 144 + 28);
	blip_flash_dls[8] = rspq_block_end();
}

void blip_flash_draw(void)
{
	int frame;

        frame = (int)blip_flash_timer;
	if (frame >= BLIP_FLASH_FRAME_CNT) {
		return;
        }

	rdpq_set_mode_fill(RGBA16(0xFF, 0xFF, 0xFF, 0xFF));
	rspq_block_run(blip_flash_dls[frame]);
}

void blip_flash_update(const float dt)
{
	blip_flash_timer += speed_fps(70) * dt;
	blip_flash_timer = CLAMP(blip_flash_timer, 0, BLIP_FLASH_FRAME_CNT);
}

void blip_flash_trigger(const bool is_visual)
{
	wav64_play(&sfx_blip, SFX_CH_BLIP);

	if (is_visual)
		blip_flash_timer = 0.f;
}

void blip_flash_destroy(void)
{
        int i;

        for (i = 0; i < BLIP_FLASH_FRAME_CNT; ++i) {
                rspq_block_free(blip_flash_dls[i]);
        }
}
