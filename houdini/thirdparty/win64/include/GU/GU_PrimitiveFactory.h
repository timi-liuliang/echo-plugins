/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimitiveFactory.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PrimitiveFactory__
#define __GU_PrimitiveFactory__

#include "GU_API.h"

class GA_PrimitiveFactory;

/// The first call to this function is _NOT_ threadsafe.  Typically, the first
/// call will occur as a result of a static initialization.
GU_API extern GA_PrimitiveFactory &GUgetFactory();

#endif
