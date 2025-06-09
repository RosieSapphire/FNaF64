#pragma once

#include <libdragon.h>

extern wav64_t fan_sfx, light_sfx, door_sfx, boop_sfx, cam_up_sfx,
               cam_down_sfx, blip_sfx, cam_scan_sfx, static_sfx, title_music,
               window_scare, error_sfx, jumpscare_sfx, jumpscare_low,
               kitchen_sfx0, kitchen_sfx1, kitchen_sfx2, kitchen_sfx3,
               deepstep_sfx, robotvoice_sfx, banging_sfx, foxy_running,
               powerdown_sfx, musicbox_sfx, chimes_sfx, cheering_sfx,
               phonecall1, phonecall2, phonecall3, phonecall4, phonecall5,
               freddylaugh1, freddylaugh2, freddylaugh3, freddyrun_sfx,
               camglitch1, camglitch2, camglitch3, camglitch4, foxy_hum,
               circus_music, girl_giggle;

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
