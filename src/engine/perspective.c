#include <math.h>
#include <libdragon.h>

#include "engine/perspective.h"

static surface_t persp_buffer;
static rspq_block_t *persp_dl;

void perspective_init(void)
{
        int div_cnt, div_width, i;

	persp_buffer = surface_alloc(FMT_RGBA16, 320, 240);
	rspq_block_begin();
	rdpq_set_mode_standard();
	div_cnt = 40;
	div_width = 320 / div_cnt;
	for(i = 0; i < div_cnt; i++) {
		float i_dist, scale_y;
                rdpq_blitparms_t params;

		i_dist = fabs((float)(i - (div_cnt >> 1)));
		scale_y = (i_dist * 0.032f);
		scale_y *= scale_y;
		scale_y += 1;

                params.tile = TILE0;
                params.s0 = div_width * i;
                params.t0 = 0;
                params.width = div_width;
                params.height = 0;
                params.flip_x = false;
                params.flip_y = false;
                params.cx = 0;
                params.cy = 120;
                params.scale_x = 1;
                params.scale_y = scale_y;
                params.theta = 0.f;
                params.filtering = false;
                params.nx = 0;
                params.ny = 0;
		rdpq_tex_blit(&persp_buffer, div_width * i, 120, &params);
	}
	persp_dl = rspq_block_end();
}

void perspective_begin(void)
{
	rdpq_attach(&persp_buffer, NULL);
}

void perspective_end(void)
{
	rdpq_detach();
	rspq_block_run(persp_dl);
}

void perspective_free(void)
{
	surface_free(&persp_buffer);
        rspq_block_free(persp_dl);
}
