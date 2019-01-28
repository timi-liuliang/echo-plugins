/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __RBD_ColliderSDF_h__
#define __RBD_ColliderSDF_h__


#include "RBD_API.h"
#include "RBD_Solver.h"
#include "RBD_Object.h"
#include "RBD_Utils.h"

#include <SIM/SIM_Collider.h>
#include <SIM/SIM_Utils.h>

class SIM_Object;
class SIM_Random;
class SIM_SDF;
class RBD_State;
class RBD_ContactGraph;
class RBD_ObjectTree;

class RBD_API RBD_ColliderSDF : public SIM_Collider
{
public:
    /// Finds intersection of RBD objects.  Fills the given queue with all
    /// of the two way intersections.
    void		intersectRBDObjects(SIM_IsectPriorityQueue &queue,
					RBD_Object *obj_a, RBD_Object *obj_b,
					bool findany,
					bool finddeepest,
					fpreal depthtol,
					bool ignorevel,
					bool usesdfvel,
					bool usepointvel) const;

protected:
    explicit		 RBD_ColliderSDF(const SIM_DataFactory *factory);
    virtual		~RBD_ColliderSDF();

    /// Perform collision.
    /// - The return value is true if successful and false if the timestep
    /// should be repeated.
    /// - Thickness is a "diameter" - the thickness from one side of the
    /// cloth/hair to the other.

    virtual bool	 collideObjectsSubclass(SIM_Engine &engine,
				SIM_Object &object,
				SIM_Object &affector,
				const SIM_Time &startTime,
				const SIM_Time &endTime,
				SIM_ImpactApplyType implactapplytype,
				int impactflags) const;

     /// Find all vertices of A which are inside of B and add them
     /// to our queue.
     void		findVertexIntersections(SIM_IsectPriorityQueue &queue,
					RBD_Object *obj_a,
					RBD_Object *obj_b,
					bool reverseobj,
					bool findany,
					bool finddeepest,
					fpreal depthtol,
					bool ignorevel,
					bool usesdfvel,
					bool usepointvel,
					int child_id_a,
					int child_id_b,
					bool allow_childa_recurse = true) const;

   

    static const SIM_DopDescription *getDopDescription();
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(RBD_ColliderSDF,
			SIM_Collider,
			"Volume/Volume Collider",
			getDopDescription());
};

#endif
