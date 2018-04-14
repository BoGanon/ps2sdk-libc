#include "math_private.h"

/* The r5900 doesn't support infinities or nan and instead uses 0x7FFFFFFF and
   0xFFFFFFFF and sets the FE_INVALID flag when NAN exceptions occur. The only
   case that doesn't apply is when "sqrt.s" is used on a negative number. */

int __isnan(double x)
{
	int32_t hx,lx;
	EXTRACT_WORDS(hx, lx, x);
	return (((hx & 0x7FF00000) == 0x7FF00000) && ((lx|(hx & 0x000FFFFF))));
}

int __isnanf(float x)
{
	int ix;
	GET_FLOAT_WORD(ix,x);
	return ((ix & 0x7FFFFFFF) > 0x7F800000);
}
