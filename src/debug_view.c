#include <libdragon.h>

#include "config.h"
#include "debug_view.h"

#define DEBUG_VIEW_FONT_ID 3
#define DEBUG_VIEW_PADDING 32
#define DEBUG_VIEW_BUFFERS_MAX 256
#define DEBUG_VIEW_BUFFER_LENGTH 256

static rdpq_font_t *debug_font;
static uint8_t debug_buffers_count;
static char debug_buffers[DEBUG_VIEW_BUFFERS_MAX][DEBUG_VIEW_BUFFER_LENGTH];

void debug_view_init(void)
{
	debug_font = rdpq_font_load("rom:/custom/debug_font.font64");
	rdpq_font_style(debug_font, 0,
			&(rdpq_fontstyle_t){
				.color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
			});
	rdpq_text_register_font(DEBUG_VIEW_FONT_ID, debug_font);
}

void debug_push_int(const char *label, const int value)
{
	snprintf(debug_buffers[debug_buffers_count++], DEBUG_VIEW_BUFFER_LENGTH,
		 "%s: %d\n", label, value);
}

void debug_view_values_draw(void)
{
	const rdpq_textparms_t parms = {
		.align = ALIGN_LEFT,
		.valign = VALIGN_TOP,
		.width = DISPLAY_WIDTH - (DEBUG_VIEW_PADDING << 1),
		.height = DISPLAY_HEIGHT - (DEBUG_VIEW_PADDING << 1),
	};
	/* background boxes */
	rdpq_set_mode_standard();
	rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY_CONST);
	rdpq_set_prim_color(RGBA32(0x0, 0x0, 0x0, 0x0));
	rdpq_set_fog_color(RGBA32(0x0, 0x0, 0x0, 0x7F));
	for (int i = 0; i < debug_buffers_count; i++) {
		rdpq_fill_rectangle(DEBUG_VIEW_PADDING - 4,
				    DEBUG_VIEW_PADDING + (16 * i),
				    DISPLAY_WIDTH - DEBUG_VIEW_PADDING,
				    DEBUG_VIEW_PADDING + 16 + (16 * i));
	}
	/* actual text */
	for (int i = 0; i < debug_buffers_count; i++) {
		rdpq_text_printf(&parms, DEBUG_VIEW_FONT_ID, DEBUG_VIEW_PADDING,
				 DEBUG_VIEW_PADDING + (16 * i),
				 debug_buffers[i]);
	}
	debug_buffers_count = 0;
}

void debug_view_terminate(void)
{
	rdpq_font_free(debug_font);
}
