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

struct graphic {
	sprite_t *spr;
        bool is_loaded;
};

void graphic_load(struct graphic *o, const char *path);
void graphics_load(struct graphic *o, int num, const char **paths);
void graphic_unload(struct graphic *o);
void graphics_unload(struct graphic *o, int num);
void graphic_draw(struct graphic o, int px, int py,
                  int ox, int oy, const uint8_t flip_flags);
void graphic_draw_index_x(struct graphic o, int px, int py,
                          int w, int i, const uint8_t flip_flags);
void graphic_draw_index_y(struct graphic o, int px, int py,
                          int h, int i, const uint8_t flip_flags);
