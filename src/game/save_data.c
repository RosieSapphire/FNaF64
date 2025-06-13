#include <libdragon.h>

#include "game/save_data.h"

/*
 * This is for if you need to override the save data for debugging purposes.
 * Either that or you're a cheating little fucking cocksucker. >:3 I know what
 * kind of fucking person you are, Mr. Fart Smella. owo
 */
#define SAVE_DATA_OVERRIDE (SAVE_BEATEN_BITMASK | 0x7)

bool save_data_eeprom_failed = false;
uint8_t save_data = 1;

bool save_data_init(void)
{
#ifdef SAVE_DATA_OVERRIDE
        save_data = SAVE_DATA_OVERRIDE;

        return true;
#endif

	eepfs_entry_t save_entry;

        save_entry.path = "fnaf.dat";
        save_entry.size = sizeof(save_data);
	save_data_eeprom_failed =
                eepfs_init(&save_entry, sizeof(save_data)) != EEPFS_ESUCCESS;
	if (save_data_eeprom_failed) {
		return false;
        }

	if (!eepfs_verify_signature()) {
		debugf("EEPFS verification failed. Wiping save filesystem.\n");
		eepfs_wipe();
		save_data = 1;
		eepfs_write("fnaf.dat", &save_data, sizeof(save_data));
	}

	if (eepfs_read("fnaf.dat", &save_data, sizeof(save_data))) {
		debugf("Failed to read from EEPFS.\n");
        } else {
		debugf("Sucessfully loaded save: %u\n", save_data);
        }

        return true;
}

void save_data_free(void)
{
        save_data = 0;
        save_data_eeprom_failed = false;
}
