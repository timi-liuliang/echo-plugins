/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFStdInterp.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface for Interpolation
 */

#ifndef __GA_AIFStdInterp__
#define __GA_AIFStdInterp__

#include "GA_API.h"
#include "GA_AIFInterp.h"
#include "GA_Range.h"
#include "GA_Types.h"

#include <SYS/SYS_Types.h>

class GA_Attribute;
class GA_WeightedSum;


/// @brief Generic implementation of the GA_AIFInterp interface using AIFTuple
///
/// The GA_AIFInterp class provides a pure virtual implementation requiring
/// developers to implement all specializations of the methods.  This class
/// provides these specializations using the AIFTuple methods to get/set
/// data.
///
/// As a developer, you may want to provide the GA_AIFInterp interface, but
/// may not want to have to provide all the specialized methods.  You can
/// subclass from this interface (rather than GA_AIFInterp directly).
///
/// However, <b>be warned</b> that if the GA_AIFInterp iterface changes,
/// you may end up missing opportunities for specialization.
/// @see GA_AIFMath, GA_AIFStdMath
///
class GA_API GA_AIFStdInterp : public GA_AIFInterp 
{
public:
	     GA_AIFStdInterp() {}
    virtual ~GA_AIFStdInterp();

    /// Note: Weighted sums can only be performed on a single element, and thus
    /// don't have a generic GA_AttributeOperand method.
    /// These methods @b must be implemented for an interpolation interface.
    virtual bool	startSum(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum) const = 0;
    virtual bool	addSum(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				const GA_Attribute &a, GA_Offset ai) const = 0;
    virtual bool	endSum(GA_Attribute &d, GA_Offset di,
				const GA_WeightedSum &sum,
				fpreal scale_factor = 1) const = 0;

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

    virtual bool	smooth(GA_Attribute &d, GA_Offset di,
				fpreal min, fpreal max,
				const GA_Attribute &a, GA_Offset ai) const = 0;
    virtual bool	smooth(GA_Attribute &d, const GA_Range &di,
				fpreal min, fpreal max,
				const GA_Attribute &a, const GA_Range &ai)const;

    virtual bool	fit(GA_Attribute &d, GA_Offset di,
				const GA_Attribute &a, GA_Offset ai,
				fpreal omin, fpreal omax,
				fpreal nmin, fpreal nmax) const = 0;
    virtual bool	fit(GA_Attribute &d, const GA_Range &di,
				const GA_Attribute &a, const GA_Range &ai,
				fpreal omin, fpreal omax,
				fpreal nmin, fpreal nmax) const = 0;

    /// @{
    /// Methods implemented in GA_AIFInterp
    GA_AIFINTERP_OPERAND_METHODS(GA_AIFInterp)
    /// @}
};

#endif
