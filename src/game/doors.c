#include "engine/object.h"
#include "engine/util.h"

#include "game/office.h"
#include "game/buttons.h"
#include "game/freddy.h"
#include "game/bonnie.h"
#include "game/chica.h"
#include "game/foxy.h"
#include "game/doors.h"

#define LEFT    0
#define RIGHT   1
#define FRAMES 15

float door_timers[2];
object_t frames[FRAMES];
const char *frame_paths[FRAMES] = {
	"rom:/left_door0.ci8.sprite",
	"rom:/left_door1.ci8.sprite",
	"rom:/left_door2.ci8.sprite",
	"rom:/left_door3.ci8.sprite",
	"rom:/left_door4.ci8.sprite",
	"rom:/left_door5.ci8.sprite",
	"rom:/left_door6.ci8.sprite",
	"rom:/left_door7.ci8.sprite",
	"rom:/left_door8.ci8.sprite",
	"rom:/left_door9.ci8.sprite",
	"rom:/left_door10.ci8.sprite",
	"rom:/left_door11.ci8.sprite",
	"rom:/left_door12.ci8.sprite",
	"rom:/left_door13.ci8.sprite",
	"rom:/left_door14.ci8.sprite",
};

void doors_load(void)
{
	objects_load(frames, FRAMES, frame_paths);
	door_timers[0] = 0;
	door_timers[1] = 0;
}

void doors_unload(void)
{
	objects_unload(frames, FRAMES);
}

void doors_draw(void)
{
	if(foxy_is_scaring || bonnie_is_jumpscaring ||
			chica_is_jumpscaring || freddy_is_jumpscaring)
		return;

	rdpq_set_mode_copy(true);
	object_draw(frames[(int)door_timers[LEFT]], 72 + office_turn, -1, 0, 0);
	rdpq_set_mode_standard();
	rdpq_mode_alphacompare(true);
	object_draw_flipped(frames[(int)door_timers[RIGHT]],
			1270 + office_turn, -2, 0, 0);
}

void doors_update(double dt)
{
	const float door_speed = speed_fps(50);
	if(button_state & BUTTON_LEFT_DOOR)
		door_timers[LEFT] += dt * door_speed;
	else
		door_timers[LEFT] -= dt * door_speed;

	if(button_state & BUTTON_RIGHT_DOOR)
		door_timers[RIGHT] += dt * door_speed;
	else
		door_timers[RIGHT] -= dt * door_speed;

	door_timers[LEFT] = clampf(door_timers[LEFT], 0, FRAMES - 1);
	door_timers[RIGHT] = clampf(door_timers[RIGHT], 0, FRAMES - 1);
}
