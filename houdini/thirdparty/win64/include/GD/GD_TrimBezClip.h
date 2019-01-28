/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This handles bezier clipping of trim pieces, allowing fast
 * point in tests.
 * 
 */

#ifndef __GD_TrimBezClip_h_
#define __GD_TrimBezClip_h_

#include "GD_API.h"
#include <UT/UT_VectorTypes.h>
#include <GA/GA_Defines.h>
class GD_TrimPieceRBezCurve;

enum GD_TrimBezClipType
{
    GD_TrimBezClipA = 0,
    GD_TrimBezClipBCross = 1,
    GD_TrimBezClipBNoCross = 2,
    GD_TrimBezClipC = 3,
    GD_TrimBezClipAorB = 4,
    GD_TrimBezClipUnknown = 5
};


class GD_API GD_TrimBezClip
{
public:
    // Initialize for point in tests:
    GD_TrimBezClip();
    ~GD_TrimBezClip();

    int			getIsectParity(const GD_TrimPieceRBezCurve &curve, 
				       const UT_Vector2 &pt, float tol = 1E-4F);

private:
    // Intializes CV for order.
    GD_TrimBezClip(int order);

    // If left or right NULL, does not evaluate it.
    // ucut e [0,1]
    int				 splitAt(GD_TrimBezClip *left, 
					 GD_TrimBezClip *right,
					 float ucut,
					 UT_Vector2 bloss[][GA_MAXORDER]) 
					    const;

    // Returns maximum size of CV convex hull:
    float			 sizeMax();

    // Find min/max x/y isections:
    void			 findXCVIsections(float &umin, float &umax);
    void			 findYCVIsections(float &umin, float &umax);

    GD_TrimBezClipType		 classify(float tol);

    // Private stack of clips
    GD_TrimBezClip		*myNext;

    GD_TrimBezClipType	 	 myType;
    UT_Vector2			*myCV;
    int				 myDeg;
    unsigned int		 myCVsize;

    // Blossom array:
    UT_Vector2			*myBloss;
};

#endif
