/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_DetailGroupPair.h
 *
 * COMMENTS:
 *
 */

#ifndef __GU_DetailGroupPair_H__
#define __GU_DetailGroupPair_H__

#include "GU_API.h"
class GU_Detail;
class GA_Group;

class GU_API GU_DetailGroupPair
{
public:
    GU_DetailGroupPair()
    {
	myGdp = 0;
	myGroup = 0;
    }
    const GU_Detail	*myGdp;
    const GA_Group	*myGroup;
};

#endif
