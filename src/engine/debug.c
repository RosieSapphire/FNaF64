#include <malloc.h>
#include <libdragon.h>

#include "engine/debug.h"

dvalue_t *vals;
int val_cnt = 0;
rdpq_font_t *font;

void debug_load(void)
{
	vals = malloc(0);
	font = rdpq_font_load("rom:/custom/debug_font.font64");
	rdpq_font_style(font, 0, &(rdpq_fontstyle_t){
		.color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
	});
	rdpq_text_register_font(1, font);
}

void debug_add(const char *name, void *val, enum val_type type)
{
	vals = realloc(vals, ++val_cnt * sizeof(*vals));
	vals[val_cnt - 1].name = name;
	vals[val_cnt - 1].val = val;
	vals[val_cnt - 1].type = type;
}

void debug_clear(void)
{
	vals = realloc(vals, 0);
	val_cnt = 0;
}

void debug_draw(void)
{
	for(int i = 0; i < val_cnt; i++) {
		dvalue_t v = vals[i];

		char buf[128];
		if(v.type == DV_INT){
			sprintf(buf, "%s: %d\n", v.name, *(int *)v.val);
			rdpq_text_print(NULL, 1, 32, 32 + 12 * i, buf);
		}

		if(v.type == DV_FLOAT) {
			sprintf(buf, "%s: %f\n", v.name, *(float *)v.val);
			rdpq_text_print(NULL, 1, 32, 32 + 12 * i, buf);
		}

		if(v.type == DV_BOOL) {
			sprintf(buf, "%s: %s\n", v.name,
					*(bool *)v.val > 0 ? "TRUE" : "FALSE");
			rdpq_text_print(NULL, 1, 32, 32 + 12 * i, buf);
		}
	}
}
