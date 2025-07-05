#pragma once

#include <stdbool.h>

#define BLIP_FRAME_INFO_MAX_RECT_CNT 3

/*
 * This is for procedurally generating your own custom blip animations. I
 * just used this for recreating the original game's, of which there are two:
 * One is the main flash used for cameras, the title, nights ending, etc.
 * The other one is _just_ used on the title screen and periodically flashes
 * in and out. You put how many rectangles are on each frame, and out of them
 * where they start and end. The most rectangles that ever get used for
 * the base game's blip flashes are 3, and that's only from the title's one.
 */
struct blip_frame_info {
        int rect_cnt;
        int rect_y_vals[BLIP_FRAME_INFO_MAX_RECT_CNT][2];
};

rspq_block_t **blip_create_from_info(const struct blip_frame_info *info,
                                     const int info_cnt);
void blip_flash_init(void);
void blip_flash_draw(void);
void blip_flash_update(const float dt);
void blip_flash_trigger(const bool is_visual);
/* Cringe triple-pointer. :/ */
void blip_destroy(rspq_block_t ***dls, const int dl_cnt);
void blip_flash_free(void);
