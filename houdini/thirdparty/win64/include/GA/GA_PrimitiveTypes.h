/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PrimitiveTypes.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_PrimitiveTypes__
#define __GA_PrimitiveTypes__

#include "GA_API.h"

// The list of identifiers for the factory primitive types.  Any user defined
// types will be assigned unique identifiers following these.  In general, no
// assumptions about these factory identifiers remaining unchanged should be
// made.  See GA_PrimitiveDefinition.h for more.

enum
{
    GA_PRIMNONE = 0,

    GA_PRIMPOLY,
    GA_PRIMNURBCURVE,
    GA_PRIMBEZCURVE,

    GA_PRIMMESH,
    GA_PRIMNURBSURF,
    GA_PRIMBEZSURF,

    GA_PRIMPASTESURF,

    GA_PRIMCIRCLE,
    GA_PRIMSPHERE,
    GA_PRIMTUBE,
    
    GA_PRIMPART,

    GA_PRIMMETABALL,
    GA_PRIMMETASQUAD,
    GA_PRIMMETALINE,
    GA_PRIMMETABEZ,
    GA_PRIMMETATRI,

    GA_PRIMTRIFAN,
    GA_PRIMTRISTRIP,
    GA_PRIMTRIBEZIER,
    GA_PRIMVOLUME,

    GA_PRIMTETRAHEDRON,

    GA_PRIMPOLYSOUP,
    GA_PRIMVDB,

    GA_PRIMINTERNALSENTINEL	// sentinel value
};

#endif
