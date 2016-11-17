#include "fenv.h"

const fenv_t __default_fenv = (FE_ZERO_DENORMALS | FE_TOWARDZERO);

