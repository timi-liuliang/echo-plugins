/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFMath.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface class for math methods
 */

#ifndef __GA_AIFMath__
#define __GA_AIFMath__

#include "GA_API.h"
#include "GA_Types.h"

#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Types.h>


class GA_Attribute;


/// @brief Attribute Interface class to perform numeric operations on attributes
///
/// This class provides the interface to perform numerical operations on
/// attribute data.  Each attribute type may provide this interface if it
/// makes sense.@n
/// <b>Basic mathematicl operations.</b>
///  - madd(d, a, b)		@code d = d + a*b @endcode
///  - scaleoffset(d, s, o)	@code d = d*s + o @endcode
///  - add(d, a, b)		@code d = a + b @endcode
///  - mul(d, a, b)		@code d = a * b @endcode
///  - sub(d, a, b)		@code d = a - b @endcode
///  - div(d, a, b)		@code d = a / b @endcode
///
/// <b>Common mathematical operations</b>
///  - normalize(d, a)		@code d = normalize(a) @endcode
///
/// <b>Matrix Operations</b>
///  - rowVecMult
///  - rowVecMult
///  - rowVecMult3
///  - colVecMult
///  - colVecMult
///  - colVecMult3
///
/// @b Signatures @n
/// When performing operations on attributes, there are different variable
/// types.
///   - A constant value (scalar or tuple)
///   - A single element of the attribute array (specified by an offset)
///   - A selection of the attribute array (specified by an iterator)
/// Operations are defined as the permutations of all these different variable
/// types.  The destination (LHS) must always be writable (i.e. an attribute,
/// not a constant).
///
/// Many operations may also be performed on one or more components of the
/// tuple of values.  The methods which take a @b component parameter will
/// use the component to work only on a single component of the tuple.  If
/// the component parameter is less than zero, the operation will be applied
/// to all components.
///
/// For example, the add() operation has the following signatures
///  @code
///	add(GA_Attribute &d, GA_Offset di, const GA_Attribute &a, GA_Offset ai, fpreal b, int component);
///	add(GA_Attribute &d, GA_Offset di, const GA_Attribute &a, GA_Offset ai, const fpreal32 *b, int tuple_size, int component);
///	add(GA_Attribute &d, GA_Offset di, const GA_Attribute &a, GA_Offset ai, const fpreal64 *b, int tuple_size, int component);
///	add(GA_Attribute &d, GA_Offset di, const GA_Attribute &a, GA_Offset ai, const GA_Attribute &b, GA_Offset bi, int component);
///	add(GA_Attribute &d, const GA_Range &di, const GA_Attribute &a, const GA_Range &ai, fpreal b, int component);
///	add(GA_Attribute &d, const GA_Range &di, const GA_Attribute &a, const GA_Range &ai, const fpreal32 *b, int tuple_size, int component);
///	add(GA_Attribute &d, const GA_Range &di, const GA_Attribute &a, const GA_Range &ai, const fpreal64 *b, int tuple_size, int component);
///	add(GA_Attribute &d, const GA_Range &di, const GA_Attribute &a, const GA_Range &ai, const GA_Attribute &b, GA_Offset bi, int component);
///	add(GA_Attribute &d, const GA_Range &di, const GA_Attribute &a, const GA_Range &ai, const GA_Attribute &b, const GA_Range &bi, int component);
///  @endcode
///
/// To assist with being able to implement all these operations with minimal
/// effort, please see the GA_AttributeOperand class.  All of the specialized
/// methods will simply call into the method which handles the operand.  If you
/// wish to specialize the methods to be more efficient, you can do that by
/// overriding the specific method.  In the case of add(), the generic
/// operation is.
/// @code
///	add(GA_AttributeOperantd &dest, GA_AttributeOperand &a, GA_AttributeOperand &b);
/// @endcode
///
class GA_API GA_AIFMath 
{
public:
	     GA_AIFMath() {}
    virtual ~GA_AIFMath();

    /// Create an AIFMath which performs no arithmetic on any objects.  This is
    /// different than not having an AIFMath.
    static const GA_AIFMath	*getNoMath();

    /// Specialization macro for binary operations.  This macro provides 9
    /// specializations for various combinations of parameters.  In the base
    /// class, these methods simply call the generic implementations of the
    /// operations, returning false if the attribute doesn't support required
    /// elements.
    #define GA_MATH_SPEC_BINARY_PURE(NAME) \
	virtual bool	NAME(GA_Attribute &d, GA_Offset di, \
				const GA_Attribute &a, GA_Offset ai, \
				fpreal b, int component=-1) const = 0; \
	virtual bool	NAME(GA_Attribute &d, GA_Offset di, \
				const GA_Attribute &a, GA_Offset ai, \
				const fpreal32 *b, int tuple_size, \
				int component=-1) const = 0; \
	virtual bool	NAME(GA_Attribute &d, GA_Offset di, \
				const GA_Attribute &a, GA_Offset ai, \
				const fpreal64 *b, int tuple_size, \
				int component=-1) const = 0; \
	virtual bool	NAME(GA_Attribute &d, GA_Offset di, \
				const GA_Attribute &a, GA_Offset ai, \
				const GA_Attribute &b, GA_Offset bi, \
				int component=-1) const = 0;

    /// Convenience define for sub-class implementations
    #define GA_MATH_SPEC_BINARY(NAME)	\
	virtual bool	NAME(GA_Attribute &d, GA_Offset di, \
				const GA_Attribute &a, GA_Offset ai, \
				fpreal b, int component=-1) const; \
	virtual bool	NAME(GA_Attribute &d, GA_Offset di, \
				const GA_Attribute &a, GA_Offset ai, \
				const fpreal32 *b, int tuple_size, \
				int component=-1) const; \
	virtual bool	NAME(GA_Attribute &d, GA_Offset di, \
				const GA_Attribute &a, GA_Offset ai, \
				const fpreal64 *b, int tuple_size, \
				int component=-1) const; \
	virtual bool	NAME(GA_Attribute &d, GA_Offset di, \
				const GA_Attribute &a, GA_Offset ai, \
				const GA_Attribute &b, GA_Offset bi, \
				int component=-1) const;


    GA_MATH_SPEC_BINARY_PURE( madd )
    GA_MATH_SPEC_BINARY_PURE( add )
    GA_MATH_SPEC_BINARY_PURE( mul )
    GA_MATH_SPEC_BINARY_PURE( sub )

    // DTYPE is the double precision type (i.e. UT_DMatrix4).  FTYPE is the
    // single precision type (i.e. UT_Matrix4).  NAME is the name of the
    // method.
    #define GA_MATH_SPEC_TRANSFORM_PURE(DTYPE, FTYPE, NAME)	\
	virtual bool	NAME(const DTYPE &xform, \
				GA_Attribute &d, GA_Offset di, \
				const GA_Attribute &s, GA_Offset si) const=0; \
	virtual bool	NAME(const FTYPE &xform, \
				GA_Attribute &d, GA_Offset di, \
				const GA_Attribute &s, GA_Offset si) const=0;

    #define GA_MATH_SPEC_TRANSFORM(DTYPE, FTYPE, NAME)	\
	virtual bool	NAME(const DTYPE &xform, \
				GA_Attribute &d, GA_Offset di, \
				const GA_Attribute &s, GA_Offset si) const; \
	virtual bool	NAME(const FTYPE &xform, \
				GA_Attribute &d, GA_Offset di, \
				const GA_Attribute &s, GA_Offset si) const;

    // Specializations for transform
    GA_MATH_SPEC_TRANSFORM_PURE(UT_DMatrix4, UT_Matrix4, rowVecMult)
    GA_MATH_SPEC_TRANSFORM_PURE(UT_DMatrix3, UT_Matrix3, rowVecMult)
    GA_MATH_SPEC_TRANSFORM_PURE(UT_DMatrix4, UT_Matrix4, rowVecMult3)
    GA_MATH_SPEC_TRANSFORM_PURE(UT_DMatrix4, UT_Matrix4, colVecMult)
    GA_MATH_SPEC_TRANSFORM_PURE(UT_DMatrix3, UT_Matrix3, colVecMult)
    GA_MATH_SPEC_TRANSFORM_PURE(UT_DMatrix4, UT_Matrix4, colVecMult3)
};

#endif
