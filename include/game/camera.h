#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include <libdragon.h>

#include "engine/update.h"

enum camera_views {
	CAM_1A, CAM_1B, CAM_1C,
	CAM_2A, CAM_2B, CAM_3,
	CAM_4A, CAM_4B, CAM_5,
	CAM_6, CAM_7, CAM_COUNT
};

extern const int cam_button_pos[CAM_COUNT][2];

#define AT_DOOR CAM_COUNT
#define YOURE_FUCKED (AT_DOOR + 1)
#define ROOM_SPOT_BIT 16
#define FACE_GLITCH_MASK 0x3E0
#define FACE_GLITCH_SHIFT 5

extern bool camera_is_using, camera_is_visible,
       camera_was_using, camera_was_visible;
extern int camera_states[CAM_COUNT];
extern float camera_glitch_timer;
extern int cam_selected;

void camera_load(void);
void camera_unload(void);
void camera_flip_draw(void);
void camera_view_draw(void);
void camera_ui_draw(void);
void camera_update(update_parms_t uparms);

#endif /* GAME_CAMERA_H */
