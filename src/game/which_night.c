#include "engine/graphic.h"
#include "engine/util.h"

#include "game/blip.h"
#include "game/game.h"
#include "game/bonnie.h"
#include "game/chica.h"
#include "game/foxy.h"
#include "game/freddy.h"
#include "game/texture_index.h"
#include "game/save_data.h"
#include "game/which_night.h"

#define WHICH_NIGHT_TIMER_FADEOUT (70.f / 60.f)
#define WHICH_NIGHT_TIMER_END (130.f / 60.f)

static const int which_night_ai_lut[6][4] = {
	{ 0,  0,  0,  0},
	{ 3,  1,  1,  0},
	{ 0,  5,  2,  1},
        /*
         * FIXME: On night 4, Freddy is supposed to have a random 50%
         * chance of his AI either being 1 or 2.
         */
	{ 2,  4,  6,  2},
	{ 5,  7,  5,  3},
	{10, 12,  6,  4},
};

static struct graphic which_night_gfx_atlas;
static float which_night_timer;
static bool  which_night_is_loaded = false;

static void which_night_load(void)
{
        int night_cur;

        /* Graphics */
	graphic_load(&which_night_gfx_atlas, TX_WHICH_NIGHT_ATLAS);

	blip_trigger(true);
	which_night_is_loaded = true;

        /* Variables */
	which_night_timer = 0.f;

        /* If we're doing custom night, don't change the AI. */
        night_cur = SAVE_NIGHT_NUM(save_data);
	if (night_cur == 7) {
		return;
        }

        /* AI Change based on night. */
	bonnie_ai_level   = which_night_ai_lut[night_cur - 1][0];
	chica_ai_level    = which_night_ai_lut[night_cur - 1][1];
	foxy_ai_level     = which_night_ai_lut[night_cur - 1][2];
	freddy_ai_level   = which_night_ai_lut[night_cur - 1][3];
}

static void which_night_unload(void)
{
	graphic_unload(&which_night_gfx_atlas);
	which_night_is_loaded = false;
}

void which_night_draw(void)
{
        float fade_alpha;

        if (!which_night_is_loaded) {
	        which_night_load();
        }

	rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xFF));
	rdpq_fill_rectangle(0, 0, 320, 240);
	rdpq_set_mode_standard();
	graphic_draw_index_y(which_night_gfx_atlas, 372, 270,
                             11, 0, GFX_FLIP_NONE);
	graphic_draw_index_y(which_night_gfx_atlas, 373, 336, 11,
                             SAVE_NIGHT_NUM(save_data), GFX_FLIP_NONE);

        fade_alpha =
                CLAMP((which_night_timer - WHICH_NIGHT_TIMER_FADEOUT), 0, 1);
	rdpq_set_prim_color(RGBA32(0x0, 0x0, 0x0, fade_alpha * 0xFF));
	rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	rdpq_fill_rectangle(0, 0, 320, 240);

        blip_draw();
}

enum scene which_night_update(struct update_params uparms)
{
	which_night_timer += uparms.dt;

	if (which_night_timer >= WHICH_NIGHT_TIMER_END) {
                which_night_unload();
		return SCENE_MAIN_GAME;
	}

	return SCENE_WHICH_NIGHT;
}
