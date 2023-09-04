#include "engine/sfx.h"

wav64_t fan_sfx, light_sfx, door_sfx, boop_sfx, cam_up_sfx,
       cam_down_sfx, blip_sfx, cam_scan_sfx, static_sfx, title_music,
       window_scare, error_sfx, jumpscare_sfx, jumpscare_low, kitchen_sfx0,
       kitchen_sfx1, kitchen_sfx2, kitchen_sfx3, deepstep_sfx, robotvoice_sfx,
       banging_sfx, foxy_running, powerdown_sfx, musicbox_sfx, chimes_sfx,
       cheering_sfx, phonecall1, phonecall2, phonecall3, phonecall4,
       phonecall5, freddylaugh1, freddylaugh2, freddylaugh3, freddyrun_sfx,
       camglitch1, camglitch2, camglitch3, camglitch4, foxy_hum, circus_music,
       girl_giggle;

void sfx_load(void)
{
	mixer_ch_set_vol(SFXC_FAN, 0.25f, 0.25f);
	wav64_open(&fan_sfx, "rom:/fan.wav64");
	wav64_set_loop(&fan_sfx, true);

	mixer_ch_set_vol(SFXC_LIGHT, 0.8f, 0.8f);
	wav64_open(&light_sfx, "rom:/light.wav64");
	wav64_set_loop(&light_sfx, true);

	mixer_ch_set_vol(SFXC_DOOR, 0.8f, 0.8f);
	wav64_open(&door_sfx, "rom:/door.wav64");

	mixer_ch_set_vol(SFXC_BLIP, 0.8f, 0.8f);
	wav64_open(&boop_sfx, "rom:/boop.wav64");
	wav64_open(&blip_sfx, "rom:/blip.wav64");
	wav64_open(&cam_up_sfx, "rom:/cam_up.wav64");
	wav64_open(&cam_down_sfx, "rom:/cam_down.wav64");

	mixer_ch_set_vol(SFXC_CAMERA, 0.8f, 0.8f);
	wav64_open(&cam_scan_sfx, "rom:/cam_scan.wav64");

	mixer_ch_set_vol(SFXC_AMBIENCE, 0.6f, 0.6f);
	wav64_open(&title_music, "rom:/title_music.wav64");
	wav64_set_loop(&title_music, true);

	mixer_ch_set_vol(SFXC_STATIC, 0.8f, 0.8f);
	wav64_open(&static_sfx, "rom:/static.wav64");

	wav64_open(&window_scare, "rom:/windowscare.wav64");
	wav64_open(&error_sfx, "rom:/error.wav64");

	mixer_ch_set_vol(SFXC_JUMPSCARE, 0.8f, 0.8f);
	wav64_open(&jumpscare_sfx, "rom:/jumpscare.wav64");
	wav64_open(&jumpscare_low, "rom:/jumpscare_low.wav64");

	mixer_ch_set_vol(SFXC_KITCHEN, 0, 0);
	wav64_open(&kitchen_sfx0, "rom:/kitchen0.wav64");
	wav64_open(&kitchen_sfx1, "rom:/kitchen1.wav64");
	wav64_open(&kitchen_sfx2, "rom:/kitchen2.wav64");
	wav64_open(&kitchen_sfx3, "rom:/kitchen3.wav64");

	mixer_ch_set_vol(SFXC_FOOTSTEPS, 0, 0);
	wav64_open(&deepstep_sfx, "rom:/deepstep.wav64");

	mixer_ch_set_vol(SFXC_ROBOTVOICE, 0, 0);
	wav64_open(&robotvoice_sfx, "rom:/robotvoice.wav64");
	wav64_set_loop(&robotvoice_sfx, true);

	wav64_open(&banging_sfx, "rom:/banging.wav64");
	wav64_open(&foxy_running, "rom:/foxy_running.wav64");
	wav64_open(&powerdown_sfx, "rom:/powerdown.wav64");

	mixer_ch_set_vol(SFXC_MUSICBOX, 0.8f, 0.8f);
	wav64_open(&musicbox_sfx, "rom:/musicbox.wav64");
	wav64_open(&chimes_sfx, "rom:/chimes.wav64");
	wav64_open(&cheering_sfx, "rom:/cheering.wav64");

	mixer_ch_set_vol(SFXC_PHONECALL, 0.8f, 0.8f);
	wav64_open(&phonecall1, "rom:/phonecall1.wav64");
	wav64_open(&phonecall2, "rom:/phonecall2.wav64");
	wav64_open(&phonecall3, "rom:/phonecall3.wav64");
	wav64_open(&phonecall4, "rom:/phonecall4.wav64");
	wav64_open(&phonecall5, "rom:/phonecall5.wav64");

	wav64_open(&freddylaugh1, "rom:/freddylaugh1.wav64");
	wav64_open(&freddylaugh2, "rom:/freddylaugh2.wav64");
	wav64_open(&freddylaugh3, "rom:/freddylaugh3.wav64");

	mixer_ch_set_vol(SFXC_FREDDYRUN, 1, 1);
	wav64_open(&freddyrun_sfx, "rom:/freddyrun.wav64");

	wav64_open(&camglitch1, "rom:/camglitch1.wav64");
	wav64_open(&camglitch2, "rom:/camglitch2.wav64");
	wav64_open(&camglitch3, "rom:/camglitch3.wav64");
	wav64_open(&camglitch4, "rom:/camglitch4.wav64");

	mixer_ch_set_vol(SFXC_FOXSONG, 1, 1);
	wav64_open(&foxy_hum, "rom:/foxy_hum.wav64");

	wav64_open(&circus_music, "rom:/circus_music.wav64");
	wav64_open(&girl_giggle, "rom:/girl_giggle.wav64");
}

void sfx_stop_all(void)
{
	for(int i = 0; i < SFXC_COUNT; i++)
		mixer_ch_stop(i);
}
