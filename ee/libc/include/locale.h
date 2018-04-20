/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/** @file
    Standard locale declarations.
*/

#ifndef __LOCALE_H_

#define __need_NULL
#include <stddef.h>

#define LC_ALL	    0
#define LC_COLLATE  1
#define LC_CTYPE    2
#define LC_MONETARY 3
#define LC_NUMERIC  4
#define LC_TIME     5
#define LC_MESSAGES 6

#ifdef __cplusplus
extern "C" {
#endif

struct lconv
{
  char decimal_point[2];
  char thousands_sep[2];
  char grouping[2];
  char int_curr_symbol[2];
  char currency_symbol[2];
  char mon_decimal_point[2];
  char mon_thousands_sep[2];
  char mon_grouping[2];
  char positive_sign[2];
  char negative_sign[2];
  char int_frac_digits;
  char frac_digits;
  char p_cs_precedes;
  char p_sep_by_space;
  char n_cs_precedes;
  char n_sep_by_space;
  char p_sign_posn;
  char n_sign_posn;
};

char *setlocale(int cat, const char *loc);
struct lconv *localeconv(void);

#ifdef __cplusplus
}
#endif

#endif /*_LOCALE_H_*/

