/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO Library.
 *
 * COMMENTS:	Geometry mirroring
 *
 */
#ifndef __GEO_Mirror_h__
#define __GEO_Mirror_h__

#include "GEO_API.h"
#include <UT/UT_IntArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_Vector3.h>

class GA_PrimitiveGroup;
class GA_GBPointRedirectArray;
class GEO_Detail;

// Parameters for the mirror method
class GEO_API GEO_MirrorParms
{
public:

    // Reverse option for primitives that have been mirrored
    enum GEO_ReverseOption
    {
	GEO_MIRROR_NO_REVERSE = 0,
	GEO_MIRROR_REVERSE    = 1,
	GEO_MIRROR_REVERSE_U  = 2,
	GEO_MIRROR_REVERSE_V  = 3
    };

    // Constructor
    GEO_MirrorParms(const GEO_Detail		*src_gdp,
		    const GA_PrimitiveGroup	*prims_to_mirror,
		    UT_Vector3			 mirror_normal,
		    const UT_Vector3		&mirror_origin,
		    float			 dist,
		    bool			 keep_original,
		    GEO_ReverseOption		 reverse_direction,
		    bool			 consolidate_seam,
		    float			 consolidate_tolerance,
		    bool			 mirror_fast,
		    GA_GBPointRedirectArray	*dest_to_src_ptarray,
		    UT_IntArray			&newprims)
	: mySourceGdp(src_gdp)
	, myPrimsToMirror(prims_to_mirror)
	, myMirrorNormal(mirror_normal)
	, myMirrorOrigin(mirror_origin)
	, myDist(dist)
	, myKeepOriginal(keep_original)
	, myReverseDirection(reverse_direction)
	, myConsolidateSeam(consolidate_seam)
	, myConsolidateTolerance(consolidate_tolerance)
	, myMirrorFast(mirror_fast)
	, myDestToSrcPointArray(dest_to_src_ptarray)
	, myNewPrims(newprims)
	, myPrimBase(0)
	, myQuadricMatrices(0)
      {
      }


    const GEO_Detail		*mySourceGdp; // Source gdp
    const GA_PrimitiveGroup	*myPrimsToMirror; // Group belonging to srcgdp

    UT_Vector3			 myMirrorNormal; // must be normalized
    const UT_Vector3		&myMirrorOrigin;
    float			 myDist;
    bool			 myKeepOriginal;
    GEO_ReverseOption		 myReverseDirection;
    bool			 myConsolidateSeam;
    float			 myConsolidateTolerance;



    // myMirrorFast option, when true, tells mirror that the geometry is all
    // in place, all we have to do is move points.  The redirect array must
    // be not be null, and will be used to figure out how to map the points
    // to be mirrored.  If myMirrorFast is false and mySrcToDestPtArray exists
    // then it is filled with the information you can use next time around.
    bool			 myMirrorFast;
    GA_GBPointRedirectArray	*myDestToSrcPointArray;

    // This is an output when !myMirrorFast and an input if myMirrorFast is
    // set.  It is used rather than a group because it isn't carried around
    // with the gdp.
    UT_IntArray			&myNewPrims;

    // This is the base number from which we offset the index into the
    // quadric matrix array.  The reason we need to keep around a quadric
    // matrix array is if we're doing a fast mirror, we need to remember 
    // our original transform.  And we avoid doing the redirect stuff we
    // had to do for points.
    exint			 myPrimBase;
    UT_Array<UT_Matrix3D *>	*myQuadricMatrices;
};

#endif
