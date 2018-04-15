/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# Internal kernel string functions
*/

#ifndef __KERNEL_STRING_H__
#define __KERNEL_STRING_H__

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

#endif /* __KERNEL_STRING_H__ */

