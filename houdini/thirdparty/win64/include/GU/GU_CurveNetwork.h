/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_CurveNetwork.C (C++)
 *
 * COMMENTS:
 *	This is the filter used to create Skinned, Coons, and Gordon patches.
 */

#ifndef __GU_CurveNetwork_h__
#define __GU_CurveNetwork_h__

#include "GU_API.h"
#include "GU_Skin.h"

class GA_PrimitiveGroup;

class GU_API GU_CurveNetworkParms : public GU_SkinParms
{
public:
    GU_CurveNetworkParms(); 
    ~GU_CurveNetworkParms() { }
    virtual void InitializePrimArrays(GEO_Detail* parent_detail);

    int			 	 keepPrimitives;
    int			 	 outputPolys;
    const GA_PrimitiveGroup	*vFaces;

    UT_Array<GEO_Primitive*> *myVPrimsToProcess;
};

#endif
