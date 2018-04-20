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
 * Floating-point environment.
 */

#ifndef __FENV_H__
#define __FENV_H__

/* Define FENV_SOFTFLOAT in CFLAGS to use an emulated
   floating point environment. */

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int fenv_t;
typedef unsigned int fexcept_t;

/* Denormalized to zero */
#define FE_ZERODENORMAL	0x1000000

/* Condition bit. */
#define FE_CONDITION	0x800000

/* Cause exception bits.
   Set or cleared after every FPU operation. */
#define FE_ALL_EXCEPT_CAUSE	(FE_UNDERFLOW_CAUSE | FE_OVERFLOW_CAUSE \
				 | FE_DIVBYZERO_CAUSE | FE_INVALID_CAUSE)
#define FE_UNDERFLOW_CAUSE	0x04000
#define FE_OVERFLOW_CAUSE	0x08000
#define FE_DIVBYZERO_CAUSE	0x10000
#define FE_INVALID_CAUSE	0x20000
//#define FE_INEXACT

/* Shift amount for cause bits. */
#define FE_CAUSE_SHIFT	11

/* Sticky exceptions bits.
   Note: Not used when rounding. */
#define FE_ALL_EXCEPT	(FE_UNDERFLOW | FE_OVERFLOW | FE_DIVBYZERO | FE_INVALID)
#define FE_UNDERFLOW	0x0008
#define FE_OVERFLOW	0x0010
#define FE_DIVBYZERO	0x0020
#define FE_INVALID	0x0040

#ifdef FENV_SOFTFLOAT
 #define FE_INEXACT	0x0080
#endif

/* Rounding mode */
#define FE_TOWARDZERO	1

#ifdef FENV_SOFTFLOAT
 #define FE_TONEAREST	0
 #define FE_UPWARD	2
 #define FE_DOWNWARD	3
#endif

/* Denormalized numbers are automatically rounded to 0. */
#define FE_ZERO_DENORMALS	0x01000000

/* Condition bit. */
#define FE_COND_BIT	0x800000

/* Default floating point environment.
   Default value is (FE_ZERO_DENORMALS | FE_TOWARDZERO). */
extern const fenv_t __default_fenv;
#define FE_DFL_ENV	(&__default_fenv)

#ifndef FENV_SOFTFLOAT
#define __get_fenv(f) __asm__ __volatile__("\tcfc1 %0, $31\n\t" : "=r" (f))
#define __set_fenv(f) __asm__ __volatile__("ctc1 %0, $31" :: "r" (f))

static inline int feclearexcept(int excepts)
{
  fexcept_t status;

  excepts &= FE_ALL_EXCEPT;
  excepts |= (excepts << FE_CAUSE_SHIFT);

  __get_fenv(status);
  status &= ~excepts;
  __set_fenv(status);

  return 0;
}


static inline int fetestexcept(int excepts)
{
  fexcept_t status;

  excepts &= FE_ALL_EXCEPT;
  excepts |= (excepts << FE_CAUSE_SHIFT);

  __get_fenv(status);

  return (status & excepts);
}

static inline int feraiseexcept(int excepts)
{
  fexcept_t status;

  excepts &= FE_ALL_EXCEPT;
  excepts |= (excepts << FE_CAUSE_SHIFT);

  __get_fenv(status);
  status |= excepts;
  __set_fenv(status);

  return 0;
}

static inline int fegetexceptflag(fexcept_t *flagp, int excepts)
{

  /* The r5900 fpu does not set sticky bits when rounding.
     Make sure to store cause bits but don't set them back. */
  excepts &= FE_ALL_EXCEPT;
  excepts |= (excepts << FE_CAUSE_SHIFT);

  __get_fenv(*flagp);

  *flagp &= excepts;

  return 0;
}

static inline int fesetexceptflag(const fexcept_t *flagp, int excepts)
{
  fexcept_t status;

  /* Only set the sticky bits. flagp should be from previous
     fegetexceptflag() call so clear cause bits. */
  excepts &= FE_ALL_EXCEPT;

  __get_fenv(status);
  status &= ~FE_ALL_EXCEPT;
  status |= (*flagp & excepts);
  __set_fenv(status);

  return 0;
}

/* The r5900 FPU only supports rounding to zero. */
static inline int fegetround(void)
{
  return FE_TOWARDZERO;
}

static inline int fesetround(int round)
{
  if (round == FE_TOWARDZERO)
    return 0;

  return 1;
}

static inline int fegetenv(fenv_t *envp)
{
  __get_fenv(*envp);

  return 0;
}

static inline int fesetenv(const fenv_t *envp)
{
  __set_fenv(*envp);

  return 0;
}

/* The r5900's FPU does not trap. */
static inline int feholdexcept(fenv_t *envp)
{
  __get_fenv(*envp);
  return 0;
}

static inline int feupdateenv(const fenv_t *envp)
{
  fexcept_t status;

  __get_fenv(status);
  __set_fenv(*envp);
  feraiseexcept(status);

  return 0;
}
#else /* !FENV_SOFTFLOAT */

#define __set_env(env, flags, mask, rnd) (env = (flags | \
						 (mask << FE_CAUSE_SHIFT) | \
						 (rnd)))
#define __env_flags(env) ((env) & FE_ALL_EXCEPT)
#define __env_mask(env) ((env >> FE_CAUSE_SHIFT) & FE_ALL_EXCEPT)
#define __env_round(env)  (env & 3)
#define __fenv_static static

#include "fenv-softfloat.h"

#endif /* FENV_SOFTFLOAT */

#ifdef __cplusplus
}
#endif

#endif /*__FENV_H__*/
