#include "math_private.h"

/* The r5900 doesn't support infinities or nan and instead uses 0x7FFFFFFF and
   0xFFFFFFFF and sets the FE_DIVBYZERO or FE_OVERFLOW flag when +/-inf
   exceptions occur. */

int __isinf(double x)
{
	u_int32_t hx,lx;
	EXTRACT_WORDS(hx,lx,x);
	hx &= 0x7fffffff;
	return (((hx - 0x7ff00000) + lx) == 0);
}

int __isinff(float x)
{
	int32_t ix;
	GET_FLOAT_WORD(ix,x);
	return ((ix & 0x7FFFFFFF) == 0x7F800000);
}
