/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __GU_Attractor_h__
#define __GU_Attractor_h__

#include "GU_API.h"
#include <UT/UT_Vector3.h>
#include <GA/GA_AttributeRef.h>
#include <GA/GA_Handle.h>

class GU_Detail;
class GU_AttractorOffsets;
class GU_PtAttractorOffsets;

class GU_API GU_Attractor
{
public:
  
     GU_Attractor (void);
    ~GU_Attractor (void);

    void 	setAttractor	  (GU_Detail *gdp);
    GU_Detail	*getAttractor() const { return attractor; }

    // Get the force on a point/particle
    GA_Offset	getAttractorForce
		    (const UT_Vector3 &pos,
		     UT_Vector3 &force,
		     int singleAttractor = 0,
		     unsigned num = 0);

    void	initPointAttractorForce();
    void	cleanPointAttractorForce();

private:
    GA_Offset	getPointAttractorForce( const UT_Vector3 &pos, 
					UT_Vector3 &force,
					int singleAttractor,
					unsigned num);

    GU_Detail*			attractor;
    GU_AttractorOffsets*	attOffsets;
    GU_PtAttractorOffsets*	ptAttOffsets;
    GA_ROAttributeRef		myNmlOffset;
    GA_ROHandleV3		myNmlHandle;
    int				myComputedNormal;
};

#endif
