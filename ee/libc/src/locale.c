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
# Locale definitions.
*/
#include <locale.h>

#ifdef F_setlocale
char  _ps2sdk_locale[2] = "C";

char *setlocale(int cat, const char *loc)
{  
  if (loc == NULL)
    return _ps2sdk_locale;

  if (loc[0] == 'C' && loc[1] == '\0')
    return _ps2sdk_locale;
  
  return NULL;
}
#endif

#ifdef F_localeconv

struct lconv _ps2sdk_lconv = 
{
	".",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff
};

struct lconv *localeconv(void)
{
  return &_ps2sdk_lconv;
}
#endif
