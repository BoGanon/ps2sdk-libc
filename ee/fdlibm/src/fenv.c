#include "fenv.h"

#ifndef FENV_SOFTFLOAT
const fenv_t __default_fenv = (FE_ZERO_DENORMALS | FE_TOWARDZERO);
#else

int __softfloat_float_exception_flags;
int __softfloat_float_exception_mask;
int __softfloat_float_rounding_mode;

void __softfloat_float_raise(int excepts)
{
  excepts &= FE_ALL_EXCEPT;
  __softfloat_float_exception_flags |= excepts;
}
#endif

extern int feclearexcept(int excepts);
extern int fetestexcept(int excepts);
extern int feraiseexcept(int excepts);
extern int fegetexceptflag(fexcept_t *flagp, int excepts);
extern int fesetexceptflag(const fexcept_t *flagp, int excepts);
extern int fegetround(void);
extern int fesetround(int round);
extern int fegetenv(fenv_t *envp);
extern int fesetenv(const fenv_t *envp);
extern int feholdexcept(fenv_t *envp);
extern int feupdateenv(const fenv_t *envp);
