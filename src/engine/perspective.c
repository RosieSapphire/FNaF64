#include <math.h>
#include <libdragon.h>

#include "engine/perspective.h"

static surface_t persp_buffer;
static rspq_block_t *persp_block;

void perspective_init(void)
{
	persp_buffer = surface_alloc(FMT_RGBA16, 320, 240);
	rspq_block_begin();
	rdpq_set_mode_standard();
	int num_divs = 40;
	int div_width = 320 / num_divs;
	for (int i = 0; i < num_divs; i++) {
		float i_dist = fabs((float)(i - (num_divs >> 1)));
		float scale_y = (i_dist * 0.032f);
		scale_y *= scale_y;
		scale_y += 1;
		const rdpq_blitparms_t parms = {
			.width = div_width,
			.s0 = div_width * i,
			.scale_y = scale_y,
			.cy = 120,
		};
		rdpq_tex_blit(&persp_buffer, div_width * i, 120, &parms);
	}
	persp_block = rspq_block_end();
}

void perspective_begin(void)
{
	rdpq_attach(&persp_buffer, NULL);
}

void perspective_end(void)
{
	rdpq_detach();
	rspq_block_run(persp_block);
}
