/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VM_AVXFunc.h ( VM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VM_AVXFunc__
#define __VM_AVXFunc__

#include "VM_API.h"
#include <SYS/SYS_Types.h>

#define CPU_HAS_AVX_INSTR	1
#define VM_AVX_STYLE		1

#include <immintrin.h>
typedef __m256	v8sf;
typedef __m256i	v8si;

// Plain casting (no conversion)
// MSVC has problems casting between __m128 and __m128i, so we implement a
// custom casting routine specifically for windows.

#if defined(_MSC_VER)

static SYS_FORCE_INLINE v8sf
vm_v8sf(const v8si &a)
{
    union {
	v8si ival;
	v8sf fval;
    };
    ival = a;
    return fval;
}

static SYS_FORCE_INLINE v8si
vm_v8si(const v8sf &a)
{
    union {
	v8si ival;
	v8sf fval;
    };
    fval = a;
    return ival;
}

#define V8SF(A)		vm_v8sf(A)
#define V8SI(A)		vm_v8si(A)

#else

#define V8SF(A)		(v8sf)A
#define V8SI(A)		(v8si)A

// Intrinsic missing in gcc/clang
#define _mm256_set_m128i(v0, v1)  _mm256_insertf128_si256(_mm256_castsi128_si256(v1), (v0), 1)

#endif

#define VM_SHUFFLE_MASK_AVX(a0,a1, b0,b1)	((b1)<<6|(b0)<<4 | (a1)<<2|(a0))

template <int mask>
static SYS_FORCE_INLINE v8sf
vm_shuffle_avx(const v8sf &a, const v8sf &b)
{
    return _mm256_shuffle_ps(a, b, mask);
}

template <int mask>
static SYS_FORCE_INLINE v8si
vm_shuffle_avx(const v8si &a, const v8si &b)
{
    return v8si(_mm256_shuffle_ps(v8sf(a), v8sf(b), mask));
}

template <int A, int B, int C, int D, typename T>
static SYS_FORCE_INLINE T
vm_shuffle_avx(const T &a, const T &b)
{
    return vm_shuffle_avx<VM_SHUFFLE_MASK_AVX(A,B,C,D)>(a, b);
}

template <int mask, typename T>
static SYS_FORCE_INLINE T
vm_shuffle_avx(const T &a)
{
    return vm_shuffle_avx<mask>(a, a);
}

template <int A, int B, int C, int D, typename T>
static SYS_FORCE_INLINE T
vm_shuffle_avx(const T &a)
{
    return vm_shuffle_avx<A,B,C,D>(a, a);
}

// The _mm256_insert_epi32 intrinsic is missing in VS2015
#if defined(_MSC_VER)
static SYS_FORCE_INLINE v8si
vm_insert_avx(const v8si v, int32 a, int n)
{
    union { v8si vector; int32 comp[8]; };
    vector = v;
    comp[n] = a;
    return vector;
}
#else
static SYS_FORCE_INLINE v8si
vm_insert_avx(const v8si v, int32 a, int n)
{
    switch(n)
    {
    case 0 : return _mm256_insert_epi32(v, a, 0);
    case 1 : return _mm256_insert_epi32(v, a, 1);
    case 2 : return _mm256_insert_epi32(v, a, 2);
    case 3 : return _mm256_insert_epi32(v, a, 3);
    case 4 : return _mm256_insert_epi32(v, a, 4);
    case 5 : return _mm256_insert_epi32(v, a, 5);
    case 6 : return _mm256_insert_epi32(v, a, 6);
    case 7 : return _mm256_insert_epi32(v, a, 7);
    }
    return v;
}
#endif

static SYS_FORCE_INLINE v8sf
vm_insert_avx(const v8sf v, float a, int n)
{
    union { v8sf vector; float comp[8]; };
    vector = v;
    comp[n] = a;
    return vector;
}

// The _mm256_extract_epi32 intrinsic is missing in VS2015
#if defined(_MSC_VER)
static SYS_FORCE_INLINE int
vm_extract_avx(const v8si v, int n)
{
    union { v8si vector; int32 comp[8]; };
    vector = v;
    return comp[n];
}
#else
static SYS_FORCE_INLINE int
vm_extract_avx(const v8si v, int n)
{
    switch(n)
    {
    case 0 : return _mm256_extract_epi32(v, 0);
    case 1 : return _mm256_extract_epi32(v, 1);
    case 2 : return _mm256_extract_epi32(v, 2);
    case 3 : return _mm256_extract_epi32(v, 3);
    case 4 : return _mm256_extract_epi32(v, 4);
    case 5 : return _mm256_extract_epi32(v, 5);
    case 6 : return _mm256_extract_epi32(v, 6);
    case 7 : return _mm256_extract_epi32(v, 7);
    }
    return 0;
}
#endif

static SYS_FORCE_INLINE float
vm_extract_avx(const v8sf v, int n)
{
    union { v8sf vector; float comp[8]; };
    vector = v;
    return comp[n];
}

static SYS_FORCE_INLINE v8sf
vm_splats_avx(float a)
{
    return _mm256_set1_ps(a);
}

static SYS_FORCE_INLINE v8si
vm_splats_avx(uint32 a)
{
    SYS_FPRealUnionF	tmp;
    tmp.uval = a;
    return V8SI(vm_splats_avx(tmp.fval));
}

static SYS_FORCE_INLINE v8si
vm_splats_avx(int32 a)
{
    return _mm256_set1_epi32(a);
}

static SYS_FORCE_INLINE v8sf
vm_splats_avx(float a0, float a1, float a2, float a3,
	      float a4, float a5, float a6, float a7)
{
    return _mm256_set_ps(a7, a6, a5, a4, a3, a2, a1, a0);
}

static SYS_FORCE_INLINE v8si
vm_splats_avx(uint a0, uint a1, uint a2, uint a3,
	      uint a4, uint a5, uint a6, uint a7)
{
    return _mm256_set_epi32((int32)a7, (int32)a6, (int32)a5, (int32)a4,
			    (int32)a3, (int32)a2, (int32)a1, (int32)a0);
}

static SYS_FORCE_INLINE v8si
vm_splats_avx(int32 a0, int32 a1, int32 a2, int32 a3,
	      int32 a4, int32 a5, int32 a6, int32 a7)
{
    return _mm256_set_epi32(a7, a6, a5, a4, a3, a2, a1, a0);
}

static SYS_FORCE_INLINE v8si
vm_load_avx(const int32 v[8])
{
    return _mm256_loadu_si256((v8si *) v);
}

static SYS_FORCE_INLINE v8sf
vm_load_avx(const float v[8])
{
    return _mm256_loadu_ps(v);
}

static SYS_FORCE_INLINE void
vm_store_avx(float dst[8], v8sf value)
{
    _mm256_storeu_ps(dst, value);
}

static SYS_FORCE_INLINE v8sf
vm_negate_avx(v8sf a)
{
    return _mm256_sub_ps(_mm256_setzero_ps(), a);
}

static SYS_FORCE_INLINE v8sf
vm_abs_avx(v8sf a)
{
    return _mm256_max_ps(a, vm_negate_avx(a));
}

static SYS_FORCE_INLINE v8sf
vm_fdiv_avx(v8sf a, v8sf b)
{
    return _mm256_mul_ps(a, _mm256_rcp_ps(b));
}

static SYS_FORCE_INLINE v8sf
vm_fsqrt_avx(v8sf a)
{
    return _mm256_rcp_ps(_mm256_rsqrt_ps(a));
}

static SYS_FORCE_INLINE v8sf
vm_madd_avx(v8sf a, v8sf b, v8sf c)
{
    return _mm256_add_ps(_mm256_mul_ps(a, b), c);
}

// Some integer instructions aren't in AVX so we use SSE
#define SSE_WRAPPER_I(NAME, OP)			 \
static SYS_FORCE_INLINE v8si				 \
NAME(v8si a, v8si b)				 \
{						 \
    __m128i la = _mm256_extractf128_si256(a, 0); \
    __m128i ua = _mm256_extractf128_si256(a, 1); \
    __m128i lb = _mm256_extractf128_si256(b, 0); \
    __m128i ub = _mm256_extractf128_si256(b, 1); \
    return _mm256_set_m128i(OP(ua, ub), 	 \
			    OP(la, lb));	 \
}
SSE_WRAPPER_I(vm_int_cmpeq_avx, _mm_cmpeq_epi32)
SSE_WRAPPER_I(vm_int_cmplt_avx, _mm_cmplt_epi32)
SSE_WRAPPER_I(vm_int_cmpgt_avx, _mm_cmpgt_epi32)
SSE_WRAPPER_I(vm_int_add_avx, 	_mm_add_epi32)
SSE_WRAPPER_I(vm_int_sub_avx, 	_mm_sub_epi32)
SSE_WRAPPER_I(vm_int_and_avx, 	_mm_and_si128)
SSE_WRAPPER_I(vm_int_andnot_avx,_mm_andnot_si128)
SSE_WRAPPER_I(vm_int_or_avx, 	_mm_or_si128)
SSE_WRAPPER_I(vm_int_xor_avx, 	_mm_xor_si128)

static const v8si	theSSETrue_avx= vm_splats_avx(0xFFFFFFFF);

static SYS_FORCE_INLINE bool
vm_allbits_avx(const v8si &a)
{
    return _mm256_movemask_ps(V8SF(vm_int_cmpeq_avx(a, theSSETrue_avx))) == 0xFF;
}


#define VM_EXTRACT_AVX	vm_extract_avx
#define VM_INSERT_AVX	vm_insert_avx
#define VM_SPLATS_AVX	vm_splats_avx
#define VM_LOAD_AVX	vm_load_avx
#define VM_STORE_AVX	vm_store_avx

#define VM_CMPLT_AVX(A,B)	V8SI(_mm256_cmp_ps(A,B,_CMP_LT_OQ))
#define VM_CMPLE_AVX(A,B)	V8SI(_mm256_cmp_ps(A,B,_CMP_LE_OQ))
#define VM_CMPGT_AVX(A,B)	V8SI(_mm256_cmp_ps(A,B,_CMP_GT_OQ))
#define VM_CMPGE_AVX(A,B)	V8SI(_mm256_cmp_ps(A,B,_CMP_GE_OQ))
#define VM_CMPEQ_AVX(A,B)	V8SI(_mm256_cmp_ps(A,B,_CMP_EQ_OQ))
#define VM_CMPNE_AVX(A,B)	V8SI(_mm256_cmp_ps(A,B,_CMP_NEQ_OQ))

#define VM_ICMPLT_AVX	vm_int_cmplt_avx
#define VM_ICMPGT_AVX	vm_int_cmpgt_avx
#define VM_ICMPEQ_AVX	vm_int_cmpeq_avx

#define VM_IADD_AVX	vm_int_add_avx
#define VM_ISUB_AVX	vm_int_sub_avx

#define VM_ADD_AVX	_mm256_add_ps
#define VM_SUB_AVX	_mm256_sub_ps
#define VM_MUL_AVX	_mm256_mul_ps
#define VM_DIV_AVX	_mm256_div_ps
#define VM_SQRT_AVX	_mm256_sqrt_ps
#define VM_ISQRT_AVX	_mm256_rsqrt_ps
#define VM_INVERT_AVX	_mm256_rcp_ps
#define VM_ABS_AVX	vm_abs_avx

#define VM_FDIV_AVX	vm_fdiv_avx
#define VM_NEG_AVX	vm_negate_avx
#define VM_FSQRT_AVX	vm_fsqrt_avx
#define VM_MADD_AVX	vm_madd_avx

#define VM_MIN_AVX	_mm256_min_ps
#define VM_MAX_AVX	_mm256_max_ps

#define VM_AND_AVX	vm_int_and_avx
#define VM_ANDNOT_AVX	vm_int_andnot_avx
#define VM_OR_AVX	vm_int_or_avx
#define VM_XOR_AVX	vm_int_xor_avx

#define VM_ALLBITS_AVX	vm_allbits_avx

#define VM_SHUFFLE_AVX	vm_shuffle_avx

// Integer to float conversions
#define VM_SSE_ROUND_MASK_AVX	0x6000
#define VM_SSE_ROUND_ZERO_AVX	0x6000
#define VM_SSE_ROUND_UP_AVX	0x4000
#define VM_SSE_ROUND_DOWN_AVX	0x2000
#define VM_SSE_ROUND_NEAR_AVX	0x0000

#define GETROUND_AVX()	(_mm_getcsr()&VM_SSE_ROUND_MASK_AVX)
#define SETROUND_AVX(x)	(_mm_setcsr(x|(_mm_getcsr()&~VM_SSE_ROUND_MASK_AVX)))

// The P functions must be invoked before FLOOR, the E functions invoked
// afterwards to reset the state.

#define VM_P_FLOOR_AVX()	uint rounding = GETROUND_AVX(); \
				SETROUND_AVX(VM_SSE_ROUND_DOWN_AVX);
#define VM_FLOOR_AVX		_mm256_cvtps_epi32
#define VM_INT_AVX		_mm256_cvttps_epi32
#define VM_E_FLOOR_AVX()	SETROUND_AVX(rounding);

// Float to integer conversion
#define VM_IFLOAT_AVX		_mm256_cvtepi32_ps

#endif
