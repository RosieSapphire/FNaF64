#ifndef ENGINE_SFX_H_
#define ENGINE_SFX_H_

#include <libdragon.h>

extern wav64_t fan_sfx, light_sfx, door_sfx, boop_sfx, cam_up_sfx, cam_down_sfx,
	blip_sfx, cam_scan_sfx, static_sfx, title_music, window_scare,
	error_sfx, jumpscare_sfx, jumpscare_low, kitchen_sfx0, kitchen_sfx1,
	kitchen_sfx2, kitchen_sfx3, deepstep_sfx, robotvoice_sfx, banging_sfx,
	foxy_running, powerdown_sfx, musicbox_sfx, chimes_sfx, cheering_sfx,
	phonecall1, phonecall2, phonecall3, phonecall4, phonecall5,
	freddylaugh1, freddylaugh2, freddylaugh3, freddyrun_sfx, camglitch1,
	camglitch2, camglitch3, camglitch4, foxy_hum, circus_music, girl_giggle;

#define AUDIO_FREQUENCY 48000
#define AUDIO_BUFFER_COUNT 3

enum sfx_channels {
	SFXC_AMBIENCE,
	SFXC_PHONECALL,
	SFXC_FAN,
	SFXC_LIGHT,
	SFXC_DOOR,
	SFXC_CAMERA,
	SFXC_BLIP,
	SFXC_STATIC,
	SFXC_JUMPSCARE,
	SFXC_KITCHEN,
	SFXC_FOOTSTEPS,
	SFXC_FREDDYRUN,
	SFXC_FREDDYLAUGH,
	SFXC_ROBOTVOICE,
	SFXC_MUSICBOX,
	SFXC_FOXSONG,
	SFXC_COUNT,
};

void sfx_load(void);
void sfx_stop_all(void);

#endif /* ENGINE_SFX_H_ */
