/*	$OpenBSD: math_private.h,v 1.11 2008/12/09 20:00:35 martynas Exp $	*/
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/*
 * from: @(#)fdlibm.h 5.1 93/09/24
 */

#ifndef _MATH_PRIVATE_H_
#define _MATH_PRIVATE_H_

#if defined(i386) || defined(i486) || \
	defined(intel) || defined(x86) || defined(i86pc) || \
	defined(__alpha) || defined(__osf__)
#define __LITTLE_ENDIAN
#endif

/* The original fdlibm code used statements like:
	n0 = ((*(int*)&one)>>29)^1;		* index of high word *
	ix0 = *(n0+(int*)&x);			* high word of x *
	ix1 = *((1-n0)+(int*)&x);		* low word of x *
   to dig two 32 bit words out of the 64 bit IEEE floating point
   value.  That is non-ANSI, and, moreover, the gcc instruction
   scheduler gets it wrong.  We instead use the following macros.
   Unlike the original code, we determine the endianness at compile
   time, not at run time; I don't see much benefit to selecting
   endianness at run time.  */

/* A union which permits us to convert between a double and two 32 bit
   ints.  */

/*
 * The arm32 port is little endian except for the FP word order which is
 * big endian.
 */

typedef unsigned long long u_int64_t;
typedef long long int64_t;
typedef unsigned int u_int32_t;
typedef int int32_t;

#if defined(__BIG_ENDIAN) || defined(arm32)

typedef union
{
  double value;
  struct
  {
    u_int32_t msw;
    u_int32_t lsw;
  } parts;
  struct
  {
    u_int64_t w;
  } xparts;
} ieee_double_shape_type;

typedef union
{
  long double value;
  struct {
    u_int32_t msw;
    u_int32_t nsw;
    u_int32_t lsw;
  } parts;
} ieee_extended_shape_type;

typedef union
{
  long double value;
  struct {
    u_int32_t mswhi;
    u_int32_t mswlo;
    u_int32_t lswhi;
    u_int32_t lswlo;
  } parts;
} ieee_quad_shape_type;

#endif

#if defined(__LITTLE_ENDIAN) && !defined(arm32)

typedef union
{
  double value;
  struct
  {
    u_int32_t lsw;
    u_int32_t msw;
  } parts;
  struct
  {
    u_int64_t w;
  } xparts;
} ieee_double_shape_type;

typedef union
{
  long double value;
  struct {
    u_int32_t lswlo;
    u_int32_t lswhi;
    u_int32_t mswlo;
    u_int32_t mswhi;
  } parts;
} ieee_quad_shape_type;

typedef union
{
  long double value;
  struct {
    u_int32_t lsw;
    u_int32_t nsw;
    u_int32_t msw;
  } parts;
} ieee_extended_shape_type;

#endif

/* Get two 32 bit ints from a double.  */

#define EXTRACT_WORDS(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type ew_u;					\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)

/* Get a 64-bit int from a double. */
#define EXTRACT_WORD64(ix,d)					\
do {								\
  ieee_double_shape_type ew_u;					\
  ew_u.value = (d);						\
  (ix) = ew_u.xparts.w;						\
} while (0)

/* Get the more significant 32 bit int from a double.  */

#define GET_HIGH_WORD(i,d)					\
do {								\
  ieee_double_shape_type gh_u;					\
  gh_u.value = (d);						\
  (i) = gh_u.parts.msw;						\
} while (0)

/* Get the less significant 32 bit int from a double.  */

#define GET_LOW_WORD(i,d)					\
do {								\
  ieee_double_shape_type gl_u;					\
  gl_u.value = (d);						\
  (i) = gl_u.parts.lsw;						\
} while (0)

/* Set a double from two 32 bit ints.  */

#define INSERT_WORDS(d,ix0,ix1)					\
do {								\
  ieee_double_shape_type iw_u;					\
  iw_u.parts.msw = (ix0);					\
  iw_u.parts.lsw = (ix1);					\
  (d) = iw_u.value;						\
} while (0)

/* Set a double from a 64-bit int. */
#define INSERT_WORD64(d,ix)					\
do {								\
  ieee_double_shape_type iw_u;					\
  iw_u.xparts.w = (ix);						\
  (d) = iw_u.value;						\
} while (0)

/* Set the more significant 32 bits of a double from an int.  */

#define SET_HIGH_WORD(d,v)					\
do {								\
  ieee_double_shape_type sh_u;					\
  sh_u.value = (d);						\
  sh_u.parts.msw = (v);						\
  (d) = sh_u.value;						\
} while (0)

/* Set the less significant 32 bits of a double from an int.  */

#define SET_LOW_WORD(d,v)					\
do {								\
  ieee_double_shape_type sl_u;					\
  sl_u.value = (d);						\
  sl_u.parts.lsw = (v);						\
  (d) = sl_u.value;						\
} while (0)

/* A union which permits us to convert between a float and a 32 bit
   int.  */

typedef union
{
  float value;
  u_int32_t word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)					\
do {								\
  ieee_float_shape_type gf_u;					\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)					\
do {								\
  ieee_float_shape_type sf_u;					\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (0)

#ifdef __COMPLEX_H__
/*
 * C99 specifies that complex numbers have the same representation as
 * an array of two elements, where the first element is the real part
 * and the second element is the imaginary part.
 */
typedef union {
	float complex f;
	float a[2];
} float_complex;
typedef union {
	double complex f;
	double a[2];
} double_complex;
typedef union {
	long double complex f;
	long double a[2];
} long_double_complex;
#define	REALPART(z)	((z).a[0])
#define	IMAGPART(z)	((z).a[1])

/*
 * Inline functions that can be used to construct complex values.
 *
 * The C99 standard intends x+I*y to be used for this, but x+I*y is
 * currently unusable in general since gcc introduces many overflow,
 * underflow, sign and efficiency bugs by rewriting I*y as
 * (0.0+I)*(y+0.0*I) and laboriously computing the full complex product.
 * In particular, I*Inf is corrupted to NaN+I*Inf, and I*-0 is corrupted
 * to -0.0+I*0.0.
 *
 * The C11 standard introduced the macros CMPLX(), CMPLXF() and CMPLXL()
 * to construct complex values.  Compilers that conform to the C99
 * standard require the following functions to avoid the above issues.
 */

#ifndef CMPLXF
static inline float complex
CMPLXF(float x, float y)
{
	float_complex z;
	REALPART(z) = x;
	IMAGPART(z) = y;
	return (z.f);
}
#endif

#ifndef CMPLX
static inline double complex
CMPLX(double x, double y)
{
	double_complex z;

	REALPART(z) = x;
	IMAGPART(z) = y;
	return (z.f);
}
#endif

#endif /*__COMPLEX_H__*/

#ifdef FLT_EVAL_METHOD
/*
 * Attempt to get strict C99 semantics for assignment with non-C99 compilers.
 */
#if FLT_EVAL_METHOD == 0 || __GNUC__ == 0
#define	STRICT_ASSIGN(type, lval, rval)	((lval) = (rval))
#else /* FLT_EVAL_METHOD == 0 || __GNUC__ == 0 */
#define	STRICT_ASSIGN(type, lval, rval) do {	\
	volatile type __lval;			\
						\
	if (sizeof(type) >= sizeof(double))	\
		(lval) = (rval);		\
	else {					\
		__lval = (rval);		\
		(lval) = __lval;		\
	}					\
} while (0)
#endif /* FLT_EVAL_METHOD == 0 || __GNUC__ == 0 */
#endif /* FLT_EVAL_METHOD */

/* fdlibm kernel function */
#ifndef INLINE_REM_PIO2
int    __rem_pio2(double,double*);
#endif
double __kernel_sin(double,double,int);
double __kernel_cos(double,double);
double __kernel_tan(double,double,int);
int    __kernel_rem_pio2(double*,double*,int,int,int);

/* float precision kernel functions */
#ifndef INLINE_REM_PIO2F
int	__rem_pio2f(float,double*);
#endif
#ifndef INLINE_KERNEL_SINDF
float	__kernel_sindf(double);
#endif
#ifndef INLINE_KERNEL_COSDF
float	__kernel_cosdf(double);
#endif
#ifndef INLINE_KERNEL_TANDF
float	__kernel_tandf(double,int);
#endif

/* long double precision kernel functions */
long double __kernel_sinl(long double, long double, int);
long double __kernel_cosl(long double, long double);
long double __kernel_tanl(long double, long double, int);

typedef unsigned int uint32_t;

/*
 * Common routine to process the arguments to nan(), nanf(), and nanl().
 */
void _scan_nan(uint32_t *__words, int __num_words, const char *__s);

/*
 * TRUNC() is a macro that sets the trailing 27 bits in the mantissa
 * of an IEEE double variable to zero.  It must be expression-like
 * for syntactic reasons, and we implement this expression using
 * an inline function instead of a pure macro to avoid depending
 * on the gcc feature of statement-expressions.
 */
#define TRUNC(d)	(_b_trunc(&(d)))

static __inline void
_b_trunc(volatile double *_dp)
{
	uint32_t _lw;

	GET_LOW_WORD(_lw, *_dp);
	SET_LOW_WORD(*_dp, _lw & 0xf8000000);
}

struct Double {
	double	a;
	double	b;
};

/*
 * Functions internal to the math package, yet not static.
 */
double __exp__D(double, double);
struct Double __log__D(double);

#endif /* _MATH_PRIVATE_H_ */
