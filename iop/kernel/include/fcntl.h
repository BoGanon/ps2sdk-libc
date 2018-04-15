/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# IOP file control definitions.
*/

#ifndef __IOP_FCNTL_H__
#define __IOP_FCNTL_H__

/** @file fcnt.h */
/** @warning This file should not be included in external IOP projects.
	     It's been added to both iomanX.h and ioman.h headers to utilize
	     their unique header guards.
	     The standard symbols used for ioman/X's interface functions cause
	     standard namespace conflicts on the IOP side in sys/stat.h.
*/
#include <sys/fcntl.h>
#include <sys/stat.h>

#endif /* __IOP_FCNTL_H__ */
