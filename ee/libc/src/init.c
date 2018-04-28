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
void _ps2sdk_libc_deinit()
{
    _ps2sdk_time_deinit();
    _ps2sdk_stdio_deinit();
    _ps2sdk_alloc_deinit();
}

void _ps2sdk_libc_init()
{
    _ps2sdk_alloc_init();
    _ps2sdk_stdio_init();
    _ps2sdk_time_init();
    atexit(_ps2sdk_libc_deinit);
 
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

