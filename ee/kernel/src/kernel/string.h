/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id$
# Internal kernel string functions
*/
#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

void *	memcpy(void *, const void *, unsigned int);
void *	memset(void *, int, unsigned int);
char *	strncpy(char *, const char *, unsigned int);
unsigned int strlen(const char *);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_STRING_H */

