/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Collider_h__
#define __SIM_Collider_h__

#include "SIM_API.h"
#include "SIM_DataUtils.h"
#include "SIM_OptionsUser.h"

class SIM_Impacts;
class SIM_Object;
class SIM_ObjectArray;

/// This simple holder stores cache data associated with the collider.
class SIM_API SIM_ColliderCacheData
{
public:
    SIM_ColliderCacheData() {}

    virtual ~SIM_ColliderCacheData() {}
};

/// This is an abstract base class for all colliders.
/// A collider is any class that takes a pair of objects and generated
/// information about how those two objects intersect. See the
/// SIM_Object::getCollider() function for a description of how a
/// solver should determine the right SIM_Collider class to use for
/// a given pair of objects.
class SIM_API SIM_Collider : public SIM_Data,
			     public SIM_OptionsUser
{
public:
    /// Specifies whether the object and affector need to be reversed to
    /// perform collision detection. This flag is useful for colliders that
    /// make assumptions about the makeup of the object and affector are
    /// in the call to collideObjectsSubclass(). For example, the
    /// point collider (SIM_ColliderPoint) always treats the object as
    /// a particle system, using ray casting to follow the trajectory of
    /// each particle. But suppose an RBD Solver has a particle system
    /// as an affector object. The RBD Solver would find that the best
    /// collider to use is a point collider. Rather than requiring that
    /// the RBD Solver then somehow discover that the particle object must
    /// always be sent as the "object" parameter, the collider can have
    /// the reverse object roles flag turned on. Then when the RBD Solver
    /// calls the collider with the RBD object first and the particle object
    /// second, the collider itself will reverse the objects before passing
    /// them on to collideObjectSubclass().
    GETSET_DATA_FUNCS_B(SIM_NAME_REVERSEOBJECTROLES, ReverseObjectRoles);

    /// Defines the possible affector types when doing collision detection.
    typedef enum {
	SIM_IMPACTAPPLY_NONE,	 /// Don't do any collision detection.
	SIM_IMPACTAPPLY_ONEWAY,	 /// Object A gets Impacts from object B.
	SIM_IMPACTAPPLY_MUTUAL,	 /// Object A and B both get Impacts.
	SIM_IMPACTAPPLY_FEEDBACK /// Object A gets Impacts, B gets Feedback.
    } SIM_ImpactApplyType;

    /// Perform the collision detection for a pair of objects.
    /// This function calls the collideObjectsSubclass() function.
    /// This also attempts to do collision response.
    ///
    /// Preconditions:
    /// collideObjects expects that
    /// getEngine.getObjectAtTime(object, starttime, true) results in the
    /// object that holds the correct geometry and transform for starttime.
    /// Similarly, passing in endtime must give an object that holds the 
    /// correct geometry and transformation for endtime.
    /// The same principle applies to the affector. 
    /// Please note that starttime and endtime may be substep times.
    ///
    /// The solver that calls collideObjects may have to create substep objects
    /// at t_start and t_end to ensure that these preconditions are met.
    /// This is is not always needed for the affector, the affector may
    /// belong to a solver that isn't substepping.
    /// In that case collideObjects will use the
    /// linear interpolations for the affector's geometry and transform.
    /// 
    bool		 collideObjects(SIM_Engine &engine,
				SIM_Object &object,
				SIM_Object &affector,
				const SIM_Time &starttime,
				const SIM_Time &endtime,
				SIM_ImpactApplyType impactapplytype,
				int impactflags) const;

    /// This variant of the collider lets you specify a list of
    /// cache data.  The collider subclasses will search for a cache
    /// data they like and use it.  If none exists, they will build one
    /// and add it to the list.
    /// The caller is expected to delete any allocated cachedata.
    bool		 collideObjects(SIM_Engine &engine,
				SIM_Object &object,
				SIM_Object &affector,
				const SIM_Time &starttime,
				const SIM_Time &endtime,
				SIM_ImpactApplyType impactapplytype,
				int impactflags,
				UT_ValArray<SIM_ColliderCacheData *> &objcachedata,
				UT_ValArray<SIM_ColliderCacheData *> &affectorcachedata) const;

protected:
    /// The SIM_Collider constructor.
    explicit		 SIM_Collider(const SIM_DataFactory *factory);
    /// The SIM_Collider destructor.
    virtual		~SIM_Collider();

    /// Finds or creates the impact data on the affector that is appropriate
    /// for the given affector type. It also takes into account the value
    /// of the ReverseObjectRoles flag.
    SIM_Impacts		*getObjectImpactData(SIM_Object &object,
				SIM_ImpactApplyType impactapplytype) const;

    /// Finds or creates the impact data on the affector that is appropriate
    /// for the given affector type. It also takes into account the value
    /// of the ReverseObjectRoles flag.
    SIM_Impacts		*getAffectorImpactData(SIM_Object &affector,
				SIM_ImpactApplyType impactapplytype) const;

    /// Returns true if the collideObjects function should always be
    /// called with the affector object interpolated to the endtime.
    /// This function just calls getAffectorInterpolatedToEndTimeSubclass().
    bool		 getAffectorInterpolatedToEndTime() const;

    /// Override this method to implement your custom collision detection.
    /// This function can access any data on the main object or affector
    /// object. It can also add any extra data structures it needs to the
    /// object for efficiency (rather than recalculating the extra data
    /// each time this function is called).
    virtual bool	 collideObjectsSubclass(SIM_Engine &engine,
				SIM_Object &object,
				SIM_Object &affector,
				const SIM_Time &starttime,
				const SIM_Time &endtime,
				SIM_ImpactApplyType impactapplytype,
				int impactflags) const;

    /// This version will default to invoking the non-cached version
    /// of the collider.
    /// If this is overridden, you do not need to override
    /// collideObjectSubclass.
    virtual bool	 collideObjectsCachedSubclass(SIM_Engine &engine,
				SIM_Object &object,
				SIM_Object &affector,
				const SIM_Time &starttime,
				const SIM_Time &endtime,
				SIM_ImpactApplyType impactapplytype,
				int impactflags,
				UT_ValArray<SIM_ColliderCacheData *> &objcachedata,
				UT_ValArray<SIM_ColliderCacheData *> &affectorcachedata) const;

    /// Returns true if the collideObjects function should always be
    /// called with the affector object interpolated to the endtime.
    /// The default implementation returns true.
    virtual bool	 getAffectorInterpolatedToEndTimeSubclass() const;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_Collider, SIM_Data);
};

#endif

