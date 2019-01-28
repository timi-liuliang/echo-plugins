/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __RBD_ColliderBFA_h__
#define __RBD_ColliderBFA_h__

#include "RBD_API.h"
#include "RBD_Object.h"

#include <SIM/SIM_ColliderBFA.h>
#include <SIM/SIM_Isect.h>

class RBD_API RBD_ColliderBFA : public SIM_ColliderBFA
{
public:
    /// Finds intersection of RBD objects.  Fills the given queue with all
    /// of the two way intersections.
    void    		intersectRBDObjects(SIM_IsectPriorityQueue &queue, 
					RBD_Object *obj_a, RBD_Object *obj_b,
					bool findany, 
					bool findearliest,
					fpreal timetol,
					const SIM_Time &starttime,
					const SIM_Time &endtime,
					SIM_ImpactApplyType impactapplytype,
					int impactflags,
					int child_a_id = -1, 
					int child_b_id = -1)  const;
protected:
    explicit		 RBD_ColliderBFA(const SIM_DataFactory *factory);
    virtual		~RBD_ColliderBFA();

    virtual bool	doCheckTangle() const
    { return true; } 
    virtual BV_LeafIterator
			*createLeafIt(const SIM_Object &object,
			        const SIM_Time &timestep,
			        bool startPosOnly) const;
    virtual void	 convertLeafToPrim(const BV_LeafIterator &leafIt,
			        int leafId,
				int &outPrimId, int &outEdgeId) const;
    virtual fpreal	 getLeafRadius(const BV_LeafIterator &leafIt,
				int leafId) const;

private:
    static const SIM_DopDescription *getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(RBD_ColliderBFA,
			SIM_ColliderBFA,
			"ThinPlate/ThinPlate Collider (Bridson)",
			getDopDescription());
};

#endif

