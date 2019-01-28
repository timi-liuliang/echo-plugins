/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_GrainObject.C ( GAS Library, C++)
 *
 * COMMENTS:	Create composite-sphere grain object.
 *
 */

#ifndef __GAS_GrainObject__
#define __GAS_GrainObject__

#include "GAS_API.h"

#include <UT/UT_Array.h>
#include <UT/UT_Vector3.h>

#include <GU/GU_Detail.h>

class GAS_API GAS_GrainSphere
{
public:
    UT_Vector3		myCenter;
    fpreal		myRadius;
    int			myParticle;
    int                 mySphereID;  
};

class GAS_API GAS_GrainObject
{
public:
    GAS_GrainObject();
    ~GAS_GrainObject();

    // Build an instanced grain from a base grain and an
    // appropriate transform.
    void	instanceFrom(const GAS_GrainObject *base, 
			     int ptnum, 
			     const UT_DMatrix4 &xform);

    // Build a grain from the gdp that defines spheres
    // instanced onto a geometry.
    void        buildGrainFromGdp(const GU_Detail *gdp);

    UT_Array<GAS_GrainSphere>	mySpheres;
    int					myParticle;
};

#endif
