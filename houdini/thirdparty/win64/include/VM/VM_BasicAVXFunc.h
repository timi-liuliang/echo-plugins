/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VM_BasicAVXFunc.h ( VM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VM_BasicFunc__
#define __VM_BasicFunc__

#include "VM_API.h"
#include <SYS/SYS_Types.h>

// Uncomment this define to "test" with the basic instructions.  These are
// brain-dead slow, so they shouldn't be used in actual code.
//#define CPU_HAS_AVX_INSTR	1

class v8si;

class v8sf {
public:
    v8sf() {}
    v8sf(float a) { f[0] = f[1] = f[2] = f[3] = f[4] = f[5] = f[6] = f[7] = a; }
    v8sf(float a0, float a1, float a2, float a3,
	 float a4, float a5, float a6, float a7)
    {
	f[0] = a0;
	f[1] = a1;
	f[2] = a2;
	f[3] = a3;
	f[4] = a4;
	f[5] = a5;
	f[6] = a6;
	f[7] = a7;
    }

    operator	v8si() const;
    fpreal32	f[8];
};

class v8si {
public:
    v8si() {}
    v8si(int32 a) { i[0] = i[1] = i[2] = i[3] = i[4] = i[5] = i[6] = i[7] = a; }
    v8si(int32 a0, int32 a1, int32 a2, int32 a3,
	 int32 a4, int32 a5, int32 a6, int32 a7)
    {
	i[0] = a0;
	i[1] = a1;
	i[2] = a2;
	i[3] = a3;
	i[4] = a4;
	i[5] = a5;
	i[6] = a6;
	i[7] = a7;
    }

    operator	v8sf() const;
    int32	i[8];
};

inline
v8sf::operator v8si() const
{
    return *(v8si *)this;
}

inline
v8si::operator v8sf() const
{
    return *(v8sf *)this;
}

#define V8SF(A)		(v8sf)A
#define V8SI(A)		(v8si)A

#define vm_BASIC_IFF_AVX(OP)	\
	    v8si	r; \
	    r.i[0] = a.f[0] OP b.f[0]; \
	    r.i[1] = a.f[1] OP b.f[1]; \
	    r.i[2] = a.f[2] OP b.f[2]; \
	    r.i[3] = a.f[3] OP b.f[3]; \
	    r.i[4] = a.f[4] OP b.f[4]; \
	    r.i[5] = a.f[5] OP b.f[5]; \
	    r.i[6] = a.f[6] OP b.f[6]; \
	    r.i[7] = a.f[7] OP b.f[7]; \
	    return r;

#define vm_BASIC_CF_AVX(OP)	\
	    v8si	r; \
	    r.i[0] = a.f[0] OP b.f[0] ? 0xFFFFFFFF : 0; \
	    r.i[1] = a.f[1] OP b.f[1] ? 0xFFFFFFFF : 0; \
	    r.i[2] = a.f[2] OP b.f[2] ? 0xFFFFFFFF : 0; \
	    r.i[3] = a.f[3] OP b.f[3] ? 0xFFFFFFFF : 0; \
	    r.i[4] = a.f[4] OP b.f[4] ? 0xFFFFFFFF : 0; \
	    r.i[5] = a.f[5] OP b.f[5] ? 0xFFFFFFFF : 0; \
	    r.i[6] = a.f[6] OP b.f[6] ? 0xFFFFFFFF : 0; \
	    r.i[7] = a.f[7] OP b.f[7] ? 0xFFFFFFFF : 0; \
	    return r;

#define vm_BASIC_CI_AVX(OP)	\
	    v8si	r; \
	    r.i[0] = a.i[0] OP b.i[0] ? 0xFFFFFFFF : 0; \
	    r.i[1] = a.i[1] OP b.i[1] ? 0xFFFFFFFF : 0; \
	    r.i[2] = a.i[2] OP b.i[2] ? 0xFFFFFFFF : 0; \
	    r.i[3] = a.i[3] OP b.i[3] ? 0xFFFFFFFF : 0; \
	    r.i[4] = a.i[4] OP b.i[4] ? 0xFFFFFFFF : 0; \
	    r.i[5] = a.i[5] OP b.i[5] ? 0xFFFFFFFF : 0; \
	    r.i[6] = a.i[6] OP b.i[6] ? 0xFFFFFFFF : 0; \
	    r.i[7] = a.i[7] OP b.i[7] ? 0xFFFFFFFF : 0; \
	    return r;

#define vm_BASIC_III_AVX(OP)	\
	    v8si	r; \
	    r.i[0] = a.i[0] OP b.i[0]; \
	    r.i[1] = a.i[1] OP b.i[1]; \
	    r.i[2] = a.i[2] OP b.i[2]; \
	    r.i[3] = a.i[3] OP b.i[3]; \
	    r.i[4] = a.i[4] OP b.i[4]; \
	    r.i[5] = a.i[5] OP b.i[5]; \
	    r.i[6] = a.i[6] OP b.i[6]; \
	    r.i[7] = a.i[7] OP b.i[7]; \
	    return r;

#define vm_BASIC_FFF_AVX(OP)	\
	    v8sf	r; \
	    r.f[0] = a.f[0] OP b.f[0]; \
	    r.f[1] = a.f[1] OP b.f[1]; \
	    r.f[2] = a.f[2] OP b.f[2]; \
	    r.f[3] = a.f[3] OP b.f[3]; \
	    r.f[4] = a.f[4] OP b.f[4]; \
	    r.f[5] = a.f[5] OP b.f[5]; \
	    r.f[6] = a.f[6] OP b.f[6]; \
	    r.f[7] = a.f[7] OP b.f[7]; \
	    return r;

#define vm_BASIC_FFFF_AVX(OP1, OP2)	\
	    v8sf	r; \
	    r.f[0] = a.f[0] OP1 b.f[0] OP2 c.f[0]; \
	    r.f[1] = a.f[1] OP1 b.f[1] OP2 c.f[1]; \
	    r.f[2] = a.f[2] OP1 b.f[2] OP2 c.f[2]; \
	    r.f[3] = a.f[3] OP1 b.f[3] OP2 c.f[3]; \
	    r.f[4] = a.f[4] OP1 b.f[4] OP2 c.f[4]; \
	    r.f[5] = a.f[5] OP1 b.f[5] OP2 c.f[5]; \
	    r.f[6] = a.f[6] OP1 b.f[6] OP2 c.f[6]; \
	    r.f[7] = a.f[7] OP1 b.f[7] OP2 c.f[7]; \
	    return r;

#define vm_BASIC_UFuncF_AVX(FUNC)	\
	    v8sf	r; \
	    r.f[0] = FUNC(a.f[0]); \
	    r.f[1] = FUNC(a.f[1]); \
	    r.f[2] = FUNC(a.f[2]); \
	    r.f[3] = FUNC(a.f[3]); \
	    r.f[4] = FUNC(a.f[4]); \
	    r.f[5] = FUNC(a.f[5]); \
	    r.f[6] = FUNC(a.f[6]); \
	    r.f[7] = FUNC(a.f[7]); \
	    return r;

#define vm_BASIC_UFuncFF_AVX(FUNC)	\
	    v8sf	r; \
	    r.f[0] = FUNC(a.f[0], b.f[0]); \
	    r.f[1] = FUNC(a.f[1], b.f[1]); \
	    r.f[2] = FUNC(a.f[2], b.f[2]); \
	    r.f[3] = FUNC(a.f[3], b.f[3]); \
	    r.f[4] = FUNC(a.f[4], b.f[4]); \
	    r.f[5] = FUNC(a.f[5], b.f[5]); \
	    r.f[6] = FUNC(a.f[6], b.f[6]); \
	    r.f[7] = FUNC(a.f[7], b.f[7]); \
	    return r;

static inline v8si vm_clt_avx(const v8sf &a, const v8sf &b){ vm_BASIC_CF_AVX(<) }
static inline v8si vm_cle_avx(const v8sf &a, const v8sf &b){ vm_BASIC_CF_AVX(<=) }
static inline v8si vm_cgt_avx(const v8sf &a, const v8sf &b){ vm_BASIC_CF_AVX(>) }
static inline v8si vm_cge_avx(const v8sf &a, const v8sf &b){ vm_BASIC_CF_AVX(>=) }
static inline v8si vm_ceq_avx(const v8sf &a, const v8sf &b){ vm_BASIC_CF_AVX(==) }
static inline v8si vm_cne_avx(const v8sf &a, const v8sf &b){ vm_BASIC_CF_AVX(!=) }

static inline v8si vm_clt_avx(const v8si &a, const v8si &b){ vm_BASIC_CI_AVX(<) }
static inline v8si vm_cle_avx(const v8si &a, const v8si &b){ vm_BASIC_CI_AVX(<=) }
static inline v8si vm_cgt_avx(const v8si &a, const v8si &b){ vm_BASIC_CI_AVX(>) }
static inline v8si vm_cge_avx(const v8si &a, const v8si &b){ vm_BASIC_CI_AVX(>=) }
static inline v8si vm_ceq_avx(const v8si &a, const v8si &b){ vm_BASIC_CI_AVX(==) }
static inline v8si vm_cne_avx(const v8si &a, const v8si &b){ vm_BASIC_CI_AVX(!=) }

static inline v8si vm_add_avx(const v8si &a, const v8si &b){ vm_BASIC_III_AVX(+) }
static inline v8si vm_sub_avx(const v8si &a, const v8si &b){ vm_BASIC_III_AVX(-) }

static inline v8sf vm_add_avx(const v8sf &a, const v8sf &b){ vm_BASIC_FFF_AVX(+) }
static inline v8sf vm_sub_avx(const v8sf &a, const v8sf &b){ vm_BASIC_FFF_AVX(-) }
static inline v8sf vm_mul_avx(const v8sf &a, const v8sf &b){ vm_BASIC_FFF_AVX(*) }
static inline v8sf vm_div_avx(const v8sf &a, const v8sf &b){ vm_BASIC_FFF_AVX(/) }

static inline v8si vm_and_avx(const v8si &a, const v8si &b){ vm_BASIC_III_AVX(&) }
static inline v8si vm_or_avx(const v8si &a, const v8si &b){ vm_BASIC_III_AVX(|) }
static inline v8si vm_xor_avx(const v8si &a, const v8si &b){ vm_BASIC_III_AVX(^) }

static inline v8si vm_andnot_avx(const v8si &a, const v8si &b)
{
    v8si	r;
    r.i[0] = ~a.i[0] & b.i[0];
    r.i[1] = ~a.i[1] & b.i[1];
    r.i[2] = ~a.i[2] & b.i[2];
    r.i[3] = ~a.i[3] & b.i[3];
    r.i[4] = ~a.i[4] & b.i[4];
    r.i[5] = ~a.i[5] & b.i[5];
    r.i[6] = ~a.i[6] & b.i[6];
    r.i[7] = ~a.i[7] & b.i[7];
    return r;
}

static inline v8sf
vm_min_avx(const v8sf &a, const v8sf &b)	{ vm_BASIC_UFuncFF_AVX(SYSmin) }
static inline v8sf
vm_max_avx(const v8sf &a, const v8sf &b)	{ vm_BASIC_UFuncFF_AVX(SYSmax) }

static inline v8sf
vm_madd_avx(const v8sf &a, const v8sf &b, const v8sf &c) { vm_BASIC_FFFF_AVX(*, +) }

static inline v8sf vm_sqrt_avx(const v8sf &a)	{ vm_BASIC_UFuncF_AVX(SYSsqrt) }
static inline v8sf vm_isqrt_avx(const v8sf &a)	{ vm_BASIC_UFuncF_AVX(1/SYSsqrt) }
static inline v8sf vm_negate_avx(const v8sf &a)	{ vm_BASIC_UFuncF_AVX(-) }
static inline v8sf vm_reciprocal_avx(const v8sf &a)	{ vm_BASIC_UFuncF_AVX(1/) }
static inline v8sf vm_abs_avx(const v8sf &a)	{ vm_BASIC_UFuncF_AVX(SYSabs) }

static inline v8si vm_floor_avx(const v8sf &a)
{
    v8si	r;
    r.i[0] = (int)SYSfastFloor(a.f[0]);
    r.i[1] = (int)SYSfastFloor(a.f[1]);
    r.i[2] = (int)SYSfastFloor(a.f[2]);
    r.i[3] = (int)SYSfastFloor(a.f[3]);
    r.i[4] = (int)SYSfastFloor(a.f[4]);
    r.i[5] = (int)SYSfastFloor(a.f[5]);
    r.i[6] = (int)SYSfastFloor(a.f[6]);
    r.i[7] = (int)SYSfastFloor(a.f[7]);
    return r;
}

static inline v8si vm_intcast_avx(const v8sf &a)
{
    v8si	r;
    r.i[0] = (int)a.f[0];
    r.i[1] = (int)a.f[1];
    r.i[2] = (int)a.f[2];
    r.i[3] = (int)a.f[3];
    r.i[4] = (int)a.f[4];
    r.i[5] = (int)a.f[5];
    r.i[6] = (int)a.f[6];
    r.i[7] = (int)a.f[7];
    return r;
}

static inline v8sf vm_floatcast_avx(const v8si &a)
{
    v8sf	r;
    r.f[0] = (float)a.i[0];
    r.f[1] = (float)a.i[1];
    r.f[2] = (float)a.i[2];
    r.f[3] = (float)a.i[3];
    r.f[4] = (float)a.i[4];
    r.f[5] = (float)a.i[5];
    r.f[6] = (float)a.i[6];
    r.f[7] = (float)a.i[7];
    return r;
}

static inline v8si
vm_splats_avx(uint32 a)
{
    return v8si(a);
}

static inline v8si
vm_splats_avx(int32 a)
{
    return v8si(a);
}

static inline v8sf
vm_splats_avx(float a)
{
    return v8sf(a);
}

static inline v8si
vm_splats_avx(uint32 a, uint32 b, uint32 c, uint32 d,
	      uint32 e, uint32 f, uint32 g, uint32 h)
{
    return v8si(a, b, c, d, e, f, g, h);
}

static inline v8si
vm_splats_avx(int32 a, int32 b, int32 c, int32 d,
	      int32 e, int32 f, int32 g, int32 h)
{
    return v8si(a, b, c, d, e, f, g, h);
}

static inline v8sf
vm_splats_avx(float a, float b, float c, float d,
	      float e, float f, float g, float h)
{
    return v8sf(a, b, c, d, e, f, g, h);
}

static inline bool
vm_allbits_avx(const v8si &a)
{
    return (a.i[0] & a.i[1] & a.i[2] & a.i[3] &
	    a.i[4] & a.i[5] & a.i[6] & a.i[7]) == 0xFFFFFFFF;
}

template <int A, int B, int C, int D>
static inline v8sf
vm_shuffle_avx(const v8sf &v)
{
    v8sf vec;

    vec.f[0] = v.f[A];
    vec.f[1] = v.f[B];
    vec.f[2] = v.f[C];
    vec.f[3] = v.f[D];
    vec.f[4] = v.f[A];
    vec.f[5] = v.f[B];
    vec.f[6] = v.f[C];
    vec.f[7] = v.f[D];

    return vec;
}

static inline v8si
vm_load_avx(const int32 v[8])
{
    return v8si(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]);
}

static inline v8sf
vm_load_avx(const float v[8])
{
    return v8sf(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]);
}

static inline void
vm_store_avx(float dst[8], v8sf value)
{
    dst[0] = value.f[0];
    dst[1] = value.f[1];
    dst[2] = value.f[2];
    dst[3] = value.f[3];
    dst[4] = value.f[4];
    dst[5] = value.f[5];
    dst[6] = value.f[6];
    dst[7] = value.f[7];
}


static inline v8si
vm_insert_avx(const v8si v, int32 a, int n)
{
    v8si vec;

    vec = v;
    vec.i[n] = a;
    return vec;
}

static inline v8sf
vm_insert_avx(const v8sf v, float a, int n)
{
    v8sf vec;

    vec = v;
    vec.f[n] = a;
    return vec;
}

static inline int
vm_extract_avx(const v8si v, int n)
{
    return v.i[n];
}

static inline float
vm_extract_avx(const v8sf v, int n)
{
    return v.f[n];
}

#define VM_EXTRACT_AVX	vm_extract_avx
#define VM_INSERT_AVX	vm_insert_avx
#define VM_SPLATS_AVX	vm_splats_avx
#define VM_LOAD_AVX	vm_load_avx
#define VM_STORE_AVX	vm_store_avx

#define VM_CMPLT_AVX	vm_clt_avx
#define VM_CMPLE_AVX	vm_cle_avx
#define VM_CMPGT_AVX	vm_cgt_avx
#define VM_CMPGE_AVX	vm_cge_avx
#define VM_CMPEQ_AVX	vm_ceq_avx
#define VM_CMPNE_AVX	vm_cne_avx

#define VM_ICMPLT_AVX	vm_clt_avx
#define VM_ICMPGT_AVX	vm_cgt_avx
#define VM_ICMPEQ_AVX	vm_ceq_avx

#define VM_IADD_AVX	vm_add_avx
#define VM_ISUB_AVX	vm_sub_avx

#define VM_ADD_AVX	vm_add_avx
#define VM_SUB_AVX	vm_sub_avx
#define VM_MUL_AVX	vm_mul_avx
#define VM_DIV_AVX	vm_div_avx
#define VM_FDIV_AVX	vm_div_avx
#define VM_NEG_AVX	vm_negate_avx
#define VM_SQRT_AVX	vm_sqrt_avx
#define VM_FSQRT_AVX	vm_sqrt_avx
#define VM_ISQRT_AVX	vm_isqrt_avx
#define VM_ABS_AVX	vm_abs_avx

#define VM_MADD_AVX	vm_madd_avx
#define VM_INVERT_AVX	vm_reciprocal_avx

#define VM_MIN_AVX	vm_min_avx
#define VM_MAX_AVX	vm_max_avx

#define VM_AND_AVX	vm_and_avx
#define VM_ANDNOT_AVX	vm_andnot_avx
#define VM_OR_AVX	vm_or_avx
#define VM_XOR_AVX	vm_xor_avx

#define VM_ALLBITS_AVX	vm_allbits_avx

#define VM_SHUFFLE_AVX	vm_shuffle_avx

#define VM_P_FLOOR_AVX()
#define VM_FLOOR_AVX	vm_floor_avx
#define VM_E_FLOOR_AVX()

#define VM_INT_AVX	vm_intcast_avx

#define VM_IFLOAT_AVX	vm_floatcast_avx

#endif
