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
 * Hardware parameters
 */

#ifndef __SYS_PARAM_H__
#define __SYS_PARAM_H__

/* Ticks per second. */
#ifndef HZ
 #ifndef _IOP
  #define HZ		294912000
 #else
  #define HZ		36864000
 #endif
#endif

/* Max number of open files. */
#define NOFILE		16

/* Max size of path strings. */
#define PATHSIZE	1024

#endif /* __SYS_PARAM_H__ */
