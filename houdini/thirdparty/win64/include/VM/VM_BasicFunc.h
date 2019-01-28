/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VM_BasicFunc.h ( VM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VM_BasicFunc__
#define __VM_BasicFunc__

#include "VM_API.h"
#include <SYS/SYS_Types.h>

// Uncomment this define to "test" with the basic instructions.  These are
// brain-dead slow, so they shouldn't be used in actual code.
//#define CPU_HAS_SIMD_INSTR	1

class v4si;

class v4sf {
public:
    v4sf() {}
    v4sf(float a) { f[0] = f[1] = f[2] = f[3] = a; }
    v4sf(float a, float b, float c, float d)
    {
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
    }

    operator	v4si() const;
    fpreal32	f[4];
};

class v4si {
public:
    v4si() {}
    v4si(int32 a) { i[0] = i[1] = i[2] = i[3] = a; }
    v4si(int32 a, int32 b, int32 c, int32 d)
    {
	i[0] = a;
	i[1] = b;
	i[2] = c;
	i[3] = d;
    }

    operator	v4sf() const;
    int32	i[4];
};

inline
v4sf::operator v4si() const
{
    return *(v4si *)this;
}

inline
v4si::operator v4sf() const
{
    return *(v4sf *)this;
}

#define V4SF(A)		(v4sf)A
#define V4SI(A)		(v4si)A

#define vm_BASIC_IFF(OP)	\
	    v4si	r; \
	    r.i[0] = a.f[0] OP b.f[0]; \
	    r.i[1] = a.f[1] OP b.f[1]; \
	    r.i[2] = a.f[2] OP b.f[2]; \
	    r.i[3] = a.f[3] OP b.f[3]; \
	    return r;

#define vm_BASIC_CF(OP)	\
	    v4si	r; \
	    r.i[0] = a.f[0] OP b.f[0] ? 0xFFFFFFFF : 0; \
	    r.i[1] = a.f[1] OP b.f[1] ? 0xFFFFFFFF : 0; \
	    r.i[2] = a.f[2] OP b.f[2] ? 0xFFFFFFFF : 0; \
	    r.i[3] = a.f[3] OP b.f[3] ? 0xFFFFFFFF : 0; \
	    return r;

#define vm_BASIC_CI(OP)	\
	    v4si	r; \
	    r.i[0] = a.i[0] OP b.i[0] ? 0xFFFFFFFF : 0; \
	    r.i[1] = a.i[1] OP b.i[1] ? 0xFFFFFFFF : 0; \
	    r.i[2] = a.i[2] OP b.i[2] ? 0xFFFFFFFF : 0; \
	    r.i[3] = a.i[3] OP b.i[3] ? 0xFFFFFFFF : 0; \
	    return r;

#define vm_BASIC_III(OP)	\
	    v4si	r; \
	    r.i[0] = a.i[0] OP b.i[0]; \
	    r.i[1] = a.i[1] OP b.i[1]; \
	    r.i[2] = a.i[2] OP b.i[2]; \
	    r.i[3] = a.i[3] OP b.i[3]; \
	    return r;

#define vm_BASIC_FFF(OP)	\
	    v4sf	r; \
	    r.f[0] = a.f[0] OP b.f[0]; \
	    r.f[1] = a.f[1] OP b.f[1]; \
	    r.f[2] = a.f[2] OP b.f[2]; \
	    r.f[3] = a.f[3] OP b.f[3]; \
	    return r;

#define vm_BASIC_FFFF(OP1, OP2)	\
	    v4sf	r; \
	    r.f[0] = a.f[0] OP1 b.f[0] OP2 c.f[0]; \
	    r.f[1] = a.f[1] OP1 b.f[1] OP2 c.f[1]; \
	    r.f[2] = a.f[2] OP1 b.f[2] OP2 c.f[2]; \
	    r.f[3] = a.f[3] OP1 b.f[3] OP2 c.f[3]; \
	    return r;

#define vm_BASIC_UFuncF(FUNC)	\
	    v4sf	r; \
	    r.f[0] = FUNC(a.f[0]); \
	    r.f[1] = FUNC(a.f[1]); \
	    r.f[2] = FUNC(a.f[2]); \
	    r.f[3] = FUNC(a.f[3]); \
	    return r;

#define vm_BASIC_UFuncFF(FUNC)	\
	    v4sf	r; \
	    r.f[0] = FUNC(a.f[0], b.f[0]); \
	    r.f[1] = FUNC(a.f[1], b.f[1]); \
	    r.f[2] = FUNC(a.f[2], b.f[2]); \
	    r.f[3] = FUNC(a.f[3], b.f[3]); \
	    return r;

static inline v4si vm_clt(const v4sf &a, const v4sf &b)	{ vm_BASIC_CF(<) }
static inline v4si vm_cle(const v4sf &a, const v4sf &b)	{ vm_BASIC_CF(<=) }
static inline v4si vm_cgt(const v4sf &a, const v4sf &b)	{ vm_BASIC_CF(>) }
static inline v4si vm_cge(const v4sf &a, const v4sf &b)	{ vm_BASIC_CF(>=) }
static inline v4si vm_ceq(const v4sf &a, const v4sf &b)	{ vm_BASIC_CF(==) }
static inline v4si vm_cne(const v4sf &a, const v4sf &b)	{ vm_BASIC_CF(!=) }

static inline v4si vm_clt(const v4si &a, const v4si &b)	{ vm_BASIC_CI(<) }
static inline v4si vm_cle(const v4si &a, const v4si &b)	{ vm_BASIC_CI(<=) }
static inline v4si vm_cgt(const v4si &a, const v4si &b)	{ vm_BASIC_CI(>) }
static inline v4si vm_cge(const v4si &a, const v4si &b)	{ vm_BASIC_CI(>=) }
static inline v4si vm_ceq(const v4si &a, const v4si &b)	{ vm_BASIC_CI(==) }
static inline v4si vm_cne(const v4si &a, const v4si &b)	{ vm_BASIC_CI(!=) }

static inline v4si vm_add(const v4si &a, const v4si &b)	{ vm_BASIC_III(+) }
static inline v4si vm_sub(const v4si &a, const v4si &b)	{ vm_BASIC_III(-) }
static inline v4si vm_mul(const v4si &a, const v4si &b)	{ vm_BASIC_III(*) }

static inline v4sf vm_add(const v4sf &a, const v4sf &b)	{ vm_BASIC_FFF(+) }
static inline v4sf vm_sub(const v4sf &a, const v4sf &b)	{ vm_BASIC_FFF(-) }
static inline v4sf vm_mul(const v4sf &a, const v4sf &b)	{ vm_BASIC_FFF(*) }
static inline v4sf vm_div(const v4sf &a, const v4sf &b)	{ vm_BASIC_FFF(/) }

static inline v4si vm_and(const v4si &a, const v4si &b)	{ vm_BASIC_III(&) }
static inline v4si vm_or(const v4si &a, const v4si &b)	{ vm_BASIC_III(|) }
static inline v4si vm_xor(const v4si &a, const v4si &b)	{ vm_BASIC_III(^) }

static inline v4si vm_andnot(const v4si &a, const v4si &b)
{
    v4si	r;
    r.i[0] = ~a.i[0] & b.i[0];
    r.i[1] = ~a.i[1] & b.i[1];
    r.i[2] = ~a.i[2] & b.i[2];
    r.i[3] = ~a.i[3] & b.i[3];
    return r;
}

static inline v4sf
vm_min(const v4sf &a, const v4sf &b)	{ vm_BASIC_UFuncFF(SYSmin) }
static inline v4sf
vm_max(const v4sf &a, const v4sf &b)	{ vm_BASIC_UFuncFF(SYSmax) }

static inline v4sf
vm_madd(const v4sf &a, const v4sf &b, const v4sf &c) { vm_BASIC_FFFF(*, +) }

static inline v4sf vm_sqrt(const v4sf &a)	{ vm_BASIC_UFuncF(SYSsqrt) }
static inline v4sf vm_isqrt(const v4sf &a)	{ vm_BASIC_UFuncF(1/SYSsqrt) }
static inline v4sf vm_negate(const v4sf &a)	{ vm_BASIC_UFuncF(-) }
static inline v4sf vm_reciprocal(const v4sf &a)	{ vm_BASIC_UFuncF(1/) }
static inline v4sf vm_abs(const v4sf &a)	{ vm_BASIC_UFuncF(SYSabs) }

static inline v4si vm_floor(const v4sf &a)
{
    v4si	r;
    r.i[0] = (int)SYSfastFloor(a.f[0]);
    r.i[1] = (int)SYSfastFloor(a.f[1]);
    r.i[2] = (int)SYSfastFloor(a.f[2]);
    r.i[3] = (int)SYSfastFloor(a.f[3]);
    return r;
}

static inline v4si vm_intcast(const v4sf &a)
{
    v4si	r;
    r.i[0] = (int)a.f[0];
    r.i[1] = (int)a.f[1];
    r.i[2] = (int)a.f[2];
    r.i[3] = (int)a.f[3];
    return r;
}

static inline v4sf vm_floatcast(const v4si &a)
{
    v4sf	r;
    r.f[0] = (float)a.i[0];
    r.f[1] = (float)a.i[1];
    r.f[2] = (float)a.i[2];
    r.f[3] = (float)a.i[3];
    return r;
}

static inline v4si
vm_splats(uint32 a)
{
    return v4si(a);
}

static inline v4si
vm_splats(int32 a)
{
    return v4si(a);
}

static inline v4sf
vm_splats(float a)
{
    return v4sf(a);
}

static inline v4si
vm_splats(uint32 a, uint32 b, uint32 c, uint32 d)
{
    return v4si(a, b, c, d);
}

static inline v4si
vm_splats(int32 a, int32 b, int32 c, int32 d)
{
    return v4si(a, b, c, d);
}

static inline v4sf
vm_splats(float a, float b, float c, float d)
{
    return v4sf(a, b, c, d);
}

static inline bool
vm_allbits(const v4si &a)
{
    return (a.i[0] & a.i[1] & a.i[2] & a.i[3]) == 0xFFFFFFFF;
}

static inline bool
vm_signbits(const v4si &a)
{
    return (a.i[0] < 0 ? 1 : 0) |
	   (a.i[1] < 0 ? 2 : 0) |
	   (a.i[3] < 0 ? 3 : 0) |
	   (a.i[4] < 0 ? 4 : 0);
}

static inline bool
vm_signbits(const v4sf &a)
{
    return vm_signbits( * ((v4si *) &a) );
}

template <int A, int B, int C, int D>
static inline v4sf
vm_shuffle(const v4sf &v)
{
    v4sf vec;

    vec.f[0] = v.f[A];
    vec.f[1] = v.f[B];
    vec.f[2] = v.f[C];
    vec.f[3] = v.f[D];

    return vec;
}

static inline v4si
vm_load(const int32 v[4])
{
    return v4si(v[0], v[1], v[2], v[3]);
}

static inline v4sf
vm_load(const float v[4])
{
    return v4sf(v[0], v[1], v[2], v[3]);
}

static inline void
vm_store(float dst[4], v4sf value)
{
    dst[0] = value.f[0];
    dst[1] = value.f[1];
    dst[2] = value.f[2];
    dst[3] = value.f[3];
}


static inline v4si
vm_insert(const v4si v, int32 a, int n)
{
    v4si vec;

    vec = v;
    vec.i[n] = a;
    return vec;
}

static inline v4sf
vm_insert(const v4sf v, float a, int n)
{
    v4sf vec;

    vec = v;
    vec.f[n] = a;
    return vec;
}

static inline int
vm_extract(const v4si v, int n)
{
    return v.i[n];
}

static inline float
vm_extract(const v4sf v, int n)
{
    return v.f[n];
}

#define VM_EXTRACT	vm_extract
#define VM_INSERT	vm_insert
#define VM_SPLATS	vm_splats
#define VM_LOAD		vm_load
#define VM_STORE	vm_store

#define VM_CMPLT	vm_clt
#define VM_CMPLE	vm_cle
#define VM_CMPGT	vm_cgt
#define VM_CMPGE	vm_cge
#define VM_CMPEQ	vm_ceq
#define VM_CMPNE	vm_cne

#define VM_ICMPLT	vm_clt
#define VM_ICMPGT	vm_cgt
#define VM_ICMPEQ	vm_ceq

#define VM_IADD		vm_add
#define VM_ISUB		vm_sub
#define VM_IMUL		vm_mul

#define VM_ADD		vm_add
#define VM_SUB		vm_sub
#define VM_MUL		vm_mul
#define VM_DIV		vm_div
#define VM_FDIV		vm_div
#define VM_NEG		vm_negate
#define VM_SQRT		vm_sqrt
#define VM_FSQRT	vm_sqrt
#define VM_ISQRT	vm_isqrt
#define VM_ABS		vm_abs

#define VM_MADD		vm_madd
#define VM_INVERT	vm_reciprocal

#define VM_MIN		vm_min
#define VM_MAX		vm_max

#define VM_AND		vm_and
#define VM_ANDNOT	vm_andnot
#define VM_OR		vm_or
#define VM_XOR		vm_xor

#define VM_ALLBITS	vm_allbits

#define VM_SHUFFLE	vm_shuffle

#define VM_P_FLOOR()
#define VM_FLOOR	vm_floor
#define VM_E_FLOOR()

#define VM_INT		vm_intcast

#define VM_IFLOAT	vm_floatcast

#endif
