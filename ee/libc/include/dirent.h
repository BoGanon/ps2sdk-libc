/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2005, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id$
# dirent
*/
#ifndef __DIRENT_H__
#define __DIRENT_H__

#include <sys/types.h>

struct dirent
{
	ino_t d_ino;
	char *d_name;
};

typedef struct DIR
{
	int  d_fd;
	char d_dir[256];
} DIR;

#ifdef __cplusplus
extern "C" {
#endif

/* Open a directory
*/
DIR *opendir (const char *path);

/* Reads an entry from handle opened previously by opendir
*/
struct dirent *readdir (DIR *d);

/* Rewinds
*/
void rewinddir (DIR *d);

/* Release DIR handle
*/
int closedir (DIR *d);

#ifdef __cplusplus
}
#endif

#endif //DIRENT
