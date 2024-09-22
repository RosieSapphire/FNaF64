#include <libdragon.h>

#include "config.h"
#include "debug_view.h"

#define DEBUG_MODE 0

#if DEBUG_MODE
#define DEBUG_VIEW_FONT_ID 3
#define DEBUG_VIEW_PADDING 32
#define DEBUG_VIEW_BUFFERS_MAX 256
#define DEBUG_VIEW_BUFFER_LENGTH 256

static rdpq_font_t *debug_font;
static uint8_t debug_buffers_in_use;
static char debug_buffers[DEBUG_VIEW_BUFFERS_MAX][DEBUG_VIEW_BUFFER_LENGTH];
static int debug_view_is_visible = 1;
static float debug_blink_timer = 0.0f;
#endif /* DEBUG_MODE */

void debug_view_init(void)
{
#if DEBUG_MODE
	debug_font = rdpq_font_load("rom:/custom/debug_font.font64");
	rdpq_font_style(debug_font, 0,
			&(rdpq_fontstyle_t){
				.color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
			});
	rdpq_text_register_font(DEBUG_VIEW_FONT_ID, debug_font);
#endif /* DEBUG_MODE */
}

#if DEBUG_MODE
void debug_view_push(const char *label, const void *value, const int type)
{
	/*
	static const char *fmts[DEBUG_VALUE_COUNT] = {
		"%s: %d",
		"%s: %f",
	};
	*/
	switch (type) {
	case DEBUG_VALUE_INT:
		snprintf(debug_buffers[debug_buffers_in_use++],
			 DEBUG_VIEW_BUFFER_LENGTH, "%s: %d\n", label,
			 *((int *)value));
		break;
	case DEBUG_VALUE_FLOAT:
		snprintf(debug_buffers[debug_buffers_in_use++],
			 DEBUG_VIEW_BUFFER_LENGTH, "%s: %f\n", label,
			 *((float *)value));
		break;
	default:
		exit(EXIT_FAILURE);
	}
}
#else
void debug_view_push(__attribute__((unused)) const char *label,
		     __attribute__((unused)) const void *value,
		     __attribute__((unused)) const int type)
{
}
#endif /* DEBUG_MODE */

#if DEBUG_MODE
void debug_view_update(const int toggle_is_pressed, const float dt)
{
	debug_view_is_visible ^= toggle_is_pressed;
	debug_blink_timer += dt * 0.75f;
	debug_blink_timer -= (int)debug_blink_timer;
}
#else
void debug_view_update(__attribute__((unused))const int toggle_is_pressed,
                       __attribute__((unused))const float dt)
{
}
#endif /* DEBUG_MODE */

void debug_view_draw(void)
{
#if DEBUG_MODE
	const rdpq_textparms_t parms = {
		.align = ALIGN_LEFT,
		.valign = VALIGN_TOP,
		.width = DISPLAY_WIDTH - (DEBUG_VIEW_PADDING << 1),
		.height = DISPLAY_HEIGHT - (DEBUG_VIEW_PADDING << 1),
	};

	/* can't draw shit if it's disabled :/ */
	if (!debug_view_is_visible) {
		debug_buffers_in_use = 0;
		goto skip_debug_values;
	}

	/* background boxes */
	rdpq_set_mode_standard();
	rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY_CONST);
	rdpq_set_prim_color(RGBA32(0x0, 0x0, 0x0, 0x0));
	rdpq_set_fog_color(RGBA32(0x0, 0x0, 0x0, 0xC0));
	for (int i = 0; i < debug_buffers_in_use; i++) {
		rdpq_fill_rectangle(DEBUG_VIEW_PADDING - 4,
				    DEBUG_VIEW_PADDING + (16 * i),
				    DISPLAY_WIDTH - DEBUG_VIEW_PADDING,
				    DEBUG_VIEW_PADDING + 16 + (16 * i));
	}

	/* actual text */
	for (int i = 0; i < debug_buffers_in_use; i++) {
		rdpq_text_printf(&parms, DEBUG_VIEW_FONT_ID, DEBUG_VIEW_PADDING,
				 DEBUG_VIEW_PADDING + (16 * i),
				 debug_buffers[i]);
	}

	/* let the user know we are using a debug build */
skip_debug_values:
	if (debug_blink_timer < 0.5f) {
		debug_buffers_in_use = 0;
		return;
	}
	const int debug_build_text_pos[2] = { DISPLAY_WIDTH - 64,
					      DISPLAY_HEIGHT - 48 };
	rdpq_set_mode_standard();
	rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
	rdpq_set_prim_color(RGBA32(0x7F, 0x0, 0x0, 0x0));
	rdpq_fill_rectangle(debug_build_text_pos[0] - 5,
			    debug_build_text_pos[1],
			    debug_build_text_pos[0] + 32,
			    debug_build_text_pos[1] + 18);
	rdpq_text_printf(&parms, DEBUG_VIEW_FONT_ID, debug_build_text_pos[0],
			 debug_build_text_pos[1], "DEBUG");

	/* reset buffers for next frame */
	debug_buffers_in_use = 0;
#endif /* DEBUG_MODE */
}

void debug_view_terminate(void)
{
#if DEBUG_MODE
	rdpq_font_free(debug_font);
#endif /* DEBUG_MODE */
}
