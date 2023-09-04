#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

extern int settings_flags;
#define SET_SMOOTH_TURN_BIT     1
#define SET_MANUAL_CAM_TURN_BIT 2
#define SET_SUBTITLES_BIT       4
#define SET_LIGHT_HOLD_BIT      8 
#define SET_ROBOT_CHEAT_BIT     16
#define SET_GOOFY_MODE_BIT      32
#define SET_FAST_NIGHT_BIT      64
#define SETTING_COUNT           7

#define HOUR_LEN_SECONDS ((settings_flags & SET_FAST_NIGHT_BIT) ? 45 : 90)

#endif /* GAME_SETTINGS_H */
