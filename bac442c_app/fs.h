#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <stdbool.h>
#include <stdint.h>
#include "integer.h"

#define _FN_ETHERNET	("ethernet.cfg")
#define _FN_BACNET		("bacnet.cfg")
#define _FN_COM 		("com.cfg")
#define _FN_TCP_LISRT 	("tcp.cfg")
#define _FN_MB_LISRT 		("mb_list.cfg")
#define _FN_MSTP_LISRT ("mstp_list.cfg")


typedef enum {
	FR_OK = 0,				/* (0) Succeeded */
	FR_DISK_ERR,			/* (1) A hard error occurred in the low level disk I/O layer */
	FR_INT_ERR,				/* (2) Assertion failed */
	FR_NOT_READY,			/* (3) The physical drive cannot work */
	FR_NO_FILE,				/* (4) Could not find the file */
	FR_NO_PATH,				/* (5) Could not find the path */
	FR_INVALID_NAME,		/* (6) The path name format is invalid */
	FR_DENIED,				/* (7) Access denied due to prohibited access or directory full */
	FR_EXIST,				/* (8) Access denied due to prohibited access */
	FR_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
	FR_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
	FR_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
	FR_NOT_ENABLED,			/* (12) The volume has no work area */
	FR_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
	FR_MKFS_ABORTED,		/* (14) The f_mkfs() aborted due to any parameter error */
	FR_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
	FR_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */
	FR_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */
	FR_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > _FS_SHARE */
	FR_INVALID_PARAMETER	/* (19) Given parameter is invalid */
} FRESULT;


void FS_Init(void);
FRESULT FS_GetStatus(void);

bool FILE_ReloadParams(const char* file_name, DWORD offset, void *buf, UINT brd);
bool FILE_SaveParams(const char* file_name, DWORD offset, const void* buf, UINT btw);

#endif
