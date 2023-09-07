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
	wav64_open(&fan_sfx, "rom:/Buzz_Fan_Florescent2.wav64");
	wav64_set_loop(&fan_sfx, true);

	mixer_ch_set_vol(SFXC_LIGHT, 0.8f, 0.8f);
	wav64_open(&light_sfx, "rom:/BallastHumMedium2.wav64");
	wav64_set_loop(&light_sfx, true);

	mixer_ch_set_vol(SFXC_DOOR, 0.8f, 0.8f);
	wav64_open(&door_sfx, "rom:/SFXBible_12478.wav64");

	mixer_ch_set_vol(SFXC_BLIP, 0.8f, 0.8f);
	wav64_open(&boop_sfx, "rom:/PartyFavorraspyPart_AC01__3.wav64");
	wav64_open(&blip_sfx, "rom:/blip3.wav64");
	wav64_open(&cam_up_sfx, "rom:/CAMERA_VIDEO_LOA_60105303.wav64");
	wav64_open(&cam_down_sfx, "rom:/put_down.wav64");

	mixer_ch_set_vol(SFXC_CAMERA, 0.8f, 0.8f);
	wav64_open(&cam_scan_sfx, "rom:/MiniDV_Tape_Eject_1.wav64");

	mixer_ch_set_vol(SFXC_AMBIENCE, 0.6f, 0.6f);
	wav64_open(&title_music, "rom:/darkness_music.wav64");
	wav64_set_loop(&title_music, true);

	mixer_ch_set_vol(SFXC_STATIC, 0.8f, 0.8f);
	wav64_open(&static_sfx, "rom:/static2.wav64");

	wav64_open(&window_scare, "rom:/windowscare.wav64");
	wav64_open(&error_sfx, "rom:/error.wav64");

	mixer_ch_set_vol(SFXC_JUMPSCARE, 0.8f, 0.8f);
	wav64_open(&jumpscare_sfx, "rom:/XSCREAM.wav64");
	wav64_open(&jumpscare_low, "rom:/XSCREAM2.wav64");

	mixer_ch_set_vol(SFXC_KITCHEN, 0, 0);
	wav64_open(&kitchen_sfx0, "rom:/OVEN-DRA_1_GEN-HDF18119.wav64");
	wav64_open(&kitchen_sfx1, "rom:/OVEN-DRA_2_GEN-HDF18120.wav64");
	wav64_open(&kitchen_sfx2, "rom:/OVEN-DRA_7_GEN-HDF18121.wav64");
	wav64_open(&kitchen_sfx3, "rom:/OVEN-DRAWE_GEN-HDF18122.wav64");

	mixer_ch_set_vol(SFXC_FOOTSTEPS, 0, 0);
	wav64_open(&deepstep_sfx, "rom:/deep_steps.wav64");

	mixer_ch_set_vol(SFXC_ROBOTVOICE, 0, 0);
	wav64_open(&robotvoice_sfx, "rom:/robotvoice.wav64");
	wav64_set_loop(&robotvoice_sfx, true);

	wav64_open(&banging_sfx, "rom:/knock2.wav64");
	wav64_open(&foxy_running, "rom:/run.wav64");
	wav64_open(&powerdown_sfx, "rom:/powerdown.wav64");

	mixer_ch_set_vol(SFXC_MUSICBOX, 0.8f, 0.8f);
	wav64_open(&musicbox_sfx, "rom:/music_box.wav64");
	wav64_open(&chimes_sfx, "rom:/chimes_2.wav64");
	wav64_open(&cheering_sfx, "rom:/CROWD_SMALL_CHIL_EC049202.wav64");

	mixer_ch_set_vol(SFXC_PHONECALL, 0.8f, 0.8f);
	wav64_open(&phonecall1, "rom:/voiceover1c.wav64");
	wav64_open(&phonecall2, "rom:/voiceover2a.wav64");
	wav64_open(&phonecall3, "rom:/voiceover3.wav64");
	wav64_open(&phonecall4, "rom:/voiceover4.wav64");
	wav64_open(&phonecall5, "rom:/voiceover5.wav64");

	wav64_open(&freddylaugh1, "rom:/Laugh_Giggle_Girl_1d.wav64");
	wav64_open(&freddylaugh2, "rom:/Laugh_Giggle_Girl_2d.wav64");
	wav64_open(&freddylaugh3, "rom:/Laugh_Giggle_Girl_8d.wav64");

	mixer_ch_set_vol(SFXC_FREDDYRUN, 1, 1);
	wav64_open(&freddyrun_sfx, "rom:/running_fast3.wav64");

	wav64_open(&camglitch1, "rom:/COMPUTER_DIGITAL_L2076505.wav64");
	wav64_open(&camglitch2, "rom:/garble1.wav64");
	wav64_open(&camglitch3, "rom:/garble2.wav64");
	wav64_open(&camglitch4, "rom:/garble3.wav64");

	mixer_ch_set_vol(SFXC_FOXSONG, 1, 1);
	wav64_open(&foxy_hum, "rom:/pirate_song2.wav64");

	wav64_open(&circus_music, "rom:/circus.wav64");
	wav64_open(&girl_giggle, "rom:/Laugh_Giggle_Girl_1.wav64");
}

void sfx_stop_all(void)
{
	for(int i = 0; i < SFXC_COUNT; i++)
		mixer_ch_stop(i);
}
