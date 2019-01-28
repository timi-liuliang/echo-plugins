/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_Degrain.h
 *
 * COMMENTS:
 *	Removes grain from an image.
 */
#ifndef RU_DEGRAIN_H
#define RU_DEGRAIN_H

#include "RU_API.h"
#include "RU_Algorithm.h"

class RU_API RU_Degrain : public RU_Algorithm
{
public:
    RU_Degrain() : mySize(5) {}

    DECLARE_FILTER_OP(RU_DegrainOp);
    
    int	mySize;
};

#endif
