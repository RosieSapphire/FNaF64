#ifndef GAME_TEXTURE_INDEX_H
#define GAME_TEXTURE_INDEX_H

/* Helpers */
#define TX_ROM(PATH) ("rom:/" PATH)
#define TX_ROM_CUSTOM(PATH) ("rom:/custom/" PATH)

/* Static */
#define TX_STATIC0           TX_ROM("12.i4.sprite")
#define TX_STATIC1           TX_ROM("13.i4.sprite")
#define TX_STATIC2           TX_ROM("14.i4.sprite")
#define TX_STATIC3           TX_ROM("15.i4.sprite")
#define TX_STATIC4           TX_ROM("16.i4.sprite")
#define TX_STATIC5           TX_ROM("17.i4.sprite")
#define TX_STATIC6           TX_ROM("18.i4.sprite")
#define TX_STATIC7           TX_ROM("20.i4.sprite")

/* Freddy Face */
#define TX_FRED_FACE0        TX_ROM("431.ci8.sprite")
#define TX_FRED_FACE1        TX_ROM("440.ci8.sprite")
#define TX_FRED_FACE2        TX_ROM("441.ci8.sprite")
#define TX_FRED_FACE3        TX_ROM("442.ci8.sprite")

/* Title UI */
#define TX_TITLE_TEXT        TX_ROM("444.ia4.sprite")
#define TX_NEWGAME           TX_ROM("448.ia4.sprite")
#define TX_CONTINUE          TX_ROM("449.ia4.sprite")
#define TX_6THNIGHT          TX_ROM("443.ia4.sprite")
#define TX_CUSTOMNIGHT       TX_ROM("526.ia4.sprite")
#define TX_SELECTOR          TX_ROM("450.ia4.sprite")
#define TX_STAR              TX_ROM("432.ia4.sprite")

#define TX_SETTINGS_TEXT     TX_ROM_CUSTOM("settings_text.ia4.sprite")
#define TX_SETTINGS_OPTS     TX_ROM_CUSTOM("settings_opts.ia4.sprite")
#define TX_SETTINGS_DESCS    TX_ROM_CUSTOM("settings_descs.ia4.sprite")

#define TX_NIGHT_TEXT        TX_ROM_CUSTOM("night.ia4.sprite")
#define TX_NIGHT_NUM_ATLAS   TX_ROM_CUSTOM("night_atlas.ia4.sprite")
#define TX_BIND_BUTTONS_TEXT TX_ROM_CUSTOM("bind_buttons_text.ia4.sprite")

/* Custom Night */
#define TX_FACE_ATLAS        TX_ROM_CUSTOM("face_icons.ci8.sprite")
#define TX_AI_BUTTONS        TX_ROM_CUSTOM("ai_buttons.ci4.sprite")
#define TX_AI_NUM_ATLAS      TX_ROM_CUSTOM("ai_nums.ia4.sprite")

/* Which Night */
#define TX_WHICH_NIGHT_ATLAS TX_ROM_CUSTOM("which_night.i4.sprite")

/* Camera */
#define TX_CAM_NAME_ATLAS    TX_ROM_CUSTOM("cam_name_atlas.ia4.sprite")
#define TX_CAM_MAP           TX_ROM_CUSTOM("map.ia4.sprite")
#define TX_CAM_CORRUPTED     TX_ROM_CUSTOM("missing_footage.ia4.sprite")

#define TX_CAM_1A_EMPTY      TX_ROM("484.ci8.sprite")
#define TX_CAM_1A_FRED       TX_ROM("224.ci8.sprite")
#define TX_CAM_1A_BON_FRED   TX_ROM("223.ci8.sprite")
#define TX_CAM_1A_CHIC_FRED  TX_ROM("68.ci8.sprite")
#define TX_CAM_1A_ALL        TX_ROM("19.ci8.sprite")

#define TX_CAM_1B_EMPTY      TX_ROM("48.ci8.sprite")
#define TX_CAM_1B_FRED       TX_ROM("492.ci8.sprite")
#define TX_CAM_1B_BON0       TX_ROM("90.ci8.sprite")
#define TX_CAM_1B_BON1       TX_ROM("120.ci8.sprite")
#define TX_CAM_1B_CHIC0      TX_ROM("215.ci8.sprite")
#define TX_CAM_1B_CHIC1      TX_ROM("222.ci8.sprite")

#define TX_CAM_1C0           TX_ROM("66.ci8.sprite")
#define TX_CAM_1C1           TX_ROM("211.ci8.sprite")
#define TX_CAM_1C2           TX_ROM("338.ci8.sprite")
#define TX_CAM_1C3           TX_ROM("553.ci8.sprite")

#define TX_CAM_2A_DARK       TX_ROM("43.ci8.sprite")
#define TX_CAM_2A_EMPTY      TX_ROM("44.ci8.sprite")
#define TX_CAM_2A_BON        TX_ROM("206.ci8.sprite")

#define TX_CAM_2B_EMPTY      TX_ROM("0.ci8.sprite")
#define TX_CAM_2B_BON0       TX_ROM("188.ci8.sprite")
#define TX_CAM_2B_BON1       TX_ROM("478.ci8.sprite")
#define TX_CAM_2B_BON2       TX_ROM("479.ci8.sprite")
#define TX_CAM_2B_GOLD       TX_ROM("540.ci8.sprite")
#define TX_CAM_2B_FOX00      TX_ROM("241.ci8.sprite")
#define TX_CAM_2B_FOX01      TX_ROM("244.ci8.sprite")
#define TX_CAM_2B_FOX02      TX_ROM("246.ci8.sprite")
#define TX_CAM_2B_FOX03      TX_ROM("248.ci8.sprite")
#define TX_CAM_2B_FOX04      TX_ROM("280.ci8.sprite")
#define TX_CAM_2B_FOX05      TX_ROM("283.ci8.sprite")
#define TX_CAM_2B_FOX06      TX_ROM("285.ci8.sprite")
#define TX_CAM_2B_FOX07      TX_ROM("287.ci8.sprite")
#define TX_CAM_2B_FOX08      TX_ROM("289.ci8.sprite")
#define TX_CAM_2B_FOX09      TX_ROM("292.ci8.sprite")
#define TX_CAM_2B_FOX10      TX_ROM("306.ci8.sprite")
#define TX_CAM_2B_FOX11      TX_ROM("329.ci8.sprite")
#define TX_CAM_2B_FOX12      TX_ROM("331.ci8.sprite")

#define TX_CAM_3_EMPTY       TX_ROM("62.ci8.sprite")
#define TX_CAM_3_BON         TX_ROM("190.ci8.sprite")

#define TX_CAM_4A_EMPTY      TX_ROM("67.ci8.sprite")
#define TX_CAM_4A_CHIC0      TX_ROM("221.ci8.sprite")
#define TX_CAM_4A_CHIC1      TX_ROM("226.ci8.sprite")
#define TX_CAM_4A_FRED       TX_ROM("487.ci8.sprite")

#define TX_CAM_4B_EMPTY      TX_ROM("49.ci8.sprite")
#define TX_CAM_4B_CHIC0      TX_ROM("220.ci8.sprite")
#define TX_CAM_4B_CHIC1      TX_ROM("476.ci8.sprite")
#define TX_CAM_4B_CHIC2      TX_ROM("451.ci8.sprite")
#define TX_CAM_4B_FRED       TX_ROM("486.ci8.sprite")

#define TX_CAM_5_EMPTY       TX_ROM("83.ci8.sprite")
#define TX_CAM_5_BON0        TX_ROM("205.ci8.sprite")
#define TX_CAM_5_BON1        TX_ROM("555.ci8.sprite")

#define TX_CAM_7_EMPTY       TX_ROM("41.ci8.sprite")
#define TX_CAM_7_CHIC0       TX_ROM("219.ci8.sprite")
#define TX_CAM_7_CHIC1       TX_ROM("217.ci8.sprite")
#define TX_CAM_7_FRED        TX_ROM("249.ci8.sprite")

#define TX_CAM_BUTTON0       TX_ROM_CUSTOM("cam_button0.ci4.sprite")
#define TX_CAM_BUTTON1       TX_ROM_CUSTOM("cam_button1.ci4.sprite")

/* Office UI */
#define TX_AM_SMALL          TX_ROM_CUSTOM("am.ia4.sprite")
#define TX_HOUR_ATLAS        TX_ROM_CUSTOM("hour_atlas.ia4.sprite")
#define TX_POWER_LEFT_TEXT   TX_ROM_CUSTOM("power_left.ia4.sprite")
#define TX_USAGE_TEXT        TX_ROM_CUSTOM("usage_text.ia4.sprite")
#define TX_USAGE_ATLAS       TX_ROM_CUSTOM("usage_atlas.ci4.sprite")

/* Door Buttons */
#define TX_BUTTON_RIGHT00    TX_ROM("134.ci8.sprite")
#define TX_BUTTON_RIGHT01    TX_ROM("131.ci8.sprite")
#define TX_BUTTON_RIGHT10    TX_ROM("135.ci8.sprite")
#define TX_BUTTON_RIGHT11    TX_ROM("47.ci8.sprite")

#define TX_BUTTON_LEFT00     TX_ROM("122.ci8.sprite")
#define TX_BUTTON_LEFT01     TX_ROM("125.ci8.sprite")
#define TX_BUTTON_LEFT10     TX_ROM("124.ci8.sprite")
#define TX_BUTTON_LEFT11     TX_ROM("130.ci8.sprite")

/* Fan */
#define TX_FAN0              TX_ROM("57.ci8.sprite")
#define TX_FAN1              TX_ROM("59.ci8.sprite")
#define TX_FAN2              TX_ROM("60.ci8.sprite")

/* Bonnie Jumpscare */
#define TX_BONNIE_SCARE00    TX_ROM("301.ci8.sprite")
#define TX_BONNIE_SCARE01    TX_ROM("291.ci8.sprite")
#define TX_BONNIE_SCARE02    TX_ROM("303.ci8.sprite")
#define TX_BONNIE_SCARE03    TX_ROM("293.ci8.sprite")
#define TX_BONNIE_SCARE04    TX_ROM("294.ci8.sprite")
#define TX_BONNIE_SCARE05    TX_ROM("295.ci8.sprite")
#define TX_BONNIE_SCARE06    TX_ROM("296.ci8.sprite")
#define TX_BONNIE_SCARE07    TX_ROM("297.ci8.sprite")
#define TX_BONNIE_SCARE08    TX_ROM("298.ci8.sprite")
#define TX_BONNIE_SCARE09    TX_ROM("299.ci8.sprite")
#define TX_BONNIE_SCARE10    TX_ROM("300.ci8.sprite")

/* Camera Flip */
#define TX_CAM_FLIP00        TX_ROM("142.ci8.sprite")
#define TX_CAM_FLIP01        TX_ROM( "46.ci8.sprite")
#define TX_CAM_FLIP02        TX_ROM("144.ci8.sprite")
#define TX_CAM_FLIP03        TX_ROM("132.ci8.sprite")
#define TX_CAM_FLIP04        TX_ROM("133.ci8.sprite")
#define TX_CAM_FLIP05        TX_ROM("136.ci8.sprite")
#define TX_CAM_FLIP06        TX_ROM("137.ci8.sprite")
#define TX_CAM_FLIP07        TX_ROM("138.ci8.sprite")
#define TX_CAM_FLIP08        TX_ROM("139.ci8.sprite")
#define TX_CAM_FLIP09        TX_ROM("140.ci8.sprite")
#define TX_CAM_FLIP10        TX_ROM("141.ci8.sprite")

#endif /* GAME_TEXTURE_INDEX_H */
