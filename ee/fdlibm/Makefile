#
#  @(#)Makefile 1.4 95/01/18
#
#  ====================================================
#  Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
#
#  Developed at SunSoft, a Sun Microsystems, Inc. business.
#  Permission to use, copy, modify, and distribute this
#  software is freely granted, provided that this notice
#  is preserved.
#  ====================================================
#
#

#
# There are two options in making libm at fdlibm compile time:
# 	_IEEE_LIBM 	--- IEEE libm; smaller, and somewhat faster
#	_MULTI_LIBM	--- Support multi-standard at runtime by
#			    imposing wrapper functions defined in
#			    fdlibm.h:
#				_IEEE_MODE 	-- IEEE
#				_XOPEN_MODE 	-- X/OPEN
#				_POSIX_MODE 	-- POSIX/ANSI
#				_SVID3_MODE 	-- SVID
#
# Here is how to set up CFLAGS to create the desired libm at
# compile time:
#
# 	CFLAGS = -D_IEEE_LIBM		... IEEE libm (recommended)
#	CFLAGS = -D_SVID3_MODE	... Multi-standard supported
#					    libm with SVID as the
#					    default standard
#	CFLAGS = -D_XOPEN_MODE	... Multi-standard supported
#					    libm with XOPEN as the
#					    default standard
#	CFLAGS = -D_POSIX_MODE	... Multi-standard supported
#					    libm with POSIX as the
#					    default standard
#	CFLAGS = 			... Multi-standard supported
#					    libm with IEEE as the
#					    default standard
#
# NOTE: if scalb's second arguement is an int, then one must
# define _SCALB_INT in CFLAGS. The default prototype of scalb
# is double scalb(double, double)
#


#
# Default IEEE libm
#

EE_OBJS_DIR = obj/
EE_LIB_DIR = lib/
EE_SRC_DIR = src/
EE_INC_DIR = include/

EE_LIB = $(EE_LIB_DIR)libm.a

EE_CFLAGS = -D_IEEE_LIBM -D__LITTLE_ENDIAN

EE_OBJS = k_standard.o k_rem_pio2.o \
	k_cos.o k_sin.o k_tan.o \
	e_acos.o e_acosh.o e_asin.o e_atan2.o \
	e_atanh.o e_cosh.o e_exp.o e_fmod.o \
	e_gamma.o e_gamma_r.o e_hypot.o e_j0.o \
	e_j1.o e_jn.o e_lgamma.o e_lgamma_r.o \
	e_log.o e_log10.o e_log2.o e_pow.o e_rem_pio2.o e_remainder.o \
	e_scalb.o e_sinh.o e_sqrt.o \
	w_acos.o w_acosh.o w_asin.o w_atan2.o \
	w_atanh.o w_cosh.o w_exp.o w_fmod.o \
	w_gamma.o w_gamma_r.o w_hypot.o w_j0.o \
	w_j1.o w_jn.o w_lgamma.o w_lgamma_r.o \
	w_log.o w_log10.o w_pow.o w_remainder.o \
	w_scalb.o w_sinh.o w_sqrt.o \
	s_asinh.o s_atan.o s_cbrt.o s_ceil.o s_copysign.o \
	s_cos.o s_erf.o s_expm1.o s_fabs.o s_finite.o s_floor.o \
	s_frexp.o s_ilogb.o s_isnan.o s_ldexp.o s_lib_version.o \
	s_log1p.o s_logb.o s_matherr.o s_modf.o s_nextafter.o \
	s_rint.o s_scalbn.o s_signgam.o s_significand.o s_sin.o \
	s_tan.o s_tanh.o \
	e_acosf.o   e_powf.o        s_cosf.o       \
	e_acoshf.o  e_remainderf.o  s_erff.o      s_modff.o         \
	e_asinf.o   e_rem_pio2f.o   s_exp2f.o     s_nextafterf.o    \
	e_atan2f.o  e_scalbf.o      s_expm1f.o    s_remquof.o       \
	e_atanhf.o  e_sinhf.o       s_fabsf.o     s_rintf.o         \
	e_coshf.o   e_sqrtf.o       s_floorf.o      \
	e_expf.o    k_cosf.o        s_fmaxf.o     s_scalbnf.o       \
	e_fmodf.o   k_rem_pio2f.o   s_fminf.o     s_significandf.o  \
	e_hypotf.o  k_sinf.o        s_frexpf.o    s_sinf.o          \
	e_j0f.o     k_tanf.o        s_ilogbf.o    s_tanf.o          \
	e_j1f.o     s_asinhf.o      s_tanhf.o         \
	e_jnf.o     s_atanf.o       s_truncf.o    s_trunc.o    \
	e_log10f.o  s_cbrtf.o       s_log1pf.o    w_dremf.o         \
	e_log2f.o   s_ceilf.o       s_logbf.o     w_gammaf.o        \
	e_logf.o    s_copysignf.o   w_lgammaf.o   s_finitef.o       \
	e_lgammaf_r.o 	b_exp.o	    b_log.o	b_tgamma.o s_roundf.o \
	s_signbit.o

EE_OBJS := $(EE_OBJS:%=$(EE_OBJS_DIR)%)

all: $(EE_OBJS_DIR) $(EE_LIB_DIR) $(EE_LIB)

clean:
	rm -f -r $(EE_OBJS_DIR) $(EE_LIB_DIR)

include $(PS2SDKSRC)/Defs.make
include $(PS2SDKSRC)/ee/Rules.make
include $(PS2SDKSRC)/ee/Rules.release
