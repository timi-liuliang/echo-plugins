/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Wire.h (GU Library, C++)
 *
 * COMMENTS:	Utility methods for blending wires
 */

#ifndef __GU_Wire__
#define __GU_Wire__

#include "GU_API.h"
#include <GEO/GEO_Primitive.h>
#include <UT/UT_Array.h>
#include <UT/UT_Quaternion.h>

struct GU_API guWireSegmentParms
{
    guWireSegmentParms() {}
    guWireSegmentParms(fpreal len, const UT_Vector3 &r) : myLength(len), myRotation(r) {}
    fpreal myLength;
    UT_Vector3 myRotation; // log map of rotation
};

class GU_API GU_Wire
{
public:
    // creates a description of how to reconstruct the curve using a sequence
    // of bends and segment lengths
    static void extractWireSpecs(const GEO_Primitive *prim,
				 UT_Quaternion startorient,
				 UT_Array<guWireSegmentParms> &segs);

    // construct a curve from a sequence of bends and segment lengths
    static void buildWireFromSpecs(UT_Vector3Array &out, UT_Vector3 startpos,
				   UT_Quaternion startorient,
				   const UT_Array<guWireSegmentParms> &segs);
};

#endif
