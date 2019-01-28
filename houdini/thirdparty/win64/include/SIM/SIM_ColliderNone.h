/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ColliderNone_h__
#define __SIM_ColliderNone_h__

#include "SIM_API.h"
#include "SIM_Collider.h"

/// This class implements a collider that doesn't actually detect collisions.
/// Use this collider as a place holder when a collider is required but
/// no actual collision detection is needed.
class SIM_API SIM_ColliderNone : public SIM_Collider
{
protected:
    /// This SIM_ColliderNone constructor.
    explicit		 SIM_ColliderNone(const SIM_DataFactory *factory);
    /// This SIM_ColliderNone destructor.
    virtual		~SIM_ColliderNone();

    /// Required override of the collision detection function.
    /// This implementation does nothing.
    virtual bool	 collideObjectsSubclass(SIM_Engine &engine,
				    SIM_Object &object,
				    SIM_Object &affector,
				    const SIM_Time &starttime,
				    const SIM_Time &endtime,
				    SIM_ImpactApplyType impactapplytype,
				    int impactflags) const;
    /// There is no point in interpolating the affector, since we
    /// never even look at it. So return false here.
    virtual bool	 getAffectorInterpolatedToEndTimeSubclass() const;

private:
    static const SIM_DopDescription	*getColliderNoneDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ColliderNone,
			SIM_Collider,
			"No Collider",
			getColliderNoneDopDescription());
};

#endif

