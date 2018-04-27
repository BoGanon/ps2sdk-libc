/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2005, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * POSIX declarations for times
 */

#ifndef __SYS_TIMES_H__
#define __SYS_TIMES_H__

#ifndef __clock_t_defined
typedef unsigned long long clock_t;
#define __clock_t_defined
#endif

struct tms {
  clock_t tms_utime;  /* user time */
  clock_t tms_stime;  /* system time */
  clock_t tms_cutime; /* user time of children */
  clock_t tms_cstime; /* system time of children */
};

clock_t times(struct tms *buf);

#endif /* __SYS_TIMES_H__ */

