#ifndef ENGINE_OBJECT_H_
#define ENGINE_OBJECT_H_

#include <stdbool.h>
#include <libdragon.h>

typedef struct {
	sprite_t *spr;
	bool is_loaded;
} object_t;

void object_load(object_t *o, const char *path);
void objects_load(object_t *o, int num, const char **paths);
void object_unload(object_t *o);
void objects_unload(object_t *o, int num);
void object_draw(object_t o, int px, int py, int ox, int oy);
void object_draw_flipped(object_t o, int px, int py, int ox, int oy);
void object_draw_index_x(object_t o, int px, int py, int w, int i);
void object_draw_index_y(object_t o, int px, int py, int h, int i);

#endif /* ENGINE_OBJECT_H_ */
