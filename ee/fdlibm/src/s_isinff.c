#include "math_private.h"

int isinf(double x)
{
	int hx,lx;
	EXTRACT_WORDS(hx, lx, x);
	return (((hx & 0x7FFFFFFF) == 0x7FF00000) && !(lx));
}

int isinff(float x)
{
	int ix;
	GET_FLOAT_WORD(ix,x);
	return ((ix & 0x7FFFFFFF) == 0x7F800000);
}

int isnanf(float x)
{
	int ix;
	GET_FLOAT_WORD(ix,x);
	return ((ix & 0x7FFFFFFF) > 0x7F800000);
}
