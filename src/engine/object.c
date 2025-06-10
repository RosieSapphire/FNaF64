#include "engine/object.h"
#include "engine/util.h"

static int loaded = 0;
static int local_loaded = 0;

void object_load(struct object *o, const char *path)
{
	if (o->is_loaded)
		return;
	o->spr = sprite_load(path);
	o->is_loaded = true;
	loaded++;
	local_loaded = loaded - 8;
	debugf("%d loaded (%s)\n", local_loaded, path);
}

void objects_load(struct object *o, int num, const char **paths)
{
        int i;

	for (i = 0; i < num; ++i) {
		object_load(o + i, paths[i]);
        }
}

void object_unload(struct object *o)
{
	if (!o->is_loaded)
		return;
	sprite_free(o->spr);
	o->is_loaded = false;
	loaded--;
	local_loaded = loaded - 8;
}

void objects_unload(struct object *o, int num)
{
        int i;

	for (i = 0; i < num; ++i) {
		object_unload(o + i);
        }
}

void object_draw(struct object o, int px, int py, int ox, int oy)
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

void object_draw_flipped(struct object o, int px, int py, int ox, int oy)
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

void object_draw_index_x(struct object o, int px, int py, int w, int i)
{
	px = vcon(px);
	py = vcon(py);

	const rdpq_blitparms_t parms = {
		.s0 = w * i,
		.width = w,
	};

	rdpq_sprite_blit(o.spr, px, py, &parms);
}

void object_draw_index_y(struct object o, int px, int py, int h, int i)
{
	px = vcon(px);
	py = vcon(py);

	const rdpq_blitparms_t parms = {
		.t0 = h * i,
		.height = h,
	};

	rdpq_sprite_blit(o.spr, px, py, &parms);
}
