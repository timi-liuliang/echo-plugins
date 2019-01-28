/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFInterp.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface for Interpolation
 */

#ifndef __GA_AIFInterp__
#define __GA_AIFInterp__

#include "GA_API.h"

#include "GA_Types.h"

#include <SYS/SYS_Types.h>


class GA_Range;
class GA_Attribute;
class GA_AttributeOperand;
class GA_WeightedSum;


/// @brief Attribute Interface class to perform interpolation operations on
/// attributes.
///
/// This class provides an interface to perform interpolation operations on
/// attribute data.@n
/// - lerp(d, a, b, t)
///	@code d = SYSlerp(a, b, t) = (t-1)*a + t*b @endcode
/// - smooth(d, min, max, t)
///	@code d = SYSsmooth(min, max, t) @endcode
/// - fit(d, t, omin, omax, nmin, nmax)
///	@code d = SYSfit(t, omin, omax, nmin, nmax) @endcode
/// - fit01(d, t, nmin, nmax)
///	@code d = SYSfit(t, 0, 1, nmin, nmax) @endcode
/// - Weighted sum
///     - startSum(d, sum)@n
///       Initializes the weighted sum for a given attribute
///     - addSum(d, sum, a, w)@n
///       Add @b a*w to the sum.
///	- addSquare(d, sum, a, w)@n
///	  Add @b a*a*w to the sum
///     - endSum(d, sum, scale_factor)@n
///       Complete the sum, scaling the value.  If @b scale_factor is 1, then
///       no scaling is performed.
///     - endSquare(d, sum, scale_factor)@n
///       Complete the sum by scaling, then taking the square root of the
///       result.  If @b scale_factor is 1, then no scaling is
///       performed.
///   For example, the lerp function can be implemented as: @code
///     lerp(d, a, b, t) {
///         GA_AIFInterp        *i = d.getAIFInterp();
///         i->startSum(d, sum);
///         i->addSum(d, sum, a, 1-t);
///         i->addSum(d, sum, b, t);
///         i->finishSum(d, sum);
///     }
///     // To compute the RMS of an attribute: @code
///	rms(d, a, b, t) {
///	    GA_AIFInterp	*i = d.getAIFInterp();
///         i->startSum(d, sum);
///         i->addSquare(d, sum, a, 1);
///         i->addSquare(d, sum, b, 1);
///         i->finishSquare(d, sum, 1/sum.getTotalWeight());
///	}
///	// Compute the sum of the squares of the input
///	sumSquares(d, a, b, t) {
///	    GA_AIFInterp	*i = d.getAIFInterp();
///         i->startSum(d, sum);
///         i->addSquare(d, sum, a, 1);
///         i->addSquare(d, sum, b, 1);
///         i->finishSum(d, sum, 1.0);
///	}
/// 
class GA_API GA_AIFInterp 
{
public:
	     GA_AIFInterp() {}
    virtual ~GA_AIFInterp();

    /// Return whether this interpolator is a conditional interpolation or
    /// whether values are actually interpolated.
    virtual bool	isConditionalInterpolation() const { return false; }

    /// Note: Weighted sums can only be performed on a single element, and thus
    /// don't have a generic GA_AttributeOperand method.
    /// @param d The destination attribute
    /// @param di The index in the destination attribute used to store results
    /// @param sum Weighted sum to keep track of partial sums
    virtual bool	startSum(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum) const = 0;
    /// Add an element into the weighted sum
    /// @param d  Destination attribute
    /// @param di  Destination offset
    /// @param sum Storage for state of weighted sum
    /// @param a  Attribute to add into the sum
    /// @param ai Index of the attribute to add into the sum
    virtual bool	addSum(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				const GA_Attribute &a, GA_Offset ai) const = 0;

    /// Add the square of an element to the weighted sum
    /// @param d  Destination attribute
    /// @param di  Destination offset
    /// @param sum Storage for state of weighted sum
    /// @param a  Attribute to add into the sum
    /// @param ai Index of the attribute to add into the sum
    virtual bool	addSquare(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				const GA_Attribute &a, GA_Offset ai) const = 0;

    /// Finish a weighted sum.  The scale_factor can be used to normalize
    /// weighting.  For example: @code
    ///    endSum(d, di, sum, 1./sum.getTotalWeight());
    /// @endcode
    virtual bool	endSum(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				fpreal scale_factor = 1) const = 0;

    /// Finish a weighted sum by scaling then taking the square root of the
    /// sum.  The scale_factor can be used to normalize weighting.  For
    /// example: @code
    ///    endSquare(d, di, sum, 1./sum.getTotalWeight());
    /// @endcode
    /// A scale of 1 will result in the result being the square root of the sum.
    virtual bool	endSquare(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				fpreal scale_factor = 1) const = 0;

    /// Add an element into the weighted sum where the element refers to
    /// homogeneous coordinates. 
    ///
    /// The @c wsum should be used in the endSum() to correct the
    /// normalization.  That is, the endSum() code should look like: @code
    ///	  endSum(d, di, sum, scale_factor / wsum, ...);
    /// @endcode
    /// where the scale_factor is the non-homogeneous scale factor.
    ///
    /// @param d  Destination attribute
    /// @param di  Destination offset
    /// @param sum Storage for state of weighted sum
    /// @param a  Attribute to add into the sum
    /// @param ai Index of the attribute to add into the sum
    /// @note  The default behaviour of this method is to call addSum()
    virtual bool	addHSum(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				const GA_Attribute &a, GA_Offset ai) const;

    /// Add the square of the homogeneous input to the sum
    virtual bool	addHSquare(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				const GA_Attribute &a, GA_Offset ai) const;

    /// Finish a weighted sum for homogeneous coordinates.  The scale_factor
    /// can be used to normalize weighting.  For example: @code
    ///    endHSum(d, di, sum, 1./sum.getTotalWeight());
    /// @endcode
    virtual bool	endHSum(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				fpreal scale_factor = 1) const;
    /// Finish a weighted sum for homogeneous coordinates by scaling then
    /// taking the square root.
    /// For example: @code
    ///    endHSquare(d, di, sum, 1./sum.getTotalWeight());
    /// @endcode
    virtual bool	endHSquare(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				fpreal scale_factor = 1) const;

    /// d = SYSlerp(a, b, t);
    virtual bool	lerp(GA_AttributeOperand &d,
				 GA_AttributeOperand &a,
				 GA_AttributeOperand &b,
				 GA_AttributeOperand &t) const;
    /// d = SYSsmooth(min, max, t);
    virtual bool	smooth(GA_AttributeOperand &d,
				 GA_AttributeOperand &min,
				 GA_AttributeOperand &max,
				 GA_AttributeOperand &t) const;

    /// d = SYSfit(a, omin, omax, nmin, nmax);
    virtual bool	fit(GA_AttributeOperand &d,
				 GA_AttributeOperand &a,
				 GA_AttributeOperand &omin,
				 GA_AttributeOperand &omax,
				 GA_AttributeOperand &nmin,
				 GA_AttributeOperand &nmax) const;

    /// d = SYSfit(a, 0, 1, nmin, nmax);
    virtual bool	fit01(GA_AttributeOperand &d,
				 GA_AttributeOperand &a,
				 GA_AttributeOperand &nmin,
				 GA_AttributeOperand &nmax) const;


    /// @{
    /// Specializations of common method
    virtual bool	lerp(GA_Attribute &d, GA_Offset di,
				const GA_Attribute &a, GA_Offset ai,
				const GA_Attribute &b, GA_Offset bi,
				fpreal t) const = 0;
    virtual bool	lerp(GA_Attribute &d, const GA_Range &di,
				const GA_Attribute &a, const GA_Range &ai,
				const GA_Attribute &b, const GA_Range &bi,
				fpreal t) const = 0;
    virtual bool	lerp(GA_Attribute &d, const GA_Range &di,
				const GA_Attribute &a, const GA_Range &ai,
				const GA_Attribute &b, const GA_Range &bi,
				const GA_Attribute &t, const GA_Range &ti
			    ) const = 0;

    virtual bool	smooth(GA_Attribute &d, GA_Offset di,
				fpreal min, fpreal max,
				const GA_Attribute &a, GA_Offset ai) const = 0;
    virtual bool	smooth(GA_Attribute &d, const GA_Range &di,
				fpreal min, fpreal max,
				const GA_Attribute &a, const GA_Range &ai
			    ) const = 0;

    virtual bool	fit(GA_Attribute &d, GA_Offset di,
				const GA_Attribute &a, GA_Offset ai,
				fpreal omin, fpreal omax,
				fpreal nmin, fpreal nmax) const = 0;
    virtual bool	fit(GA_Attribute &d, const GA_Range &di,
				const GA_Attribute &a, const GA_Range &ai,
				fpreal omin, fpreal omax,
				fpreal nmin, fpreal nmax) const = 0;
    /// @}

    // The fit01() methods simply pass in 0 & 1 as omin/omax
    virtual bool	fit01(GA_Attribute &d, const GA_Range &di,
				const GA_Attribute &a, const GA_Range &ai,
				fpreal nmin, fpreal nmax) const;
    virtual bool	fit01(GA_Attribute &d, GA_Offset di,
				const GA_Attribute &a, GA_Offset ai,
				fpreal nmin, fpreal nmax) const;

    /// @{
    /// This macro can be used to implement the non-pure virtual attribute
    /// operand methods by calling the baseclass given (usually GA_AIFMath).
    /// For example, it might be used as: @code
    ///	    class MyTupleInterface : public GA_AIFTuple
    ///	    {
    ///	    public:
    ///		GA_AIFMATH_OPERAND_METHODS(GA_AIFTuple)
    ///		... implement rest of class
    ///	    }
    ///	@endcode
    #define GA_AIFINTERP_OPERAND_METHODS(BASECLASS) \
	virtual bool	lerp(GA_AttributeOperand &d, \
				 GA_AttributeOperand &a, \
				 GA_AttributeOperand &b, \
				 GA_AttributeOperand &t) const \
			{ return BASECLASS::lerp(d, a, b, t); } \
	virtual bool	smooth(GA_AttributeOperand &d, \
				 GA_AttributeOperand &min, \
				 GA_AttributeOperand &max, \
				 GA_AttributeOperand &t) const \
			{ return BASECLASS::smooth(d, min, max, t); } \
	virtual bool	fit(GA_AttributeOperand &d, \
				 GA_AttributeOperand &a, \
				 GA_AttributeOperand &omin, \
				 GA_AttributeOperand &omax, \
				 GA_AttributeOperand &nmin, \
				 GA_AttributeOperand &nmax) const \
			{ return BASECLASS::fit(d, a, omin, omax, nmin, nmax); }
    /// @}
};

#endif
