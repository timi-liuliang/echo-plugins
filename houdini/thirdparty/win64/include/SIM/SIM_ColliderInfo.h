/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ColliderInfo_h__
#define __SIM_ColliderInfo_h__

#include "SIM_API.h"
#include <UT/UT_Array.h>
#include "SIM_Collider.h"

/// This class holds all the information needed by a solver to do collision
/// detection for an object.
class SIM_API SIM_ColliderInfo
{
public:
	    		 SIM_ColliderInfo();
	    		 SIM_ColliderInfo(const SIM_ColliderInfo &src);
	    		 SIM_ColliderInfo(SIM_Object *affector,
				const SIM_Collider *collider,
				SIM_Collider::SIM_ImpactApplyType type,
				int priority);
	    		~SIM_ColliderInfo();

    SIM_Object				*getAffector() const;
    const SIM_Collider			*getCollider(SIM_Object *obj, const char *defaultlabel=0) const;
    SIM_Collider::SIM_ImpactApplyType	 getImpactApplyType() const;
    int					 getPriority() const;

    const SIM_ColliderInfo		&operator=(const SIM_ColliderInfo &src);

private:
    SIM_Object				*myAffector;
    const SIM_Collider			*myCollider;
    SIM_Collider::SIM_ImpactApplyType	 myImpactApplyType;
    int					 myPriority;
};

typedef UT_Array<SIM_ColliderInfo>	 SIM_ColliderInfoArray;

#endif

