/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFCondInterp.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface for Interpolation
 */

#ifndef __GA_AIFCondInterp__
#define __GA_AIFCondInterp__

#include "GA_API.h"
#include "GA_AIFInterp.h"
#include "GA_Range.h"
#include "GA_Types.h"

#include <SYS/SYS_Types.h>


class GA_Attribute;
class GA_WeightedSum;


/// @brief Conditional copy implementation of the GA_AIFInterp interface
/// using AIFTuple
///
/// The GA_AIFInterp class provides a pure virtual implementation requiring
/// developers to implement all specializations of the methods.  This class
/// provides these specializations using the AIFTuple methods to get/set
/// data.
///
/// This class performs interpolation by conditional copy.  That is, rather
/// than blending the values, the values are copied as discrete elements.
///
/// @see GA_AIFMath, GA_AIFStdMath, GA_AIFStdInterp
///
class GA_API GA_AIFCondInterp : public GA_AIFInterp 
{
public:
	     GA_AIFCondInterp() {}
    virtual ~GA_AIFCondInterp();

    /// Return whether this interpolator is a conditional interpolation or
    /// whether values are actually interpolated.
    virtual bool	isConditionalInterpolation() const { return true; }

    /// The weighted sum will copy over the element which has the greatest
    /// contribution to the sum.  The initialization is a no-op, so any
    /// initialization you need to perform should be done in a sub-class
    /// implementation.
    virtual bool	startSum(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum) const;
    virtual bool	addSum(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				const GA_Attribute &a, GA_Offset ai) const;
    virtual bool	addSquare(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				const GA_Attribute &a, GA_Offset ai) const;
    virtual bool	endSum(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				fpreal scale_factor = 1) const;
    virtual bool	endSquare(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				fpreal scale_factor = 1) const;

    // Specializations of some of the common methods
    virtual bool	lerp(GA_Attribute &d, GA_Offset di,
				const GA_Attribute &a, GA_Offset ai,
				const GA_Attribute &b, GA_Offset bi,
				fpreal t) const;
    virtual bool	lerp(GA_Attribute &d, const GA_Range &di,
				const GA_Attribute &a, const GA_Range &ai,
				const GA_Attribute &b, const GA_Range &bi,
				fpreal t) const;
    virtual bool	lerp(GA_Attribute &d, const GA_Range &di,
				const GA_Attribute &a, const GA_Range &ai,
				const GA_Attribute &b, const GA_Range &bi,
				const GA_Attribute &t, const GA_Range &ti)const;

    /// Since smooth() blends between floating point values, it is not
    /// implemented as a conditional copy (returns false).  Sub-classes
    /// may choose to implement this differently.
    virtual bool	smooth(GA_Attribute &d, GA_Offset di,
				fpreal min, fpreal max,
				const GA_Attribute &a, GA_Offset ai) const;
    virtual bool	smooth(GA_Attribute &d, const GA_Range &di,
				fpreal min, fpreal max,
				const GA_Attribute &a, const GA_Range &ai)const;

    virtual bool	fit(GA_Attribute &d, GA_Offset di,
				const GA_Attribute &a, GA_Offset ai,
				fpreal omin, fpreal omax,
				fpreal nmin, fpreal nmax) const;
    virtual bool	fit(GA_Attribute &d, const GA_Range &di,
				const GA_Attribute &a, const GA_Range &ai,
				fpreal omin, fpreal omax,
				fpreal nmin, fpreal nmax) const;

    /// @{
    /// Methods implemented in GA_AIFInterp
    GA_AIFINTERP_OPERAND_METHODS(GA_AIFInterp)
    /// @}
};

#endif
