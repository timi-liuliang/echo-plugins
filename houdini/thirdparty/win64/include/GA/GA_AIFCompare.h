/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFCompare.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface class for comparison methods
 */

#ifndef __GA_AIFCompare__
#define __GA_AIFCompare__

#include "GA_API.h"
#include "GA_Types.h"

#include <SYS/SYS_Types.h>


class GA_Attribute;
class GA_Range;


/// @brief Attribute Interface class to perform comparisons on attributes
///
/// This class provides the interface to compare the attribute data.  
class GA_API GA_AIFCompare 
{
public:
	     GA_AIFCompare(); 
    virtual ~GA_AIFCompare();

    /// @{
    /// Compares attributes a and b for a given offset or range, and sets
    /// the result argument to true if the two attributes are deemed equal,
    /// or to false if they are different.
    /// Returns true if the comparison operation was successful, or 
    /// false if there was some kind of error.
    virtual bool    isEqual(bool &result,
			    const GA_Attribute &a, GA_Offset ai,
			    const GA_Attribute &b, GA_Offset bi) const = 0;
    virtual bool    isEqual(bool &result,
			    const GA_Attribute &a, const GA_Range &ai,
			    const GA_Attribute &b, const GA_Range &bi) const =0;

    virtual bool    isAlmostEqual(bool &result,
			    const GA_Attribute &a, GA_Offset ai,
			    const GA_Attribute &b, GA_Offset bi,
			    int ulps = 50) const = 0;
    virtual bool    isAlmostEqual(bool &result,
			    const GA_Attribute &a, const GA_Range &ai,
			    const GA_Attribute &b, const GA_Range &bi,
			    int ulps = 50) const = 0;
    /// @}

    virtual uint    hash(const GA_Attribute &a, const GA_Offset i) const { return 0; }
};

#endif

