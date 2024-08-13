#ifndef GAME_TEXTURE_INDEX_H
#define GAME_TEXTURE_INDEX_H

/* Helpers */
#define TX_ROM(PATH) ("rom:/" PATH)
#define TX_ROM_CUSTOM(PATH) ("rom:/custom/" PATH)

/* Static */
#define TX_STATIC0 TX_ROM("i4/12.sprite")
#define TX_STATIC1 TX_ROM("i4/13.sprite")
#define TX_STATIC2 TX_ROM("i4/14.sprite")
#define TX_STATIC3 TX_ROM("i4/15.sprite")
#define TX_STATIC4 TX_ROM("i4/16.sprite")
#define TX_STATIC5 TX_ROM("i4/17.sprite")
#define TX_STATIC6 TX_ROM("i4/18.sprite")
#define TX_STATIC7 TX_ROM("i4/20.sprite")

/* Freddy Face */
#define TX_FRED_FACE0 TX_ROM("ci8/431.sprite")
#define TX_FRED_FACE1 TX_ROM("ci8/440.sprite")
#define TX_FRED_FACE2 TX_ROM("ci8/441.sprite")
#define TX_FRED_FACE3 TX_ROM("ci8/442.sprite")

/* Title UI */
#define TX_TITLE_TEXT TX_ROM_CUSTOM("title_text.ia4.sprite")
#define TX_TITLE_OPTIONS TX_ROM_CUSTOM("options.ia4.sprite")
#define TX_SELECTOR TX_ROM_CUSTOM("selector.ia4.sprite")
#define TX_STAR TX_ROM_CUSTOM("star.ia4.sprite")

#define TX_SETTINGS_TEXT TX_ROM_CUSTOM("settings_text.ia4.sprite")
#define TX_SETTINGS_OPTS TX_ROM_CUSTOM("settings_opts.ia4.sprite")
#define TX_SETTINGS_DESCS TX_ROM_CUSTOM("settings_descs.ia4.sprite")
#define TX_EEPROM_ERROR TX_ROM_CUSTOM("eeprom_error.ia4.sprite")

#define TX_NIGHT_TEXT TX_ROM_CUSTOM("night.ia4.sprite")
#define TX_NIGHT_NUM_ATLAS TX_ROM_CUSTOM("night_atlas.ia4.sprite")
#define TX_BIND_BUTTONS_TEXT TX_ROM_CUSTOM("bind_buttons_text.ia4.sprite")

#define TX_NEWSPAPER TX_ROM("ci8/574.sprite")

/* Custom Night */
#define TX_FACE_ATLAS TX_ROM_CUSTOM("face_icons.ci8.sprite")
#define TX_AI_BUTTONS TX_ROM_CUSTOM("ai_buttons.ci4.sprite")
#define TX_AI_NUM_ATLAS TX_ROM_CUSTOM("ai_nums.ia4.sprite")

/* Which Night */
#define TX_WHICH_NIGHT_ATLAS TX_ROM_CUSTOM("which_night.i4.sprite")

/* Paycheck */
#define TX_PAYCHECK1 TX_ROM("ci8/210.sprite")
#define TX_PAYCHECK2 TX_ROM("ci8/522.sprite")
#define TX_PINK_SLIP TX_ROM("ci8/523.sprite")

/* Gameover UI */
#define TX_GAME_OVER_SCREEN TX_ROM("ci8/358.sprite")
#define TX_GAME_OVER_TEXT TX_ROM_CUSTOM("game_over_text.ia4.sprite")

/* Camera */
#define TX_CAM_NAME_ATLAS TX_ROM_CUSTOM("cam_name_atlas.ia4.sprite")
#define TX_CAM_MAP TX_ROM_CUSTOM("map.ia4.sprite")
#define TX_CAM_CORRUPTED TX_ROM_CUSTOM("missing_footage.ia4.sprite")

#define TX_CAM_1A_EMPTY TX_ROM("ci8/484.sprite")
#define TX_CAM_1A_FRED TX_ROM("ci8/224.sprite")
#define TX_CAM_1A_BON_FRED TX_ROM("ci8/223.sprite")
#define TX_CAM_1A_CHIC_FRED TX_ROM("ci8/68.sprite")
#define TX_CAM_1A_ALL TX_ROM("ci8/19.sprite")

#define TX_CAM_1B_EMPTY TX_ROM("ci8/48.sprite")
#define TX_CAM_1B_FRED TX_ROM("ci8/492.sprite")
#define TX_CAM_1B_BON0 TX_ROM("ci8/90.sprite")
#define TX_CAM_1B_BON1 TX_ROM("ci8/120.sprite")
#define TX_CAM_1B_CHIC0 TX_ROM("ci8/215.sprite")
#define TX_CAM_1B_CHIC1 TX_ROM("ci8/222.sprite")

#define TX_CAM_1C0 TX_ROM("ci8/66.sprite")
#define TX_CAM_1C1 TX_ROM("ci8/211.sprite")
#define TX_CAM_1C2 TX_ROM("ci8/338.sprite")
#define TX_CAM_1C3 TX_ROM("ci8/553.sprite")

#define TX_CAM_2A_DARK TX_ROM("ci8/43.sprite")
#define TX_CAM_2A_EMPTY TX_ROM("ci8/44.sprite")
#define TX_CAM_2A_BON TX_ROM("ci8/206.sprite")

#define TX_CAM_2B_EMPTY TX_ROM("ci8/0.sprite")
#define TX_CAM_2B_BON0 TX_ROM("ci8/188.sprite")
#define TX_CAM_2B_BON1 TX_ROM("ci8/478.sprite")
#define TX_CAM_2B_BON2 TX_ROM("ci8/479.sprite")
#define TX_CAM_2B_GOLD TX_ROM("ci8/540.sprite")
#define TX_CAM_2B_FOX00 TX_ROM("ci8/241.sprite")
#define TX_CAM_2B_FOX01 TX_ROM("ci8/244.sprite")
#define TX_CAM_2B_FOX02 TX_ROM("ci8/246.sprite")
#define TX_CAM_2B_FOX03 TX_ROM("ci8/248.sprite")
#define TX_CAM_2B_FOX04 TX_ROM("ci8/280.sprite")
#define TX_CAM_2B_FOX05 TX_ROM("ci8/283.sprite")
#define TX_CAM_2B_FOX06 TX_ROM("ci8/285.sprite")
#define TX_CAM_2B_FOX07 TX_ROM("ci8/287.sprite")
#define TX_CAM_2B_FOX08 TX_ROM("ci8/289.sprite")
#define TX_CAM_2B_FOX09 TX_ROM("ci8/292.sprite")
#define TX_CAM_2B_FOX10 TX_ROM("ci8/306.sprite")
#define TX_CAM_2B_FOX11 TX_ROM("ci8/329.sprite")
#define TX_CAM_2B_FOX12 TX_ROM("ci8/331.sprite")

#define TX_CAM_3_EMPTY TX_ROM("ci8/62.sprite")
#define TX_CAM_3_BON TX_ROM("ci8/190.sprite")

#define TX_CAM_4A_EMPTY TX_ROM("ci8/67.sprite")
#define TX_CAM_4A_CHIC0 TX_ROM("ci8/221.sprite")
#define TX_CAM_4A_CHIC1 TX_ROM("ci8/226.sprite")
#define TX_CAM_4A_FRED TX_ROM("ci8/487.sprite")

#define TX_CAM_4B_EMPTY TX_ROM("ci8/49.sprite")
#define TX_CAM_4B_CHIC0 TX_ROM("ci8/220.sprite")
#define TX_CAM_4B_CHIC1 TX_ROM("ci8/476.sprite")
#define TX_CAM_4B_CHIC2 TX_ROM("ci8/451.sprite")
#define TX_CAM_4B_FRED TX_ROM("ci8/486.sprite")

#define TX_CAM_5_EMPTY TX_ROM("ci8/83.sprite")
#define TX_CAM_5_BON0 TX_ROM("ci8/205.sprite")
#define TX_CAM_5_BON1 TX_ROM("ci8/555.sprite")

#define TX_CAM_7_EMPTY TX_ROM("ci8/41.sprite")
#define TX_CAM_7_CHIC0 TX_ROM("ci8/219.sprite")
#define TX_CAM_7_CHIC1 TX_ROM("ci8/217.sprite")
#define TX_CAM_7_FRED TX_ROM("ci8/249.sprite")

#define TX_CAM_BUTTON0 TX_ROM_CUSTOM("cam_button0.ci4.sprite")
#define TX_CAM_BUTTON1 TX_ROM_CUSTOM("cam_button1.ci4.sprite")

/* Office UI */
#define TX_AM_SMALL TX_ROM_CUSTOM("am.ia4.sprite")
#define TX_HOUR_ATLAS TX_ROM_CUSTOM("hour_atlas.ia4.sprite")
#define TX_POWER_LEFT_TEXT TX_ROM_CUSTOM("power_left.ia4.sprite")
#define TX_USAGE_TEXT TX_ROM_CUSTOM("usage_text.ia4.sprite")
#define TX_USAGE_ATLAS TX_ROM_CUSTOM("usage_atlas.ci4.sprite")

/* Door Buttons */
#define TX_BUTTON_RIGHT00 TX_ROM("ci8/134.sprite")
#define TX_BUTTON_RIGHT01 TX_ROM("ci8/131.sprite")
#define TX_BUTTON_RIGHT10 TX_ROM("ci8/135.sprite")
#define TX_BUTTON_RIGHT11 TX_ROM("ci8/47.sprite")

#define TX_BUTTON_LEFT00 TX_ROM("ci8/122.sprite")
#define TX_BUTTON_LEFT01 TX_ROM("ci8/125.sprite")
#define TX_BUTTON_LEFT10 TX_ROM("ci8/124.sprite")
#define TX_BUTTON_LEFT11 TX_ROM("ci8/130.sprite")

/* Fan */
#define TX_FAN0 TX_ROM("ci8/57.sprite")
#define TX_FAN1 TX_ROM("ci8/59.sprite")
#define TX_FAN2 TX_ROM("ci8/60.sprite")

/* Bonnie Jumpscare */
#define TX_BONNIE_SCARE00 TX_ROM("ci4/301.sprite")
#define TX_BONNIE_SCARE01 TX_ROM("ci4/291.sprite")
#define TX_BONNIE_SCARE02 TX_ROM("ci4/303.sprite")
#define TX_BONNIE_SCARE03 TX_ROM("ci4/293.sprite")
#define TX_BONNIE_SCARE04 TX_ROM("ci4/294.sprite")
#define TX_BONNIE_SCARE05 TX_ROM("ci4/295.sprite")
#define TX_BONNIE_SCARE06 TX_ROM("ci4/296.sprite")
#define TX_BONNIE_SCARE07 TX_ROM("ci4/297.sprite")
#define TX_BONNIE_SCARE08 TX_ROM("ci4/298.sprite")
#define TX_BONNIE_SCARE09 TX_ROM("ci4/299.sprite")
#define TX_BONNIE_SCARE10 TX_ROM("ci4/300.sprite")

/* Freddy Dark Jumpscare */
#define TX_FREDDY_SCARE_DARK00 TX_ROM("ci4/307.sprite")
#define TX_FREDDY_SCARE_DARK01 TX_ROM("ci4/308.sprite")
#define TX_FREDDY_SCARE_DARK02 TX_ROM("ci4/309.sprite")
#define TX_FREDDY_SCARE_DARK03 TX_ROM("ci4/310.sprite")
#define TX_FREDDY_SCARE_DARK04 TX_ROM("ci4/311.sprite")
#define TX_FREDDY_SCARE_DARK05 TX_ROM("ci4/312.sprite")
#define TX_FREDDY_SCARE_DARK06 TX_ROM("ci4/313.sprite")
#define TX_FREDDY_SCARE_DARK07 TX_ROM("ci4/314.sprite")
#define TX_FREDDY_SCARE_DARK08 TX_ROM("ci4/315.sprite")
#define TX_FREDDY_SCARE_DARK09 TX_ROM("ci4/316.sprite")
#define TX_FREDDY_SCARE_DARK10 TX_ROM("ci4/317.sprite")
#define TX_FREDDY_SCARE_DARK11 TX_ROM("ci4/318.sprite")
#define TX_FREDDY_SCARE_DARK12 TX_ROM("ci4/319.sprite")
#define TX_FREDDY_SCARE_DARK13 TX_ROM("ci4/320.sprite")
#define TX_FREDDY_SCARE_DARK14 TX_ROM("ci4/321.sprite")
#define TX_FREDDY_SCARE_DARK15 TX_ROM("ci4/322.sprite")
#define TX_FREDDY_SCARE_DARK16 TX_ROM("ci4/323.sprite")
#define TX_FREDDY_SCARE_DARK17 TX_ROM("ci4/324.sprite")

/* Freddy Room Jumpscare */
#define TX_FREDDY_SCARE_ROOM00 TX_ROM("ci8/490.sprite")
#define TX_FREDDY_SCARE_ROOM01 TX_ROM("ci8/491.sprite")
#define TX_FREDDY_SCARE_ROOM02 TX_ROM("ci8/495.sprite")
#define TX_FREDDY_SCARE_ROOM03 TX_ROM("ci8/497.sprite")
#define TX_FREDDY_SCARE_ROOM04 TX_ROM("ci8/499.sprite")
#define TX_FREDDY_SCARE_ROOM05 TX_ROM("ci8/501.sprite")
#define TX_FREDDY_SCARE_ROOM06 TX_ROM("ci8/503.sprite")
#define TX_FREDDY_SCARE_ROOM07 TX_ROM("ci8/505.sprite")
#define TX_FREDDY_SCARE_ROOM08 TX_ROM("ci8/507.sprite")
#define TX_FREDDY_SCARE_ROOM09 TX_ROM("ci8/509.sprite")
#define TX_FREDDY_SCARE_ROOM10 TX_ROM("ci8/511.sprite")
#define TX_FREDDY_SCARE_ROOM11 TX_ROM("ci8/513.sprite")
#define TX_FREDDY_SCARE_ROOM12 TX_ROM("ci8/515.sprite")
#define TX_FREDDY_SCARE_ROOM13 TX_ROM("ci8/517.sprite")

/* Foxy Jumpscare */
#define TX_FOXY_SCARE0 TX_ROM("ci8/242.sprite")
#define TX_FOXY_SCARE1 TX_ROM("ci8/396.sprite")
#define TX_FOXY_SCARE2 TX_ROM("ci8/398.sprite")
#define TX_FOXY_SCARE3 TX_ROM("ci8/400.sprite")
#define TX_FOXY_SCARE4 TX_ROM("ci8/402.sprite")
#define TX_FOXY_SCARE5 TX_ROM("ci8/404.sprite")
#define TX_FOXY_SCARE6 TX_ROM("ci8/406.sprite")
#define TX_FOXY_SCARE7 TX_ROM("ci8/408.sprite")

/* Chica Jumpscare */
#define TX_CHICA_SCARE0 TX_ROM("ci4/69.sprite")
#define TX_CHICA_SCARE1 TX_ROM("ci4/228.sprite")
#define TX_CHICA_SCARE2 TX_ROM("ci4/230.sprite")
#define TX_CHICA_SCARE3 TX_ROM("ci4/232.sprite")
#define TX_CHICA_SCARE4 TX_ROM("ci4/234.sprite")
#define TX_CHICA_SCARE5 TX_ROM("ci4/236.sprite")
#define TX_CHICA_SCARE6 TX_ROM("ci4/239.sprite")
#define TX_CHICA_SCARE7 TX_ROM("ci4/281.sprite")

/* Camera Flip */
#define TX_CAM_FLIP00 TX_ROM("ci8/142.sprite")
#define TX_CAM_FLIP01 TX_ROM("ci8/46.sprite")
#define TX_CAM_FLIP02 TX_ROM("ci8/144.sprite")
#define TX_CAM_FLIP03 TX_ROM("ci8/132.sprite")
#define TX_CAM_FLIP04 TX_ROM("ci8/133.sprite")
#define TX_CAM_FLIP05 TX_ROM("ci8/136.sprite")
#define TX_CAM_FLIP06 TX_ROM("ci8/137.sprite")
#define TX_CAM_FLIP07 TX_ROM("ci8/138.sprite")
#define TX_CAM_FLIP08 TX_ROM("ci8/139.sprite")
#define TX_CAM_FLIP09 TX_ROM("ci8/140.sprite")
#define TX_CAM_FLIP10 TX_ROM("ci8/141.sprite")

/* Door Anim */
#define TX_DOOR_ANIM00 TX_ROM("ci8/88.sprite")
#define TX_DOOR_ANIM01 TX_ROM("ci8/105.sprite")
#define TX_DOOR_ANIM02 TX_ROM("ci8/89.sprite")
#define TX_DOOR_ANIM03 TX_ROM("ci8/91.sprite")
#define TX_DOOR_ANIM04 TX_ROM("ci8/92.sprite")
#define TX_DOOR_ANIM05 TX_ROM("ci8/93.sprite")
#define TX_DOOR_ANIM06 TX_ROM("ci8/94.sprite")
#define TX_DOOR_ANIM07 TX_ROM("ci8/95.sprite")
#define TX_DOOR_ANIM08 TX_ROM("ci8/96.sprite")
#define TX_DOOR_ANIM09 TX_ROM("ci8/97.sprite")
#define TX_DOOR_ANIM10 TX_ROM("ci8/98.sprite")
#define TX_DOOR_ANIM11 TX_ROM("ci8/99.sprite")
#define TX_DOOR_ANIM12 TX_ROM("ci8/100.sprite")
#define TX_DOOR_ANIM13 TX_ROM("ci8/101.sprite")
#define TX_DOOR_ANIM14 TX_ROM("ci8/102.sprite")

/* Room States */
#define TX_ROOM_NORMAL TX_ROM("ci8/39.sprite")
#define TX_ROOM_LEFT_EMPTY TX_ROM("ci8/58.sprite")
#define TX_ROOM_LEFT_BONNIE TX_ROM("ci8/225.sprite")
#define TX_ROOM_RIGHT_EMPTY TX_ROM("ci8/127.sprite")
#define TX_ROOM_RIGHT_CHICA TX_ROM("ci8/227.sprite")
#define TX_ROOM_POWERDOWN0 TX_ROM("ci8/304.sprite")
#define TX_ROOM_POWERDOWN1 TX_ROM("ci8/305.sprite")

/* Hallucinations */
#define TX_HALLUCINATION0 TX_ROM("ci4/525.sprite")
#define TX_HALLUCINATION1 TX_ROM("ci4/543.sprite")
#define TX_HALLUCINATION2 TX_ROM("ci4/545.sprite")
#define TX_HALLUCINATION3 TX_ROM("ci4/544.sprite")

/* Night End */
#define TX_END_FIVE TX_ROM_CUSTOM("five.i4.sprite")
#define TX_END_SIX TX_ROM_CUSTOM("six.i4.sprite")
#define TX_END_AM TX_ROM_CUSTOM("am_big.i4.sprite")

/* Golden Freddy */
#define TX_GOLDEN_FREDDY_SIT TX_ROM("ci8/573.sprite")
#define TX_GOLDEN_FREDDY_SCARE TX_ROM("ci8/548.sprite")

#endif /* GAME_TEXTURE_INDEX_H */
