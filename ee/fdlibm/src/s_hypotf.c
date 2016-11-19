#include "math.h"
#include "math_private.h"

float
hypotf(float x, float y)
{
	float r;
	int32_t ix,iy;
	GET_FLOAT_WORD(ix,x);
	GET_FLOAT_WORD(iy,y);
	ix &= 0x7fffffff;
	iy &= 0x7fffffff;
	SET_FLOAT_WORD(x,ix);
	SET_FLOAT_WORD(y,iy);
	if (ix > iy) {
	    if(ix < 0x7f800000) {
		r = y/x;
		return x*sqrtf(1.0f + r*r);
	    }
	}
	else {
	    if(iy < 0x7f800000) {
		r = x/y;
		return y*sqrtf(1.0f + r*r);
	    }
	}

	/* Inf */
	if((ix-0x7f800000)==0) return x;
	if((iy-0x7f800000)==0) return y;

	/* NaN */
	return (x-y);
}
