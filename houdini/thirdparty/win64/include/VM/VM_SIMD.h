/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VM_SIMD.C ( VM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VM_SIMD__
#define __VM_SIMD__

#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_StaticAssert.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_TypeTraits.h>
#include <SYS/SYS_Visibility.h>
#include <cstdint>

//#define FORCE_NON_SIMD

// Some older versions of glibc don't correctly align the stack for extra
// threads (this problem should have been fixed in 2.3.4 - see
// http://sources.redhat.com/bugzilla/show_bug.cgi?id=723).  Instead of
// checking the glibc version, check the compiler version and assume newer
// compilers are used with newer glibc.

#if defined(FORCE_NON_SIMD)
    #include "VM_BasicFunc.h"
#else
    #if defined(CELLRSX) || defined(PPC)
	#include "VM_AltivecFunc.h"
    #elif defined(LINUX) && SYS_IS_GCC_GE(3, 4) && defined(__SSE2__)
	#include "VM_SSEFunc.h"
    #elif defined(WIN32)
	#include "VM_SSEFunc.h"
    #elif defined(MBSD_INTEL)
	#include "VM_SSEFunc.h"
    #else
	#include "VM_BasicFunc.h"
    #endif
#endif

class v4uf;

class v4uu {
public:
    // NOTE: For unknown reasons, BVH construction is significantly
    //       slower on GCC 5.4 if v4uu and v4uf are POD types, so I put
    //       back the previous code.
#if 0
    SYS_FORCE_INLINE v4uu() noexcept = default;
    SYS_FORCE_INLINE v4uu(const v4si &v) : vector(v) {
        SYS_STATIC_ASSERT_MSG(SYSisPOD<v4uu>(), "v4uu should be POD, for better performance in UT_Array, etc.");
    }
    SYS_FORCE_INLINE v4uu(const v4uu &v) noexcept = default;
    SYS_FORCE_INLINE v4uu &operator=(const v4uu &v) noexcept = default;
#else
    SYS_FORCE_INLINE v4uu() {}
    SYS_FORCE_INLINE v4uu(const v4si &v) : vector(v) {}
    SYS_FORCE_INLINE v4uu(const v4uu &v) : vector(v.vector) {}
    SYS_FORCE_INLINE v4uu operator=(const v4uu &v)
    { vector = v.vector; return *this; }
#endif
    explicit SYS_FORCE_INLINE v4uu(int32 v) { vector = VM_SPLATS(v); }
    explicit SYS_FORCE_INLINE v4uu(const int32 v[4])
    { vector = VM_LOAD(v); }
    SYS_FORCE_INLINE v4uu(int32 a, int32 b, int32 c, int32 d)
    { vector = VM_SPLATS(a, b, c, d); }

    // Assignment
    SYS_FORCE_INLINE v4uu operator=(int32 v)
		{ vector = v4uu(v).vector; return *this; }
    SYS_FORCE_INLINE v4uu operator=(v4si v)
		{ vector = v; return *this; }

    SYS_FORCE_INLINE void condAssign(const v4uu &val, const v4uu &c)
		{ *this = (c & val) | ((!c) & *this); }

    // Comparison
    SYS_FORCE_INLINE v4uu operator == (const v4uu &v) const
		{ return v4uu(VM_ICMPEQ(vector, v.vector)); }
    SYS_FORCE_INLINE v4uu operator != (const v4uu &v) const
		{ return ~(*this == v); }
    SYS_FORCE_INLINE v4uu operator >  (const v4uu &v) const
		{ return v4uu(VM_ICMPGT(vector, v.vector)); }
    SYS_FORCE_INLINE v4uu operator <  (const v4uu &v) const
		{ return v4uu(VM_ICMPLT(vector, v.vector)); }
    SYS_FORCE_INLINE v4uu operator >= (const v4uu &v) const
		{ return ~(*this < v); }
    SYS_FORCE_INLINE v4uu operator <= (const v4uu &v) const
		{ return ~(*this > v); }

    SYS_FORCE_INLINE v4uu operator == (int32 v) const { return *this == v4uu(v); }
    SYS_FORCE_INLINE v4uu operator != (int32 v) const { return *this != v4uu(v); }
    SYS_FORCE_INLINE v4uu operator >  (int32 v) const { return *this > v4uu(v); }
    SYS_FORCE_INLINE v4uu operator <  (int32 v) const { return *this < v4uu(v); }
    SYS_FORCE_INLINE v4uu operator >= (int32 v) const { return *this >= v4uu(v); }
    SYS_FORCE_INLINE v4uu operator <= (int32 v) const { return *this <= v4uu(v); }
    
    // Basic math
    SYS_FORCE_INLINE v4uu operator+(const v4uu &r) const
		 { return v4uu(VM_IADD(vector, r.vector)); }
    SYS_FORCE_INLINE v4uu operator-(const v4uu &r) const
		 { return v4uu(VM_ISUB(vector, r.vector)); }
#if defined(VM_IMUL)
    SYS_FORCE_INLINE v4uu operator*(const v4uu &r) const
		 { return v4uu(VM_IMUL(vector, r.vector)); }
#else
    SYS_FORCE_INLINE v4uu operator*(const v4uu &r) const
    {
    	return v4uu((*this)[0] * r[0],
    	            (*this)[1] * r[1],
    	            (*this)[2] * r[2],
    	            (*this)[3] * r[3]);
    }
#endif
    SYS_FORCE_INLINE v4uu operator+=(const v4uu &r) { return (*this = *this + r); }
    SYS_FORCE_INLINE v4uu operator-=(const v4uu &r) { return (*this = *this - r); }
    SYS_FORCE_INLINE v4uu operator*=(const v4uu &r) { return (*this = *this * r); }
    SYS_FORCE_INLINE v4uu operator+(int32 r) const { return *this + v4uu(r); }
    SYS_FORCE_INLINE v4uu operator-(int32 r) const { return *this - v4uu(r); }
    SYS_FORCE_INLINE v4uu operator*(int32 r) const { return *this * v4uu(r); }
    SYS_FORCE_INLINE v4uu operator+=(int32 r) { return (*this = *this + r); }
    SYS_FORCE_INLINE v4uu operator-=(int32 r) { return (*this = *this - r); }
    SYS_FORCE_INLINE v4uu operator*=(int32 r) { return (*this = *this * r); }

    // Modulo
    SYS_FORCE_INLINE v4uu operator%(const v4uu &r) const
    {
    	return v4uu((*this)[0] % r[0],
    	            (*this)[1] % r[1],
    	            (*this)[2] % r[2],
    	            (*this)[3] % r[3]);
    }
    SYS_FORCE_INLINE v4uu operator%(int r) const
    {
    	return v4uu((*this)[0] % r,
    	            (*this)[1] % r,
    	            (*this)[2] % r,
    	            (*this)[3] % r);
    }
    SYS_FORCE_INLINE v4uu operator%=(const v4uu &r) { return (*this = *this % r); }
    SYS_FORCE_INLINE v4uu operator%=(int r) { return (*this = *this % r); }

    
    // logical/bitwise
    
    SYS_FORCE_INLINE v4uu operator||(const v4uu &r) const
		{ return v4uu(VM_OR(vector, r.vector)); }
    SYS_FORCE_INLINE v4uu operator&&(const v4uu &r) const
		{ return v4uu(VM_AND(vector, r.vector)); }
    SYS_FORCE_INLINE v4uu operator^(const v4uu &r) const
		{ return v4uu(VM_XOR(vector, r.vector)); }
    SYS_FORCE_INLINE v4uu operator!() const
 		{ return *this == v4uu(0); }
    
    SYS_FORCE_INLINE v4uu operator|(const v4uu &r) const { return *this || r; }
    SYS_FORCE_INLINE v4uu operator&(const v4uu &r) const { return *this && r; }
    SYS_FORCE_INLINE v4uu operator~() const
		{ return *this ^ v4uu(0xFFFFFFFF); }
    SYS_FORCE_INLINE void operator|=(const v4uu &r) { vector = VM_OR(vector, r.vector); }
    SYS_FORCE_INLINE void operator&=(const v4uu &r) { vector = VM_AND(vector, r.vector); }
    SYS_FORCE_INLINE void operator^=(const v4uu &r) { vector = VM_XOR(vector, r.vector); }

    // component
    SYS_FORCE_INLINE int32 operator[](int idx) const { return VM_EXTRACT(vector, idx); }
    SYS_FORCE_INLINE void setComp(int idx, int32 v) { vector = VM_INSERT(vector, v, idx); }

    v4uf toFloat() const;

    operator v4uf() const;

public:
    v4si vector;
};

class v4uf {
public:
    SYS_FORCE_INLINE v4uf() noexcept = default;
    // NOTE: For unknown reasons, BVH construction is significantly
    //       slower on GCC 5.4 if v4uu and v4uf are POD types, so I put
    //       back the previous code.
#if 0
    SYS_FORCE_INLINE v4uf(const v4uf &v) noexcept = default;
    SYS_FORCE_INLINE v4uf(const v4sf &v) noexcept : vector(v) {
        SYS_STATIC_ASSERT_MSG(SYSisPOD<v4uf>(), "v4uf should be POD, for better performance in UT_Array, etc.");
    }
    SYS_FORCE_INLINE v4uf &operator=(const v4uf &v) noexcept = default;
#else
    SYS_FORCE_INLINE v4uf(const v4sf &v) noexcept : vector(v) {}
    SYS_FORCE_INLINE v4uf(const v4uf &v) noexcept : vector(v.vector) {}
    SYS_FORCE_INLINE v4uf operator=(const v4uf &v) noexcept
    { vector = v.vector; return *this; }
#endif
    explicit SYS_FORCE_INLINE v4uf(float v) { vector = VM_SPLATS(v); }
    explicit SYS_FORCE_INLINE v4uf(const float v[4])
    { vector = VM_LOAD(v); }
    SYS_FORCE_INLINE v4uf(float a, float b, float c, float d)
    { vector = VM_SPLATS(a, b, c, d); }

    // Assignment
    SYS_FORCE_INLINE v4uf operator=(float v)
		{ vector = v4uf(v).vector; return *this; }
    SYS_FORCE_INLINE v4uf operator=(v4sf v) noexcept
		{ vector = v; return *this; }

    SYS_FORCE_INLINE void condAssign(const v4uf &val, const v4uu &c)
		{ *this = (val & c) | (*this & ~c); }

    // Comparison
    SYS_FORCE_INLINE v4uu operator == (const v4uf &v) const
		{ return v4uu(VM_CMPEQ(vector, v.vector)); }
    SYS_FORCE_INLINE v4uu operator != (const v4uf &v) const
		{ return v4uu(VM_CMPNE(vector, v.vector)); }
    SYS_FORCE_INLINE v4uu operator >  (const v4uf &v) const
		{ return v4uu(VM_CMPGT(vector, v.vector)); }
    SYS_FORCE_INLINE v4uu operator <  (const v4uf &v) const
		{ return v4uu(VM_CMPLT(vector, v.vector)); }
    SYS_FORCE_INLINE v4uu operator >= (const v4uf &v) const
		{ return v4uu(VM_CMPGE(vector, v.vector)); }
    SYS_FORCE_INLINE v4uu operator <= (const v4uf &v) const
		{ return v4uu(VM_CMPLE(vector, v.vector)); }

    SYS_FORCE_INLINE v4uu operator == (float v) const { return *this == v4uf(v); }
    SYS_FORCE_INLINE v4uu operator != (float v) const { return *this != v4uf(v); }
    SYS_FORCE_INLINE v4uu operator >  (float v) const { return *this > v4uf(v); }
    SYS_FORCE_INLINE v4uu operator <  (float v) const { return *this < v4uf(v); }
    SYS_FORCE_INLINE v4uu operator >= (float v) const { return *this >= v4uf(v); }
    SYS_FORCE_INLINE v4uu operator <= (float v) const { return *this <= v4uf(v); }
    

    // Basic math
    SYS_FORCE_INLINE v4uf operator+(const v4uf &r) const
		 { return v4uf(VM_ADD(vector, r.vector)); }
    SYS_FORCE_INLINE v4uf operator-(const v4uf &r) const
		 { return v4uf(VM_SUB(vector, r.vector)); }
    SYS_FORCE_INLINE v4uf operator-() const
		 { return v4uf(VM_NEG(vector)); }
    SYS_FORCE_INLINE v4uf operator*(const v4uf &r) const
		 { return v4uf(VM_MUL(vector, r.vector)); }
    SYS_FORCE_INLINE v4uf operator/(const v4uf &r) const
		 { return v4uf(VM_DIV(vector, r.vector)); }

    SYS_FORCE_INLINE v4uf operator+=(const v4uf &r) { return (*this = *this + r); }
    SYS_FORCE_INLINE v4uf operator-=(const v4uf &r) { return (*this = *this - r); }
    SYS_FORCE_INLINE v4uf operator*=(const v4uf &r) { return (*this = *this * r); }
    SYS_FORCE_INLINE v4uf operator/=(const v4uf &r) { return (*this = *this / r); }

    SYS_FORCE_INLINE v4uf operator+(float r) const { return *this + v4uf(r); }
    SYS_FORCE_INLINE v4uf operator-(float r) const { return *this - v4uf(r); }
    SYS_FORCE_INLINE v4uf operator*(float r) const { return *this * v4uf(r); }
    SYS_FORCE_INLINE v4uf operator/(float r) const { return *this / v4uf(r); }
    SYS_FORCE_INLINE v4uf operator+=(float r) { return (*this = *this + r); }
    SYS_FORCE_INLINE v4uf operator-=(float r) { return (*this = *this - r); }
    SYS_FORCE_INLINE v4uf operator*=(float r) { return (*this = *this * r); }
    SYS_FORCE_INLINE v4uf operator/=(float r) { return (*this = *this / r); }
    
    // logical/bitwise
    
    SYS_FORCE_INLINE v4uf operator||(const v4uu &r) const
		{ return v4uf(V4SF(VM_OR(V4SI(vector), r.vector))); }
    SYS_FORCE_INLINE v4uf operator&&(const v4uu &r) const
		{ return v4uf(V4SF(VM_AND(V4SI(vector), r.vector))); }
    SYS_FORCE_INLINE v4uf operator^(const v4uu &r) const
		{ return v4uf(V4SF(VM_XOR(V4SI(vector), r.vector))); }
    SYS_FORCE_INLINE v4uf operator!() const
 		{ return v4uf(V4SF((*this == v4uf(0.0F)).vector)); }
    
    SYS_FORCE_INLINE v4uf operator||(const v4uf &r) const
		{ return v4uf(V4SF(VM_OR(V4SI(vector), V4SI(r.vector)))); }
    SYS_FORCE_INLINE v4uf operator&&(const v4uf &r) const
		{ return v4uf(V4SF(VM_AND(V4SI(vector), V4SI(r.vector)))); }
    SYS_FORCE_INLINE v4uf operator^(const v4uf &r) const
		{ return v4uf(V4SF(VM_XOR(V4SI(vector), V4SI(r.vector)))); }
    
    SYS_FORCE_INLINE v4uf operator|(const v4uu &r) const { return *this || r; }
    SYS_FORCE_INLINE v4uf operator&(const v4uu &r) const { return *this && r; }
    SYS_FORCE_INLINE v4uf operator~() const
		{ return *this ^ v4uu(0xFFFFFFFF); }

    SYS_FORCE_INLINE v4uf operator|(const v4uf &r) const { return *this || r; }
    SYS_FORCE_INLINE v4uf operator&(const v4uf &r) const { return *this && r; }

    // component
    SYS_FORCE_INLINE float operator[](int idx) const { return VM_EXTRACT(vector, idx); }
    SYS_FORCE_INLINE void setComp(int idx, float v) { vector = VM_INSERT(vector, v, idx); }

    // more math
    SYS_FORCE_INLINE v4uf abs() const { return v4uf(VM_ABS(vector)); }
    SYS_FORCE_INLINE v4uf clamp(const v4uf &low, const v4uf &high) const
		 { return v4uf(
			 VM_MIN(VM_MAX(vector, low.vector), high.vector)); }
    SYS_FORCE_INLINE v4uf clamp(float low, float high) const
		 { return v4uf(VM_MIN(VM_MAX(vector,
				 v4uf(low).vector), v4uf(high).vector)); }
    SYS_FORCE_INLINE v4uf recip() const { return v4uf(VM_INVERT(vector)); }
    
    /// This is a lie, it is a signed int.
    SYS_FORCE_INLINE v4uu toUnsignedInt() const { return VM_INT(vector); }
    SYS_FORCE_INLINE v4uu toSignedInt() const { return VM_INT(vector); }

    v4uu floor() const
    {
        VM_P_FLOOR();
        v4uu result = VM_FLOOR(vector);
        VM_E_FLOOR();
        return result;
    }

    /// Returns the integer part of this float, this becomes the
    /// 0..1 fractional component.
    v4uu splitFloat()
    {
        v4uu base = toSignedInt();
        *this -= base.toFloat();
        return base;
    }

    template <int A, int B, int C, int D>
    SYS_FORCE_INLINE v4uf swizzle() const
    { 
        return VM_SHUFFLE<A,B,C,D>(vector);
    }

    SYS_FORCE_INLINE v4uu isFinite() const
    {
        // If the exponent is the maximum value, it's either infinite or NaN.
        const v4si mask = VM_SPLATS(0x7F800000);
        return ~v4uu(VM_ICMPEQ(VM_AND(V4SI(vector), mask), mask));
    }

public:
    v4sf vector;
};

SYS_FORCE_INLINE v4uf
v4uu::toFloat() const
{
    return v4uf(VM_IFLOAT(vector));
}
SYS_FORCE_INLINE
v4uu::operator v4uf() const
{
    return toFloat();
}

static SYS_FORCE_INLINE v4uf
operator*(float r, const v4uf &v) noexcept
{
    return v * v4uf(r);
}

//
// Custom vector operations
//

static SYS_FORCE_INLINE v4uf
sqrt(const v4uf &a)
{
    return v4uf(VM_SQRT(a.vector));
}

static SYS_FORCE_INLINE v4uf
fabs(const v4uf &a)
{
    return a.abs();
}

// Use this operation to mask disabled values to 0
// rval = !a ? b : 0;

static SYS_FORCE_INLINE v4uf
andn(const v4uu &a, const v4uf &b)
{
    return v4uf(V4SF(VM_ANDNOT(a.vector, V4SI(b.vector))));
}

static SYS_FORCE_INLINE v4uu
andn(const v4uu &a, const v4uu &b)
{
    return v4uu(VM_ANDNOT(a.vector, b.vector));
}

// rval = a ? b : c;
static SYS_FORCE_INLINE v4uf
ternary(const v4uu &a, const v4uf &b, const v4uf &c)
{
    return (b & a) | andn(a, c);
}

static SYS_FORCE_INLINE v4uu
ternary(const v4uu &a, const v4uu &b, const v4uu &c)
{
    return (b & a) | andn(a, c);
}

// rval = !(a && b)
static SYS_FORCE_INLINE v4uu
nand(const v4uu &a, const v4uu &b)
{
    return !v4uu(VM_AND(a.vector, b.vector));
}

static SYS_FORCE_INLINE v4uf
vmin(const v4uf &a, const v4uf &b)
{
    return v4uf(VM_MIN(a.vector, b.vector));
}

static SYS_FORCE_INLINE v4uf
vmax(const v4uf &a, const v4uf &b)
{
    return v4uf(VM_MAX(a.vector, b.vector));
}

static SYS_FORCE_INLINE v4uf
clamp(const v4uf &a, const v4uf &b, const v4uf &c)
{
    return vmax(vmin(a, c), b);
}

static SYS_FORCE_INLINE v4uf
clamp(const v4uf &a, float b, float c)
{
    return vmax(vmin(a, v4uf(c)), v4uf(b));
}

// Returns an integer with the lower 4 bits set
// if the corresponding sign bit in a is set.
// Useful after comparisons as comparisons will
// be -1 or 0; so no need for an allbits test.
// 1 is a[0], 2 is a[1], and 8 is a[3].
static SYS_FORCE_INLINE int
signbits(const v4uu &a)
{
    return vm_signbits(a.vector);
}

static SYS_FORCE_INLINE int
signbits(const v4uf &a)
{
    return vm_signbits(a.vector);
}

static SYS_FORCE_INLINE bool
allbits(const v4uu &a)
{
    return vm_allbits(a.vector);
}

static SYS_FORCE_INLINE bool
anybits(const v4uu &a)
{
    return !allbits(~a);
}

static SYS_FORCE_INLINE v4uf
madd(const v4uf &v, const v4uf &f, const v4uf &a)
{
    return v4uf(VM_MADD(v.vector, f.vector, a.vector));
}

static SYS_FORCE_INLINE v4uf
madd(const v4uf &v, float f, float a)
{
    return v4uf(VM_MADD(v.vector, v4uf(f).vector, v4uf(a).vector));
}

static SYS_FORCE_INLINE v4uf
madd(const v4uf &v, float f, const v4uf &a)
{
    return v4uf(VM_MADD(v.vector, v4uf(f).vector, a.vector));
}

static SYS_FORCE_INLINE v4uf
msub(const v4uf &v, const v4uf &f, const v4uf &s)
{
    return madd(v, f, -s);
}

static SYS_FORCE_INLINE v4uf
msub(const v4uf &v, float f, float s)
{
    return madd(v, f, -s);
}

static SYS_FORCE_INLINE v4uf
lerp(const v4uf &a, const v4uf &b, const v4uf &w)
{
    v4uf w1 = v4uf(1.0F) - w;
    return madd(a, w1, b*w);
}

static SYS_FORCE_INLINE v4uf
luminance(const v4uf &r, const v4uf &g, const v4uf &b,
	  float rw, float gw, float bw)
{
    return v4uf(madd(r, v4uf(rw), madd(g, v4uf(gw), b * bw)));
}

static SYS_FORCE_INLINE float
dot3(const v4uf &a, const v4uf &b)
{
    v4uf res = a*b;
    return res[0] + res[1] + res[2];
}

static SYS_FORCE_INLINE float
dot4(const v4uf &a, const v4uf &b)
{
    v4uf res = a*b;
    return res[0] + res[1] + res[2] + res[3];
}

static SYS_FORCE_INLINE float
length(const v4uf &a)
{
    return SYSsqrt(dot3(a, a));
}

static SYS_FORCE_INLINE v4uf
normalize(const v4uf &a)
{
    return a / length(a);
}

static SYS_FORCE_INLINE v4uf
cross(const v4uf &a, const v4uf &b)
{
    return v4uf(a[1]*b[2] - a[2]*b[1],
		a[2]*b[0] - a[0]*b[2],
		a[0]*b[1] - a[1]*b[0], 0);
}

// Convert 4 fpreal32 to 4 fpreal16's, returned as an int64
#if defined(AMD64) && (defined(__SSE4_1__) || defined(_MSC_VER))
#include <smmintrin.h> // SSE 4.1
static SYS_FORCE_INLINE int64
VMconvert4F32ToF16(v4si input)
{
    // Separate sign bit and the rest
    __m128i sign_mask = _mm_set1_epi32(0x80000000);
    __m128i sign = _mm_and_si128(sign_mask, input);
    __m128i positive = _mm_andnot_si128(sign_mask, input); // order matters here

    // Move the sign bit to its new location.
    sign = _mm_srli_epi32(sign,16);

    // Clamp the value to avoid problems below.
    __m128i min_value = _mm_set1_epi32((127-15)<<23);
    __m128i max_value = _mm_set1_epi32((127+16)<<23);
    positive = _mm_max_epi32(positive, min_value); // SSE4.1
    positive = _mm_min_epi32(positive, max_value); // SSE4.1

    // Isolate the bit that's going to determine whether we round up or down.
    __m128i bit0_mask = _mm_set1_epi32(1);
    __m128i round_bit = _mm_srli_epi32(positive, (23-10-1));
    round_bit = _mm_and_si128(round_bit, bit0_mask);

    // Isolate the exponent, change it from excess-127 to excess-15,
    // then shift it into its new location.
    __m128i exponent_diff = _mm_set1_epi32(127-15);
    __m128i exponent = _mm_srli_epi32(positive, 23);
    exponent = _mm_sub_epi32(exponent, exponent_diff);
    exponent = _mm_slli_epi32(exponent, 10);

    // Isolate the mantissa bits that we'll be keeping.
    __m128i ten_bits_mask = _mm_set1_epi32(0x3FF);
    positive = _mm_srli_epi32(positive, 23-10);
    positive = _mm_and_si128(positive, ten_bits_mask);

    // Combine the exponent, mantissa, and sign
    __m128i f16s = _mm_or_si128(exponent, positive);
    f16s = _mm_or_si128(f16s, sign);

    // Add the rounding bit
    f16s = _mm_add_epi32(f16s, round_bit);

    // Pack the 4 low 16 bits for each 32-bit integer into
    // first 4 16-bit integers, (and last 4 16-bit integers,
    // but we don't care about those.)
    f16s = _mm_packus_epi32(f16s,f16s); // SSE4.1

    // Just extract out the first 4 16-bit integers.
    return _mm_cvtsi128_si64(f16s);
}
#else
static SYS_FORCE_INLINE int64
VMconvert4F32ToF16(v4si input)
{
    int64		result;
    const fpreal32 *	src = reinterpret_cast<const fpreal32 *>(&input);
    fpreal16 *		dst = reinterpret_cast<fpreal16 *>(&result);

    for (int i = 0; i < 4; ++i)
	dst[i] = fpreal16(src[i]);

    return result;
}
#endif
static SYS_FORCE_INLINE int64
VMconvert4F32ToF16(v4sf input)
{
    return VMconvert4F32ToF16(V4SI(input));
}

// SYS version of everything

static SYS_FORCE_INLINE v4uu
SYSmin(const v4uu &a, const v4uu &b)
{
    return ternary(a < b, a, b);
}
static SYS_FORCE_INLINE v4uu
SYSmax(const v4uu &a, const v4uu &b)
{
    return ternary(a > b, a, b);
}
static SYS_FORCE_INLINE v4uu
SYSclamp(const v4uu &a, const v4uu &b, const v4uu &c)
{
    return SYSmax(SYSmin(a, c), b);
}
static SYS_FORCE_INLINE v4uu
SYSclamp(const v4uu &a, int b, int c)
{
    return SYSmax(SYSmin(a, v4uu(c)), v4uu(b));
}

static SYS_FORCE_INLINE v4uf
SYSmin(const v4uf &a, const v4uf &b)
{
    return v4uf(VM_MIN(a.vector, b.vector));
}

static SYS_FORCE_INLINE v4uf
SYSmax(const v4uf &a, const v4uf &b)
{
    return v4uf(VM_MAX(a.vector, b.vector));
}

static SYS_FORCE_INLINE v4uf
SYSlerp(const v4uf &a, const v4uf &b, const v4uf &t)
{
    v4uf diff = b-a;
    return madd(diff, t, a);
}

static SYS_FORCE_INLINE v4uf
SYSlerp(const v4uf &a, const v4uf &b, const float t)
{
    v4uf diff = b-a;
    return madd(diff, t, a);
}
static SYS_FORCE_INLINE v4uf
SYSclamp(const v4uf &a, const v4uf &b, const v4uf &c)
{
    return vmax(vmin(a, c), b);
}

static SYS_FORCE_INLINE v4uf
SYSclamp(const v4uf &a, float b, float c)
{
    return vmax(vmin(a, v4uf(c)), v4uf(b));
}

static SYS_FORCE_INLINE v4uu
SYSfastFloor(const v4uf &a)
{
    // The following are two versions of floor.
    //
    // 1.
    // This floor is a bit problematic around 0. For example if x is a
    // very small (in magnitude) denormal number, then floor(x) = -1
    // and x - floor(x) = 1, which could be unexpected.
    return ternary(a < v4uf(0.f), a.toSignedInt() - 1, a.toSignedInt());
    //
    // 2.
    //
    // This version is broken on some platforms. On thoth,
    // SYSfastFloor(v4uf(-0.716626)) == v4ui(0)
    //
    //return a.floor();

}
static SYS_FORCE_INLINE v4uu
SYSfloor(const v4uf &a)
{
    return a.floor();
}

// Currently there is no specific support for signed integers
typedef v4uu v4ui;

// Assuming that ptr is an array of elements of type STYPE, this operation
// will return the index of the first element that is aligned to (1<<ASIZE)
// bytes.
#define VM_ALIGN(ptr, ASIZE, STYPE)	\
		((((1<<ASIZE)-(intptr_t)ptr)&((1<<ASIZE)-1))/sizeof(STYPE))

#endif
