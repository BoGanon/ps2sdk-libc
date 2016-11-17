#include "math_private.h"

int __isfinite(double x)
{
	int32_t hx;
	GET_HIGH_WORD(hx,x);
	return ((hx & 0x7FF00000) != 0x7FF00000);
}

int __isfinitef(float x)
{
	int32_t ix;
	GET_FLOAT_WORD(ix,x);
	return ((ix & 0x7F800000) != 0x7F800000);
}
