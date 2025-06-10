#include "engine/sfx.h"

wav64_t sfx_fan;
wav64_t sfx_light;
wav64_t sfx_door;
wav64_t sfx_boop;
wav64_t sfx_cam_up;
wav64_t sfx_cam_down;
wav64_t sfx_blip;
wav64_t sfx_cam_scan;
wav64_t sfx_static;
wav64_t sfx_title_music;
wav64_t sfx_window_scare;
wav64_t sfx_error;
wav64_t sfx_jumpscare;
wav64_t sfx_jumpscare_low;
wav64_t sfx_kitchen_0;
wav64_t sfx_kitchen_1;
wav64_t sfx_kitchen_2;
wav64_t sfx_kitchen_3;
wav64_t sfx_deep_step;
wav64_t sfx_robot_voice;
wav64_t sfx_banging;
wav64_t sfx_foxy_running;
wav64_t sfx_power_down;
wav64_t sfx_music_box;
wav64_t sfx_chimes;
wav64_t sfx_cheering;
wav64_t sfx_phone_call_1;
wav64_t sfx_phone_call_2;
wav64_t sfx_phone_call_3;
wav64_t sfx_phone_call_4;
wav64_t sfx_phone_call_5;
wav64_t sfx_freddy_laugh_1;
wav64_t sfx_freddy_laugh_2;
wav64_t sfx_freddy_laugh_3;
wav64_t sfx_freddy_run;
wav64_t sfx_cam_glitch_1;
wav64_t sfx_cam_glitch_2;
wav64_t sfx_cam_glitch_3;
wav64_t sfx_cam_glitch_4;
wav64_t sfx_foxy_hum;
wav64_t sfx_circus_music;
wav64_t sfx_girl_giggle;

void sfx_load(void)
{
	mixer_ch_set_vol(SFX_CH_FAN, 0.25f, 0.25f);
	wav64_open(&sfx_fan, "rom:/Buzz_Fan_Florescent2.wav64");
	wav64_set_loop(&sfx_fan, true);

	mixer_ch_set_vol(SFX_CH_LIGHT, 0.8f, 0.8f);
	wav64_open(&sfx_light, "rom:/BallastHumMedium2.wav64");
	wav64_set_loop(&sfx_light, true);

	mixer_ch_set_vol(SFX_CH_DOOR, 0.8f, 0.8f);
	wav64_open(&sfx_door, "rom:/SFXBible_12478.wav64");

	mixer_ch_set_vol(SFX_CH_BLIP, 0.8f, 0.8f);
	wav64_open(&sfx_boop, "rom:/PartyFavorraspyPart_AC01__3.wav64");
	wav64_open(&sfx_blip, "rom:/blip3.wav64");
	wav64_open(&sfx_cam_up, "rom:/CAMERA_VIDEO_LOA_60105303.wav64");
	wav64_open(&sfx_cam_down, "rom:/put_down.wav64");

	mixer_ch_set_vol(SFX_CH_CAMERA, 0.8f, 0.8f);
	wav64_open(&sfx_cam_scan, "rom:/MiniDV_Tape_Eject_1.wav64");

	mixer_ch_set_vol(SFX_CH_AMBIENCE, 0.6f, 0.6f);
	wav64_open(&sfx_title_music, "rom:/darkness_music.wav64");
	wav64_set_loop(&sfx_title_music, true);

	mixer_ch_set_vol(SFX_CH_STATIC, 0.8f, 0.8f);
	wav64_open(&sfx_static, "rom:/static2.wav64");

	wav64_open(&sfx_window_scare, "rom:/windowscare.wav64");
	wav64_open(&sfx_error, "rom:/error.wav64");

	mixer_ch_set_vol(SFX_CH_JUMPSCARE, 0.8f, 0.8f);
	wav64_open(&sfx_jumpscare, "rom:/XSCREAM.wav64");
	wav64_open(&sfx_jumpscare_low, "rom:/XSCREAM2.wav64");

	mixer_ch_set_vol(SFX_CH_KITCHEN, 0, 0);
	wav64_open(&sfx_kitchen_0, "rom:/OVEN-DRA_1_GEN-HDF18119.wav64");
	wav64_open(&sfx_kitchen_1, "rom:/OVEN-DRA_2_GEN-HDF18120.wav64");
	wav64_open(&sfx_kitchen_2, "rom:/OVEN-DRA_7_GEN-HDF18121.wav64");
	wav64_open(&sfx_kitchen_3, "rom:/OVEN-DRAWE_GEN-HDF18122.wav64");

	mixer_ch_set_vol(SFX_CH_FOOTSTEPS, 0, 0);
	wav64_open(&sfx_deep_step, "rom:/deep_steps.wav64");

	mixer_ch_set_vol(SFX_CH_ROBOTVOICE, 0, 0);
	wav64_open(&sfx_robot_voice, "rom:/robotvoice.wav64");
	wav64_set_loop(&sfx_robot_voice, true);

	wav64_open(&sfx_banging, "rom:/knock2.wav64");
	wav64_open(&sfx_foxy_running, "rom:/run.wav64");
	wav64_open(&sfx_power_down, "rom:/powerdown.wav64");

	mixer_ch_set_vol(SFX_CH_MUSICBOX, 0.8f, 0.8f);
	wav64_open(&sfx_music_box, "rom:/music_box.wav64");
	wav64_open(&sfx_chimes, "rom:/chimes_2.wav64");
	wav64_open(&sfx_cheering, "rom:/CROWD_SMALL_CHIL_EC049202.wav64");

	mixer_ch_set_vol(SFX_CH_PHONECALL, 0.8f, 0.8f);
	wav64_open(&sfx_phone_call_1, "rom:/voiceover1c.wav64");
	wav64_open(&sfx_phone_call_2, "rom:/voiceover2a.wav64");
	wav64_open(&sfx_phone_call_3, "rom:/voiceover3.wav64");
	wav64_open(&sfx_phone_call_4, "rom:/voiceover4.wav64");
	wav64_open(&sfx_phone_call_5, "rom:/voiceover5.wav64");

	wav64_open(&sfx_freddy_laugh_1, "rom:/Laugh_Giggle_Girl_1d.wav64");
	wav64_open(&sfx_freddy_laugh_2, "rom:/Laugh_Giggle_Girl_2d.wav64");
	wav64_open(&sfx_freddy_laugh_3, "rom:/Laugh_Giggle_Girl_8d.wav64");

	mixer_ch_set_vol(SFX_CH_FREDDYRUN, 1, 1);
	wav64_open(&sfx_freddy_run, "rom:/running_fast3.wav64");

	wav64_open(&sfx_cam_glitch_1, "rom:/COMPUTER_DIGITAL_L2076505.wav64");
	wav64_open(&sfx_cam_glitch_2, "rom:/garble1.wav64");
	wav64_open(&sfx_cam_glitch_3, "rom:/garble2.wav64");
	wav64_open(&sfx_cam_glitch_4, "rom:/garble3.wav64");

	mixer_ch_set_vol(SFX_CH_FOXSONG, 1, 1);
	wav64_open(&sfx_foxy_hum, "rom:/pirate_song2.wav64");

	wav64_open(&sfx_circus_music, "rom:/circus.wav64");
	wav64_open(&sfx_girl_giggle, "rom:/Laugh_Giggle_Girl_1.wav64");
}

void sfx_stop_all_channels(void)
{
        int i;

	for(i = 0; i < SFX_CH_CNT; i++) {
		mixer_ch_stop(i);
        }
}

void sfx_unload(void)
{
        wav64_close(&sfx_fan);
        wav64_close(&sfx_light);
        wav64_close(&sfx_door);
        wav64_close(&sfx_boop);
        wav64_close(&sfx_cam_up);
        wav64_close(&sfx_cam_down);
        wav64_close(&sfx_blip);
        wav64_close(&sfx_cam_scan);
        wav64_close(&sfx_static);
        wav64_close(&sfx_title_music);
        wav64_close(&sfx_window_scare);
        wav64_close(&sfx_error);
        wav64_close(&sfx_jumpscare);
        wav64_close(&sfx_jumpscare_low);
        wav64_close(&sfx_kitchen_0);
        wav64_close(&sfx_kitchen_1);
        wav64_close(&sfx_kitchen_2);
        wav64_close(&sfx_kitchen_3);
        wav64_close(&sfx_deep_step);
        wav64_close(&sfx_robot_voice);
        wav64_close(&sfx_banging);
        wav64_close(&sfx_foxy_running);
        wav64_close(&sfx_power_down);
        wav64_close(&sfx_music_box);
        wav64_close(&sfx_chimes);
        wav64_close(&sfx_cheering);
        wav64_close(&sfx_phone_call_1);
        wav64_close(&sfx_phone_call_2);
        wav64_close(&sfx_phone_call_3);
        wav64_close(&sfx_phone_call_4);
        wav64_close(&sfx_phone_call_5);
        wav64_close(&sfx_freddy_laugh_1);
        wav64_close(&sfx_freddy_laugh_2);
        wav64_close(&sfx_freddy_laugh_3);
        wav64_close(&sfx_freddy_run);
        wav64_close(&sfx_cam_glitch_1);
        wav64_close(&sfx_cam_glitch_2);
        wav64_close(&sfx_cam_glitch_3);
        wav64_close(&sfx_cam_glitch_4);
        wav64_close(&sfx_foxy_hum);
        wav64_close(&sfx_circus_music);
        wav64_close(&sfx_girl_giggle);
}
