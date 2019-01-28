/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Promote.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_Promote__
#define __GU_Promote__

#include "GU_API.h"

#include <GA/GA_Types.h>
#include <UT/UT_UniquePtr.h>

class GU_Detail;
class GA_Attribute;

/// Promote attributes from one owner to another
class GU_API GU_Promote
{
public:
    enum PROMOTE_METHOD
    {
	GU_PROMOTE_MAX,		// Choose maximum value of input
	GU_PROMOTE_MIN,		// Choose minimum value
	GU_PROMOTE_MEAN,	// Average values
	GU_PROMOTE_MODE,	// Mode (most common)
	GU_PROMOTE_MEDIAN,	// Choose median value
	GU_PROMOTE_SUM,		// Sum of values
	GU_PROMOTE_SUMSQUARE,	// Sum of the squares
	GU_PROMOTE_RMS,		// Root Mean Square
	GU_PROMOTE_FIRST,	// First
	GU_PROMOTE_LAST,	// Last
    };

    /// Promote the given attribute to the new attribute class.  The new
    /// attribute is returned.
    static GA_Attribute	*promote(GU_Detail &gdp,
				GA_Attribute *attrib,
				GA_AttributeOwner new_owner,
				bool destroy_existing = true,
				PROMOTE_METHOD method = GU_PROMOTE_MEAN,
				const char *new_name=NULL,
                                const GA_Attribute *piece_attrib = nullptr);

    /// Promote the given (tuple) attribute to the new attribute class into a
    /// newly created detached attribute.
    static UT_UniquePtr<GA_Attribute>
			 create(GA_AttributeOwner dst_owner,
				const GA_Attribute &src_attrib,
				PROMOTE_METHOD method = GU_PROMOTE_MEAN);

private:
};

#endif

