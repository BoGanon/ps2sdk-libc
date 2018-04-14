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
*/
#ifndef __COMPLEX_H__
#define __COMPLEX_H__

#if defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define complex _Complex
#define _Complex_I 1.0iF
//#define imaginary _Imaginary
//#define _Imaginary_I iF
#define I _Complex_I
#endif

#endif /*__COMPLEX_H__*/

