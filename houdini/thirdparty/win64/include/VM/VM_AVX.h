/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VM_AVX.C ( VM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VM_AVX__
#define __VM_AVX__

#include <SYS/SYS_Visibility.h>
#include <SYS/SYS_Math.h>
#include <cstdint>

#if defined(FORCE_NON_SIMD)
    #include "VM_BasicAVXFunc.h"
#else
    #include "VM_AVXFunc.h"
#endif

class v8uf;

class v8uu {
public:
    SYS_FORCE_INLINE v8uu() {}
    SYS_FORCE_INLINE v8uu(const v8si &v) : vector(v)		  {}
    SYS_FORCE_INLINE v8uu(const v8uu &v) : vector(v.vector)       {} 
    explicit SYS_FORCE_INLINE v8uu(int32 v) { vector = VM_SPLATS_AVX(v); }
    explicit SYS_FORCE_INLINE v8uu(const int32 v[8])
		{ vector = VM_LOAD_AVX(v); }
    SYS_FORCE_INLINE v8uu(int32 a, int32 b, int32 c, int32 d,
			  int32 e, int32 f, int32 g, int32 h)
		{ vector = VM_SPLATS_AVX(a, b, c, d, e, f, g, h); }

    // Assignment
    SYS_FORCE_INLINE v8uu operator=(int32 v)
		{ vector = v8uu(v).vector; return *this; }
    SYS_FORCE_INLINE v8uu operator=(v8si v)
		{ vector = v; return *this; }
    SYS_FORCE_INLINE v8uu operator=(const v8uu &v)
		{ vector = v.vector; return *this; }
    
    SYS_FORCE_INLINE void condAssign(const v8uu &val, const v8uu &c)
		{ *this = (c & val) | ((!c) & *this); }

    // Comparison
    SYS_FORCE_INLINE v8uu operator == (const v8uu &v) const
		{ return v8uu(VM_ICMPEQ_AVX(vector, v.vector)); }
    SYS_FORCE_INLINE v8uu operator != (const v8uu &v) const
		{ return ~(*this == v); }
    SYS_FORCE_INLINE v8uu	operator >  (const v8uu &v) const
		{ return v8uu(VM_ICMPGT_AVX(vector, v.vector)); }
    SYS_FORCE_INLINE v8uu	operator <  (const v8uu &v) const
		{ return v8uu(VM_ICMPLT_AVX(vector, v.vector)); }
    SYS_FORCE_INLINE v8uu	operator >= (const v8uu &v) const
		{ return ~(*this < v); }
    SYS_FORCE_INLINE v8uu	operator <= (const v8uu &v) const
		{ return ~(*this > v); }

    SYS_FORCE_INLINE v8uu operator == (int32 v) const { return *this == v8uu(v); }
    SYS_FORCE_INLINE v8uu operator != (int32 v) const { return *this != v8uu(v); }
    SYS_FORCE_INLINE v8uu operator >  (int32 v) const { return *this > v8uu(v); }
    SYS_FORCE_INLINE v8uu operator <  (int32 v) const { return *this < v8uu(v); }
    SYS_FORCE_INLINE v8uu operator >= (int32 v) const { return *this >= v8uu(v); }
    SYS_FORCE_INLINE v8uu operator <= (int32 v) const { return *this <= v8uu(v); }
    
    // Basic math
    SYS_FORCE_INLINE v8uu operator+(const v8uu &r) const
		 { return v8uu(VM_IADD_AVX(vector, r.vector)); }
    SYS_FORCE_INLINE v8uu operator-(const v8uu &r) const
		 { return v8uu(VM_ISUB_AVX(vector, r.vector)); }
    SYS_FORCE_INLINE v8uu operator+=(const v8uu &r) { return (*this = *this + r); }
    SYS_FORCE_INLINE v8uu operator-=(const v8uu &r) { return (*this = *this - r); }
    SYS_FORCE_INLINE v8uu operator+(int32 r) const { return *this + v8uu(r); }
    SYS_FORCE_INLINE v8uu operator-(int32 r) const { return *this - v8uu(r); }
    SYS_FORCE_INLINE v8uu operator+=(int32 r) { return (*this = *this + r); }
    SYS_FORCE_INLINE v8uu operator-=(int32 r) { return (*this = *this - r); }

    // logical/bitwise
    
    SYS_FORCE_INLINE v8uu operator||(const v8uu &r) const
		{ return v8uu(VM_OR_AVX(vector, r.vector)); }
    SYS_FORCE_INLINE v8uu operator&&(const v8uu &r) const
		{ return v8uu(VM_AND_AVX(vector, r.vector)); }
    SYS_FORCE_INLINE v8uu operator^(const v8uu &r) const
		{ return v8uu(VM_XOR_AVX(vector, r.vector)); }
    SYS_FORCE_INLINE v8uu operator!() const
 		{ return *this == v8uu(0); }
    
    SYS_FORCE_INLINE v8uu  operator|(const v8uu &r) const { return *this || r; }
    SYS_FORCE_INLINE v8uu  operator&(const v8uu &r) const { return *this && r; }
    SYS_FORCE_INLINE v8uu  operator~() const
		{ return *this ^ v8uu(0xFFFFFFFF); }

    // component
    SYS_FORCE_INLINE int32 operator[](int idx) const { return VM_EXTRACT_AVX(vector, idx); }
    SYS_FORCE_INLINE void setComp(int idx, int32 v) { vector = VM_INSERT_AVX(vector, v, idx); }

    SYS_FORCE_INLINE v8uf toFloat() const;

public:
    v8si	 vector;
};

class v8uf {
public:
     SYS_FORCE_INLINE v8uf() {}
     SYS_FORCE_INLINE v8uf(const v8sf &v) : vector(v)              {} 
     SYS_FORCE_INLINE v8uf(const v8uf &v) : vector(v.vector)       {} 
     explicit SYS_FORCE_INLINE v8uf(float v) { vector = VM_SPLATS_AVX(v); }
     explicit SYS_FORCE_INLINE v8uf(const float v[8])
		{ vector = VM_LOAD_AVX(v); }
     SYS_FORCE_INLINE v8uf(float a, float b, float c, float d,
			   float e, float f, float g, float h)
		{ vector = VM_SPLATS_AVX(a, b, c, d, e, f , g, h); }

    // Assignment
    SYS_FORCE_INLINE v8uf operator=(float v)
		{ vector = v8uf(v).vector; return *this; }
    SYS_FORCE_INLINE v8uf operator=(v8sf v)
		{ vector = v; return *this; }
    SYS_FORCE_INLINE v8uf operator=(const v8uf &v)
		{ vector = v.vector; return *this; }
    
    SYS_FORCE_INLINE void condAssign(const v8uf &val, const v8uu &c)
		{ *this = (val & c) | (*this & ~c); }

    // Comparison
    SYS_FORCE_INLINE v8uu operator == (const v8uf &v) const
		{ return v8uu(VM_CMPEQ_AVX(vector, v.vector)); }
    SYS_FORCE_INLINE v8uu operator != (const v8uf &v) const
		{ return v8uu(VM_CMPNE_AVX(vector, v.vector)); }
    SYS_FORCE_INLINE v8uu operator >  (const v8uf &v) const
		{ return v8uu(VM_CMPGT_AVX(vector, v.vector)); }
    SYS_FORCE_INLINE v8uu operator <  (const v8uf &v) const
		{ return v8uu(VM_CMPLT_AVX(vector, v.vector)); }
    SYS_FORCE_INLINE v8uu operator >= (const v8uf &v) const
		{ return v8uu(VM_CMPGE_AVX(vector, v.vector)); }
    SYS_FORCE_INLINE v8uu operator <= (const v8uf &v) const
		{ return v8uu(VM_CMPLE_AVX(vector, v.vector)); }

    SYS_FORCE_INLINE v8uu operator == (float v) const { return *this == v8uf(v); }
    SYS_FORCE_INLINE v8uu operator != (float v) const { return *this != v8uf(v); }
    SYS_FORCE_INLINE v8uu operator >  (float v) const { return *this > v8uf(v); }
    SYS_FORCE_INLINE v8uu operator <  (float v) const { return *this < v8uf(v); }
    SYS_FORCE_INLINE v8uu operator >= (float v) const { return *this >= v8uf(v); }
    SYS_FORCE_INLINE v8uu operator <= (float v) const { return *this <= v8uf(v); }
    

    // Basic math
    SYS_FORCE_INLINE v8uf operator+(const v8uf &r) const
		 { return v8uf(VM_ADD_AVX(vector, r.vector)); }
    SYS_FORCE_INLINE v8uf operator-(const v8uf &r) const
		 { return v8uf(VM_SUB_AVX(vector, r.vector)); }
    SYS_FORCE_INLINE v8uf operator-() const
		 { return v8uf(VM_NEG_AVX(vector)); }
    SYS_FORCE_INLINE v8uf operator*(const v8uf &r) const
		 { return v8uf(VM_MUL_AVX(vector, r.vector)); }
    SYS_FORCE_INLINE v8uf operator/(const v8uf &r) const
		 { return v8uf(VM_DIV_AVX(vector, r.vector)); }

    SYS_FORCE_INLINE v8uf operator+=(const v8uf &r) { return (*this = *this + r); }
    SYS_FORCE_INLINE v8uf operator-=(const v8uf &r) { return (*this = *this - r); }
    SYS_FORCE_INLINE v8uf operator*=(const v8uf &r) { return (*this = *this * r); }
    SYS_FORCE_INLINE v8uf operator/=(const v8uf &r) { return (*this = *this / r); }

    SYS_FORCE_INLINE v8uf operator+(float r) const { return *this + v8uf(r); }
    SYS_FORCE_INLINE v8uf operator-(float r) const { return *this - v8uf(r); }
    SYS_FORCE_INLINE v8uf operator*(float r) const { return *this * v8uf(r); }
    SYS_FORCE_INLINE v8uf operator/(float r) const { return *this / v8uf(r); }
    SYS_FORCE_INLINE v8uf operator+=(float r) { return (*this = *this + r); }
    SYS_FORCE_INLINE v8uf operator-=(float r) { return (*this = *this - r); }
    SYS_FORCE_INLINE v8uf operator*=(float r) { return (*this = *this * r); }
    SYS_FORCE_INLINE v8uf operator/=(float r) { return (*this = *this / r); }
    
    // logical/bitwise
    
    SYS_FORCE_INLINE v8uf operator||(const v8uu &r) const
		{ return v8uf(V8SF(VM_OR_AVX(V8SI(vector), r.vector))); }
    SYS_FORCE_INLINE v8uf operator&&(const v8uu &r) const
		{ return v8uf(V8SF(VM_AND_AVX(V8SI(vector), r.vector))); }
    SYS_FORCE_INLINE v8uf operator^(const v8uu &r) const
		{ return v8uf(V8SF(VM_XOR_AVX(V8SI(vector), r.vector))); }
    SYS_FORCE_INLINE v8uf operator!() const
 		{ return v8uf(V8SF((*this == v8uf(0.0F)).vector)); }
    
    SYS_FORCE_INLINE v8uf operator||(const v8uf &r) const
		{ return v8uf(V8SF(VM_OR_AVX(V8SI(vector), V8SI(r.vector)))); }
    SYS_FORCE_INLINE v8uf operator&&(const v8uf &r) const
		{ return v8uf(V8SF(VM_AND_AVX(V8SI(vector), V8SI(r.vector)))); }
    SYS_FORCE_INLINE v8uf operator^(const v8uf &r) const
		{ return v8uf(V8SF(VM_XOR_AVX(V8SI(vector), V8SI(r.vector)))); }
    
    SYS_FORCE_INLINE v8uf operator|(const v8uu &r) const { return *this || r; }
    SYS_FORCE_INLINE v8uf operator&(const v8uu &r) const { return *this && r; }
    SYS_FORCE_INLINE v8uf operator~() const
		{ return *this ^ v8uu(0xFFFFFFFF); }

    SYS_FORCE_INLINE v8uf operator|(const v8uf &r) const { return *this || r; }
    SYS_FORCE_INLINE v8uf operator&(const v8uf &r) const { return *this && r; }

    // component
    SYS_FORCE_INLINE float operator[](int idx) const { return VM_EXTRACT_AVX(vector, idx); }
    SYS_FORCE_INLINE void setComp(int idx, float v) { vector = VM_INSERT_AVX(vector, v, idx); }

    // more math
    SYS_FORCE_INLINE v8uf abs() const { return v8uf(VM_ABS_AVX(vector)); }
    SYS_FORCE_INLINE v8uf clamp(const v8uf &low, const v8uf &high) const
		 { return v8uf(VM_MIN_AVX(VM_MAX_AVX(vector, low.vector), high.vector)); }
    SYS_FORCE_INLINE v8uf clamp(float low, float high) const
		 { return v8uf(VM_MIN_AVX(VM_MAX_AVX(vector,v8uf(low).vector), v8uf(high).vector)); }
    SYS_FORCE_INLINE v8uf recip() const { return v8uf(VM_INVERT_AVX(vector)); }
    
    /// This is a lie, it is a signed int.
    SYS_FORCE_INLINE v8uu toUnsignedInt() const { return VM_INT_AVX(vector); }
    SYS_FORCE_INLINE v8uu toSignedInt() const { return VM_INT_AVX(vector); }

    SYS_FORCE_INLINE v8uu floor() const
    {
	v8uu		result;
	VM_P_FLOOR_AVX();
	result = VM_FLOOR_AVX(vector);
	VM_E_FLOOR_AVX();
	return result;
    }

    /// Returns the integer part of this float, this becomes the
    /// 0..1 fractional component.
    SYS_FORCE_INLINE v8uu splitFloat()
    {
	v8uu	base;
	base = toSignedInt();
	*this -= base.toFloat();
	return base;
    }

    template <int A, int B, int C, int D>
    SYS_FORCE_INLINE v8uf swizzle() const
    { 
	return VM_SHUFFLE_AVX<A,B,C,D>(vector);
    }

public:
    v8sf	 vector;
};

SYS_FORCE_INLINE v8uf
v8uu::toFloat() const
{
    return v8uf(VM_IFLOAT_AVX(vector));
}

//
// Custom vector operations
//

static SYS_FORCE_INLINE v8uf
sqrt(const v8uf &a)
{
    return v8uf(VM_SQRT_AVX(a.vector));
}

static SYS_FORCE_INLINE v8uf
fabs(const v8uf &a)
{
    return a.abs();
}

// Use this operation to mask disabled values to 0
// rval = !a ? b : 0;

static SYS_FORCE_INLINE v8uf
andn(const v8uu &a, const v8uf &b)
{
    return v8uf(V8SF(VM_ANDNOT_AVX(a.vector, V8SI(b.vector))));
}

static SYS_FORCE_INLINE v8uu
andn(const v8uu &a, const v8uu &b)
{
    return v8uu(VM_ANDNOT_AVX(a.vector, b.vector));
}

// rval = a ? b : c;
static SYS_FORCE_INLINE v8uf
ternary(const v8uu &a, const v8uf &b, const v8uf &c)
{
    return (b & a) | andn(a, c);
}

static SYS_FORCE_INLINE v8uu
ternary(const v8uu &a, const v8uu &b, const v8uu &c)
{
    return (b & a) | andn(a, c);
}

// rval = !(a && b)
static SYS_FORCE_INLINE v8uu
nand(const v8uu &a, const v8uu &b)
{
    return !v8uu(VM_AND_AVX(a.vector, b.vector));
}

static SYS_FORCE_INLINE v8uf
vmin(const v8uf &a, const v8uf &b)
{
    return v8uf(VM_MIN_AVX(a.vector, b.vector));
}

static SYS_FORCE_INLINE v8uf
vmax(const v8uf &a, const v8uf &b)
{
    return v8uf(VM_MAX_AVX(a.vector, b.vector));
}

static SYS_FORCE_INLINE v8uf
clamp(const v8uf &a, const v8uf &b, const v8uf &c)
{
    return vmax(vmin(a, c), b);
}

static SYS_FORCE_INLINE v8uf
clamp(const v8uf &a, float b, float c)
{
    return vmax(vmin(a, v8uf(c)), v8uf(b));
}

static SYS_FORCE_INLINE bool
allbits(const v8uu &a)
{
    return vm_allbits_avx(a.vector);
}

static SYS_FORCE_INLINE bool
anybits(const v8uu &a)
{
    return !allbits(~a);
}

static SYS_FORCE_INLINE v8uf
madd(const v8uf &v, const v8uf &f, const v8uf &a)
{
    return v8uf(VM_MADD_AVX(v.vector, f.vector, a.vector));
}

static SYS_FORCE_INLINE v8uf
madd(const v8uf &v, float f, float a)
{
    return v8uf(VM_MADD_AVX(v.vector, v8uf(f).vector, v8uf(a).vector));
}

static SYS_FORCE_INLINE v8uf
madd(const v8uf &v, float f, const v8uf &a)
{
    return v8uf(VM_MADD_AVX(v.vector, v8uf(f).vector, a.vector));
}

static SYS_FORCE_INLINE v8uf
msub(const v8uf &v, const v8uf &f, const v8uf &s)
{
    return madd(v, f, -s);
}

static SYS_FORCE_INLINE v8uf
msub(const v8uf &v, float f, float s)
{
    return madd(v, f, -s);
}

static SYS_FORCE_INLINE v8uf
lerp(const v8uf &a, const v8uf &b, const v8uf &w)
{
    v8uf w1 = v8uf(1.0F) - w;
    return madd(a, w1, b*w);
}

static SYS_FORCE_INLINE v8uf
luminance(const v8uf &r, const v8uf &g, const v8uf &b,
	  float rw, float gw, float bw)
{
    return v8uf(madd(r, v8uf(rw), madd(g, v8uf(gw), b * bw)));
}

// These operations are somewhat ill-defined for 8-vectors
#if 0
static inline float
dot3(const v8uf &a, const v8uf &b)
{
    v8uf res = a*b;
    return res[0] + res[1] + res[2];
}

static inline float
dot4(const v8uf &a, const v8uf &b)
{
    v8uf res = a*b;
    return res[0] + res[1] + res[2] + res[3];
}

static inline float
length(const v8uf &a)
{
    return SYSsqrt(dot3(a, a));
}

static inline v8uf
normalize(const v8uf &a)
{
    return a / length(a);
}

static inline v8uf
cross(const v8uf &a, const v8uf &b)
{
    return v8uf(a[1]*b[2] - a[2]*b[1],
		a[2]*b[0] - a[0]*b[2],
		a[0]*b[1] - a[1]*b[0], 0,

		a[5]*b[6] - a[6]*b[5],
		a[6]*b[4] - a[4]*b[6],
		a[4]*b[5] - a[5]*b[4], 0);
}
#endif

// Currently there is no specific support for signed integers
typedef v8uu v8ui;

// Assuming that ptr is an array of elements of type STYPE, this operation
// will return the index of the first element that is aligned to (1<<ASIZE)
// bytes.
#define VM_ALIGN_AVX(ptr, ASIZE, STYPE)	\
		((((1<<ASIZE)-(intptr_t)ptr)&((1<<ASIZE)-1))/sizeof(STYPE))

#endif
