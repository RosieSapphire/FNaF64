#pragma once

#include <stdbool.h>
#include <libdragon.h>

struct object {
	sprite_t *spr;
	bool is_loaded;
};

void object_load(struct object *o, const char *path);
void objects_load(struct object *o, int num, const char **paths);
void object_unload(struct object *o);
void objects_unload(struct object *o, int num);
void object_draw(struct object o, int px, int py, int ox, int oy);
void object_draw_flipped(struct object o, int px, int py, int ox, int oy);
void object_draw_index_x(struct object o, int px, int py, int w, int i);
void object_draw_index_y(struct object o, int px, int py, int h, int i);
