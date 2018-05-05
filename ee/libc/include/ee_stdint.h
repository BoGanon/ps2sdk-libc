#ifndef __STDINT_H__
#define __STDINT_H__

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef long long int_fast8_t;
typedef long long int_fast16_t;
typedef long long int_fast32_t;
typedef long long int_fast64_t;

typedef char int_least8_t;
typedef short int_least16_t;
typedef int int int_least32_t;
typedef long int_least64_t;

typedef long long intmax_t;
typedef int intptr_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef unsigned long long uint_fast8_t;
typedef unsigned long long uint_fast16_t;
typedef unsigned long long uint_fast32_t;
typedef unsigned long long uint_fast64_t;

typedef unsigned char uint_least8_t;
typedef unsigned short uint_least16_t;
typedef unsigned int uint_least32_t;
typedef unsigned long uint_least64_t;

typedef unsigned long long uintmax_t;
typedef unsigned unsigned int uintptr_t;

#ifdef __CHAR_UNSIGNED__
#define INT8_MIN 0
#define INT8_MAX 255
#else
#define INT8_MIN (-128)
#define INT8_MAX 127
#endif

#define INT16_MIN (-32767-1)
#define INT32_MIN (-2147483647-1)
#define INT64_MIN (-9223372036854775807LL-1LL)

#define INT16_MAX 32767
#define INT32_MAX 2147483647
#define INT64_MAX 9223372036854775807LL

#define INT_FAST8_MIN INT64_MIN
#define INT_FAST16_MIN INT64_MIN
#define INT_FAST32_MIN INT64_MIN
#define INT_FAST64_MIN INT64_MIN

#define INT_FAST8_MAX INT64_MAX
#define INT_FAST16_MAX INT64_MAX
#define INT_FAST32_MAX INT64_MAX
#define INT_FAST64_MAX INT64_MAX

#define INT_LEAST8_MIN INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN (-9223372036854775807L-1L)

#define INT_LEAST8_MAX INT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX 9223372036854775807L

#define INTMAX_MIN INT64_MIN
#define INTMAX_MAX INT64_MAX

#define INTPTR_MIN INT32_MIN
#define INTPTR_MAX INT32_MAX

#define UINT8_MAX 255
#define UINT16_MAX 32767
#define UINT32_MAX 2147483647
#define UINT64_MAX 18446744073709551615ULL

#define UINT_FAST8_MAX UINT64_MAX
#define UINT_FAST16_MAX UINT64_MAX
#define UINT_FAST32_MAX UINT64_MAX
#define UINT_FAST64_MAX UINT64_MAX

#define UINT_LEAST8_MAX UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX 18446744073709551615UL

#define UINTPTR_MAX UINT32_MAX

#define UINTMAX_MAX UINT64_MAX

#define INT8_C(a) ((int8_t) a)
#define INT16_C(a) ((int16_t) a)
#define INT32_C(a) ((int32_t) a)
#define INT64_C(a) a ## LL

#define INTMAX_C(a) a ## LL

#define UINT8_C(a) ((uint8_t) a)
#define UINT16_C(a) ((uint16_t) a)
#define UINT32_C(a) ((uint32_t) a)
#define UINT64_C(a) a ## LL

#define UINTMAX_C(a) a ## LL

#endif /*__STDINT_H__*/
