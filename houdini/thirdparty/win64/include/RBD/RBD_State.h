/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __RBD_State_h__
#define __RBD_State_h__

#include "RBD_API.h"
#include "RBD_Utils.h"
#include <SIM/SIM_Motion.h>
#include <UT/UT_Matrix.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Vector3Array.h>

class GU_SDF;
class SIM_Object;
class SIM_ObjectArray;
class SIM_Constraint;
class RBD_Object;
class SIM_Geometry;
class SIM_Force;

// This class contains basic state information that applies to rigid body
// objects, such as position and velocity, and some physical attributes.
class RBD_API RBD_State : public SIM_Motion
{
public:
    // Named access functions for getting at our data.
    GETSET_DATA_FUNCS_M3(RBD_NAME_ITENSOR, InertialTensor);
    GET_DATA_FUNC_M3(RBD_NAME_ITENSORLOCALINV, InertialTensorLocalInv);
    GETSET_DATA_FUNCS_B(RBD_NAME_INHERITVELOCITY, InheritVelocity);
    GETSET_DATA_FUNCS_B(RBD_NAME_COMPUTECOM, ComputeCOM);
    GETSET_DATA_FUNCS_B(RBD_NAME_COMPUTEMASS, ComputeMass);
    GETSET_DATA_FUNCS_B(RBD_NAME_COMPUTEINERTIALTENSOR,
			ComputeInertialTensor);
    GETSET_DATA_FUNCS_I(RBD_NAME_COMPUTEINERTIALTENSORTYPE,
			ComputeInertialTensorType);
    GET_DATA_FUNC_F(RBD_NAME_DENSITY, Density);
    GETSET_DATA_FUNCS_F(RBD_NAME_MASS, Mass);
    GETSET_DATA_FUNCS_F(RBD_NAME_INERTIALTENSORSTIFFNESS,
			InertialTensorStiffness);

    GETSET_DATA_FUNCS_S(RBD_NAME_GLUEOBJECT, GlueObject);
    GETSET_DATA_FUNCS_F(RBD_NAME_GLUETHRESHOLD, GlueThreshold);
    GETSET_DATA_FUNCS_F(RBD_NAME_GLUEIMPULSE, GlueImpulse);
    GETSET_DATA_FUNCS_F(RBD_NAME_GLUEIMPULSEHALFLIFE, GlueImpulseHalfLife);

    /// Updates our values based on a given SDF or Geometry.
    /// If SDF is null, will use the geometry and assume a thin plate model.
    /// This will update the mass, COM, and ITensor provided
    /// the relevant compute flags are true.
    /// It will also recalculate your linear & angular velocity from
    /// the underlying geometry if inherit is turned on.
    /// updatelinearvel controls whether the linear velocity should be updated 
    /// when computing the COM.  
    /// It has no effect on calculations for inheriting vel
    void		 updatePhysicalValues(RBD_Object *obj, 
					      const GU_SDF *sdf, 
					      const SIM_Geometry *geo,
					      bool updatelinearvel);

    // Methods to calculate new values based on old values and a time step.

    /// Integrates the position of this object forward by a timestep.
    /// The current velocity is treated as constant.
    void		 getNewPosition(const SIM_Object &object,
					const SIM_Time &time,
					const SIM_Time &timestep,
					UT_Vector3 &newpos,
					UT_Quaternion &neworient) const;

    /// Integrates velocity forward by one timestep.
    /// This will iterate over all forces and soft constraints
    /// to construct the required acceleration & apply it.
    void		 getNewVelocity(RBD_Object *object,
					const SIM_Time &time,
					const SIM_Time &timestep,
					UT_Vector3 &newvel,
					UT_Vector3 &newangvel) const;

    /// Integrates velocity forward by one timestep.
    /// This uses the specified center of mass for sampling the
    /// force function rather than the objects pivot.
    void		 getNewVelocity(RBD_Object *object,
					const SIM_Time &time,
					const SIM_Time &timestep,
					const UT_Vector3 &com,
					UT_Vector3 &newvel,
					UT_Vector3 &newangvel) const;

    /// Takes an array of points in space with associated velocities.
    /// Computes the angular velocity and velocity that best matches
    /// the given values.  Each point/velocity pair is treated as
    /// equally important.
    /// Note that center of mass is only needed to compute the linear
    /// component.
    static void		 projectRigidMotion(
				    UT_Vector3 &vel,
				    UT_Vector3 &angvel,
				    const UT_Vector3 &com,
				    const UT_Vector3Array &posarray,
				    const UT_Vector3Array &velarray);

    /// Applies an impulse to this object.  This results in an instantaneous
    /// change in velocity.
    const UT_Vector3	 getNewVelocity(fpreal impulse,
					const UT_Vector3 &normal) const;
    const UT_Vector3	 getNewAngularVelocity(fpreal impulse,
					       const UT_Vector3 &pos,
					       const UT_Vector3 &normal) const;

    /// Iterate over all hard constraints and constrain
    /// the pos and orient to those constraints.  The new values
    /// are output in pos & orient.
    /// Note you likely want to constrain with time' = (time + timestep)
    /// if you solved with time.
    void		 constrainPosition(SIM_Object &object, 
				const SIM_Time &time,
				const SIM_Time &timestep,
				UT_Vector3 &pos,
				UT_Quaternion &orient) const;

    /// Determines the new pos and orient that best satisfies the set
    /// of constraints.  hard_objpos and hard_goal pos are both in world
    /// coordinates.  The new pos and orient should be such that the
    /// change will map hard_objpos onto hard_goalpos.
    void		constrainAccordingToPositions(
				    const UT_Vector3Array &hard_objpos, 
				    const UT_Vector3Array &hard_goalpos, 
				    const UT_Matrix3 &hard_filter,
				    UT_Vector3 &pos, 
				    UT_Quaternion &orient) const;

    /// Iterate over all the hard constraints and constraint
    /// the velocity according to those constraints.
    /// The new values are output in pos & orient.
    /// Note you likely want to constrain with time' = (time + timestep)
    /// if you solved with time.
    /// This only resolves nail constraints.  Pin constraints are
    /// handled at the RBD_Solver level as they can affect other
    /// objects.
    void		 constrainVelocity(SIM_Object &object,
				const SIM_ObjectArray &solvingobjects,
				const SIM_Time &time,
				const SIM_Time &timestep,
				UT_Vector3 &vel,
				UT_Vector3 &angvel) const;

    /// This utility method solves for the impulse required to
    /// create induce the given relative velocity at the given point.
    /// This does *not* take the current velocity into consideration!
    /// If the returned impulse is applied, the new velocity at the
    /// point would change by relative velocity.
    UT_Vector3		 solveForImpulse(const UT_Vector3 &pos,
					 const UT_Vector3 &relvel) const;

    /// These act as setPosition, setOrientation
    /// and setVelocity, setAngularVelocity.
    /// The main difference is that they will properly propagate to
    /// frozen sub objects.
    void		 changePosition(RBD_Object *obj,
					const UT_Vector3 &pos, 
					const UT_Quaternion &orient);
    void		 changeVelocity(RBD_Object *obj,
					const UT_Vector3 &vel,
					const UT_Vector3 &angvel);

    /// This changes the pivot point of the object to the new value.
    /// The new value is given in object cooridinates!
    /// This will adjust both Position and Pivot so the object's position
    /// doesn't change as a result.
    /// This also changes the inertial tensor to account for the new
    /// "center of mass".
    /// Changing the inertial tensor in this fashion is NOT invertible!
    /// updatelinearvel controls whether the linear velocity should be updated 
    /// taking into account the angular velocity
    void		 changePivot(const UT_Vector3 &pivot, 
				     bool updatetensor,
				     bool updatelinearvel = true);

    /// These alert the frozen object to impulses, causing it
    /// to break off if they become large enough.
    void		 accumulateGlueImpulse(fpreal impulse);

    /// This decays the frozen impulse according to the given 
    /// timestep.
    void		 decayGlueImpulse(SIM_Time timestep);
				
protected:
    explicit		 RBD_State(const SIM_DataFactory *factory);
    virtual		~RBD_State();

    /// Override setOrientation because we need to update our local
    /// inertial tensor when we do this.
    virtual void	 optionChangedSubclass(const char *name);

    /// Determines what sort of freedom of movement is available
    /// if the given constraints hold.
    /// The obj list is the world coordinates of points on the geometry
    /// The goal list is the world coordinates of where those points
    /// should be.
    /// This sets the objpos to where an impulse should be given.
    /// goalpos is where objpos should be.  Axis is the only axis
    /// of rotation allowed.  neworient specifies the required change
    /// of orientation (in world space) for the FULL case.
    enum constrainType { CON_NONE, CON_SINGLEPOINT, CON_AXIS, CON_FULL };

    constrainType	findConstraintType(const UT_Vector3Array &geo,
					   const UT_Vector3Array &anchor,
					   UT_Vector3 &objpos,
					   UT_Vector3 &goalpos,
					   UT_Vector3 &objaxis,
					   UT_Vector3 &goalaxis,
					   UT_Quaternion &orient) const;
    
private:
    void		 updateInertialTensor();
    static const SIM_DopDescription	*getStateDopDescription();

    /// Iterate over all force subdata & add the forces into sumforces
    /// and sumtorques.
    void		 calculateForceForces(const RBD_Object *obj,
				const UT_Vector3 &com,
				UT_Vector3 &sumforces,
				UT_Vector3 &sumtorques,
				UT_Matrix &sumDfDx,
				UT_Matrix &sumDfDv) const;

    /// Iterate over all soft constraints and adds the forces
    /// that are created by those constraints.
    void		 calculateConstraintForces(SIM_Object &object, 
				const UT_Vector3 &com,
				const SIM_Time &time,
				const SIM_Time &timestep,
				UT_Vector3 &sumforces, 
				UT_Vector3 &sumtorques,
				UT_Matrix &sumDfDx,
				UT_Matrix &sumDfDv) const;
    /// Iterate over all impacts in our feedback data to create forces.
    void		 calculateImpactForces(RBD_Object *object, 
				const UT_Vector3 &com,
				const SIM_Time &time,
				const SIM_Time &timestep,
				const char *impactsdataname,
				UT_Vector3 &sumforces, 
				UT_Vector3 &sumtorques) const;

    /// Methods to calculate physical properties either from the SDF
    /// or from geometry.  The latter will assume a ThinPlate model.
    fpreal		 computeMassFromSDF(const GU_SDF *sdf);
    fpreal		 computeMassFromGeometry(const SIM_Geometry *geo);

    void		 computeCenterOfMassFromSDF(UT_Vector3 &com, 
					    const GU_SDF *sdf);
    void		 computeCenterOfMassFromGeometry(UT_Vector3 &com, 
					    const SIM_Geometry *geo);

    void		 computeInertialTensorFromSDF(UT_DMatrix3 &tensor,
					    const UT_Vector3 &com,
					    const GU_SDF *sdf);
    void		 computeInertialTensorFromGeometry(UT_DMatrix3 &tensor,
					    const UT_Vector3 &com,
					    const SIM_Geometry *geo);
    fpreal   		 computePrimMass(const GU_Detail *gdp, 
					    const GEO_Primitive *prim) const;
    UT_Vector3		 computePrimBaryCenter(const GU_Detail *gdp,
					    const GEO_Primitive *prim) const;

    // Samples a force, querying the force for its optimal sampling
    // metric and then invoking the proper sampleFooForce.
    void		 sampleForce(const RBD_Object *obj,
				     const UT_Vector3 &com,
				     const SIM_Force *forcedata,
				     UT_Vector3 &sumforce,
				     UT_Vector3 &sumtorque,
				     UT_Matrix &sumDfDx,
				     UT_Matrix &sumDfDv) const;

    // Samples a force over the volume of this object.
    void		 sampleVolumeForce(const RBD_Object *obj,
					const UT_Vector3 &com,
					const SIM_Force *forcedata,
					UT_Vector3 &sumforce,
					UT_Vector3 &sumtorque,
					UT_Matrix &sumDfDx,
					UT_Matrix &sumDfDv) const;
    // Samples the force over the area of this object
    void		 sampleAreaForce(const RBD_Object *obj,
					const UT_Vector3 &com,
					const SIM_Force *forcedata,
					UT_Vector3 &sumforce,
					UT_Vector3 &sumtorque,
					UT_Matrix &sumDfDx,
					UT_Matrix &sumDfDv) const;
    // Samples the force over the given point sample.
    void		 samplePointForce(const RBD_Object *obj,
					const UT_Vector3 &com,
					const SIM_Force *forcedata,
					UT_Vector3 &sumforce,
					UT_Vector3 &sumtorque,
					UT_Matrix &sumDfDx,
					UT_Matrix &sumDfDv) const;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(RBD_State,
			SIM_Motion,
			"RBD State",
			getStateDopDescription());
};

#endif

