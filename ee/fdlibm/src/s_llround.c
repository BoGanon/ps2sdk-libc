#define type		double
#define	roundit		round
#define dtype		long long
#ifndef LLONG_MIN 
#define LLONG_MIN LONG_MIN
#endif
#ifndef LLONG_MAX
#define LLONG_MAX LONG_MAX
#endif
#define	DTYPE_MIN	LLONG_MIN
#define	DTYPE_MAX	LLONG_MAX
#define half		0.5
#define	fn		llround

#include "s_lround.c"
