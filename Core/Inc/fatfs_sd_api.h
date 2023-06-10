#ifndef INC_FATFS_SD_API_H_
#define INC_FATFS_SD_API_H_
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "fatfs_sd.h"
#include "fatfs.h"

bool SDCardInit();
bool SDCardWrite(const char* msg);
bool SDCardHasLowSpace();
bool SDCardEnd();

#endif
