/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PackedContextext.h (GU Library, C++)
 *
 * COMMENTS:
 *	Provides context information to share between multiple unpack
 *	operations.
 */

#ifndef __GU_PackedContext__
#define __GU_PackedContext__

#include "GU_API.h"
#include <GA/GA_SharedDataContext.h>
#include <GA/GA_SharedDataHandle.h>

class GU_API GU_PackedContext
{
public:
	     GU_PackedContext();
    virtual ~GU_PackedContext();

    GA_SharedDataContext	&sharedDataContext() { return mySharedDataContext; }

protected:

    GA_SharedDataContext	mySharedDataContext;
};

#endif


