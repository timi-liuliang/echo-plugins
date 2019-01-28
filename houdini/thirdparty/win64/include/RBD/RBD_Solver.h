/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __RBD_Solver__
#define __RBD_Solver__

#include "RBD_API.h"

#include <SIM/SIM_Collider.h>
#include <SIM/SIM_Isect.h>
#include <SIM/SIM_OptionsUser.h>
#include <SIM/SIM_Solver.h>
#include <GU/GU_SDF.h>

#include "RBD_Object.h"
#include "RBD_Utils.h"

class SIM_Object;
class SIM_Random;
class SIM_SDF;
class RBD_State;
class RBD_ContactGraph;
class RBD_ObjectTree;


#define RBD_NAME_COLLISIONITERATIONS	"collisioniterations"
#define RBD_NAME_CONTACTITERATIONS	"contactiterations"
#define RBD_NAME_SUBCONTACTITERATIONS	"subcontactiterations"
#define RBD_NAME_SHOCKPROPAGATION	"shockpropagation"
#define RBD_NAME_USEEDGES		"useedges"
#define RBD_NAME_FLAGPENETRATION	"flagpenetration"
#define RBD_NAME_MINIMUMSUBSTEPS	"minimumsubsteps"
#define RBD_NAME_MAXIMUMSUBSTEPS	"maximumsubsteps"
#define RBD_NAME_CFLCOND		"cflcond"
#define RBD_NAME_RESOLVEPENETRATION	"resolvepenetration"
#define RBD_NAME_GLUEIGNORESRESTING	"glueignoresrestingobjects"

/// This is a RBD-RBD meta solver.  It handles the cases where
/// RBD objects have a Mutual affect relationship with each other.
class RBD_API RBD_Solver : public SIM_Solver,
			      public SIM_OptionsUser
{
public:
    GETSET_DATA_FUNCS_I(RBD_NAME_COLLISIONITERATIONS, CollisionIterations)
    GETSET_DATA_FUNCS_I(RBD_NAME_CONTACTITERATIONS, ContactIterations)
    GETSET_DATA_FUNCS_I(RBD_NAME_SUBCONTACTITERATIONS, SubContactIterations)
    GETSET_DATA_FUNCS_I(RBD_NAME_SHOCKPROPAGATION, ShockPropagation)
    GETSET_DATA_FUNCS_I(RBD_NAME_RESOLVEPENETRATION, ResolvePenetrations)
    GETSET_DATA_FUNCS_B(RBD_NAME_FLAGPENETRATION, FlagPenetration);
    GETSET_DATA_FUNCS_B(RBD_NAME_GLUEIGNORESRESTING, GlueIgnoresRestingObjects);
    GETSET_DATA_FUNCS_B(RBD_NAME_USEPOINTVELOCITY, UsePointVelocity);
    GETSET_DATA_FUNCS_B(RBD_NAME_USESDFVELOCITY, UseSDFVelocity);
    GETSET_DATA_FUNCS_I(RBD_NAME_CULLMODE, CullMode);

    GETSET_DATA_FUNCS_B(RBD_NAME_ADDIMPACTS, AddImpacts);
    GETSET_DATA_FUNCS_I(RBD_NAME_CONTACTGROUPMETHOD, ContactGroupMethod);
    GETSET_DATA_FUNCS_F(RBD_NAME_CONTACTGROUPTOL, ContactGroupTol);

    GET_DATA_FUNC_I(RBD_NAME_MINIMUMSUBSTEPS, MinimumSubSteps);
    GET_DATA_FUNC_I(RBD_NAME_MAXIMUMSUBSTEPS, MaximumSubSteps);
    GET_DATA_FUNC_F(RBD_NAME_CFLCOND, CFLCond);
    
    /// This is public as we need it in our contact graph builder.
    bool	 doObjectsCollide(RBD_Object *obja, 
				  RBD_Object *objb, 
				  const SIM_Time &time, 
				  const SIM_Time &timestep) const;
    
    /// This method creates an RBD_ObjectTree of the type requested
    /// by our cull mode.
    RBD_ObjectTree	*createObjectTree() const;

    /// Takes the solve info attached to the object, checks to
    /// see if it was created by us, if so, casts to RBD_Object
    /// and returns.  Can return null if the object wasn't initialized
    /// or was done with another solver.
    RBD_Object		*convertToRBDObject(const SIM_Object *obj) const;

    /// This should be kept in sync with the contact group method
    /// menu.
    enum rbdContactGroupMethod
    {
	CONTACTGROUP_NONE = 0,
	CONTACTGROUP_CLOSESTPOINT,
	CONTACTGROUP_AVERAGE
    };
    
protected:
    explicit		 RBD_Solver(const SIM_DataFactory *factory);
    virtual		~RBD_Solver();

    virtual SIM_Result	 solveObjectsSubclass(SIM_Engine &engine,
				SIM_ObjectArray &objects,
				SIM_ObjectArray &newobjects,
				SIM_ObjectArray &feedbacktoobjects,
				const SIM_Time &timestep);
    virtual void	 getImpulseMassMatrixSubclass(
				const SIM_Object &object,
				const UT_Vector3 &impulseworldpos,
				UT_DMatrix3 &immatrix) const;
    virtual void	 getPointImpulseMassMatrixSubclass(
				const SIM_Object &object,
				int ptnum, UT_DMatrix3 &immatrix) const;
    virtual fpreal	 getPropertyAtPositionSubclass(const SIM_Object &object,
				const UT_Vector3 &worldspacepos,
				const SIM_Property &property) const;
    virtual fpreal	 getPropertyAtPointSubclass(const SIM_Object &object,
				int ptnum, const SIM_Property &property) const;
    virtual void	 getDefaultColliderLabelSubclass(
				const SIM_Object &object,
				UT_String &label) const;
    virtual void	 getDefaultColliderSubclass(
				const SIM_Object &object,
				const UT_String &colliderlabel,
				UT_String &collidertype,
				bool &colliderreverseobjectroles) const;

private:
    static const SIM_DopDescription *getSolverRBDDopDescription();

    /// Get a property value by looking at the RBD_State of an object.
    fpreal		getPropertyFromState(const SIM_Object &object,
				const SIM_Property &property) const;

    /// Applies the stacking solver.
    void		solveStackObjects(SIM_ObjectArray &objects,
				SIM_ObjectArray &newobjects,
				const SIM_Time &time,
				const SIM_Time &timestep);

    // Stack solving methods:

    /// Creates a SIM_Random data on all the objects in the array. This
    /// data is used by the stack solver.
    SIM_Random		*createRandomData();

    /// Resolve contact forces.
    /// Returns true if any contact forces occurred.
    bool		resolveContactForces(const RBD_ContactGraph &graph,
				    const RBD_ObjectArray &objects,
				    const SIM_Time &time,
				    const SIM_Time &timestep,
				    bool applyshock) const;

    /// Resolve actual penetration.
    /// Attempts to jiggle objects so they do not penetrate at all.
    /// All updates are done purely on position.  Penetration ignores
    /// velocity (so two objects moving apart, but overlapping, will
    /// be jiggled)
    /// The contact graph is used to provide a hierarchy for the resolution
    /// Returns true if any jiggling occurred.
    bool		resolvePenetrations(const RBD_ContactGraph &graph,
				    const UT_ValArray<RBD_Object *> &objects,
				    const SIM_Time &time,
				    const SIM_Time &timestep) const;

    /// Fixes any penetrations between obj & rest by moving obj so that
    /// they no longer penetrate.  Returns true if any change occurred.
    /// Restitution is what percentage to move them.
    bool		fixPenetrations(RBD_Object *obj, RBD_Object *rest,
				    const SIM_Collider *collider,
				    fpreal restitution,
				    const SIM_Time time,
				    const SIM_Time timestep) const;
				    
    /// Resolve elastic object collisions.
    /// Returns true if there were any elastic collisions.
    bool		resolveElasticCollisions(RBD_ObjectArray &objects,
				    const SIM_Time &time,
				    const SIM_Time &timestep) const;

    /// Applies forces to all of our pins so as to ensure they
    /// have no relative velocity.
    void		 resolvePinConstraintVelocities(RBD_Object *obj) const;

    /// Moves all of our pinned objects so they satisfy their
    /// pin constraints.
    /// This is done in a shock propagation style manner - a, possibly
    /// arbitrary, hierarchy is setup and each object is satisfied in
    /// turn, treating previous objects as locked.
    void		 shockPropagatePinConstraints(const RBD_ObjectArray &objects) const;
    
    /// Collide two objects elastically.  Ignores rest of world,
    /// applies impulses until each intersecting point is seperating
    /// at least once.
    /// infinitemass set to true treats object b as infinite mass,
    /// even if it actually has finite mass.
    bool         elasticCollide(RBD_Object *obj_a, RBD_Object *obj_b,
			bool infinitemass,
		    	fpreal restitution,
	    		bool onlyfirst,
    			bool usebounce,
    			const SIM_Time time,
    	    		const SIM_Time timestep,
			const SIM_Collider *collider,
		    	SIM_Collider::SIM_ImpactApplyType affectortype) const;

    /// Extracts the next SIM_Isect from the queue.  Uses the contact
    /// grouping method to merge contacts of similar depth.
    SIM_Isect		*groupContacts(SIM_IsectPriorityQueue &queue, 
					RBD_Object *obj_a,
					RBD_Object *obj_b,
					bool infinitemass) const;

    /// Given a point of intersection, find and apply the impulse
    /// required to cause the point to be seperating.  (If it is
    /// already seperating, ignore)
    bool		collidePoint(RBD_Object *obj_a, RBD_Object *obj_b,
				    bool infinitemass,
				    const SIM_Isect *isect,
				    fpreal restitution,
				    bool usebounce,
				    const SIM_Time curtime,
				    bool usesdfvel,
				    bool usepointvel,
				    int child_id_a = -1,
				    int child_id_b = -1) const;

    /// Determines the possible frozen sub object that is closest
    /// to the given point in the SDF sense.
    /// Returns true if a new closest child is found.
    /// If onlytoplevel is true, the only possible objects returned
    /// are the original obj or its children.  Subobjects are still
    /// tested, but their respective parent inherits ownership.
    bool		 findClosestChild(RBD_Object *obj, 
				    const UT_Vector3 &pos,
				    RBD_Object *&closest, fpreal &mindist,
				    bool onlytoplevel = false,
				    int child_id = -1) const;
    
    /// Given an impulse and position, applies the impulse to the
    /// object.  This is used for collision impulses.  The idea is
    /// that by sending them through this interface we can track
    /// what impulses have been applied to the object.
    /// The opposite impulse will be sent to object b, if it is given.
    /// restingcollision is set if this is a result of objects resting
    /// on each other rather than elastically colliding.
    void		 applyCollisionImpulse(RBD_Object *obj_a,
				    RBD_Object *obj_b,
				    int obj_b_id,
				    fpreal scale,
				    const UT_Vector3 &pos,
				    const UT_Vector3 &impulse,
				    const SIM_Time curtime,
				    int child_id_a, 
				    int child_id_b,
				    bool infiniteforceVSinfinitemass,
				    bool restingcollision) const;

    /// Adds the given impact to the SIM_Impact list for the given
    /// pair of objects.
    /// The given impulse vector does not have to be normalized.
    void		 addImpactData(RBD_Object *obj_a,
				    RBD_Object *obj_b,
				    int obj_b_id,
				    const UT_Vector3 &pos,
				    const UT_Vector3 &nml,
				    fpreal strength,
				    const SIM_Time curtime,
				    bool restingcollision) const;

    /// Collide two objects using the collideObjects() from the collider
    bool          collideObjects (RBD_Object *obj_a, RBD_Object *obj_b,
		    	const SIM_Time startTime,
	    		const SIM_Time endTime,
			SIM_Collider::SIM_ImpactApplyType affectortype) const;
    
    SIM_Time		 getCurSolveTime() const
			 { return myCurSolveTime; }
    void		 setCurSolveTime(SIM_Time time)
			 { myCurSolveTime = time; }
    SIM_Time		 getCurSolveStep() const
			 { return myCurSolveStep; }
    void		 setCurSolveStep(SIM_Time step)
			 { myCurSolveStep = step; }

    bool		 doAddImpacts() const
			 { return myAddImpacts; }
    bool		 doGlueIgnoresResting() const
			 { return myGlueIgnoresResting; }

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(RBD_Solver,
			SIM_Solver,
			"RBD Solver",
			getSolverRBDDopDescription());

private:
    // Flag variables used during the simulation

    /// Flags if anything broke during the simulation.  Lets us know
    /// we need to rebuild topology dependent constructs.
    mutable bool		myBreakOccurred;

    /// Determins if we should add SIM_Impact data for every 
    /// collision we process.
    /// This is done to avoid possibly expensive parameter evaluation
    /// in deeply nested functions.
    mutable bool		myAddImpacts;
    mutable bool		myGlueIgnoresResting;

    /// Current solve time.
    SIM_Time			myCurSolveTime;
    SIM_Time			myCurSolveStep;

    SIM_Engine			*myEngine;
};

#endif

