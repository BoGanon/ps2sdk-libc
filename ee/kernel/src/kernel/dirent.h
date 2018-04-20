/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * Declarations for directory I/O not part of libc.
 * Only libfileio or libfileXio need to be linked.
 */

#ifndef __KERNEL_DIRENT_H__
#define __KERNEL_DIRENT_H__

#include <sys/types.h>

/** @addtogroup unistd
    @{
*/

struct dirent
{
	ino_t d_ino;
	char d_name[256];
};

typedef struct DIR
{
	int  d_fd;
	char d_dir[256];
} DIR;

/** Open a directory.

    @param path  Directory to open

    @return      A pointer to a directory
    @retval NULL Failure

    @warning     Only a single open directory is supported
*/
DIR *opendir (const char *path);

/** Read entry from open directory

    @param  d    Pointer to directory opened by opendir

    @return      A pointer to a directory entry.
    @retval NULL Failure
*/
struct dirent *readdir (DIR *d);

/** Rewind directory to beginning

    @param d Pointer to directory to rewind

    @note    Rewind is done by closing and opening directory.

    @return  If an error occurs, the directory structure at d will no longer
	     hold a valid file descriptor.
*/
void rewinddir (DIR *d);

/** Close directory

    @param d   Open directory to close
    
    @retval 0  Success
    @retval <0 Failure
*/
int closedir (DIR *d);

/** End of addtogroup unistd
    @}
*/

#endif /* __KERNEL_DIRENT_H__ */
