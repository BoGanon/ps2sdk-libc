/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
## FREEBSD MSUN (FreeBSD/2-clause BSD/Simplified BSD License)
#
#    Copyright 1992-2011 The FreeBSD Project. All rights reserved.
#
#       Redistribution and use in source and binary forms, with or without
#       modification, are permitted provided that the following conditions are
#       met:
#
#    1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#    2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#    THIS SOFTWARE IS PROVIDED BY THE FREEBSD PROJECT ``AS IS'' AND ANY
#    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE FREEBSD PROJECT OR
#    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
>
#    The views and conclusions contained in the software and documentation
#    are those of the authors and should not be interpreted as representing
#    official policies, either expressed or implied, of the FreeBSD
#    Project.
#
## FDLIBM
#
#    Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
#
#    Developed at SunPro, a Sun Microsystems, Inc. business.
#    Permission to use, copy, modify, and distribute this
#    software is freely granted, provided that this notice
#    is preserved.
*/

/*
 * @file
 * Common math
 */

#ifndef __MATH_H__
#define __MATH_H__

#if (defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || __cplusplus
  #define MATH_STDC 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ANSI */
#define HUGE_VAL ((double)0x7ff0000000000000LL)

extern double      fabs(double);
extern double      fmod(double, double);

extern double      exp(double);
extern double      log(double);
extern double      log10(double);

extern double      pow(double, double);
extern double      sqrt(double);

extern double      sin(double);
extern double      cos(double);
extern double      tan(double);
extern double      asin(double);
extern double      acos(double);
extern double      atan(double);
extern double      atan2(double, double);

extern double      sinh(double);
extern double      cosh(double);
extern double      tanh(double);

extern double      ceil(double);
extern double      floor(double);

extern double      frexp(double, int *);
extern double      ldexp(double, int);
extern double      modf(double, double*);


#if !defined(__STRICT_ANSI__) || defined(MATH_STDC)
#if defined (FLT_EVAL_METHOD)
  #if FLT_EVAL_METHOD == 0
    typedef float float_t;
    typedef double double_t;
  #elif FLT_EVAL_METHOD == 1
    typedef double float_t;
    typedef double double_t;
  #elif FLT_EVAL_METHOD == 2
    typedef long double float_t;
    typedef long double double_t;
  #elif FLT_EVAL_METHOD == -1
  #else
    typedef float float_t;
    typedef double double_t;
  #endif
#else
  typedef float float_t;
  typedef double double_t;
#endif /* FLT_EVAL_METHOD */

#define HUGE_VALF		((float)0x7f800000)
//#define HUGE_VALL
#ifdef FENV_SOFTFLOAT
#define INFINITY		((float)0x7f800000)
#define NAN			((float)0x7fC00000)
#endif

#define MATH_ERRNO		1
#define MATH_ERREXCEPT		2
#define math_errhandling	MATH_ERREXCEPT

#define	FP_ILOGB0		(-2147483647)
#define	FP_ILOGBNAN		2147483647

#endif /* STDC */

/* Long double precision */
#if !defined(__STRICT_ANSI__) || defined(MATH_STDC)
/*
extern long double fabsl(long double);
extern long double fmodl(long double, long double);
extern long double remainderl(long double, long double);
extern long double remquol(long double, long double, int *);
extern long double fmal(long double, long double, long double);
extern long double fmaxl(long double, long double);
extern long double fminl(long double, long double);
extern long double fdiml(long double, long double);
extern long double nanl(const char *);

extern long double expl(long double);
extern long double exp2l(long double);
extern long double expm1l(long double);
extern long double logl(long double);
extern long double log10l(long double);
extern long double log2l(long double);
extern long double log1pl(long double);

extern long double powl(long double, long double);
extern long double sqrtl(long double);
extern long double cbrtl(long double);
extern long double hypotl(long double, long double);

extern long double sinl(long double);
extern long double cosl(long double);
extern long double tanl(long double);
extern long double asinl(long double);
extern long double acosl(long double);
extern long double atanl(long double);
extern long double atan2l(long double, long double);

extern long double sinhl(long double);
extern long double coshl(long double);
extern long double tanhl(long double);
extern long double asinhl(long double);
extern long double acoshl(long double);
extern long double atanhl(long double);

extern long double erfl(long double);
extern long double erfcl(long double);
extern long double tgammal(long double);
extern long double lgammal(long double);

extern long double ceill(long double);
extern long double floorl(long double);
extern long double truncl(long double);
extern long double roundl(long double);
extern long        lroundl(long double);
extern long long   llroundl(long double);
extern long double nearbyintl(long double);
extern long double rintl(long double);
extern long        lrintl(long double);
extern long long   llrintl(long double);

extern long double frexpl(long double, int *);
extern long double ldexpl(long double, int);
extern long double modfl(long double, long double*);
extern long double scalbnl(long double, int);
extern long double scalblnl(long double, long);
extern int         ilogbl(long double);
extern long double logbl(long double);
extern long double nextafterl(long double, long double);
extern long double nexttowardl(long double, long double);
extern long double copysignl(long double, long double);
*/
#endif

/* Double precision */
#if !defined(__STRICT_ANSI__) || defined(MATH_STDC)
extern double      remainder(double, double);
extern double      remquo(double, double, int *);
extern double      fmin(double, double);
extern double      fmax(double, double);
extern double      fma(double, double, double);
extern double      fdim(double, double);
extern double      nan(const char *);

extern double      exp2(double);
extern double      expm1(double);
extern double      log2(double);
extern double      log1p(double);

extern double      cbrt(double);
extern double      hypot(double, double);

extern double      asinh(double);
extern double      acosh(double);
extern double      atanh(double);

extern double      erf(double);
extern double      erfc(double);
extern double      tgamma(double);
extern double      lgamma(double);

extern double      trunc(double);
extern double      round(double);
extern long        lround(double);
extern long long   llround(double);
extern double      nearbyint(double);
extern double      rint(double);
extern long        lrint(double);
extern long long   llrint(double);

extern double      scalbn(double, int);
extern double      scalbln(double, long);
extern int         ilogb(double);
extern double      logb(double);
extern double      nextafter(double, double);
extern double      nexttoward(double, long double);
extern double      copysign(double, double);
#endif

/* Single precision */
#if !defined(__STRICT_ANSI__) || defined(MATH_STDC)
extern float       fabsf(float);
extern float       fmodf(float, float);
extern float       remainderf(float, float);
extern float       remquof(float, float, int *);
extern float       fmaf(float, float, float);
extern float       fmaxf(float, float);
extern float       fminf(float, float);
extern float       fdimf(float, float);
extern float       nanf(const char *);

extern float       expf(float);
extern float       exp2f(float);
extern float       expm1f(float);
extern float       logf(float);
extern float       log10f(float);
extern float       log2f(float);
extern float       log1pf(float);

extern float       powf(float, float);
extern float       sqrtf(float);
extern float       cbrtf(float);
extern float       hypotf(float, float);

extern float       sinf(float);
extern float       cosf(float);
extern float       tanf(float);
extern float       asinf(float);
extern float       acosf(float);
extern float       atanf(float);
extern float       atan2f(float, float);

extern float       sinhf(float);
extern float       coshf(float);
extern float       tanhf(float);
extern float       asinhf(float);
extern float       acoshf(float);
extern float       atanhf(float);

extern float       erff(float);
extern float       erfcf(float);
/* Uses double precision call. */
extern float       tgammaf(float);
extern float       lgammaf(float);

extern float       ceilf(float);
extern float       floorf(float);
extern float       truncf(float);
extern float       roundf(float);
extern long        lroundf(float);
extern long long   llroundf(float);
extern float       nearbyintf(float);
extern float       rintf(float);
extern long        lrintf(float);
extern long long   llrintf(float);

extern float       frexpf(float, int *);
extern float       ldexpf(float, int);
extern float       modff(float, float *);
extern float       scalbnf(float, int);
extern float       scalblnf(float, long);
extern int         ilogbf(float);
extern float       logbf(float);
extern float       nextafterf(float, float);
extern float       nexttowardf(float, long double);
extern float       copysignf(float, float);
#endif

/* Comparison */
extern int         __fpclassify(double);
extern int         __fpclassifyf(float);
extern int         __isfinite(double);
extern int         __isfinitef(float);
extern int         __isinf(double);
extern int         __isinff(float);
extern int         __isnan(double);
extern int         __isnanf(float);
extern int         __isnormal(double);
extern int         __isnormalf(float);
extern int         __signbit(double);
extern int         __signbitf(float);

#if !defined(__STRICT_ANSI__) || defined(MATH_STDC)

#define FP_NORMAL		0
#define FP_SUBNORMAL		1
#define FP_ZERO			2
#define FP_INFINITE		3
#define FP_NAN			4

#define            fpclassify(x) \
  ((sizeof(x) == sizeof(float)) ? __fpclassifyf(x) : \
   (sizeof(x) == sizeof(double)) ? __fpclassify(x) : 0)
#define            isfinite(x) \
  ((sizeof(x) == sizeof(float)) ? __isfinitef(x) : \
   (sizeof(x) == sizeof(double)) ? __isfinite(x) : 0)
#define            isinf(x) \
  ((sizeof(x) == sizeof(float)) ? __isinff(x) : \
   (sizeof(x) == sizeof(double)) ? __isinf(x) : 0)
#define            isnan(x) \
  ((sizeof(x) == sizeof(float)) ? __isnanf(x) : \
   (sizeof(x) == sizeof(double)) ? __isnan(x) : 0)
#define            isnormal(x) \
  ((sizeof(x) == sizeof(float)) ? __isnormalf(x) : \
   (sizeof(x) == sizeof(double)) ? __isnormal(x) : 0)
#define            signbit(x) \
  ((sizeof(x) == sizeof(float)) ? __signbitf(x) : \
   (sizeof(x) == sizeof(double)) ? __signbit(x) : 0)
#define            isunordered(x,y) \
  (isnan(x) || isnan(y))
#define            isgreater(x,y) \
  (!isunordered(x,y) && (x > y))
#define            isgreaterequal(x,y) \
  (!isunordered(x,y) && (x >= y))
#define            isless(x,y) \
  (!isunordered(x,y) && (x < y))
#define            islessequal(x,y) \
  (!isunordered(x,y) && (x <= y))
#define            islessgreater(x,y) \
  (!isunordered(x,y) && ((x < y) || (x > y)))
#endif /* STDC_VERSION >= 19901L */

/* POSIX */
#ifndef __STRICT_ANSI__
#define finitef(x) __isfinitef(x)

#define	MAXFLOAT	((float)3.40282346638528860e+38)

#define M_E		2.7182818284590452354
#define M_LOG2E		1.4426950408889634074
#define M_LOG10E	0.43429448190325182765
#define M_LN2		0.69314718055994530942
#define M_LN10		2.30258509299404568402
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.78539816339744830962
#define M_1_PI		0.31830988618379067154
#define M_2_PI		0.63661977236758134308
#define M_2_SQRTPI	1.12837916709551257390
#define M_SQRT2		1.41421356237309504880
#define M_SQRT1_2	0.70710678118654752440

extern int signgam;

extern double      j0(double);
extern float       j0f(float);

extern double      j1(double);
extern float       j1f(float);

extern double      jn(int, double);
extern float       jnf(int, float);

extern double      y0(double);
extern float       y0f(float);

extern double      y1(double);
extern float       y1f(float);

extern double      yn(int, double);
extern float       ynf(int, float);

extern double      lgamma_r(double,int*);
#endif

#ifdef __cplusplus
}
#endif

#endif /*__MATH_H__*/

