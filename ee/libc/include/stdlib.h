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
 * Definition file for our stdlib.
 */

#ifndef __STDLIB_H__
#define __STDLIB_H__

#define __need_size_t
#define __need_wchar_t
#define __need_NULL
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* exit status constants. */
#define EXIT_SUCCESS                   0
#define EXIT_FAILURE                   1

/* multibyte maximum character constant. */
#define MB_CUR_MAX                     4

#ifndef __DIV_T_DEFINED
#define __DIV_T_DEFINED
typedef struct {
  int quot;
  int rem;
} div_t;
#endif

#ifndef __LDIV_T_DEFINED
#define __LDIV_T_DEFINED
typedef struct {
  long quot;
  long rem;
} ldiv_t;
#endif

#ifndef __STRICT_ANSI__
#ifndef __LLDIV_T_DEFINED
#define __LLDIV_T_DEFINED
typedef struct {
  long long quot;
  long long rem;
} lldiv_t;
#endif
#endif

/* we don't check for any previously defined value. This HAS to be that. */
#define        RAND_MAX        2147483647

/* function declarations. */

void          *malloc(size_t size);
void          *realloc(void *ptr, size_t size);
void          *calloc(size_t n, size_t size);
void          free(void * ptr);

void          abort(void) __attribute__ ((noreturn));
int           abs(int);
int           atexit(void (*)(void));
double        atof(const char *);
void          exit(int) __attribute__ ((noreturn));
//int           atoi(const char *);
//long          atol(const char *);
//#define       atoi(x) strtol(x, NULL, 10)
//#define       atol atoi
void          *bsearch(const void *, const void *, size_t, size_t, int (*)(const void *, const void *));
div_t         div(int, int);
char          *getenv(const char *);
long          labs(long);
ldiv_t        ldiv(long, long);
#ifndef __STRICT_ANSI__
long long     llabs(long long);
lldiv_t       lldiv(long long, long long);
#endif
int           rand(void);
int           setenv(const char *, const char *, int);
void          srand(unsigned int);
double        strtod(const char *, char **);
long          strtol(const char *, char **, int);
unsigned long strtoul(const char *, char **, int);

#ifndef INLINE
# if __GNUC__ && !__GNUC_STDC_INLINE__
#  define INLINE extern __inline__
# else
#  define INLINE inline
# endif
#endif
INLINE int atoi(const char * x) { return strtol(x, NULL, 10); }
INLINE long atol(const char * x) { return strtol(x, NULL, 10); }
#undef INLINE

void          qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

/* UTF-8 multi-byte string functions */
int           mblen(const char *, size_t);
int           mbslen(const char *s);
size_t        mbstowcs(wchar_t *, const char *, size_t);
int           mbtowc(wchar_t *, const char *, size_t);
size_t        wcstombs(char *, const wchar_t *, size_t);
int           wctomb(char *, wchar_t);

//char          *_gcvt(double, size_t, char *);
char          *_itoa(int, char *, int);
char          *_ltoa(long, char *, int);
#ifndef __STRICT_ANSI__
char          *_lltoa(long long, char *, int);
#endif

// blah! C++ is evil.
int system (const char * string);
int putenv (char *);

#ifdef __cplusplus
}

/* C++ is evil, let's defeat it... */
#define _CPP_CSTDLIB 1

#endif

#endif /* __STDLIB_H__ */
