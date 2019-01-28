/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	subdivided surfaces (C++)
 *
 * COMMENTS:
 */

#ifndef __GQ_Subdivide_h__
#define __GQ_Subdivide_h__

#include "GQ_API.h"
enum GQ_DivideChoice
{
    GQ_NO_DIVISION,
    GQ_DIVIDE_EDGES,
    GQ_TRIANGULATE
};

class GQ_API GQ_SubdivideParms
{
public:
    GQ_SubdivideParms()
    {
	iterations = 1;
	pullHoles = true;
	bias = 1;
	stitchHoles = false;
	divideChoice = GQ_DIVIDE_EDGES;
	smoothvertex = true;
	consistentTopology = false;
	linearCreases = false;
    }

    int			iterations;
    int			pullHoles;
    float		bias;
    int			stitchHoles;
    GQ_DivideChoice	divideChoice;
    int			smoothvertex;
    bool		consistentTopology;
    bool		linearCreases;
};
    
#endif
