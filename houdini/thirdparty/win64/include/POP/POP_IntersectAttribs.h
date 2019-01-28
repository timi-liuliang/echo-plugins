/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	POP_IntersectAttribs.h (POP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __POP_INTERSECTATTRIBS_H_INCLUDED__
#define __POP_INTERSECTATTRIBS_H_INCLUDED__

#include "POP_API.h"
#include "POP_ContextData.h"
#include "GA_PageHandleUtil.h"

#include <GA/GA_PageHandle.h>

#include <hboost/fusion/include/adapt_struct.hpp>
#include <hboost/fusion/include/adapt_struct_named.hpp>
#include <hboost/fusion/include/define_struct.hpp>


#define POP_INIT_ATTRIB(NAME)	\
	    my##NAME(data->get##NAME##Offset().getAttribute()) \
	    /**/

#define POP_DECLARE_ATTRIB(NAME, TYPE) \
	    GA_RWPageHandle##TYPE		    my##NAME; \
	    GA_RWPageHandle##TYPE::value_type	    my##NAME##Value; \
	    /**/

// Helper class for use with POP_IntersectBase
struct POP_API POP_IntersectAttribs
{
    POP_IntersectAttribs(POP_ContextData *data)
	: POP_INIT_ATTRIB(HitPos)
	, POP_INIT_ATTRIB(HitPosUV)
	, POP_INIT_ATTRIB(HitNormal)
	, POP_INIT_ATTRIB(HitUV)
	, POP_INIT_ATTRIB(HitDiffuse)
	, POP_INIT_ATTRIB(NumHit)
	, POP_INIT_ATTRIB(HitID)
	, POP_INIT_ATTRIB(HitTime)
	, POP_INIT_ATTRIB(Distance)
	, myPos(data->getDetail()->getP())
	, POP_INIT_ATTRIB(PosPrim)
	, POP_INIT_ATTRIB(PosUV)
	, POP_INIT_ATTRIB(Backtrack)
	, POP_INIT_ATTRIB(SpeedLimit)
    {
    }

    // Used by getAttributePointers() and setAttributePointers()
    POP_DECLARE_ATTRIB(HitPos,V3)
    POP_DECLARE_ATTRIB(HitPosUV,V2)
    POP_DECLARE_ATTRIB(HitNormal,V3)
    POP_DECLARE_ATTRIB(HitUV,V3)
    POP_DECLARE_ATTRIB(HitDiffuse,V3)
    POP_DECLARE_ATTRIB(NumHit,I)
    POP_DECLARE_ATTRIB(HitID,I)
    POP_DECLARE_ATTRIB(HitTime,F)
    POP_DECLARE_ATTRIB(Distance,F)

    // Other attributes used by the various intersect methods
    POP_DECLARE_ATTRIB(Pos,V3)
    POP_DECLARE_ATTRIB(PosPrim,I)
    POP_DECLARE_ATTRIB(PosUV,V2)
    POP_DECLARE_ATTRIB(Backtrack,V4)
    POP_DECLARE_ATTRIB(SpeedLimit,V2)
};

#undef POP_DECLARE_ATTRIB
#undef POP_INIT_ATTRIB

// Create hboost::fusion sequence for all members of POP_IntersectAttribs
HBOOST_FUSION_ADAPT_STRUCT(
    POP_IntersectAttribs,
    (GA_RWPageHandleV3,	myHitPos)
    (GA_RWPageHandleV2,	myHitPosUV)
    (GA_RWPageHandleV3,	myHitNormal)
    (GA_RWPageHandleV3,	myHitUV)
    (GA_RWPageHandleV3,	myHitDiffuse)
    (GA_RWPageHandleI,	myNumHit)
    (GA_RWPageHandleI,	myHitID)
    (GA_RWPageHandleF,	myHitTime)
    (GA_RWPageHandleF,	myDistance)
    //
    (GA_RWPageHandleV3,	myPos)
    (GA_RWPageHandleI,	myPosPrim)
    (GA_RWPageHandleV2,	myPosUV)
    (GA_RWPageHandleV4,	myBacktrack)
    (GA_RWPageHandleV2,	mySpeedLimit)
)

// Create POP_AdaptedAttribs::IntersectBase as a hboost::fusion sequence 
// of POP_IntersectAttribs.
HBOOST_FUSION_ADAPT_STRUCT_NAMED_NS(
    POP_IntersectAttribs,
    (POP_AdaptedAttribs),
    IntersectBase,
    (GA_RWPageHandleV3,	myHitPos)
    (GA_RWPageHandleV2,	myHitPosUV)
    (GA_RWPageHandleV3,	myHitNormal)
    (GA_RWPageHandleV3,	myHitUV)
    (GA_RWPageHandleV3,	myHitDiffuse)
    (GA_RWPageHandleI,	myNumHit)
    (GA_RWPageHandleI,	myHitID)
    (GA_RWPageHandleF,	myHitTime)
    (GA_RWPageHandleF,	myDistance)
)

// Create POP_CollideAttribs as the set of attributes used for collision
HBOOST_FUSION_DEFINE_STRUCT(
    , POP_CollideAttribs,
    (GA_RWPageHandleI,	myState)
    (GA_ROPageHandleF,	myLife)
    (GA_RWPageHandleV3,	myVelocity)
    (GA_RWPageHandleV3,	myAccel)
)

POP_API extern void POPbindCollideAttribs(
			POP_CollideAttribs &collide_attribs,
			POP_ContextData *data);


#endif // __POP_INTERSECTATTRIBS_H_INCLUDED__
