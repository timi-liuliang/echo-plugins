/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VEX_PodTypes.h (VEX Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VEX_PodTypes__
#define __VEX_PodTypes__

#include <VM/VM_Math.h>
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>
#include <UT/UT_Matrix2.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_Assert.h>
#include <algorithm>

enum class VEX_Precision
{
    VEX_32,			// 32 bit precision
    VEX_64,			// 64 bit precision
};
constexpr VEX_Precision VEX_32 = VEX_Precision::VEX_32;
constexpr VEX_Precision VEX_64 = VEX_Precision::VEX_64;

template <VEX_Precision PREC> class VEX_PrecisionResolver;

template <>
class VEX_PrecisionResolver<VEX_32>
{
public:
    using int_type = int32;
    using float_type = fpreal32;
    using vec2_type = UT_Vector2F;
    using vec3_type = UT_Vector3F;
    using vec4_type = UT_Vector4F;
    using mat2_type = UT_Matrix2F;
    using mat3_type = UT_Matrix3F;
    using mat4_type = UT_Matrix4F;
    using quat_type = UT_QuaternionF;
    static constexpr int bitsize = 32;
};

template <>
class VEX_PrecisionResolver<VEX_64>
{
public:
    using int_type = int64;
    using float_type = fpreal64;
    using vec2_type = UT_Vector2D;
    using vec3_type = UT_Vector3D;
    using vec4_type = UT_Vector4D;
    using mat2_type = UT_Matrix2D;
    using mat3_type = UT_Matrix3D;
    using mat4_type = UT_Matrix4D;
    using quat_type = UT_QuaternionD;
    static constexpr int bitsize = 64;
};

template <VEX_Precision P> using VEXfloat = typename VEX_PrecisionResolver<P>::float_type;
template <VEX_Precision P> using VEXint = typename VEX_PrecisionResolver<P>::int_type;
template <VEX_Precision P> using VEXvec2 = typename VEX_PrecisionResolver<P>::vec2_type;
template <VEX_Precision P> using VEXvec3 = typename VEX_PrecisionResolver<P>::vec3_type;
template <VEX_Precision P> using VEXvec4 = typename VEX_PrecisionResolver<P>::vec4_type;
template <VEX_Precision P> using VEXmat2 = typename VEX_PrecisionResolver<P>::mat2_type;
template <VEX_Precision P> using VEXmat3 = typename VEX_PrecisionResolver<P>::mat3_type;
template <VEX_Precision P> using VEXmat4 = typename VEX_PrecisionResolver<P>::mat4_type;
template <VEX_Precision P> using VEXquaternion = typename VEX_PrecisionResolver<P>::quat_type;

template <typename T>
static inline void
VEXpodFill(T *a, T *b, const T &v)
    { std::fill(a, b, v); }

/// Filling with a disabled predicate
template <typename T>
static inline void
VEXpodFill(T *a, const T *b, const T &val, const uint32 *disabled)
{ 
    for (; a != b; ++a, ++disabled)
	*a = *disabled ? *a : val;
}

template <> inline void
VEXpodFill<fpreal32>(fpreal32 *a, fpreal32 *b, const fpreal32 &v)
{
    VM_Math::set(a, v, b-a);
}

template <> inline void
VEXpodFill(fpreal32 *a, const fpreal32 *b,
	const fpreal32 &val, const uint32 *disabled)
{
    VM_Math::set(a, val, b-a, disabled);
}

template <typename T>
static inline T *
VEXpodCopy(const T *a, const T *b, T *dest)
{
    UT_ASSERT(dest <= a || dest >= b);
    return std::copy(a, b, dest);
}

/// Copying with a disabled predicate
template <typename T>
static inline T *
VEXpodCopy(const T *a, const T *b, T *dest, const uint32 *disabled)
{ 
    UT_ASSERT(dest <= a || dest >= b);
    for (; a != b; ++a, ++dest, ++disabled)
	*dest = *disabled ? *dest : *a;
    return dest;
}

template <>
inline fpreal32 *
VEXpodCopy<fpreal32>(const fpreal32 *a, const fpreal32 *b, fpreal32 *dest,
	const uint32 *disabled)
{
    UT_ASSERT(dest <= a || dest >= b);
    VM_Math::set(dest, a, b-a, disabled);
    return dest + (b-a);
}

template <>
inline int32 *
VEXpodCopy<int32>(const int32 *a, const int32 *b, int32 *dest,
	const uint32 *disabled)
{
    UT_ASSERT(dest <= a || dest >= b);
    VM_Math::set(dest, a, b-a, disabled);
    return dest + (b-a);
}

namespace VEX {
    inline namespace Literal {
	// 32 bit constants:  32_vi or 3.1415_vf
	SYS_FORCE_INLINE
	VEXint<VEX_32> operator "" _vi(unsigned long long int v)
	    { return v; }
	SYS_FORCE_INLINE
	VEXfloat<VEX_32> operator "" _vf(long double v)
	    { return v; }

	// 64 bit constants:  32_vl or 3.1415_vd
	SYS_FORCE_INLINE
	VEXint<VEX_64> operator "" _vl(unsigned long long int v)
	    { return v; }
	SYS_FORCE_INLINE
	VEXfloat<VEX_64> operator "" _vd(long double v)
	    { return v; }
    }
}

#endif
