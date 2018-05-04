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
 * The global init/deinit code for our crt0.
 */

#include <stdlib.h>

void _ps2sdk_alloc_init();
void _ps2sdk_alloc_deinit();
void _ps2sdk_stdio_init();
void _ps2sdk_stdio_deinit();
void _ps2sdk_time_init();
void _ps2sdk_time_deinit();

int chdir(const char *path);

#if defined(F_init_libc) || defined(DOXYGEN)
extern void (*__preinit_array_end [])(void) __attribute__((weak));
extern void (*__preinit_array_start [])(void) __attribute((weak));
extern void (*__init_array_start [])(void) __attribute((weak));
extern void (*__init_array_end [])(void) __attribute((weak));
extern void (*__fini_array_start [])(void) __attribute((weak));
extern void (*__fini_array_end [])(void) __attribute((weak));

void _init();
void _fini();

void __libc_init()
{
    _ps2sdk_alloc_init();
    _ps2sdk_stdio_init();
    _ps2sdk_time_init();
}

void __libc_deinit()
{
    _ps2sdk_time_deinit();
    _ps2sdk_stdio_deinit();
    _ps2sdk_alloc_deinit();
}

void __libc_init_array()
{
    int i;

    for (i = 0; i < (__preinit_array_end - __preinit_array_start); i++)
      __preinit_array_start[i]();

    _init();

    for (i = 0; i < (__init_array_end - __init_array_start); i++)
      __init_array_start[i]();
}

void __libc_fini_array()
{
    int i;

    /* Run through destructors in reverse order. */
    for (i = (__fini_array_end - __fini_array_start) - 1; i >=0; --i)
      __fini_array_start[i]();

    _fini();
}
#endif

#if defined(F_init_args) || defined(DOXYGEN)
void _ps2sdk_args_parse(int argc, char ** argv)
{
    if (argc == 0) // naplink!
    {
	chdir("host:");
    } else {
	char * p, * s = 0;
	// let's find the last slash, or at worst, the :
	for (p = argv[0]; *p; p++) {
	    if ((*p == '/') || (*p == '\\') || (*p == ':')) {
		s = p;
	    }
	}
	// Nothing?! strange, let's use host.
	if (!s) {
	    chdir("host:");
	} else {
	    char backup = *(++s);
	    *s = 0;
	    chdir(argv[0]);
	    *s = backup;
	}
    }
}
#endif

