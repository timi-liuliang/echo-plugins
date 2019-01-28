/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This simply contains a list of token identifiers for primitive
 *	types.  Currently, there may be up to 32 types of primitives
 *	supported.  This may change (but not easily).  From the list
 *	below, it is apparent that this should be lots of primitives
 *	to work with.
 *
 *	Each primitive type must have a unique bit so that code can
 *	quickly search through the primitives it knows about.
 *
 */

#ifndef __GEO_PrimType_h__
#define __GEO_PrimType_h__

// The list of identifiers for the factory primitive types.  Any user defined
// types will be assigned unique identifiers following these.  In general, no
// assumptions about these factory identifiers remaining unchanged should be
// made.  See GA_PrimitiveDefinition.h for more.

#include <GA/GA_PrimitiveTypes.h>
#include <GA/GA_PrimitiveFamilyMask.h>

enum GEO_PrimitiveType
{
    GEO_PRIMNONE = GA_PRIMNONE,

    GEO_PRIMPOLY = GA_PRIMPOLY,
    GEO_PRIMNURBCURVE = GA_PRIMNURBCURVE,
    GEO_PRIMBEZCURVE = GA_PRIMBEZCURVE,

    GEO_PRIMMESH = GA_PRIMMESH,
    GEO_PRIMNURBSURF = GA_PRIMNURBSURF,
    GEO_PRIMBEZSURF = GA_PRIMBEZSURF,

    GEO_PRIMPASTESURF = GA_PRIMPASTESURF,

    GEO_PRIMCIRCLE = GA_PRIMCIRCLE,
    GEO_PRIMSPHERE = GA_PRIMSPHERE,
    GEO_PRIMTUBE = GA_PRIMTUBE,
    
    GEO_PRIMPART = GA_PRIMPART,

    GEO_PRIMMETABALL = GA_PRIMMETABALL,
    GEO_PRIMMETASQUAD = GA_PRIMMETASQUAD,
    GEO_PRIMMETALINE = GA_PRIMMETALINE,
    GEO_PRIMMETABEZ = GA_PRIMMETABEZ,
    GEO_PRIMMETATRI = GA_PRIMMETATRI,

    GEO_PRIMTRIFAN = GA_PRIMTRIFAN,
    GEO_PRIMTRISTRIP = GA_PRIMTRISTRIP,
    GEO_PRIMTRIBEZIER = GA_PRIMTRIBEZIER,
    GEO_PRIMVOLUME = GA_PRIMVOLUME,

    GEO_PRIMTETRAHEDRON = GA_PRIMTETRAHEDRON,

    GEO_PRIMPOLYSOUP = GA_PRIMPOLYSOUP,

    GEO_PRIMVDB = GA_PRIMVDB,

    GEO_NUM_INTERNAL_PRIM_TYPES	// sentinel value
};

// Convenience defines
#define GEO_FAMILY_FACE		GA_FAMILY_FACE
#define GEO_FAMILY_CURVE	GA_FAMILY_CURVE
#define GEO_FAMILY_HULL		GA_FAMILY_HULL
#define GEO_FAMILY_TPSURF	GA_FAMILY_TPSURF
#define GEO_FAMILY_QUADRIC	GA_FAMILY_QUADRIC
#define GEO_FAMILY_META		GA_FAMILY_META
#define GEO_FAMILY_SPLINE	GA_FAMILY_SPLINE
#define GEO_FAMILY_VOLUMEELEMENT	GA_FAMILY_VOLUMEELEMENT

#endif
