#include "engine/graphic.h"
#include "engine/util.h"

#define GLOBAL_GRAPHICS_CNT 8

static int loaded_cnt = 0;
static int local_loaded_cnt = 0;

void graphic_load(struct graphic *o, const char *path)
{
        if (o->is_loaded) {
                return;
        }

	o->spr = sprite_load(path);
	loaded_cnt++;
	local_loaded_cnt = loaded_cnt - GLOBAL_GRAPHICS_CNT;
#ifdef GRAPHIC_DEBUG_ENABLED
        debugf("%d loaded_cnt (%s)\n", local_loaded_cnt, path);
#endif
        o->is_loaded = true;
}

void graphics_load(struct graphic *o, int num, const char **paths)
{
        int i;

	for (i = 0; i < num; ++i) {
		graphic_load(o + i, paths[i]);
        }
}

void graphic_unload(struct graphic *o)
{
        if (!o->is_loaded) {
                return;
        }

	sprite_free(o->spr);
        o->spr = NULL;
	loaded_cnt--;
	local_loaded_cnt = loaded_cnt - 8;
        o->is_loaded = false;
}

void graphics_unload(struct graphic *o, int num)
{
        int i;

	for (i = 0; i < num; ++i) {
		graphic_unload(o + i);
        }
}

void graphic_draw(struct graphic o, int px, int py,
                  int ox, int oy, const uint8_t flip_flags)
{
	rdpq_blitparms_t parms;

	px = vcon(px);
	py = vcon(py);
	ox = vcon(ox);
	oy = vcon(oy);

        parms.tile = TILE0;
        parms.s0 = 0;
        parms.t0 = 0;
        parms.width = 0;
        parms.height = 0;
        parms.flip_x = (flip_flags & GFX_FLIP_X) >> GFX_FLIP_X_SHIFT;
        parms.flip_y = (flip_flags & GFX_FLIP_Y) >> GFX_FLIP_Y_SHIFT;
        parms.cx = ox;
        parms.cy = oy;
        parms.scale_x = 1.f;
        parms.scale_y = 1.f;
        parms.theta = 0.f;
        parms.filtering = false;
        parms.nx = 0;
        parms.ny = 0;

	rdpq_sprite_blit(o.spr, px, py, &parms);
}

void graphic_draw_index_x(struct graphic o, int px, int py,
                          int w, int i, const uint8_t flip_flags)
{
	rdpq_blitparms_t parms;

	px = vcon(px);
	py = vcon(py);

        parms.tile = TILE0;
        parms.s0 = w * i;
        parms.t0 = 0;
        parms.width = w;
        parms.height = 0;
        parms.flip_x = (flip_flags & GFX_FLIP_X) >> GFX_FLIP_X_SHIFT;
        parms.flip_y = (flip_flags & GFX_FLIP_Y) >> GFX_FLIP_Y_SHIFT;
        parms.cx = 0;
        parms.cy = 0;
        parms.scale_x = 1.f;
        parms.scale_y = 1.f;
        parms.theta = 0.f;
        parms.filtering = false;
        parms.nx = 0;
        parms.ny = 0;

	rdpq_sprite_blit(o.spr, px, py, &parms);
}

void graphic_draw_index_y(struct graphic o, int px, int py,
                          int h, int i, const uint8_t flip_flags)
{
	rdpq_blitparms_t parms;

	px = vcon(px);
	py = vcon(py);

        parms.tile = TILE0;
        parms.s0 = 0;
        parms.t0 = h * i;
        parms.width = 0;
        parms.height = h;
        parms.flip_x = (flip_flags & GFX_FLIP_X) >> GFX_FLIP_X_SHIFT;
        parms.flip_y = (flip_flags & GFX_FLIP_Y) >> GFX_FLIP_Y_SHIFT;
        parms.cx = 0;
        parms.cy = 0;
        parms.scale_x = 1.f;
        parms.scale_y = 1.f;
        parms.theta = 0.f;
        parms.filtering = false;
        parms.nx = 0;
        parms.ny = 0;

	rdpq_sprite_blit(o.spr, px, py, &parms);
}
