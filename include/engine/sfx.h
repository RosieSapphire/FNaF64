#pragma once

#include <libdragon.h>

extern wav64_t sfx_fan;
extern wav64_t sfx_light;
extern wav64_t sfx_door;
extern wav64_t sfx_boop;
extern wav64_t sfx_cam_up;
extern wav64_t sfx_cam_down;
extern wav64_t sfx_blip;
extern wav64_t sfx_cam_scan;
extern wav64_t sfx_static;
extern wav64_t sfx_title_music;
extern wav64_t sfx_window_scare;
extern wav64_t sfx_error;
extern wav64_t sfx_jumpscare;
extern wav64_t sfx_jumpscare_low;
extern wav64_t sfx_kitchen_0;
extern wav64_t sfx_kitchen_1;
extern wav64_t sfx_kitchen_2;
extern wav64_t sfx_kitchen_3;
extern wav64_t sfx_deep_step;
extern wav64_t sfx_robot_voice;
extern wav64_t sfx_banging;
extern wav64_t sfx_foxy_running;
extern wav64_t sfx_power_down;
extern wav64_t sfx_music_box;
extern wav64_t sfx_chimes;
extern wav64_t sfx_cheering;
extern wav64_t sfx_phone_call_1;
extern wav64_t sfx_phone_call_2;
extern wav64_t sfx_phone_call_3;
extern wav64_t sfx_phone_call_4;
extern wav64_t sfx_phone_call_5;
extern wav64_t sfx_freddy_laugh_1;
extern wav64_t sfx_freddy_laugh_2;
extern wav64_t sfx_freddy_laugh_3;
extern wav64_t sfx_freddy_run;
extern wav64_t sfx_cam_glitch_1;
extern wav64_t sfx_cam_glitch_2;
extern wav64_t sfx_cam_glitch_3;
extern wav64_t sfx_cam_glitch_4;
extern wav64_t sfx_foxy_hum;
extern wav64_t sfx_circus_music;
extern wav64_t sfx_girl_giggle;

enum sfx_channels {
	SFX_CH_AMBIENCE,
	SFX_CH_PHONECALL,
	SFX_CH_FAN,
	SFX_CH_LIGHT,
	SFX_CH_DOOR,
	SFX_CH_CAMERA,
	SFX_CH_BLIP,
	SFX_CH_STATIC,
	SFX_CH_JUMPSCARE,
	SFX_CH_KITCHEN,
	SFX_CH_FOOTSTEPS,
	SFX_CH_FREDDYRUN,
	SFX_CH_FREDDYLAUGH,
	SFX_CH_ROBOTVOICE,
	SFX_CH_MUSICBOX,
	SFX_CH_FOXSONG,
	SFX_CH_CNT
};

void sfx_load(void);
void sfx_stop_all_channels(void);
void sfx_unload(void);
