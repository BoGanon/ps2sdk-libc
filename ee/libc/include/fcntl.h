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
 * POSIX File control declarations.
 */

#ifndef __FCTNL_H__
#define __FCNTL_H__

#include <sys/stat.h>

#ifndef __pid_t_defined
typedef int pid_t;
#define __pid_t_defined
#endif

#ifndef __mode_t_defined
typedef int mode_t;
#define __mode_t_defined
#endif

#ifndef __off_t_defined
typedef long long off_t;
#define __off_t_defined
#endif

#include <unistd.h>

#define O_RDONLY	0x0001
#define O_WRONLY	0x0002
#define O_RDWR		0x0003

#define O_NONBLOCK	0x0010

#define O_APPEND	0x0100
#define O_CREAT		0x0200
#define O_TRUNC		0x0400
#define	O_EXCL		0x0800

#ifdef __cplusplus
extern "C" {
#endif

int open(const char *fname, int flags, ...);

#ifdef __cplusplus
}
#endif

#endif /*__FCNTL_H__*/
