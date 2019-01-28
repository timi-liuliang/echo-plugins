/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VM_AltivecFunc.h ( VM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VM_AltivecFunc__
#define __VM_AltivecFunc__

#include "VM_API.h"
#include <SYS/SYS_Types.h>
#include <altivec.h>

#define CPU_HAS_SIMD_INSTR	1
#define VM_ALTIVEC_STYLE	1
#define VM_ALTIVEC_VECTOR	1

typedef vector float	v4sf;
typedef vector int	v4si;

#define V4SF(A)		(v4sf)A
#define V4SI(A)		(v4si)A

#define V4SF_CONST(val)	(vector float){val,val,val,val}
#define V4SI_CONST(val)	(vector int){val,val,val,val}

static inline v4sf
vm_vec_reciprocal(const v4sf &a)
{
    // Get the estimate
    vector float	est = vec_re(a);

    // Now, one round of Newton-Raphson refinement
    return vec_madd(vec_nmsub(est, a, V4SF_CONST(1)), est, est);
}

static inline v4sf
vm_vec_fdiv(v4sf a, v4sf b)	// Fast division
{
    return vec_madd(a, vec_re(b), V4SF_CONST(0));
}

static inline v4sf
vm_vec_qdiv(v4sf a, v4sf b)	// Higher Quality division
{
    return vec_madd(a, vm_vec_reciprocal(b), V4SF_CONST(0));
}

static inline v4sf
vm_vec_fsqrt(v4sf a)		// Fast sqrt()
{
    v4sf	mask = (v4sf)vec_cmpgt(a, V4SF_CONST(0));
    return vec_and(mask, vec_re(vec_rsqrte(a)));
}

static inline v4sf
vm_vec_qisqrt(v4sf a)		// Higher quality rsqrte()
{
    // Get the root reciprocal estimate
    vector float zero = V4SF_CONST(0);
    vector float p5 = V4SF_CONST(0.5);
    vector float one = V4SF_CONST(1);
    vector float est = vec_rsqrte(a);

    // Now perform one round of Newton-Raphson refinement
    vector float est2 = vec_madd(est, est, zero);
    vector float half = vec_madd(est, p5, zero);
    return vec_madd(vec_nmsub(a, est2, one), half, est);
}

static inline v4sf
vm_vec_qsqrt(v4sf a)		// Higher Quality sqrt
{
    v4sf	mask = (v4sf)vec_cmpgt(a, V4SF_CONST(0));
    return vec_and(mask, vm_vec_reciprocal(vm_vec_qisqrt(a)));
}

static inline v4sf
vm_vec_mul(v4sf a, v4sf b)
{
    return vec_madd(a, b, V4SF_CONST(0));
}

static inline v4si
vm_vec_andnot(v4si a, v4si b)
{
    return vec_andc(b, a);
}

static inline v4sf
vm_vec_negate(v4sf a)
{
    return vec_sub(V4SF_CONST(0), a);
}

static inline v4si
vm_vec_cmpneq(v4sf a, v4sf b)
{
    return (v4si)vec_xor(vec_cmpeq(a, b), V4SI_CONST(0xFFFFFFFF));
}

static inline v4si
vm_vec_cmpneq(v4si a, v4si b)
{
    return (v4si)vec_xor(vec_cmpeq(a, b), V4SI_CONST(0xFFFFFFFF));
}

static inline v4si
vm_vec_cmple(v4sf a, v4sf b)
{
    return (v4si)vec_cmple(a, b);
}

static inline v4si
vm_vec_cmple(v4si a, v4si b)
{
    return (v4si)vec_xor(vec_cmpgt(a, b), V4SI_CONST(0xFFFFFFFF));
}

static inline v4si
vm_vec_cmpge(v4sf a, v4sf b)
{
    return (v4si)vec_cmpge(a, b);
}

static inline v4si
vm_vec_cmpge(v4si a, v4si b)
{
    return (v4si)vec_xor(vec_cmplt(a, b), V4SI_CONST(0xFFFFFFFF));
}

static inline bool
vm_allbits(const v4si &a)
{
    return vec_all_eq(a, V4SI_CONST(0xFFFFFFFF));
}

static inline void
vm_splats(v4si &v, uint32 a)
{
    uint32 *vi = (uint32 *)&v;
    vi[0] = vi[1] = vi[2] = vi[3] = a;
}

static inline void
vm_splats(v4sf &v, float a)
{
    float *vf = (float *)&v;
    vf[0] = vf[1] = vf[2] = vf[3] = a;
}

static inline void
vm_splats(v4si &v, uint32 a, uint32 b, uint32 c, uint32 d)
{
    uint32 *vi = (uint32 *)&v;
    vi[0] = a;
    vi[1] = b;
    vi[2] = c;
    vi[3] = d;
}

static inline void
vm_splats(v4sf &v, float a, float b, float c, float d)
{
    float *vf = (float *)&v;
    vf[0] = a;
    vf[1] = b;
    vf[2] = c;
    vf[3] = d;
}

template <int A, int B, int C, int D>
static inline v4sf
vm_shuffle(const v4sf &v)
{
    vector unsigned char permute;
    unsigned char *p = (unsigned char *)&permute;
    int a = A * 4;
    int b = B * 4;
    int c = C * 4;
    int d = D * 4;
    p[0] = (unsigned char)a;
    p[1] = (unsigned char)a+1;
    p[2] = (unsigned char)a+2;
    p[3] = (unsigned char)a+3;
    p[4] = (unsigned char)b;
    p[5] = (unsigned char)b+1;
    p[6] = (unsigned char)b+2;
    p[7] = (unsigned char)b+3;
    p[8] = (unsigned char)c;
    p[9] = (unsigned char)c+1;
    p[10] = (unsigned char)c+2;
    p[11] = (unsigned char)c+3;
    p[12] = (unsigned char)d;
    p[13] = (unsigned char)d+1;
    p[14] = (unsigned char)d+2;
    p[15] = (unsigned char)d+3;

    return vec_perm(v, v, permute);
}

#define VM_SPLATS	vm_splats
#define VM_CMPLT	(v4si)vec_cmplt
#define VM_CMPLE	(v4si)vm_vec_cmple
#define VM_CMPGT	(v4si)vec_cmpgt
#define VM_CMPGE	(v4si)vm_vec_cmpge
#define VM_CMPEQ	(v4si)vec_cmpeq
#define VM_CMPNE	(v4si)vm_vec_cmpneq

#define VM_ICMPLT	(v4si)vec_cmplt
#define VM_ICMPGT	(v4si)vec_cmpgt
#define VM_ICMPEQ	(v4si)vec_cmpeq

#define VM_IADD		vec_add
#define VM_ISUB		vec_sub
#error VM_IMUL is not implemented!

#define VM_ADD		vec_add
#define VM_SUB		vec_sub
#define VM_MUL		vm_vec_mul
#define VM_DIV		vm_vec_qdiv
#define VM_SQRT		vm_vec_qsqrt
#define VM_ISQRT	vec_rsqrte
#define VM_INVERT	vm_vec_reciprocal
#define VM_ABS		vec_abs

#define VM_FDIV		vm_vec_fdiv
#define VM_FSQRT	vm_vec_fsqrt
#define VM_NEG		vm_vec_negate
#define VM_MADD		vec_madd

#define VM_MIN		vec_min
#define VM_MAX		vec_max

#define VM_AND		vec_and
#define VM_ANDNOT	vm_vec_andnot
#define VM_OR		vec_or
#define VM_XOR		vec_xor

#define VM_ALLBITS	vm_allbits

#define VM_SHUFFLE	vm_shuffle

static inline v4sf
vm_vec_floatcast(const v4si i)
{
    return vec_ctf(i, 0);
}

static inline v4si
vm_vec_intcast(const v4sf f)
{
    return vec_cts(f, 0);
}

static inline v4si
vm_vec_floor(const v4sf &f)
{
    return vec_cts(vec_floor(f), 0);
}

#define VM_P_FLOOR()
#define VM_FLOOR		vm_vec_floor
#define VM_E_FLOOR()

#define VM_INT			vm_vec_intcast

// Float to integer conversion
#define VM_IFLOAT		vm_vec_floatcast

#endif
