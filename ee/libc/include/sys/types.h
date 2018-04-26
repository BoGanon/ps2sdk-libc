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

#define __need_size_t
#include <stddef.h>

#ifndef __clock_t_defined
typedef unsigned long long clock_t;
#define __clock_t_defined
#endif

#ifndef __time_t_defined
typedef unsigned long time_t;
#define __time_t_defined
#endif

#ifndef __blkcnt_t_defined
typedef long long blkcnt_t;
#define __blkcnt_t_defined
#endif

#ifndef __blksize_t_defined
typedef signed blksize_t;
#define __blksize_t_defined
#endif

#ifndef __dev_t_defined
typedef int dev_t;
#define __dev_t_defined
#endif

typedef unsigned fsblkcnt_t;
typedef unsigned fsfilcnt_t;

#ifndef __gid_t_defined
typedef int gid_t;
#define __gid_t_defined
#endif

#ifndef __pid_t_defined
typedef int pid_t;
#define __pid_t_defined
#endif

#ifndef __uid_t_defined
typedef int uid_t;
#define __uid_t_defined
#endif

typedef int id_t;

#ifndef __ino_t_defined
typedef unsigned ino_t;
#define __ino_t_defined
#endif

#ifndef __mode_t_defined
typedef int mode_t;
#define __mode_t_defined
#endif

#ifndef __nlink_t_defined
typedef int nlink_t;
#define __nlink_t_defined
#endif

#ifndef __off_t_defined
typedef long long off_t;
#define __off_t_defined
#endif

typedef long long ssize_t;

/* pthread_* */

/* trace_* */

#endif /*__TYPES_H__*/
