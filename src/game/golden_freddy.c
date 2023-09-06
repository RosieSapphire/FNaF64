#include <stdlib.h>

#include "engine/object.h"
#include "engine/util.h"
#include "engine/sfx.h"

#include "game/office.h"
#include "game/camera.h"
#include "game/hallucinations.h"
#include "game/bonnie.h"
#include "game/golden_freddy.h"

static float attempt_timer;
static object_t yellow_bear;
static object_t yellow_scare;
static float jumpscare_timer;

int golden_freddy_progress;

void golden_freddy_load(void)
{
	attempt_timer = 0;
	object_load(&yellow_bear, "rom:/yellow_bear.ci8.sprite");
	object_load(&yellow_scare, "rom:/yellow_scare.ci8.sprite");
	jumpscare_timer = 0.0f;
	golden_freddy_progress = 0;
}

void golden_freddy_draw_in_room(void)
{
	if(golden_freddy_progress != 3)
		return;

	rdpq_set_mode_copy(true);
	object_draw(yellow_bear, 660 + office_turn, 478, 270, 260);
}

void golden_freddy_draw_scare(void)
{
	rdpq_set_mode_copy(true);
	object_draw(yellow_scare, 0, 0, 0, 0);
}

void golden_freddy_update(double dt)
{
	switch(golden_freddy_progress) {
	case 0:
		bool attempt;
		attempt_timer = wrapf(attempt_timer + dt, 1.0f, &attempt);
		if(attempt && (rand() % 100000) == 1) {
			golden_freddy_progress = 1;
		}
		return;

	case 1:
		if(cam_selected == CAM_2B && camera_is_visible &&
				bonnie_cam != CAM_2B) {
			golden_freddy_progress = 2;
			mixer_ch_set_vol(SFXC_FREDDYLAUGH, 1.0f, 1.0f);
       			wav64_play(&girl_giggle, SFXC_FREDDYLAUGH);
		}

		return;

	case 2:
		if(!camera_is_visible) {
			hallucinations_trigger();
			golden_freddy_progress = 3;
			return;
		}

		return;

	case 3:
		if(camera_is_visible)
			golden_freddy_progress = 5;

		jumpscare_timer += dt * 60;
		if(jumpscare_timer >= 300) {
			sfx_stop_all();
			wav64_play(&jumpscare_low, SFXC_JUMPSCARE);
			golden_freddy_progress++;
			jumpscare_timer = 0;
		}
		return;

	case 4:
		jumpscare_timer += dt;
		if(jumpscare_timer >= 1.5f)
			golden_freddy_progress = 6;
		return;
	
	default:
		return;
	}
}

void golden_freddy_unload(void)
{
	object_unload(&yellow_scare);
	object_unload(&yellow_bear);
}
