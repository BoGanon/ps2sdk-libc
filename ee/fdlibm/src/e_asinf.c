/* e_asinf.c -- float version of e_asin.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

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

#include "math.h"
#include "math_private.h"

static const float
one  =  1.0f,			/* 0x3F800000 */
huge =  1.000e+30f,
pio2acc = 1.5707964e+00f,	/* 0x3fc90fdb */
#if defined(__DOWNWARD) || defined(__TOWARDZERO)
pio2dwn = 1.5707963e+00f,	/* 0x3fc90fda */
#endif
/* coefficient for R(x^2) */
pS0 =  1.6666587e-01f,
pS1 = -4.2743422e-02f,
pS2 = -8.656363e-03f,
qS1 = -7.0662963e-01f;

float
asinf(float x)
{
	float s,t,w,p,q;
	int32_t hx,ix;

	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;

	if(ix<0x3f000000) {	/* |x| < |0.5| */
	    if(ix<0x39800000) {	/* |x| < 2**-12 */
		if(huge+x>one)
		    return x;	/* return x with inexact if x!=0*/
	    }
	    t = x*x;
	    p = (pS0+t*(pS1+t*pS2));
	    q = one+t*qS1;
	    w = (t*x*p)/q;
	    return x+w;
	}

	if (ix<0x3f800000) {	/* |1|>|x|>=|0.5| */
	    w = one-fabsf(x);
	    t = w*0.5f;
	    p = (pS0+t*(pS1+t*pS2));
	    q = one+t*qS1;
	    s = sqrtf(t);
	    w = s+((s*t*p)/q);
#if defined(DOWNWARD) || defined(TOWARDZERO)
	    t = -2.0f*w+pio2dwn;
#else
	    t = -2.0f*w+pio2acc;
#endif
	    return (hx > 0 ? t : -t);
	}

	if(ix==0x3f800000)	/* |x| == 1 */
	    /* asin(+-1) = +-pi/2 with inexact */
	    return (hx > 0 ? pio2acc : -pio2acc);

	return (x-x)/0.0f;	/* asin(|x|>1) is NaN */
}
