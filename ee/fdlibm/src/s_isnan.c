#include "math_private.h"

int isnan(double x)
{
	int hx,lx;
	EXTRACT_WORDS(hx, lx, x);
	return (((hx & 0x7FFFFFFF) == 0x7FF00000) && (lx != 0));
}

