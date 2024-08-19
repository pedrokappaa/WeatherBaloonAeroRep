#include "fatfs_sd_api.h"

// SD Card Init

FATFS fs;						// file system data
FIL fil;						// file pointer
FRESULT fresult;				// to store the result
char buffer[1024];		// to store data
UINT br, bw;					// file r/w count
FATFS *pfs;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;

bool SDCardInit()
{
	// Mount SD Card
	if(f_mount(&fs, "", 0) != FR_OK)
		return false;

	HAL_Delay(10);

	// Open file to write
	if(f_open(&fil, "data.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) != FR_OK)
		return false;

	return true;
}

bool SDCardWrite(const char* msg)
{
	f_puts(msg, &fil);
	f_sync(&fil);

	return true;
}

bool SDCardHasLowSpace()
{
	if(f_getfree("", &fre_clust, &pfs) != FR_OK)
		return true;

	freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);
	// totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);

	if(freeSpace < 1) // < 1kb
		return true;
	else
		return false;
}

bool SDCardEnd()
{
	// Close file
	if(f_close(&fil) != FR_OK)
		return false;

	HAL_Delay(10);

	// Unmount SDCARD
	if(f_mount(NULL, "", 1) != FR_OK)
		return false;

	return true;
}
