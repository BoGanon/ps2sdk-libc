/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright (c) 2003 Marcus R. Brown <mrbrown@0xd6.org>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * IOMAN and IOMANX file control constants.
 */

#ifndef __SYS_IO_FCNTL_H__
#define __SYS_IO_FCNTL_H__

#define IO_RDONLY	0x0001
#define IO_WRONLY	0x0002
#define IO_RDWR		0x0003
#define IO_DIROPEN	0x0008
#define IO_NONBLOCK	0x0010
#define IO_APPEND	0x0100
#define IO_CREAT	0x0200
#define IO_TRUNC	0x0400
#define	IO_EXCL		0x0800

#define IO_SEEK_SET	0
#define IO_SEEK_CUR	1
#define IO_SEEK_END	2

#endif /* __SYS_IO_FCNTL_H__ */
