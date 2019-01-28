/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_Types.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_Types__
#define __GT_Types__

#include "GT_API.h"
#include <SYS/SYS_Types.h>

enum GT_Storage
{
    GT_STORE_INVALID=-1,
    GT_STORE_UINT8,
    GT_STORE_INT8,
    GT_STORE_INT16,
    GT_STORE_INT32,
    GT_STORE_INT64,
    GT_STORE_REAL16,	GT_STORE_FPREAL16=GT_STORE_REAL16,
    GT_STORE_REAL32,	GT_STORE_FPREAL32=GT_STORE_REAL32,
    GT_STORE_REAL64,	GT_STORE_FPREAL64=GT_STORE_REAL64,
    GT_STORE_STRING,

    GT_NUM_STORAGE_TYPES
};

enum GT_Type
{
    GT_TYPE_NONE = -1,	// Implicit type based on data
    GT_TYPE_POINT,	// Position
    GT_TYPE_HPOINT,	// Homogeneous position
    GT_TYPE_VECTOR,	// Direction vector
    GT_TYPE_NORMAL,	// Normal
    GT_TYPE_COLOR,	// Color
    GT_TYPE_QUATERNION,	// Quaternion
    GT_TYPE_MATRIX3,	// 3x3 Matrix
    GT_TYPE_MATRIX,	// Matrix
    GT_TYPE_ST,		// Parametric interval
    GT_TYPE_HIDDEN,	// "Private" (hidden)
    GT_TYPE_BOX2,       // 2x2 Bounding box
    GT_TYPE_BOX,        // 3x3 Bounding box
    GT_TYPE_TEXTURE,    // Texture coordinate
    GT_TYPE_INDEXPAIR,	// Capture index/weight pair
};

enum GT_Basis
{
    GT_BASIS_INVALID = -1,

    GT_BASIS_LINEAR,
    GT_BASIS_BEZIER,
    GT_BASIS_BSPLINE,
    GT_BASIS_CATMULLROM,
    GT_BASIS_CATMULL_ROM = GT_BASIS_CATMULLROM,
    GT_BASIS_HERMITE,
    GT_BASIS_POWER,

    GT_NUM_BASIS_TYPES
};

/// Subdivision schemes
enum GT_Scheme
{
    GT_CATMULL_CLARK,
    GT_LOOP,
    GT_BILINEAR
};

enum GT_Owner
{
    GT_OWNER_INVALID = -1,

    GT_OWNER_VERTEX,
    GT_OWNER_POINT,
    GT_OWNER_UNIFORM,
    GT_OWNER_DETAIL,

    GT_OWNER_SHARED	= GT_OWNER_POINT,
    GT_OWNER_PRIMITIVE	= GT_OWNER_UNIFORM,
    GT_OWNER_CONSTANT	= GT_OWNER_DETAIL,

    GT_OWNER_MAX
};

enum GT_IndexingMode
{
    GT_INDEXING_SMALL,	// Use small data types for indexing (i.e. int32)
    GT_INDEXING_LARGE,	// Use large data types for indexing (i.e. int64)
    GT_INDEXING_QUICK,	// Perform a quick check to see which indexing to use
    GT_INDEXING_CHECK,	// Check, perhaps using a more expensive test
};

enum GT_CacheEventType
{
    GT_CACHE_DETAIL_DELETE,	// Delete all references to detail
    GT_CACHE_ATTRIBUTE_CHANGE,	// Attribute value change
    GT_CACHE_ATTRIBUTE_CREATE,	// Attribute created
    GT_CACHE_ATTRIBUTE_DELETE,	// Attribute deleted
    GT_CACHE_INDEXMAP_CHANGE,	// Index map change (grow/shrink)
    GT_CACHE_GROUP_CHANGE,	// Group was changed
    GT_CACHE_GROUP_CREATE,	// New group added
    GT_CACHE_GROUP_DELETE,	// Group deleted
};

typedef const char	*GT_String;
typedef int64		 GT_Size;
typedef int64		 GT_Offset;

template <typename T> constexpr GT_Storage
GTstorage() { return GT_STORE_INVALID; }

template <> constexpr GT_Storage GTstorage<uint8>() { return GT_STORE_UINT8; }
template <> constexpr GT_Storage GTstorage<int8>() { return GT_STORE_INT8; }
template <> constexpr GT_Storage GTstorage<int16>() { return GT_STORE_INT16; }
template <> constexpr GT_Storage GTstorage<int32>() { return GT_STORE_INT32; }
template <> constexpr GT_Storage GTstorage<int64>() { return GT_STORE_INT64; }
template <> constexpr GT_Storage GTstorage<fpreal16>() { return GT_STORE_REAL16; }
template <> constexpr GT_Storage GTstorage<fpreal32>() { return GT_STORE_REAL32; }
template <> constexpr GT_Storage GTstorage<fpreal64>() { return GT_STORE_REAL64; }
template <> constexpr GT_Storage GTstorage<GT_String>() { return GT_STORE_STRING; }

/// @{
/// Perform mapping between enum and string name
GT_API extern int		 GTsizeof(GT_Storage storage);
GT_API extern const char	*GTstorage(GT_Storage storage);
GT_API extern GT_Storage	 GTstorage(const char *storage);
GT_API extern const char	*GTtype(GT_Type type);
GT_API extern GT_Type		 GTtype(const char *type);
GT_API extern const char	*GTbasis(GT_Basis basis);
GT_API extern GT_Basis		 GTbasis(const char *basis);
GT_API extern const char	*GTscheme(GT_Scheme scheme);
GT_API extern GT_Scheme		 GTscheme(const char *scheme,
					GT_Scheme def=GT_CATMULL_CLARK);
GT_API extern const char	*GTowner(GT_Owner owner);
GT_API extern GT_Owner		 GTowner(const char *owner);
/// @}

/// Compute the number of spans for a given basis and cv count.  For basis
/// types other than linear, the order is assumed to be 4.
GT_API extern GT_Size		 GTbasisSpans(GT_Basis basis,
					GT_Size count, bool wrapped,
					int order = 4);
GT_API extern GT_Size		 GTbasisStep(GT_Basis basis, int order = 4);

static inline constexpr bool GTisInteger(GT_Storage s)
			{
			    return s == GT_STORE_INT32 ||
				s == GT_STORE_INT64 ||
				s == GT_STORE_INT16 ||
				s == GT_STORE_INT8 ||
				s == GT_STORE_UINT8;
			}
static inline constexpr bool GTisFloat(GT_Storage s)
			{
			    return s == GT_STORE_REAL16 ||
				s == GT_STORE_REAL32 ||
				s == GT_STORE_REAL64;
			}
static inline constexpr bool GTisString(GT_Storage s)
			{ return s == GT_STORE_STRING; }

template <typename T> static inline int64
GThandleMemoryUsage(const T &handle)
{
    return handle ? handle->getMemoryUsage() : 0;
}

#endif
