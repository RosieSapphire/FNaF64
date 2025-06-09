#include <libdragon.h>

#include "game/save_data.h"

bool save_data_eeprom_failed = false;
uint8_t save_data = 1;

bool save_data_init(void)
{
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
