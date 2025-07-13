#pragma once

// #define GRAPHIC_DEBUG_ENABLED

#include <stdbool.h>
#include <libdragon.h>

enum {
        GFX_FLIP_X_SHIFT,
        GFX_FLIP_Y_SHIFT
};

enum {
        GFX_FLIP_NONE = 0,
        GFX_FLIP_X    = (1 << GFX_FLIP_X_SHIFT),
        GFX_FLIP_Y    = (1 << GFX_FLIP_Y_SHIFT)
};

enum {
        GFX_IS_LOADED_SHIFT,
        GFX_SCALE_DOWN_SHIFT,
        GFX_FLAG_CNT
};

enum {
        GFX_IS_LOADED  = (1 << GFX_IS_LOADED_SHIFT),
        GFX_SCALE_DOWN = (1 << GFX_SCALE_DOWN_SHIFT)
};

struct graphic {
        sprite_t *spr;
        uint8_t flags;
};

void graphic_load(struct graphic *gfx, const char *path, const bool scale_down);
void graphics_load(struct graphic *arr, const int cnt,
                   const char **paths, const bool scale_down);
void graphic_unload(struct graphic *gfx);
void graphics_unload(struct graphic *arr, const int cnt);
void graphic_draw(const struct graphic *gfx, const int px, const int py,
                  const int ox, const int oy, const uint8_t flip_flags);
void graphic_draw_index_x(const struct graphic *gfx, const int px, const int py,
                          const int w, const int i, const uint8_t flip_flags);
void graphic_draw_index_y(const struct graphic *gfx, const int px, const int py,
                          const int h, const int i, const uint8_t flip_flags);
