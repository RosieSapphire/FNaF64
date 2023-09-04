#include "engine/object.h"
#include "engine/util.h"
#include "engine/sfx.h"

#include "game/office.h"
#include "game/freddy.h"
#include "game/bonnie.h"
#include "game/chica.h"
#include "game/foxy.h"
#include "game/fan.h"

#define ANIM_FRAMES 3

static float timer;
static object_t anim[ANIM_FRAMES];
static const char *anim_paths[ANIM_FRAMES] = {
	"rom:/fan0.ci8.sprite",
	"rom:/fan1.ci8.sprite",
	"rom:/fan2.ci8.sprite",
};

void fan_load(void)
{
	timer = 0.0f;
	objects_load(anim, ANIM_FRAMES, anim_paths);
	mixer_ch_set_vol(SFXC_FAN, 0.25f, 0.25f);
	wav64_play(&fan_sfx, SFXC_FAN);
}

void fan_unload(void)
{
	objects_unload(anim, ANIM_FRAMES);
}

void fan_draw(void)
{
	if(foxy_is_scaring || bonnie_is_jumpscaring ||
			chica_is_jumpscaring || freddy_is_jumpscaring)
		return;

	rdpq_set_mode_copy(false);
	object_draw(anim[(int)timer], 868 + office_turn, 400, 88, 97);
}

void fan_update(double dt)
{
	timer = wrapf(timer + dt * speed_fps(99), ANIM_FRAMES, NULL);
}
