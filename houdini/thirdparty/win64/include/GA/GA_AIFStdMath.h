/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFStdMath.h ( GA Library, C++)
 *
 * COMMENTS:	Simple implementation of AIFMath.  This method simply calls
 *		the generic methods which use AIFTuple to get/set data
 *		in the attribute.
 *
 *		Although convenient, this class may perform worse than
 *		specialized versions which know more about the internal
 *		attribute structure.
 */

#ifndef __GA_AIFMathDev__
#define __GA_AIFMathDev__

#include "GA_API.h"
#include "GA_AIFMath.h"

#include <UT/UT_VectorTypes.h>


class GA_AttributeOperand;


/// @brief Generic implmentation of the AIFMath implementation using AIFTuple
///
/// The GA_AIFMath class provides a pure virtual interface requiring developers
/// to implement all specializations of the arithmetic operations.  However,
/// there are a lot of methods to implement.  If your attribute provides an
/// AIFTuple numeric interface, AIFStdMath will use this interface to perform
/// many operations.  If you sub-class from AIFStdMath instead of AIFMath you
/// can still specialize bottleneck operations while providing a complete
/// interface.
///
/// However, <b>be warned</b>, if the AIFMath interface changes, you may
/// end up missing opportunities for specialization.
class GA_API GA_AIFStdMath : public GA_AIFMath 
{
public:
	     GA_AIFStdMath() {}
    virtual ~GA_AIFStdMath();
    /// Use the define from GA_AIFMath to declare binary methods
    GA_MATH_SPEC_BINARY( madd )
    GA_MATH_SPEC_BINARY( add )
    GA_MATH_SPEC_BINARY( mul )
    GA_MATH_SPEC_BINARY( sub )
    // Specializations for transform
    GA_MATH_SPEC_TRANSFORM(UT_DMatrix4, UT_Matrix4, rowVecMult)
    GA_MATH_SPEC_TRANSFORM(UT_DMatrix3, UT_Matrix3, rowVecMult)
    GA_MATH_SPEC_TRANSFORM(UT_DMatrix4, UT_Matrix4, rowVecMult3)
    GA_MATH_SPEC_TRANSFORM(UT_DMatrix4, UT_Matrix4, colVecMult)
    GA_MATH_SPEC_TRANSFORM(UT_DMatrix3, UT_Matrix3, colVecMult)
    GA_MATH_SPEC_TRANSFORM(UT_DMatrix4, UT_Matrix4, colVecMult3)

    /// @{
    /// Operand based implementations
    bool	add(GA_AttributeOperand &dest,
				GA_AttributeOperand &a,
				GA_AttributeOperand &b,
				int component = -1) const;
    bool	mul(GA_AttributeOperand &dest,
				GA_AttributeOperand &a,
				GA_AttributeOperand &b,
				int component = -1) const;
    bool	sub(GA_AttributeOperand &dest,
				GA_AttributeOperand &a,
				GA_AttributeOperand &b,
				int component = -1) const;
    bool	madd(GA_AttributeOperand &dest,
				GA_AttributeOperand &a,
				GA_AttributeOperand &b,
				int component = -1) const;
    bool	normalize(GA_AttributeOperand &dest,
				GA_AttributeOperand &src) const;
    bool	rowVecMult(GA_AttributeOperand &dest,
				GA_AttributeOperand &src,
				const UT_Matrix4 &xform) const;
    bool	rowVecMult(GA_AttributeOperand &dest,
				GA_AttributeOperand &src,
				const UT_DMatrix4 &xform) const;
    bool	rowVecMult(GA_AttributeOperand &dest,
				GA_AttributeOperand &src,
				const UT_Matrix3 &xform) const;
    bool	rowVecMult(GA_AttributeOperand &dest,
				GA_AttributeOperand &src,
				const UT_DMatrix3 &xform) const;
    bool	colVecMult(GA_AttributeOperand &dest,
				GA_AttributeOperand &src,
				const UT_Matrix4 &xform) const;
    bool	colVecMult(GA_AttributeOperand &dest,
				GA_AttributeOperand &src,
				const UT_DMatrix4 &xform) const;
    bool	colVecMult(GA_AttributeOperand &dest,
				GA_AttributeOperand &src,
				const UT_Matrix3 &xform) const;
    bool	colVecMult(GA_AttributeOperand &dest,
				GA_AttributeOperand &src,
				const UT_DMatrix3 &xform) const;
    bool	rowVecMult3(GA_AttributeOperand &dest,
				GA_AttributeOperand &src,
				const UT_Matrix4 &xform) const;
    bool	rowVecMult3(GA_AttributeOperand &dest,
				GA_AttributeOperand &src,
				const UT_DMatrix4 &xform) const;
    bool	colVecMult3(GA_AttributeOperand &dest,
				GA_AttributeOperand &src,
				const UT_Matrix4 &xform) const;
    bool	colVecMult3(GA_AttributeOperand &dest,
				GA_AttributeOperand &src,
				const UT_DMatrix4 &xform) const;
    template <typename MTYPE>
    bool	ptransform(GA_AttributeOperand &dest,
			    GA_AttributeOperand &src,
			    const MTYPE &xform) const
		    { return rowVecMult(dest, src, xform); }
    template <typename MTYPE>
    bool	vtransform(GA_AttributeOperand &dest,
			    GA_AttributeOperand &src,
			    const MTYPE &xform) const
		    { return rowVecMult3(dest, src, xform); }
    template <typename MTYPE>
    bool	ntransform(GA_AttributeOperand &dest,
			    GA_AttributeOperand &src,
			    const MTYPE &xform) const
		    { return colVecMult3(dest, src, xform); }
};

#endif
