#include <malloc.h>

#include "engine/sfx.h"
#include "engine/util.h"

#include "game/blip_flash.h"

#define BLIP_FLASH_FRAME_CNT 9

static float blip_flash_timer;
static rspq_block_t **blip_flash_dls;

rspq_block_t **blip_create_from_info(const struct blip_frame_info *info,
                                     const int info_cnt)
{
        int i;
        rspq_block_t **blocks = calloc(info_cnt, sizeof(*blocks));

        rdpq_set_mode_fill(RGBA16(0xFF, 0xFF, 0xFF, 0xFF));

        for (i = 0; i < info_cnt; ++i) {
                const struct blip_frame_info *inf_cur;
                int j;

                inf_cur = info + i;
                rspq_block_begin();
                for (j = 0; j < inf_cur->rect_cnt; ++j)
                        rdpq_fill_rectangle(0, inf_cur->rect_y_vals[j][0],
                                            320, inf_cur->rect_y_vals[j][1]);

                blocks[i] = rspq_block_end();
        }

        return blocks;
}

void blip_flash_init(void)
{
        const struct blip_frame_info inf[BLIP_FLASH_FRAME_CNT] = {
                { 1, { {   0, 240 }, {  -1,  -1 }, {  -1,  -1 } } },
                { 2, { {  52, 145 }, { 146, 173 }, {  -1,  -1 } } },
                { 2, { {  33, 126 }, { 190, 218 }, {  -1,  -1 } } },
                { 2, { {  40,  67 }, {  80, 173 }, {  -1,  -1 } } },
                { 2, { {   0,  40 }, { 166, 194 }, {  -1,  -1 } } },
                { 2, { {  86, 114 }, { 127, 220 }, {  -1,  -1 } } },
                { 1, { { 176, 204 }, {  -1,  -1 }, {  -1,  -1 } } },
                { 1, { {  34,  62 }, {  -1,  -1 }, {  -1,  -1 } } },
                { 1, { { 144, 172 }, {  -1,  -1 }, {  -1,  -1 } } }
        };

        blip_flash_dls = blip_create_from_info(inf, BLIP_FLASH_FRAME_CNT);
}

void blip_flash_draw(void)
{
        int frame;

        frame = (int)blip_flash_timer;
        if (frame >= BLIP_FLASH_FRAME_CNT)
                return;

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

void blip_destroy(rspq_block_t ***dls, const int dl_cnt)
{
        int i;

        for (i = 0; i < dl_cnt; ++i)
                rspq_block_free((*dls)[i]);

        free(*dls);
}

void blip_flash_free(void)
{
        blip_destroy(&blip_flash_dls, BLIP_FLASH_FRAME_CNT);
}
