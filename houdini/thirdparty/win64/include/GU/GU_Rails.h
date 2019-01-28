/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Rails (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __GU_Rails_h__
#define __GU_Rails_h__

#include "GU_API.h"
#include "GU_Types.h"
#include <UT/UT_Vector3.h>

class GA_PrimitiveGroup;
class GU_Detail;

class GU_API GU_RailXSection 
{
public:
    GU_RailXSection(); 
   ~GU_RailXSection();

    UT_Vector3 	Ivec, Jvec;	// I and J component axes
    UT_Vector3 	midpoint;	// midpoint of the attach points 
    float	length;		// half length between two attach points
};

class GU_API GU_RailParms
{
public:
    GU_RailParms();
   ~GU_RailParms() {}

    const GU_Detail		*rails;		// the rails
    const GU_Detail		*xSection;     	// the cross-section
    GU_CrossSectionType 	 cycleType; 	// cycleType
    int 			 attach1, attach2; // vertices to attach to
    float 			 scale;        	// scale
    float 			 roll;         	// pitch
    int 			 useDirection;  // direction toggle
    float 			 nx, ny, nz;  	// user specified direction
    int 		 	 noFlipping;    // attempt to fix the angle
    int 			 pairRails;     // join the rails in pairs
    int 			 joinFirstLast; // join first and last rails
    int 			 stretchToRails; // stretch the xsect to rails
    int                          outputGroups;  // outut rail groups switch
    char                        *railGrpName;   // raail groups prefix name
    const GA_PrimitiveGroup	*railGroup;    	// optional rail group
    const GA_PrimitiveGroup 	*xGroup;    	// optional xsec group
};

#endif
