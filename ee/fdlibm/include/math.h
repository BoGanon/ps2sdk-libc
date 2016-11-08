#ifndef __MATH_H__
#define __MATH_H__

#ifdef __cplusplus
extern "C" {
#endif

//extern int isinf(double);
extern int isinff(float);
extern int isnan(double);
extern int isnanf(float);
extern int finite(double);
extern int finitef(float);
extern int signbit(double);
extern int signbitf(float);

/*
long double acoshl(long double);
long double acosl(long double);
long double asinhl(long double);
long double asinl(long double);
long double atanl(long double);
long double atan2l(long double, long double);
long double atanhl(long double);
long double cbrtl(long double);
long double ceill(long double);
long double copysignl(long double, long double);
long double cosl(long double);
long double coshl(long double);
long double erfl(long double);
long double erfcl(long double);
long double expl(long double);
long double exp2l(long double);
long double expm1l(long double);
long double fabsl(long double);
long double fdiml(long double, long double);
long double floorl(long double);
long double fmal(long double, long double, long double);
long double fmaxl(long double, long double);
long double fminl(long double, long double);
long double fmodl(long double, long double);
long double frexpl(long double, int *);
long double hypotl(long double, long double);
int         ilogbl(long double);
long double ldexpl(long double, int);
long double lgammal(long double);
long long   llrintl(long double);
long long   llroundl(long double);
long double logl(long double);
long double log10l(long double);
long double log1pl(long double);
long double log2l(long double);
long double logbl(long double);
long double modfl(long double, long double*);
long double nanl(const char *);
long double nearbyintl(long double);
long double nextafterl(long double, long double);
long double nexttowardl(long double, long double);
long double powl(long double, long double);
long double remainderl(long double, long double);
long double remquol(long double, long double, int *);
long double rintl(long double);
long double roundl(long double);
long double scalblnl(long double, long);
long double scalbnl(long double, int);
long double sinl(long double);
long double sinhl(long double);
long double sqrtl(long double);
long double tanl(long double);
long double tanhl(long double);
long double tgammal(long double);
long double truncl(long double);
*/

extern double      acos(double);
extern float       acosf(float);
extern double      acosh(double);
extern float       acoshf(float);
extern double      asin(double);
extern float       asinf(float);
extern double      asinh(double);
extern float       asinhf(float);
extern double      atan(double);
extern float       atanf(float);
extern double      atan2(double, double);
extern float       atan2f(float, float);
extern double      atanh(double);
extern float       atanhf(float);
extern double      cbrt(double);
extern float       cbrtf(float);
extern double      ceil(double);
extern float       ceilf(float);
extern double      copysign(double, double);
extern float       copysignf(float, float);
extern double      cos(double);
extern float       cosf(float);
extern double      cosh(double);
extern float       coshf(float);
extern double      erf(double);
extern float       erff(float);
extern double      erfc(double);
extern float       erfcf(float);
extern double     exp(double);
extern float       expf(float);
//extern double      exp2(double); equivalent to pow(2.0,x);
extern float       exp2f(float);
extern double     expm1(double);
extern float       expm1f(float);
extern double     fabs(double);
extern float       fabsf(float);
//extern double      fdim(double, double);
//extern float       fdimf(float, float);
extern double     floor(double);
extern float       floorf(float);
//extern double      fma(double, double, double);
//extern float       fmaf(float, float, float);
//extern double      fmax(double, double);
extern float       fmaxf(float, float);
//extern double      fmin(double, double);
extern float       fminf(float, float);
extern double      fmod(double, double);
extern float       fmodf(float, float);
extern double      frexp(double, int *);
extern float       frexpf(float, int *);
extern double      hypot(double, double);
extern float       hypotf(float, float);
extern int         ilogb(double);
extern int         ilogbf(float);
extern double      ldexp(double, int);
extern float       ldexpf(float, int);
extern double      lgamma(double);
extern float       lgammaf(float);
//extern long long   llrint(double);
//extern long long   llrintf(float);
//extern long long   llround(double);
//extern long long   llroundf(float);
extern double      log(double);
extern float       logf(float);
extern double      log10(double);
extern float       log10f(float);
extern double      log1p(double);
extern float       log1pf(float);
extern double      log2(double);
extern float       log2f(float);
extern double      logb(double);
extern float       logbf(float);
//extern long        lrint(double);
//extern long        lrintf(float);
//extern long        lrintl(long double);
//extern long        lround(double);
//extern long        lroundf(float);
//extern long        lroundl(long double);
extern double      modf(double, double*);
extern float       modff(float, float *);
//extern double      nan(const char *);
//extern float       nanf(const char *);
//extern double      nearbyint(double);
//extern float       nearbyintf(float);
extern double      nextafter(double, double);
extern float       nextafterf(float, float);
//extern double      nexttoward(double, long double);
//extern float       nexttowardf(float, long double);
extern double      pow(double, double);
extern float       powf(float, float);
extern double      remainder(double, double);
extern float       remainderf(float, float);
//extern double      remquo(double, double, int *);
extern float       remquof(float, float, int *);
extern double      rint(double);
extern float       rintf(float);
//extern double      round(double);
extern float       roundf(float);
extern double      scalb(double,double);
extern float       scalbf(float,float);
//extern double      scalbln(double, long);
//extern float       scalblnf(float, long);
extern double      scalbn(double, int);
extern float       scalbnf(float, int);
extern double      sin(double);
extern float       sinf(float);
extern double      sinh(double);
extern float       sinhf(float);
extern double      sqrt(double);
extern float       sqrtf(float);
extern double      tan(double);
extern float       tanf(float);
extern double      tanh(double);
extern float       tanhf(float);
extern double      tgamma(double);
//extern float       tgammaf(float);
extern double      trunc(double);
extern float       truncf(float);

/* Nonstandard */
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

#ifdef __cplusplus
}
#endif

#endif /*__MATH_H__*/

