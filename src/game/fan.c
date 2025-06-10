#include "engine/object.h"
#include "engine/util.h"
#include "engine/sfx.h"

#include "game/office.h"
#include "game/freddy.h"
#include "game/bonnie.h"
#include "game/chica.h"
#include "game/foxy.h"
#include "game/texture_index.h"
#include "game/fan.h"

#define FAN_FRAME_CNT 3

static float fan_timer;
static object_t fan_anim[FAN_FRAME_CNT];
static const char *fan_anim_paths[FAN_FRAME_CNT] = {
	TX_FAN0, TX_FAN1, TX_FAN2,
};

void fan_unload(void);

void fan_load(void)
{
	fan_timer = 0.0f;
	objects_load(fan_anim, FAN_FRAME_CNT, fan_anim_paths);
	mixer_ch_set_vol(SFX_CH_FAN, 0.25f, 0.25f);
	wav64_play(&sfx_fan, SFX_CH_FAN);
}

void fan_draw(void)
{
        /* TODO: Change these into bitflags on the game itself possibly. */
	if (foxy_is_scaring || bonnie_is_jumpscaring ||
	   chica_is_jumpscaring || freddy_is_jumpscaring)
		return;

	rdpq_set_mode_copy(false);
	object_draw(fan_anim[(int)fan_timer], 868 + office_turn, 400, 88, 97);
}

void fan_update(const float dt)
{
	fan_timer = wrapf(fan_timer + dt * speed_fps(99), FAN_FRAME_CNT, NULL);
}

void fan_unload(void)
{
	objects_unload(fan_anim, FAN_FRAME_CNT);
}
