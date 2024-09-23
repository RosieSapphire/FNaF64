#include <libdragon.h>

#include "game/save_data.h"

uint8_t save_data;

int save_data_read(void)
{
	eepfs_entry_t save_entry = { "fnaf.dat", sizeof(save_data) };
	if (eepfs_init(&save_entry, sizeof(save_data)) != EEPFS_ESUCCESS)
		return false;

	if (!eepfs_verify_signature()) {
		debugf("EEPFS verification failed. Wiping save filesystem.\n");
		eepfs_wipe();
		save_data = 1;
		eepfs_write("fnaf.dat", &save_data, sizeof(save_data));
	}

	if (eepfs_read("fnaf.dat", &save_data, sizeof(save_data)) !=
	    EEPFS_ESUCCESS)
		debugf("Failed to read from EEPFS.\n");
	else
		debugf("Sucessfully loaded save: %u\n", save_data);

	return true;
}
