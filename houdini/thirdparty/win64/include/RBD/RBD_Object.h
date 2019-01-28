/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RBD_Object.h ( RBD Library, C++)
 *
 * COMMENTS: 
 *	This holds all the data we care about for RBD Objects
 *	This allows us to avoid having to re-acquire the data
 *	in innermost loops.
 */

#ifndef __RBD_Object_H__
#define __RBD_Object_H__

#include "RBD_API.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_Map.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_TokenString.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Vector3Array.h>
#include <SIM/SIM_ObjectArray.h>
#include <SIM/SIM_Solver.h>
#include <SIM/SIM_Collider.h>
#include <SIM/SIM_ObjectSolveInfo.h>
#include <GU/GU_SDF.h>
#include "RBD_SharedAffectorList.h"

class SIM_Object;
class SIM_Geometry;
class SIM_Engine;
class SIM_Time;
class SIM_SDF;
class SIM_Impacts;
class SIM_ConAnchorSpatial;
class RBD_Object;
class RBD_Solver;
class RBD_State;
class RBD_SphereTree;
class RBD_SpherePointTree;
class RBD_SphereEdgeTree;

typedef UT_ValArray<RBD_Object *> RBD_ObjectArray;

class RBD_API RBD_Object : public SIM_ObjectSolveInfo
{
public:
			 RBD_Object(const SIM_Solver *solver, SIM_Object *obj);
    virtual 		~RBD_Object();

    /// These cache the common RBD structures so we don't have
    /// to look them up every time we fetch them.
    SIM_Object		*getObject() const { return myObject; }
    RBD_State		*getState() const { return myState; }
    SIM_Impacts		*getImpacts();
    const SIM_SDF	*getSDF() const { return mySDF; }
    const SIM_Geometry	*getGeometry() const { return myGeometry; }

    const RBD_SphereTree*getSphereTree() const;

    /// This is the radius of this object from the pivot position.
    /// It is thus orientation independent.
    fpreal		 getRadius() const { return myRadius; }

    /// Returns true if this object can intersect anywhere in space
    /// Used for things like implicit planes.
    bool		 isInfiniteExtent() const;

    /// This returns the bounding box of the object inside it's
    /// own frame.  This must be transformed by the orientation
    /// to get the world bounding box.
    void		 getBBox(UT_BoundingBox &bbox) const;

    /// Accumulates into the  given arrays all of our point's positions
    /// and velocities.  Both are given in world space.
    void		 accumulatePointVelocity(UT_Vector3Array &pos,
						 UT_Vector3Array &vel) const;

    /// Stashes the current state internally.
    void		 stashState();

    /// Reloads our impact data structure from the SIM_Object.  This must
    /// be done whenever preserveImpacts is called.
    void		 reloadImpacts(
			    SIM_Collider::SIM_ImpactApplyType impactaplytype);

    /// Copies our NewImpact data into Impacts.
    /// This is a pass through to SIM_Object, but ensures our cached
    /// impact structure is cleared.
    void		 preserveImpacts(SIM_Engine *engine);

    /// Restores the position of the state using changePosition.
    void		 restoreStatePos();
    /// Restores the velocity of the state using changeVelocity.
    void		 restoreStateVel();
    /// Restores the full state
    void		 restoreState();

    /// This returns a list of all the objects that are frozen to us.
    void		 getGlueSubObjects(RBD_ObjectArray &objlist) const;

    /// Discover the number of glued subobjects and retrieve a specific
    /// one
    int			 getNumGlueSubObjects() const;
    RBD_Object		*getNthGlueSubObject(int i) const;

    /// Returns our parent object, null if not glued.
    RBD_Object		*getGlueParent() const { return myGlueParent; }

    /// This returns true if we have any frozen sub objects.
    bool		 hasGlueSubObjects() const;

    /// Marks that obj is glued to this.
    void		 addGlueSubObject(RBD_Object *obj);

    /// Removes that object as being glued to this.
    void		 removeGlueSubObject(RBD_Object *obj);

    /// Returns true if the given object is one of our glue children.
    bool		 hasAsGlueChild(RBD_Object *obj);

    /// Return if this object is glued to somewhere else.
    bool		 isGlued() const { return myIsGlued; }

    /// Builds the impulse model matrix for the given
    /// world space coordinate.  This matrix when multiplied by
    /// an impulse will give the change in velocity at that point
    /// caused by the impulse.
    /// 
    /// This takes hard constraints into consideration.
    /// Builds the impulse model matrix describing the velocity change
    /// at point p given a force at point f.
    /// This is Mfp-1 in my naming scheme.
    void		 buildImpulseModel(UT_DMatrix3 &k,
					   const UT_Vector3 &f,
					   const UT_Vector3 &p) const;

    /// Builds the impulse model matrix describing motion of P on *this
    /// given an a force at f on *this.
    /// This is Mzfp-1(A) in my naming scheme, where this is Z.
    void		 buildImpulseModelForA(UT_DMatrix3 &k,
					       const UT_Vector3 &f,
					       const UT_Vector3 &p,
					       RBD_Object *A) const;

    /// Determines what hard constraints are present at this time
    /// step and updates its internal model to reflect theses.
    /// We need the end time to be able to acquire non-interpolated
    /// objects to handle live pin constraints.
    void		 buildConstraints(const RBD_Solver *solver,
					  const SIM_Time &time,
					  const SIM_Time &endtime);

    /// Removes the constraints added.  This restores stuff like the
    /// pivot to its original value.
    void		 removeConstraints();

    /// Applies an impulse to this object.
    /// This results in an instantaneous change in velocity and
    /// angular velocity.
    void		 applyImpulse(const UT_Vector3 &pos,
				      fpreal impulse,
				      const UT_Vector3 &normal);
    /// Applies impulses from SIM_Impacts data.
    void		 applyImpacts(const SIM_Impacts *impacts,
				      const SIM_Time &time,
				      const SIM_Time &timestep);

    /// Integrate position.  Uses current velocity.
    void		 integratePosition(const SIM_Time &time,
					   const SIM_Time &timestep);
    
    /// Integrate velocity.  Finds & applies all forces.
    void		 integrateVelocity(const SIM_Time &time,
					   const SIM_Time &timestep);

    /// Builds the SDF and updates the state from the sdf,
    /// unless sdf is already built.
    void		 initSDF();

    /// Updates the state from geometry, unless already calculated.
    void		 initFromGeometry();

    /// Calculates the radius, unless already calculated.
    void		 initRadius();

    /// Calculates the bounding box.
    void		 initBBox(bool useSDF);

    /// Gathers all of our builder requests.
    void		 gatherBuilderRequests(UT_Array<GU_SDFDelayedBuilder> &buildrequests);
    
    /// Initialize for collisions
    void		 initForCollisions();
    
    /// Calculates the property value at the given world position.
    fpreal		 getPropertyAtPosition(
				const SIM_Property &property, 
				const UT_Vector3 &pos) const;

    /// Rebuilds the list of affectors for this object.
    /// Interpolates the affectors for the given time.
    void		 rebuildAffectorList(const RBD_Solver *solver,
				SIM_Engine &engine, 
				const SIM_Time &time,
				const SIM_Time &timestep,
				RBD_SharedAffectorListArray &affectorlists,
                                UT_TokenString::Map<RBD_SharedAffectorList*> &affectorhash);

    /// Gets the transform matrix from self to world from stashed state
    void		 getStashedTransform(UT_DMatrix4 &xform) const;

    void		 findOverlapIdx(const RBD_Solver *solver,
					UT_IntArray &overlap)
			 { if( mySharedAffectorList )
			       mySharedAffectorList->findOverlapIdx(solver,
								    this,
								    overlap);
			   else
			       overlap.entries(0); }
    void		 setAffectorTreeDirty()
			 { if( mySharedAffectorList )
			       mySharedAffectorList->setObjectTreeDirty(); }
    
    int			 getNumAffectors() const
			 { return mySharedAffectorList 
					? mySharedAffectorList->entries() 
					: 0; }
    RBD_Object		*getAffector(int j) const
			 { return (*mySharedAffectorList)(j); }
    const SIM_Collider	*getCollider(int j, const char *defaultlabel) const
			 { return mySharedAffectorList->
					getCollider(j, myObject, defaultlabel); }
    SIM_Collider::SIM_ImpactApplyType	 getImpactApplyType(int j) const
			 { return mySharedAffectorList->getImpactApplyType(j); }

    // The array index value is used by the solver to keep track of the
    // RBD_Object's position within an array. It is a temporary value with
    // meaning only for the solver.
    int			 getArrayIndex() const
			 { return myArrayIndex; }
    void		 setArrayIndex(int index)
			 { myArrayIndex = index; }

    bool		 hasNailConstraints() const
			 { return myHasConstraints; }

    UT_ValArray<SIM_ColliderCacheData *> &getCollideCache() { return myCollideCache; }
    void		 clearCollideCache();

    void		 cachePhysicalValues();
    void		 restorePhysicalValues();

    // Are we currently processing pin constraints for this object?
    mutable bool	 myPinProcessFlag;
    RBD_ObjectArray	 myPinObjects;

    // This is the position of the anchors.  It is stored in
    // our object space of this for AnchorPos and of myPinObjects for
    // goal pos.
    UT_Vector3Array	 myPinAnchorPos;
    UT_Vector3Array	 myPinAnchorGoalPos;

protected:
    // Iterates over all constraints attached to piece.  All hard
    // constraints are added to our own list of pin constraints
    // and our list of hard positions.
    // Recurses to all glued subobjects.
    void		 accumulateConstraints(const RBD_Solver *solver,
				RBD_Object *piece,
				UT_Vector3Array &hard_objpos,
				UT_Vector3Array &hard_goalpos,
				UT_Matrix3 &hard_spatialfilter,
				UT_Matrix3 &hard_rotationalfilter,
				const SIM_Time &time,
				const SIM_Time &endtime);

private:
    /// Gets the property for a specific position by trying to find
    /// the closest point to that position.   The closest point will
    /// then return it's attribute value (if any) or the rbd state
    /// corresponding to that.
    /// This recurses down the glue hierarchy.
    void		 findClosestPropertyAtPosition(fpreal &newvalue,
				    const SIM_Property &property,
				    const UT_Vector3 &pos, 
				    bool &found, fpreal &closestdist) const;
 
    SIM_Object				*myObject;
    RBD_State				*myState;
    SIM_Impacts				*myImpacts;
    RBD_SharedAffectorList		*mySharedAffectorList;
    int					 myArrayIndex;

    const SIM_Geometry			*myGeometry;
    const SIM_SDF			*mySDF;

    // Stashes whether we have these attributes & what their
    // offsets are.
    mutable GA_ROAttributeRef		 myPropertyOffset[SIM_PROPERTY_COUNT];
    mutable bool			 myPropertyLookedUp[SIM_PROPERTY_COUNT];
    mutable const RBD_SphereTree	*mySphereTree;

    // Our radius, -1 if not yet computed.
    fpreal		 myRadius;

    // Our bounding box, in local coords.
    bool		 myBBoxValid;
    UT_BoundingBox	 myBBox;

    // This is where we stash our orientation.
    UT_Vector3		 myStashPos;
    UT_Quaternion	 myStashOrient;
    UT_Vector3		 myStashVel;
    UT_Vector3		 myStashAngVel;

    // Something glued will not simulate.
    bool		 myIsGlued;
    // Stores if we have constraints.
    bool		 myHasConstraints;

    // Pre-constraint values for these.
    UT_Matrix3		 myCacheTensor;
    UT_Vector3		 myCachePivot;
    UT_Vector3		 myMomentArm;
    UT_Matrix3		 mySpatialFilter;
    UT_Matrix3		 myRotationalFilter;

    // This is used temporarily during the solving process to track
    // what subobjects are present.
    RBD_Object		*myGlueParent;
    RBD_ObjectArray	 myGlueSubObjects;

    // Cached physical properties of standalone RBD Object (excludes glued
    // children).
    bool		 myHasCachedPhysicalValues;
    fpreal		 myCachedMass;
    UT_Vector3		 myCachedCenterOfMass;
    UT_DMatrix3		 myCachedInertialTensor;

    UT_ValArray<SIM_ColliderCacheData *>	myCollideCache;

    bool		 myHasInitFromGeometry;
};

#endif
