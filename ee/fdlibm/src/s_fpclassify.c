#include "math_private.h"
#include "math.h"

int __fpclassify(double x)
{
	int32_t hx,lx;
	EXTRACT_WORDS(hx,lx,x);
	if (isnan(x))
		return FP_NAN;

	else if (isinf(x))
		return FP_INFINITE;

	else if (isnormal(x))
		return FP_NORMAL;

	else if (!x || ((hx & 0x80000000) && !((hx & 0x7fffffff)|lx)))
		return FP_ZERO;

	return FP_SUBNORMAL;
}

int __fpclassifyf(float x)
{
	int32_t ix;
	GET_FLOAT_WORD(ix,x);

	if (isnan(x))
		return FP_NAN;

	else if (isinf(x))
		return FP_INFINITE;

	else if (isnormal(x))
		return FP_NORMAL;

	else if (!x || ((ix & 0x80000000) && !(ix & 0x7fffffff)))
		return FP_ZERO;

	return FP_SUBNORMAL;
}

