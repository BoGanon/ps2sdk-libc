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
 * POSIX miscellaneous data types
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include <stddef.h>

#if defined(_EE) || defined(_R5900)
#include <sys/time.h>
#endif

typedef long long blkcnt_t;
typedef signed blksize_t;

typedef int dev_t;

typedef unsigned fsblkcnt_t;
typedef unsigned fsfilcnt_t;

typedef int gid_t;
typedef int pid_t;
typedef int uid_t;
typedef int id_t;

typedef unsigned ino_t;
typedef int mode_t;
typedef int nlink_t;
typedef long long off_t;

typedef long long ssize_t;

/* pthread_* */

/* trace_* */

#endif /*__TYPES_H__*/
