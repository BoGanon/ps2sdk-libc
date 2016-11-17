#include "math_private.h"

/* The r5900 doesn't support infinities or nan and instead uses 0x7FFFFFFF and
   0xFFFFFFFF and sets the FE_DIVBYZERO or FE_OVERFLOW flag when +/-inf 
   exceptions occur. */
int __isinf(double x)
{
	int hx,lx;
	EXTRACT_WORDS(hx,lx,x);
	return (((hx & 0x7FFFFFFF) == 0x7FF00000) && !(lx));
}

int __isinff(float x)
{
	int ix;
	GET_FLOAT_WORD(ix,x);
	return ((ix & 0x7FFFFFFF) == 0x7F800000);
}
