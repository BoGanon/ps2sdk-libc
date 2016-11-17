#include "math_private.h"

int __signbit(double d)
{
	int32_t hx;
	GET_HIGH_WORD(hx,d);
	return ((hx & 0x80000000) != 0);
}

int __signbitf(float f)
{
	int32_t ix;
	GET_FLOAT_WORD(ix,f);
	return ((ix & 0x80000000) != 0);
}
