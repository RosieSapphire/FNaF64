#include "engine/object.h"
#include "engine/util.h"

static int loaded = 0;
static int local_loaded = 0;

void object_load(object_t *o, const char *path)
{
	if(o->is_loaded)
		return;
	o->spr = sprite_load(path);
	o->is_loaded = true;
	loaded++;
	local_loaded = loaded - 8;
}

void objects_load(object_t *o, int num, const char **paths)
{
	for(int i = 0; i < num; i++)
		object_load(o + i, paths[i]);
}

void object_unload(object_t *o)
{
	if(!o->is_loaded)
		return;
	sprite_free(o->spr);
	o->is_loaded = false;
	loaded--;
	local_loaded = loaded - 8;
}

void objects_unload(object_t *o, int num)
{
	for(int i = 0; i < num; i++)
		object_unload(o + i);
}

void object_draw(object_t o, int px, int py, int ox, int oy)
{
	px = vcon(px);
	py = vcon(py);
	ox = vcon(ox);
	oy = vcon(oy);

	const rdpq_blitparms_t parms = {
		.cx = ox, .cy = oy,
	};

	rdpq_sprite_blit(o.spr, px, py, &parms);
}

void object_draw_flipped(object_t o, int px, int py, int ox, int oy)
{
	px = vcon(px);
	py = vcon(py);
	ox = vcon(ox);
	oy = vcon(oy);

	const rdpq_blitparms_t parms = {
		.flip_x = true,
		.cx = ox, .cy = oy,
	};

	rdpq_sprite_blit(o.spr, px, py, &parms);
}

void object_draw_index_x(object_t o, int px, int py, int w, int i)
{
	px = vcon(px);
	py = vcon(py);

	const rdpq_blitparms_t parms = {
		.s0 = w * i,
		.width = w,
	};

	rdpq_sprite_blit(o.spr, px, py, &parms);
}

void object_draw_index_y(object_t o, int px, int py, int h, int i)
{
	px = vcon(px);
	py = vcon(py);

	const rdpq_blitparms_t parms = {
		.t0 = h * i,
		.height = h,
	};

	rdpq_sprite_blit(o.spr, px, py, &parms);
}
