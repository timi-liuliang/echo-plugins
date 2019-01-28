/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PrimitiveTypeId.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_PrimitiveTypeIdHash__
#define __GA_PrimitiveTypeIdHash__

#include "GA_API.h"
#include "GA_PrimitiveTypeId.h"
#include <SYS/SYS_Hash.h>
#include <SYS/SYS_Inline.h>

static SYS_FORCE_INLINE SYS_HashType
hash_value(const GA_PrimitiveTypeId &pi)
{
    return SYShash(pi.get()); 
}

#endif
