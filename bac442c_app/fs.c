#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "fs.h"

FRESULT fs_state= FR_NO_FILESYSTEM;

void FS_Init(void)
{
    fs_state = FR_OK;
}

FRESULT FS_GetStatus(void)
{
	return fs_state;
}

bool FILE_ReloadParams(const char* file_name, DWORD offset, void *buf, UINT brd)
{
    FILE *p = fopen(file_name, "w");
    fseek(p, offset, SEEK_SET);
    fread(buf, 1, brd, p);
    fclose(p);
	return true;
}

bool FILE_SaveParams(const char* file_name, DWORD offset, const void* buf, UINT btw)
{
    FILE *p = fopen(file_name, "w");
    fseek(p, offset, SEEK_SET);
    fwrite(buf, 1, btw, p);
    fclose(p);
	return true;
}



