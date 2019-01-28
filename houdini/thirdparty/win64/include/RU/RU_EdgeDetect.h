/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_EdgeDetect.h
 *
 * COMMENTS:
 *	Does a edgeDetect mult and/or shift.
 */
#ifndef RU_EDGEDETECT_H
#define RU_EDGEDETECT_H

#include "RU_API.h"
#include "RU_Algorithm.h"

#define EDGE_DETECT_PREWITT	0
#define EDGE_DETECT_SOBEL	1

class RU_API RU_EdgeDetect : public RU_Algorithm
{
public:
		 RU_EdgeDetect();
    virtual	~RU_EdgeDetect() { ; }

    DECLARE_FILTER_OP(RU_EdgeDetectOp)
    
    // Parameters.
    int		myType;
    float	mySX;
    float	mySY;
};


#endif
