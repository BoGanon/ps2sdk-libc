#include "math.h"
#include "math_private.h"

double hypot(double x, double y)
{
	double r;
	int32_t hx,hy;
	u_int32_t lx,ly;
	EXTRACT_WORDS(hx,lx,x);
	EXTRACT_WORDS(hy,ly,y);
	hx &= 0x7fffffff;
	hy &= 0x7fffffff;
	SET_HIGH_WORD(x,hx);
	SET_HIGH_WORD(y,hy);
	if (hx > hy) {
	    if(hx < 0x7ff00000) {
		r = y/x;
		return x*sqrt(1.0 + r*r);
	    }
	}
	else {
	    if(hy < 0x7ff00000) {
		r = x/y;
		return y*sqrt(1.0 + r*r);
	    }
	}

	/* Inf */
	if((hx-0x7ff00000+lx)==0) return x;
	if((hy-0x7ff00000+ly)==0) return y;

	/* NaN */
	return (x-y);
}
