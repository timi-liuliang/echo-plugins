/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library.
 *
 * COMMENTS:	Parameters for EdgeCrease.
 *
 */
#ifndef __GU_EdgeCreaseParms_h__
#define __GU_EdgeCreaseParms_h__

#include "GU_API.h"
#include <UT/UT_Vector4.h>

class GA_EdgeGroup;

enum GU_EdgeCreaseAction
{
    GU_EDGECREASE_ADDTO,
    GU_EDGECREASE_SET,
    GU_EDGECREASE_DELETE
};

class GU_API GU_EdgeCreaseParms
{
public:
    GU_EdgeCreaseParms() :
	myGroup(0),
	myAction(GU_EDGECREASE_ADDTO),
	myCreaseValue(0.0f),
	myColdColour(0.0f, 0.0f, 1.0f),
	myHotColour (1.0f, 0.0f, 0.0f),
	mySetColour(false),
	myAttribName("creaseweight")
	{}

    const GA_EdgeGroup		*myGroup;
    GU_EdgeCreaseAction		 myAction;
    float			 myCreaseValue;
    UT_Vector3			 myColdColour;
    UT_Vector3			 myHotColour;
    bool			 mySetColour;
    const char			*myAttribName;
};


#endif
