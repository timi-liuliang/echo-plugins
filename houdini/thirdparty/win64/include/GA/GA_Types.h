/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Types.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __GA_Types__
#define __GA_Types__

#include "GA_API.h"
#include <UT/UT_Assert.h>
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>


// -------------------------------------------------------------
// GA Enums/Defines
// -------------------------------------------------------------
/// The GA_AttributeOwner enum is used to differentiate between the different
/// types of attribute arrays stored in a GA_Detail.  The different classes
/// represent
///   -# unique point attribute data (one per vertex on a primitive)
///   -# shared point attribute data (attributes shared multiple vertices)
///   -# per-primitive attribute data (one per primitive)
///   -# attribute data stored on the detail itself (single value)
enum GA_AttributeOwner
{
    GA_ATTRIB_VERTEX,		// Unique vertex data
    GA_ATTRIB_POINT,		// Shared vertex data
    GA_ATTRIB_PRIMITIVE,	// Primitive attribute data
    GA_ATTRIB_GLOBAL,	        // Global data

    GA_ATTRIB_OWNER_N,		// Number of GA_AttributeOwner enums

    GA_ATTRIB_DETAIL = GA_ATTRIB_GLOBAL,// Detail data, same as global
    GA_ATTRIB_INVALID = -1      // Not a valid attribute owner
};

/// Attributes (and other data) may be stored with different precision and
/// storage types.
enum GA_Storage
{
    GA_STORE_INVALID=-1,	// Invalid storage
    GA_STORE_BOOL,		// Bool storage (bit-array)
    GA_STORE_UINT8,		// 8-bit unsigned integer
    GA_STORE_INT8,		// 8-bit signed integer
    GA_STORE_INT16,		// 16-bit integer
    GA_STORE_INT32,		// 32-bit integer
    GA_STORE_INT64,		// 64-bit integer
    GA_STORE_REAL16,		// 16-bit real
    GA_STORE_REAL32,		// 32-bit real
    GA_STORE_REAL64,		// 64-bit real
    GA_STORE_STRING,		// ASCII string data
    // GA_STORE_UTF8,		// Future?
    // GA_STORE_UTF16,		// Future?
    // GA_STORE_UTF32,		// Future?
};

/// Often one does not care about the precision.  In this case
/// the storage class gives a good idea which AIFs will be supported.
enum GA_StorageClass
{
    GA_STORECLASS_INVALID = -1,		// Invalid storage
    GA_STORECLASS_INT,			// Any integer type
    GA_STORECLASS_REAL,			// Any real type
    GA_STORECLASS_FLOAT = GA_STORECLASS_REAL,
    GA_STORECLASS_STRING,
    GA_STORECLASS_OTHER
};

/// Type qualifiers on attributes.  These qualifiers can help to interpret the
/// data associated with an attribute, but are not required.
enum GA_TypeInfo
{
    /// Data has no numeric representation
    GA_TYPE_VOID = -1,
    /// Data represents a position in space.  Token "point"
    GA_TYPE_POINT,
    /// Data represents a position and homogeneous coordinate.  The position is
    /// stored in non-homogeneous space (i.e. the w coordinate is not
    /// multiplied through).  Token "hpoint"
    GA_TYPE_HPOINT,
    /// Data represents a direction vector.  Token "vector"
    GA_TYPE_VECTOR,
    /// Data represents a normal vector.  Token "normal"
    GA_TYPE_NORMAL,
    /// Data represents a color.  Token "color"
    GA_TYPE_COLOR,
    /// Data represents a transform matrix.  Token "matrix"
    GA_TYPE_TRANSFORM,
    /// Data represents a quaternion.  Token "quaternion"
    GA_TYPE_QUATERNION,
    /// Data represents an index-pair.  Token "indexpair"
    GA_TYPE_INDEXPAIR,

    /// When a numeric attribute is created with integer storage, the attribute
    /// will be tagged as a non-arithmetic integer.  This means that
    /// mathematical operations will not typically be performed (i.e. integer
    /// values will not be averaged).  This works well for most cases of
    /// integers under Houdini.
    GA_TYPE_NONARITHMETIC_INTEGER,
    /// Arithmetic integers will be modified under arithmetic operations.
    /// Their values will be averaged etc.
    /// @note This is not the default behaviour for numeric attributes
    GA_TYPE_ARITHMETIC_INTEGER,

    /// Data represents a coordinate in texture space, a.k.a. uv/uvw,
    /// but not a primitive uv/uvw.
    GA_TYPE_TEXTURE_COORD
};

/// Scope qualifiers on attributes.  Each valid scope has its own namespace,
/// along with other consequences.
enum GA_AttributeScope
{
    /// Data has no numeric representation
    GA_SCOPE_INVALID = -1,
    /// Standard user attribute level
    //    GA_OPTION_EXPORT_ON_SAVE/MERGE defaults to true
    GA_SCOPE_PUBLIC,
    // Internal attribute level
    //    GA_OPTION_EXPORT_ON_SAVE/MERGE defaults to false
    GA_SCOPE_PRIVATE,
    // Group attribute level
    //    GA_OPTION_EXPORT_ON_SAVE/MERGE defaults to false,
    //    because GA_Group saves/merges it, not GA_Attribute.
    GA_SCOPE_GROUP
};

/// An ordinal enum for the different types of groups in GA
enum GA_GroupType
{
    GA_GROUP_INVALID	= -1,
    GA_GROUP_POINT	= 0,
    GA_GROUP_PRIMITIVE	= 1,
    GA_GROUP_EDGE	= 2,
    GA_GROUP_BREAKPOINT	= 3,
    GA_GROUP_VERTEX	= 4,
    GA_GROUP_N
};

/// Currently SOP_Node::parseAllGroups takes a mask of different types of
/// groups.  This enum allows group types to be packed into a bit field.
enum GA_GroupMaskType
{
    GA_GMASK_NONE	= 0,
    GA_GMASK_POINT	= 1 << GA_GROUP_POINT,
    GA_GMASK_PRIMITIVE	= 1 << GA_GROUP_PRIMITIVE,
    GA_GMASK_EDGE	= 1 << GA_GROUP_EDGE,
    GA_GMASK_BREAKPOINT	= 1 << GA_GROUP_BREAKPOINT,
    GA_GMASK_VERTEX	= 1 << GA_GROUP_VERTEX,
    GA_GMASK_FULL_MASK	= ((1 << GA_GROUP_N)-1)
};

// Maximum order for NURBs/Bezier bases
#define GA_MAX_ORDER	11

// Types of knot spacing for NURBs operations
enum GA_KnotSpaceType
{
    GA_KNOT_SPACE_UNIFORM,
    GA_KNOT_SPACE_AVERAGING
};

// Basis parametrization types
enum GA_ParameterizationType
{
    GA_PARAMETERIZATION_UNIFORM,
    GA_PARAMETERIZATION_CHORD,
    GA_PARAMETERIZATION_CENTRIPETAL,
    GA_PARAMETERIZATION_APPROXARC
};

/// Strategies that may be used for attribute data IDs, usually when merging
/// or copying attributes.
///
/// @note Most functions with a parameter of this type typically treat it as
/// a hint, and may ignore it when the requested strategy is not appropriate.
enum GA_DataIdStrategy
{
    /// The default strategy, where the caller does not have to explicitly
    /// worry about attribute data IDs.
    GA_DATA_ID_BUMP = 0,

    /// Attributes should inherit the data IDs from their corresponding source
    /// attribute.  The caller will typically call GA_Attribute::bumpDataId()
    /// on any attributes that they later modify.
    GA_DATA_ID_CLONE = 1
};

/// Attributes may paritition their data in pages of GA_PAGE_SIZE offsets.
#define GA_PAGE_BITS		 10
#define GA_PAGE_SIZE		 (1 << GA_PAGE_BITS)
#define GA_PAGE_MASK		 (GA_PAGE_SIZE - 1)
#define GA_DEFRAGMENT_OCCUPANCY	 1.00

// -------------------------------------------------------------
// GA Types
// When indexing points, vertices or primitives, use
// GA_Index (ordered) or GA_Offset (data offset)
// -------------------------------------------------------------

/// Defines the bit width for index and offset types in GA
typedef exint	GA_Size;

/// Define the strictness of GA_Offset/GA_Index
#if !defined(GA_STRICT_TYPES) && UT_ASSERT_LEVEL > 2
    #define GA_STRICT_TYPES	0
#endif

#if defined(GA_STRICT_TYPES)

#if (UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_NORMAL)
#define GA_MAGIC_BAD_VALUE 0x7BADF00D7F00DBAD
#endif

#if (UT_ASSERT_LEVEL < UT_ASSERT_LEVEL_NORMAL)
#include <SYS/SYS_TypeDecorate.h>
#endif

#include <SYS/SYS_Compiler.h>
#include <SYS/SYS_Hash.h>
#include <SYS/SYS_TypeTraits.h>
#include <ostream>

#if defined(MBSD)
    #define GA_DEFINE_ORDINAL_METHODS_EXTRA \
	size_t operator*(size_t v) const { return myVal * v; } \
        friend size_t operator*(size_t v, const ThisType &a) { return v * a.myVal; }
#else
    #define GA_DEFINE_ORDINAL_METHODS_EXTRA
#endif


#define GA_DEFINE_ORDINAL_METHODS \
	GA_DEFINE_ORDINAL_METHODS_EXTRA \
    bool	    operator==(const ThisType &that) const { return (myVal == that.myVal); } \
    bool	    operator!=(const ThisType &that) const { return (myVal != that.myVal); } \
    bool	    operator<(const ThisType &that)  const { return (myVal <  that.myVal); } \
    bool	    operator<=(const ThisType &that) const { return (myVal <= that.myVal); } \
    bool	    operator>(const ThisType &that)  const { return (myVal >  that.myVal); } \
    bool	    operator>=(const ThisType &that) const { return (myVal >= that.myVal); } \
    exint	    operator&(exint v) const	{ return myVal & v; } \
    exint	    operator&(int v) const	{ return myVal & v; } \
    exint	    operator&(uint v) const	{ return myVal & v; } \
    exint	    operator>>(exint v) const	{ return myVal >> v; } \
    exint	    operator>>(int v) const	{ return myVal >> v; } \
    exint	    operator>>(uint v) const	{ return myVal >> v; } \
    exint	    operator*(exint v) const	{ return myVal * v; } \
    exint	    operator*(int v) const	{ return myVal * v; } \
    exint	    operator*(uint v) const	{ return myVal * v; } \
    exint	    operator*(uint64 v) const	{ return exint(myVal * v); } \
    ThisType	    operator+(const ThisType &that) const { return ThisType(myVal + that.myVal); } \
    ThisType	    operator+(exint v) const	{ return ThisType(myVal + v); } \
    ThisType	    operator+(uint64 v) const	{ return ThisType(myVal + v); } \
    ThisType	    operator+(int v) const	{ return ThisType(myVal + v); } \
    ThisType	    operator+(uint v) const	{ return ThisType(myVal + v); } \
    friend ThisType operator+(exint v, const ThisType &a)	{ return ThisType(a.myVal + v); } \
    friend ThisType operator+(uint64 v, const ThisType &a)	{ return ThisType(a.myVal + v); } \
    friend ThisType operator+(int v, const ThisType &a)		{ return ThisType(a.myVal + v); } \
    friend ThisType operator+(uint v, const ThisType &a)	{ return ThisType(a.myVal + v); } \
    friend exint    operator*(exint v, const ThisType &a)	{ return ThisType(a.myVal * v); } \
    friend exint    operator*(uint64 v, const ThisType &a)	{ return ThisType(a.myVal * v); } \
    friend exint    operator*(int v, const ThisType &a)		{ return ThisType(a.myVal * v); } \
    friend exint    operator*(uint v, const ThisType &a)	{ return ThisType(a.myVal * v); } \
    /* NOTE: ThisType-ThisType must return something that can be divided \
             by (unsigned int)2 in order to support TBB blocked ranges.*/ \
    exint           operator/(uint v)   const { return myVal/v; } \
    exint           operator/(int v)    const { return myVal/v; } \
    exint           operator/(uint64 v) const { return myVal/v; } \
    exint           operator/(int64 v)  const { return myVal/v; } \
    ThisType	    operator-(const ThisType &that) const	{ return ThisType(myVal - that.myVal); } \
    ThisType	    operator-(exint v) const	{ return ThisType(myVal - v); } \
    ThisType	    operator-(uint64 v) const	{ return ThisType(myVal - v); } \
    ThisType	    operator-(int v) const	{ return ThisType(myVal - v); } \
    ThisType	    operator-(uint v) const	{ return ThisType(myVal - v); } \
    ThisType	    operator+=(const ThisType &that)	{ myVal += that.myVal; return *this; } \
    ThisType	    operator+=(exint v)	{ myVal += v; return *this; } \
    ThisType	    operator+=(uint64 v){ myVal += v; return *this; } \
    ThisType	    operator+=(int v)	{ myVal += v; return *this; } \
    ThisType	    operator+=(uint v)	{ myVal += v; return *this; } \
    ThisType	    operator-=(const ThisType &that)	{ myVal -= that.myVal; return *this; } \
    ThisType	    operator-=(exint v)	{ myVal -= v; return *this; } \
    ThisType	    operator-=(uint64 v){ myVal -= v; return *this; } \
    ThisType	    operator-=(int v)	{ myVal -= v; return *this; } \
    ThisType	    operator-=(uint v)	{ myVal -= v; return *this; } \
    ThisType &	    operator++()	{ ++myVal; return *this; } \
    ThisType	    operator++(int)	{ return ThisType(myVal++); } \
    ThisType &	    operator--()	{ --myVal; return *this; } \
    ThisType	    operator--(int)	{ return ThisType(myVal--); } \
    ThisType	    operator-()		{ return ThisType(-myVal); } \
    friend std::ostream &operator<< (std::ostream &stream, const ThisType &a) { return stream << a.myVal; } \
    friend ThisType SYSmax(const ThisType &a, const ThisType &b) \
    { return ThisType(SYSmax(a.myVal, b.myVal)); } \
    friend ThisType SYSmin(const ThisType &a, const ThisType &b) \
    { return ThisType(SYSmin(a.myVal, b.myVal)); } \
    friend ThisType SYSclamp(const ThisType &a, const ThisType &b, const ThisType &c) \
    { return ThisType(SYSclamp(a.myVal, b.myVal, c.myVal)); } \
    friend ThisType UTbumpAlloc(const ThisType &a) \
    { return ThisType(UTbumpAlloc(a.myVal)); } \

    /**/

template <typename TAG, typename BASE_TYPE>
class GA_API_TMPL GA_OrdinalType
{
private:
    typedef	GA_OrdinalType<TAG, BASE_TYPE> ThisType;

public:
		GA_OrdinalType()
#if (UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_NORMAL)
                    : myVal(BASE_TYPE(GA_MAGIC_BAD_VALUE))
#endif
                {}
    explicit	GA_OrdinalType(const BASE_TYPE &val) : myVal(val) { }

    operator	BASE_TYPE() const	    { return myVal; }

    GA_DEFINE_ORDINAL_METHODS

private:
    BASE_TYPE	myVal;
};

class GA_Index;

/// GA_Offset defines an offset into the attribute table of a GA_IndexMap.
/// Properties:
///   - Not allowed to be converted from a GA_Index
///   - Not implicitly convertible from a GA_Size
class GA_API_TMPL GA_Offset
{
    typedef	GA_Offset	    ThisType;

public:
		GA_Offset()
#if (UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_NORMAL)
                    : myVal(GA_MAGIC_BAD_VALUE)
#endif
                {}
    explicit	GA_Offset(GA_Size val) : myVal(val) { }

    operator	int64() const	    { return myVal; }
    operator	uint64() const	    { return myVal; }

    // NOTE: These should generally be avoided, but are mostly here
    //       for templated code that over-instantiates for bogus type
    //       conversions.
    operator	int32() const	    { return myVal; }
    operator	uint32() const	    { return myVal; }
    operator	int16() const	    { return myVal; }
    operator	uint16() const	    { return myVal; }
    operator	int8() const	    { return myVal; }
    operator	uint8() const	    { return myVal; }
#if defined(MBSD)
    // NOTE: These ones are because on Mac GCC, long and size_t are treated as
    //       types different from int64 and uint64 somehow (they are both
    //       64-bit integers).
    operator	long() const	    { return myVal; }
    operator	unsigned long() const { return myVal; }
#endif

    GA_DEFINE_ORDINAL_METHODS

private:
    GA_Offset(const GA_Index &v); // DISALLOW

    /// Disallow boolean operations, because they're almost guaranteed
    /// to be porting errors checking if a pointer is NULL or non-NULL
    /// instead of checking whether the offset is zero.
    SYS_SAFE_BOOL operator bool() const;

    /// Disallow comparison with integers, for example, the
    /// "if (ptoff==0)" case, since it's most likely a porting error.
    bool operator==(exint v) const;
    bool operator==(uint64 v) const;
    bool operator==(int v) const;
    bool operator==(uint v) const;
    bool operator!=(exint v) const;
    bool operator!=(uint64 v) const;
    bool operator!=(int v) const;
    bool operator!=(uint v) const;

    GA_Size	myVal;
};
UT_IFNOT_ASSERT(SYS_DECLARE_IS_POD(GA_Offset))

inline SYS_HashType hash_value(const GA_Offset &off)
{
    return SYShash(GA_Size(off));
}

/// GA_Index defines an ordered index of a GA_IndexMap.
/// Properties:
///   - Not allowed to be converted from a GA_Offset
///   - Implicitly convertible from and to a GA_Size
class GA_API_TMPL GA_Index
{
private:
    typedef	GA_Index	    ThisType;

public:
		GA_Index()
#if (UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_NORMAL)
                    : myVal(GA_MAGIC_BAD_VALUE)
#endif
                {}
		GA_Index(const GA_Size &val) : myVal(val) { }

    operator	int64() const	    { return myVal; }
    operator	uint64() const	    { return myVal; }

    // NOTE: These should generally be avoided, but are mostly here
    //       for templated code that over-instantiates for bogus type
    //       conversions.
    operator	int32() const	    { return myVal; }
    operator	uint32() const	    { return myVal; }
    operator	int16() const	    { return myVal; }
    operator	uint16() const	    { return myVal; }
    operator	int8() const	    { return myVal; }
    operator	uint8() const	    { return myVal; }
#if defined(MBSD)
    // NOTE: This one is because on Mac GCC, long is treated as a type
    //       different from int64 somehow (they are both 64-bit integers).
    operator	long() const	    { return myVal; }
    operator	unsigned long() const { return myVal; }
#endif

    GA_DEFINE_ORDINAL_METHODS

    // NOTE: These methods are to avoid having to change the ton of code that
    //       uses ints, etc, as interchangeable with GA_Index
    // Equality comparisons with integers were re-allowed, since most porting
    // errors would involve GA_Offset, not GA_Index, and not being able to compare
    // getNumPoints() with 0 is counterintuitive.
    bool operator< (exint v)  const { return (myVal <  v); }
    bool operator<=(exint v)  const { return (myVal <= v); }
    bool operator> (exint v)  const { return (myVal >  v); }
    bool operator>=(exint v)  const { return (myVal >= v); }
    bool operator==(exint v)  const { return (myVal == v); }
    bool operator!=(exint v)  const { return (myVal != v); }
    bool operator< (uint64 v) const { return (myVal <  v); }
    bool operator<=(uint64 v) const { return (myVal <= v); }
    bool operator> (uint64 v) const { return (myVal >  v); }
    bool operator>=(uint64 v) const { return (myVal >= v); }
    bool operator==(uint64 v) const { return (myVal == v); }
    bool operator!=(uint64 v) const { return (myVal != v); }
    bool operator< (int v)    const { return (myVal <  v); }
    bool operator<=(int v)    const { return (myVal <= v); }
    bool operator> (int v)    const { return (myVal >  v); }
    bool operator>=(int v)    const { return (myVal >= v); }
    bool operator==(int v)    const { return (myVal == v); }
    bool operator!=(int v)    const { return (myVal != v); }
    bool operator< (uint v)   const { return (myVal <  v); }
    bool operator<=(uint v)   const { return (myVal <= v); }
    bool operator> (uint v)   const { return (myVal >  v); }
    bool operator>=(uint v)   const { return (myVal >= v); }
    bool operator==(uint v)   const { return (myVal == v); }
    bool operator!=(uint v)   const { return (myVal != v); }
#if defined(MBSD)
    // NOTE: This one is because on Mac GCC, long is treated as a type
    //       different from int64 somehow (they are both 64-bit integers).
    bool operator< (unsigned long v)    const { return (myVal <  v); }
    bool operator<=(unsigned long v)    const { return (myVal <= v); }
    bool operator> (unsigned long v)    const { return (myVal >  v); }
    bool operator>=(unsigned long v)    const { return (myVal >= v); }
    bool operator==(unsigned long v)    const { return (myVal == v); }
    bool operator!=(unsigned long v)    const { return (myVal != v); }
#endif

    friend bool operator<( exint v,  const GA_Index &a) { return (v <  a.myVal); }
    friend bool operator<=(exint v,  const GA_Index &a) { return (v <= a.myVal); }
    friend bool operator>( exint v,  const GA_Index &a) { return (v >  a.myVal); }
    friend bool operator>=(exint v,  const GA_Index &a) { return (v >= a.myVal); }
    friend bool operator==(exint v,  const GA_Index &a) { return (v == a.myVal); }
    friend bool operator!=(exint v,  const GA_Index &a) { return (v != a.myVal); }
    friend bool operator<( uint64 v, const GA_Index &a) { return (v <  a.myVal); }
    friend bool operator<=(uint64 v, const GA_Index &a) { return (v <= a.myVal); }
    friend bool operator>( uint64 v, const GA_Index &a) { return (v >  a.myVal); }
    friend bool operator>=(uint64 v, const GA_Index &a) { return (v >= a.myVal); }
    friend bool operator==(uint64 v, const GA_Index &a) { return (v == a.myVal); }
    friend bool operator!=(uint64 v, const GA_Index &a) { return (v != a.myVal); }
    friend bool operator<( int v,    const GA_Index &a) { return (v <  a.myVal); }
    friend bool operator<=(int v,    const GA_Index &a) { return (v <= a.myVal); }
    friend bool operator>( int v,    const GA_Index &a) { return (v >  a.myVal); }
    friend bool operator>=(int v,    const GA_Index &a) { return (v >= a.myVal); }
    friend bool operator==(int v,    const GA_Index &a) { return (v == a.myVal); }
    friend bool operator!=(int v,    const GA_Index &a) { return (v != a.myVal); }
    friend bool operator<( uint v,   const GA_Index &a) { return (v <  a.myVal); }
    friend bool operator<=(uint v,   const GA_Index &a) { return (v <= a.myVal); }
    friend bool operator>( uint v,   const GA_Index &a) { return (v >  a.myVal); }
    friend bool operator>=(uint v,   const GA_Index &a) { return (v >= a.myVal); }
    friend bool operator==(uint v,   const GA_Index &a) { return (v == a.myVal); }
    friend bool operator!=(uint v,   const GA_Index &a) { return (v != a.myVal); }
#if defined(MBSD)
    // NOTE: This one is because on Mac GCC, long is treated as a type
    //       different from int64 somehow (they are both 64-bit integers).
    friend bool operator<( unsigned long v,   const GA_Index &a) { return (v <  a.myVal); }
    friend bool operator<=(unsigned long v,   const GA_Index &a) { return (v <= a.myVal); }
    friend bool operator>( unsigned long v,   const GA_Index &a) { return (v >  a.myVal); }
    friend bool operator>=(unsigned long v,   const GA_Index &a) { return (v >= a.myVal); }
    friend bool operator==(unsigned long v,   const GA_Index &a) { return (v == a.myVal); }
    friend bool operator!=(unsigned long v,   const GA_Index &a) { return (v != a.myVal); }
#endif

    /// Boolean operations were re-allowed for if (getNumPoints()), because
    /// most porting errors would involve checking GA_Offset, not GA_Index.
    SYS_SAFE_BOOL operator bool() const  { return myVal != 0; }

private:
    GA_Index(const GA_Offset &v); // DISALLOW

    GA_Size	myVal;
};
UT_IFNOT_ASSERT(SYS_DECLARE_IS_POD(GA_Index))

inline SYS_HashType hash_value(const GA_Index &idx)
{
    return SYShash(GA_Size(idx));
}

#undef GA_DEFINE_ORDINAL_BINARY_OPS
#undef GA_DEFINE_ORDINAL_METHODS


class GA_PageNumTag {};
typedef GA_OrdinalType<GA_PageNumTag, GA_Size> GA_PageNum;
UT_IFNOT_ASSERT(SYS_DECLARE_IS_POD(GA_PageNum))


class GA_PageOffTag {};
typedef GA_OrdinalType<GA_PageOffTag, GA_Size> GA_PageOff;
UT_IFNOT_ASSERT(SYS_DECLARE_IS_POD(GA_PageOff))

inline bool GAisValid(GA_Offset v)
{
    UT_ASSERT(v != GA_Offset(GA_MAGIC_BAD_VALUE));
    return v >= GA_Offset(0);
}
inline bool GAisValid(GA_Index v)
{
    UT_ASSERT(v != GA_Index(GA_MAGIC_BAD_VALUE));
    return v >= GA_Index(0);
}

namespace UT {
template<typename T>
struct DefaultClearer;

template<>
struct DefaultClearer<GA_Offset>
{
    static void clear(GA_Offset &v)
    { v = GA_Offset(std::numeric_limits<exint>::min()); }
    static bool isClear(GA_Offset v)
    { return v == GA_Offset(std::numeric_limits<exint>::min()); }
    static void clearConstruct(GA_Offset *p) { clear(*p); }

    static const bool clearNeedsDestruction = false;
};

template<>
struct DefaultClearer<GA_Index>
{
    static void clear(GA_Index &v)
    { v = GA_Index(std::numeric_limits<exint>::min()); }
    static bool isClear(GA_Index v)
    { return v == GA_Index(std::numeric_limits<exint>::min()); }
    static void clearConstruct(GA_Index *p) { clear(*p); }

    static const bool clearNeedsDestruction = false;
};
}

namespace std {
template<>
struct hash<GA_Offset>
{
    size_t operator()(GA_Offset v)
    { return std::hash<exint>()((exint)v); }
};
template<>
struct hash<GA_Index>
{
    size_t operator()(GA_Index v)
    { return std::hash<exint>()((exint)v); }
};
}

GA_API size_t format(char *buf, size_t bufsize, const GA_Index &v);
GA_API size_t format(char *buf, size_t bufsize, const GA_Offset &v);

#else

/// GA_Index is a contiguous index into an element in a GA_Detail structure.
/// Since the list of elements can contain gaps, the index should be translated 
/// into a GA_Offset prior to lookup. The index remains constant if the element
/// list is defragmented, but will be invalid if an element, prior to a given 
/// index, is inserted or deleted.
/// \sa GA_Detail::pointOffset, GA_Detail::pointIndex, 
/// \sa GA_Detail::primitiveOffset, GA_Detail::primitiveIndex
/// \sa GA_Detail::vertexOffset, GA_Detail::vertexIndex
typedef GA_Size	GA_Index;

/// GA_Offset is a, possibly, non-contiguous offset to an element in a 
/// GA_Detail structure. GA_Offsets remain constant even if an element prior to
/// it is deleted, but will be invalidated if an element is inserted prior to 
/// it, or if the element list is defragmented. 
typedef GA_Size	GA_Offset;


typedef GA_Size	GA_PageNum;
typedef GA_Size	GA_PageOff;

#endif

inline bool GAisValid(GA_Size v) {
#if defined(GA_STRICT_TYPES)
    UT_ASSERT(v != GA_Size(GA_MAGIC_BAD_VALUE));
#endif
    return v >= 0;
}

inline GA_PageNum GAgetPageNum(GA_Offset v)
{
    return GA_PageNum(v >> GA_PAGE_BITS);
}
inline GA_PageOff GAgetPageOff(GA_Offset v)
{
    return GA_PageOff(v & GA_PAGE_MASK);
}
inline bool	 GAisFullPage(GA_Offset start, GA_Offset end)
{
    return (GAgetPageOff(start) == 0 &&
	    GAgetPageOff(end) == 0 &&
	    (GAgetPageNum(start) == GAgetPageNum(end)-1));
}

inline GA_Offset GAgetPageBoundary(const GA_Offset &start,
				   const GA_Offset &end)
{
    return GA_Offset(SYSmin(GA_Size(end), GA_Size(start) - GA_Size(GAgetPageOff(start)) + GA_PAGE_SIZE));
}

#define GA_INVALID_INDEX    GA_Index(-1)
#define GA_INVALID_OFFSET   GA_Offset(-1)

/// Details are always at index and offset zero in their own detail index map.
#define GA_DETAIL_INDEX     GA_Index(0)
#define GA_DETAIL_OFFSET    GA_Offset(0)

typedef UT_ValArray<GA_Index>	    GA_IndexArray;
typedef UT_ValArray<GA_Offset>	    GA_OffsetArray;

typedef int64			GA_DataId;
#define GA_INVALID_DATAID	GA_DataId(-1)


/// @{
/// At the current time, intrinsic handles are an integer.
/// This may change in the future.
typedef int	GA_LocalIntrinsic;
typedef int	GA_GlobalIntrinsic;
#define GA_INVALID_INTRINSIC_HANDLE	-1
static inline bool	GAisValidGlobalIntrinsic(GA_GlobalIntrinsic h)
				    { return h >= 0; }
static inline bool	GAisValidLocalIntrinsic(GA_LocalIntrinsic h)
				    { return h >= 0; }
/// @}

// -------------------------------------------------------------
// Functions to map storage enums to tokens.
// -------------------------------------------------------------
/// Lookup the size in bytes of a storage type (inaccurate for bool)
GA_API extern unsigned		 GAsizeof(GA_Storage store);

/// Compare precision of GA_Storage types.  The function will return:
///	-2 invalid comparison (i.e. comparing a string to a float)
///	-1 if a is less precise than b
///	 0 if a and be have the same precision
///	 1 if a is more precise than b
GA_API extern int		 GAcomparePrecision(GA_Storage a, GA_Storage b);

/// Lookup the owner name from the owner type
GA_API extern const char	*GAowner(GA_AttributeOwner owner);
/// Lookup the owner type from the owner name
GA_API extern GA_AttributeOwner	 GAowner(const char *owner);

/// Lookup the scope name from the scope type
GA_API extern const char	*GAscope(GA_AttributeScope scope);
/// Lookup the scope type from the scope name
GA_API extern GA_AttributeScope	 GAscope(const char *scope);

/// Lookup the storage name from the storage type
GA_API extern const char	*GAstorage(GA_Storage store);
/// Lookup the storage type from the storage name
GA_API extern GA_Storage	 GAstorage(const char *store);

/// Lookup the storage label (descriptive name) from the storage type
GA_API extern const char	*GAstorageLabel(GA_Storage store);
/// Lookup the storage type from the storage label
GA_API extern GA_Storage	 GAstorageLabel(const char *store);



/// Lookup the storage name from the storage type
GA_API extern const char	*GAstorageClass(GA_StorageClass store);
/// Lookup the intrinsic storage type from the storage name
GA_API extern GA_StorageClass	 GAstorageClass(const char *store);

/// Lookup the type-info name from the type-info type
GA_API extern const char	*GAtypeinfo(GA_TypeInfo type);
/// Lookup the type-info type from the type-info name
GA_API extern GA_TypeInfo	 GAtypeinfo(const char *type);

/// Lookup the type-info label (descriptive name) from the type-info type
GA_API extern const char	*GAtypeinfoLabel(GA_TypeInfo type);
/// Lookup the type-info type from the type-info label (descriptive name)
GA_API extern GA_TypeInfo	 GAtypeinfoLabel(const char *type);

/// Lookup the owner name from the owner type
GA_API extern const char	*GAgroupType(GA_GroupType owner);
/// Lookup the owner type from the owner name
GA_API extern GA_GroupType	 GAgroupType(const char *owner);


/// Convenience method to determine whether an integer represents a valid
/// storage type.
static inline bool	GAisStorageType(int s)
			{
			    return s >= GA_STORE_BOOL && s <= GA_STORE_STRING;
			}

/// Convenience method to determine whether storage is real valued
static inline bool	GAisFloatStorage(GA_Storage s)
			{
			    return s == GA_STORE_REAL16 ||
				s == GA_STORE_REAL32 ||
				s == GA_STORE_REAL64;
			}

/// Convenience method to determine whether storage is integer valued
static inline bool	GAisIntStorage(GA_Storage s)
			{
			    return
				s == GA_STORE_UINT8 ||
				s == GA_STORE_INT8 ||
				s == GA_STORE_INT16 ||
				s == GA_STORE_INT32 ||
				s == GA_STORE_INT64;
			}

/// Convenience method to determine whether storage is numeric (real/integer)
static inline bool	GAisNumericStorage(GA_Storage s)
			{
			    return GAisFloatStorage(s) || GAisIntStorage(s);
			}

/// Convenience method to determine if type info is a transforming type
static inline bool	GAisTransformingType(GA_TypeInfo t)
			{
			    return
				t == GA_TYPE_VECTOR ||
				t == GA_TYPE_NORMAL ||
				t == GA_TYPE_POINT ||
				t == GA_TYPE_HPOINT ||
				t == GA_TYPE_TRANSFORM ||
				t == GA_TYPE_QUATERNION;
			}

static inline GA_StorageClass GAstorageClass(GA_Storage s)
{
    switch (s)
    {
	case GA_STORE_INVALID:
	    return GA_STORECLASS_INVALID;
	case GA_STORE_BOOL:
	case GA_STORE_UINT8:
	case GA_STORE_INT8:
	case GA_STORE_INT16:
	case GA_STORE_INT32:
	case GA_STORE_INT64:
	    return GA_STORECLASS_INT;
	case GA_STORE_REAL16:
	case GA_STORE_REAL32:
	case GA_STORE_REAL64:
	    return GA_STORECLASS_REAL;
	case GA_STORE_STRING:
	    return GA_STORECLASS_STRING;
    }

    return GA_STORECLASS_OTHER;
}

static inline GA_Storage GAgetDefaultStorage(GA_StorageClass s)
{
    switch (s)
    {
	case GA_STORECLASS_INT:
	    return GA_STORE_INT32;
	case GA_STORECLASS_REAL:
	    return GA_STORE_REAL32;
	case GA_STORECLASS_STRING:
	    return GA_STORE_STRING;
	default:
	    // There's no default storage for this storage class
	    return GA_STORE_INVALID;
    }
}

#endif
