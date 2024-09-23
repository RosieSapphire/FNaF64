#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

extern int settings_flags;
enum {
	SET_SMOOTH_TURN_BIT = (1 << 0),
	SET_MANUAL_CAM_TURN_BIT = (1 << 1),
	SET_SUBTITLES_BIT = (1 << 2),
	SET_LIGHT_HOLD_BIT = (1 << 3),
	SET_ROBOT_CHEAT_BIT = (1 << 4),
	SET_GOOFY_MODE_BIT = (1 << 5),
	SET_FAST_NIGHT_BIT = (1 << 6),
	SETTING_COUNT = 7,
};

#define HOUR_LEN_SECONDS ((settings_flags & SET_FAST_NIGHT_BIT) ? 45 : 90)

#endif /* GAME_SETTINGS_H */
