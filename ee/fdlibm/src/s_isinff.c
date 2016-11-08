#include "math_private.h"

#ifdef __STDC__
	int isinff(float x)
#else
	int isinff(x)
	float x;
#endif
{
	return ( ((((unsigned)x) & 0x7F800000) == 0x7F800000)
		&& ((((unsigned)x) & 0x007FFFFF) == 0) );
}

#ifdef __STDC__
	int isnanf(float x)
#else
	int isnanf(x)
	float x;
#endif
{
	return ( ((((unsigned)x) & 0x7F800000) == 0x7F800000)
		&& ((((unsigned)x) & 0x007FFFFF) != 0) );
}

