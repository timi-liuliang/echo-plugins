/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFDelta.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface for detecting modified elements
 */

#ifndef __GA_AIFDelta__
#define __GA_AIFDelta__

#include "GA_API.h"
#include "GA_Range.h"

class GA_Attribute;

class GA_API GA_AIFDelta 
{
public:
	     GA_AIFDelta();
    virtual ~GA_AIFDelta();

    virtual GA_Range changedElements(const GA_Attribute *a, const GA_Attribute *b) const = 0;
};

#endif
