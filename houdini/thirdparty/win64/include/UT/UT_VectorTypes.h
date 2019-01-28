/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_VectorTypes.h (Utility library, C++)
 *
 * COMMENTS:
 *		This class prototypes our vector types, such
 *		as UT_QuaternionR, UT_Vector3R, etc.
 */

#ifndef __UT_VectorTypes__
#define __UT_VectorTypes__

#include "UT_API.h"

#include <SYS/SYS_Types.h>
#include <SYS/SYS_TypeDecorate.h>

// Define this so that H11 users can use this to determine if they have
// UT_Vector3R, UT_Matrix3R, etc.
#define UT_VECTORTYPES_HAVE_FPREAL 1

template <typename T> class UT_API UT_Vector2T;
typedef UT_Vector2T<fpreal>	    UT_Vector2R;
typedef UT_Vector2T<fpreal64>	    UT_Vector2D;
typedef UT_Vector2T<fpreal32>	    UT_Vector2F;
typedef UT_Vector2T<fpreal16>	    UT_Vector2H;
typedef UT_Vector2T<float>	    UT_Vector2;		// deprecated
typedef UT_Vector2T<int32>	    UT_Vector2i;
typedef UT_Vector2T<int64>	    UT_Vector2I;

template <typename T> class UT_API UT_Vector3T;
typedef UT_Vector3T<fpreal>	    UT_Vector3R;
typedef UT_Vector3T<fpreal64>	    UT_Vector3D;
typedef UT_Vector3T<fpreal32>	    UT_Vector3F;
typedef UT_Vector3T<fpreal16>	    UT_Vector3H;
typedef UT_Vector3T<float>	    UT_Vector3;		// deprecated
typedef UT_Vector3T<int32>	    UT_Vector3i;
typedef UT_Vector3T<int64>	    UT_Vector3I;

template <typename T> class UT_API UT_Vector4T;
typedef UT_Vector4T<fpreal>	    UT_Vector4R;
typedef UT_Vector4T<fpreal64>	    UT_Vector4D;
typedef UT_Vector4T<fpreal32>	    UT_Vector4F;
typedef UT_Vector4T<fpreal16>	    UT_Vector4H;
typedef UT_Vector4T<float>	    UT_Vector4;		// deprecated
typedef UT_Vector4T<int32>	    UT_Vector4i;
typedef UT_Vector4T<int64>	    UT_Vector4I;

class UT_XformOrder;

template <typename T> class UT_API UT_QuaternionT;
typedef UT_QuaternionT<fpreal>	    UT_QuaternionR;
typedef UT_QuaternionT<fpreal16>    UT_QuaternionH;
typedef UT_QuaternionT<fpreal32>    UT_QuaternionF;
typedef UT_QuaternionT<fpreal64>    UT_QuaternionD;
typedef UT_QuaternionT<float>	    UT_Quaternion;	// deprecated

template <typename T> class UT_API UT_Matrix2T;
typedef UT_Matrix2T<fpreal>	    UT_Matrix2R;
typedef UT_Matrix2T<fpreal32>	    UT_Matrix2F;
typedef UT_Matrix2T<fpreal64>	    UT_Matrix2D;
typedef UT_Matrix2T<float>	    UT_Matrix2;		// deprecated
typedef UT_Matrix2T<double>	    UT_DMatrix2;	// deprecated

template <typename T> class UT_API UT_Matrix3T;
typedef UT_Matrix3T<fpreal>	    UT_Matrix3R;
typedef UT_Matrix3T<fpreal32>	    UT_Matrix3F;
typedef UT_Matrix3T<fpreal64>	    UT_Matrix3D;
typedef UT_Matrix3T<float>	    UT_Matrix3;		// deprecated
typedef UT_Matrix3T<double>	    UT_DMatrix3;	// deprecated

template <typename T> class UT_API UT_Matrix4T;
typedef UT_Matrix4T<fpreal>	    UT_Matrix4R;
typedef UT_Matrix4T<fpreal32>	    UT_Matrix4F;
typedef UT_Matrix4T<fpreal64>	    UT_Matrix4D;
typedef UT_Matrix4T<float>	    UT_Matrix4;		// deprecated
typedef UT_Matrix4T<double>	    UT_DMatrix4;	// deprecated


//
// Declare the following types as POD so that UT_ValArray can be optimized
//
SYS_DECLARE_IS_POD(UT_Vector2F)
SYS_DECLARE_IS_POD(UT_Vector3F)
SYS_DECLARE_IS_POD(UT_Vector4F)
SYS_DECLARE_IS_POD(UT_Vector2D)
SYS_DECLARE_IS_POD(UT_Vector3D)
SYS_DECLARE_IS_POD(UT_Vector4D)
SYS_DECLARE_IS_POD(UT_Vector2I)
SYS_DECLARE_IS_POD(UT_Vector3I)
SYS_DECLARE_IS_POD(UT_Vector4I)
SYS_DECLARE_IS_POD(UT_Vector2i)
SYS_DECLARE_IS_POD(UT_Vector3i)
SYS_DECLARE_IS_POD(UT_Vector4i)

SYS_DECLARE_IS_POD(UT_QuaternionH)
SYS_DECLARE_IS_POD(UT_QuaternionF)
SYS_DECLARE_IS_POD(UT_QuaternionD)
SYS_DECLARE_IS_POD(UT_Matrix2F)
SYS_DECLARE_IS_POD(UT_Matrix2D)
SYS_DECLARE_IS_POD(UT_Matrix3F)
SYS_DECLARE_IS_POD(UT_Matrix3D)
SYS_DECLARE_IS_POD(UT_Matrix4F)
SYS_DECLARE_IS_POD(UT_Matrix4D)

//
// Declare common array types
//
template <typename T> class UT_API_TMPL UT_ValArray;

typedef UT_ValArray<fpreal>	    UT_FprealArray;
typedef UT_ValArray<fpreal32>	    UT_Fpreal32Array;
typedef UT_ValArray<fpreal64>	    UT_Fpreal64Array;
typedef UT_ValArray<float>	    UT_FloatArray;
typedef UT_ValArray<double>	    UT_DoubleArray;
typedef UT_ValArray<int>	    UT_IntArray;
typedef UT_ValArray<exint>	    UT_ExintArray;
typedef UT_ValArray<int16>	    UT_Int16Array;
typedef UT_ValArray<int32>	    UT_Int32Array;
typedef UT_ValArray<int64>	    UT_Int64Array;

typedef UT_ValArray<UT_Vector2R>    UT_Vector2RArray;
typedef UT_ValArray<UT_Vector3R>    UT_Vector3RArray;
typedef UT_ValArray<UT_Vector4R>    UT_Vector4RArray;
typedef UT_ValArray<UT_Vector2F>    UT_Vector2FArray;
typedef UT_ValArray<UT_Vector3F>    UT_Vector3FArray;
typedef UT_ValArray<UT_Vector4F>    UT_Vector4FArray;
typedef UT_ValArray<UT_Vector2D>    UT_Vector2DArray;
typedef UT_ValArray<UT_Vector3D>    UT_Vector3DArray;
typedef UT_ValArray<UT_Vector4D>    UT_Vector4DArray;
typedef UT_ValArray<UT_Vector2H>    UT_Vector2HArray;
typedef UT_ValArray<UT_Vector3H>    UT_Vector3HArray;
typedef UT_ValArray<UT_Vector4H>    UT_Vector4HArray;

typedef UT_ValArray<UT_Vector2I>    UT_Vector2IArray;
typedef UT_ValArray<UT_Vector3I>    UT_Vector3IArray;
typedef UT_ValArray<UT_Vector4I>    UT_Vector4IArray;
typedef UT_ValArray<UT_Vector2i>    UT_Vector2iArray;
typedef UT_ValArray<UT_Vector3i>    UT_Vector3iArray;
typedef UT_ValArray<UT_Vector4i>    UT_Vector4iArray;

typedef UT_ValArray<UT_Matrix2F>    UT_Matrix2FArray;
typedef UT_ValArray<UT_Matrix3F>    UT_Matrix3FArray;
typedef UT_ValArray<UT_Matrix4F>    UT_Matrix4FArray;
typedef UT_ValArray<UT_Matrix2D>    UT_Matrix2DArray;
typedef UT_ValArray<UT_Matrix3D>    UT_Matrix3DArray;
typedef UT_ValArray<UT_Matrix4D>    UT_Matrix4DArray;

typedef UT_ValArray<UT_QuaternionR> UT_QuaternionRArray;
typedef UT_ValArray<UT_QuaternionF> UT_QuaternionFArray;
typedef UT_ValArray<UT_QuaternionD> UT_QuaternionDArray;

typedef UT_ValArray<UT_Vector2>	    UT_Vector2Array;	// deprecated
typedef UT_ValArray<UT_Vector3>	    UT_Vector3Array;	// deprecated
typedef UT_ValArray<UT_Vector4>	    UT_Vector4Array;	// deprecated


//
// Type Conversion
//
template< typename FT, template<typename> class V >
inline const V<fpreal>
UTconvertToR(
    const V<FT>& v
)
{
    return V<fpreal>(v);
}
template< typename FT, template<typename> class V >
inline const V<double>
UTconvertToD(
    const V<FT>& v
)
{
    return V<double>(v);
}
template< typename FT, template<typename> class V >
inline const V<float>
UTconvertToF(
    const V<FT>& v
)
{
    return V<float>(v);
}
#if (SYS_SIZEOF_FPREAL == 4)
    #define UT_R_FROM_D(VAL)	    UTconvertToR(VAL)
    #define UT_R_FROM_F(VAL)	    (VAL)
    #define UT_D_FROM_R(VAL)	    UTconvertToD(VAL)
    #define UT_F_FROM_R(VAL)	    (VAL)
#elif (SYS_SIZEOF_FPREAL == 8)
    #define UT_R_FROM_F(VAL)	    UTconvertToR(VAL)
    #define UT_R_FROM_D(VAL)	    (VAL)
    #define UT_F_FROM_R(VAL)	    UTconvertToF(VAL)
    #define UT_D_FROM_R(VAL)	    (VAL)
#else
    #error Missing FPREAL type
#endif


template<typename T>
static inline const char *UT_TypeShortName();

template<>
inline const char *UT_TypeShortName<fpreal16>() { return "H"; }

template<>
inline const char *UT_TypeShortName<fpreal32>() { return "F"; }

template<>
inline const char *UT_TypeShortName<fpreal64>() { return "D"; }

template<>
inline const char *UT_TypeShortName<int32>() { return "i"; }

template<>
inline const char *UT_TypeShortName<int64>() { return "I"; }


// Type promotion
// This is used to define promoted types for specific use cases.
// E.g. We define PreciseType as the promoted version of a type to use in
// calculations where we want extra precision (e.g. norm calculations)

template<typename T>
struct UT_API UT_TypePromoteT
{
    // Default precise promotion is T
    typedef T PreciseType;
};

template<>
struct UT_API UT_TypePromoteT<fpreal32>
{
    // Use double precision for precise calculations
    typedef fpreal64 PreciseType;
};

// Templated typedefs for convenience

// e.g. UT_PreciseT<fpreal32> is type fpreal64
template <typename T>
using UT_PreciseT = typename UT_TypePromoteT<T>::PreciseType;

#endif
