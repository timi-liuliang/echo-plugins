/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GD_PrimType.h (GD Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __GD_PrimType_h__
#define __GD_PrimType_h__

#include <GA/GA_PrimitiveFamilyMask.h>
#include <GA/GA_PrimitiveTypes.h>

// The list of identifiers for the factory primitive types.  Any user defined
// types will be assigned unique identifiers following these.  In general, no
// assumptions about these factory identifiers remaining unchanged should be
// made.  See GA_PrimitiveDefinition.h for more.
enum
{
    GD_PRIMNONE = GA_PRIMNONE,

    GD_PRIMPOLY = GA_PRIMPOLY,
    GD_PRIMNURBCURVE = GA_PRIMNURBCURVE,
    GD_PRIMBEZCURVE = GA_PRIMBEZCURVE,

    GD_NUM_INTERNAL_PRIM_TYPES	// sentinel value
};

// Convenience family masks for grouping the factory types to allow for fast
// iteration and detection.
#define	GD_FAMILY_FACE		GA_FAMILY_FACE
#define	GD_FAMILY_CURVE		GA_FAMILY_CURVE

#endif
