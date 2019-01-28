/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Shrink Tools (C++)
 *
 * COMMENTS:	Shrink Tools header file
 *
 */

#ifndef __GU_ShrinkTools_h__
#define __GU_ShrinkTools_h__

#include "GU_API.h"
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Types.h>

namespace GU_ShrinkTools
{
    // Compute the interesection of a line segment with a particular plane given
    // by normal and distance from the origin.
    // pre: the line is defined by the two positions: pos1 and pos2
    //	 the plane is defined by the normal and the dist which is the
    //	 orthogonal distance from the origin to the plane
    //	 IMPORTANT: the line segment is assumed to be intersecting the plane
    // post: the output is alpha which is the normalized distance from the point v
    //	 to the computed intersection x.
    //	 if line is close to prependicular to normal then x is the midpoint
    //	 if pos1 is too close to pos2 then x is u
    GU_API extern void computeLinePlaneIntersection(const UT_Vector4R &pos1,
						    const UT_Vector4R &pos2,
				    	     	    const UT_Vector3R &normal,
				    	     	    const fpreal      &dist,
				    	     	    fpreal	      &alpha,
				    	     	    UT_Vector4R       &x );
	    
    // same as above but ignoring the 4th coordinate
    GU_API extern void computeLinePlaneIntersection(const UT_Vector3R &pos1,
						    const UT_Vector3R &pos2,
				    	     	    const UT_Vector3R &normal,
				    	     	    const fpreal      &dist,
				    	     	    fpreal	      &alpha,
				    	     	    UT_Vector3R       &x );
};
#endif
