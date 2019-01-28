/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Force_h__
#define __SIM_Force_h__

#include "SIM_API.h"
#include "SIM_DataUtils.h"
#include "SIM_PhysicalParms.h"
#include <UT/UT_Matrix.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>

class GU_ConstDetailHandle;
class SIM_Geometry;
class SIM_Position;
class SIM_Motion;
class SIM_ForceResolver;

/// This class is used for callbacks from SIM_Force::getPointForces().
class SIM_API SIM_PointForceCallback
{
public:
			 SIM_PointForceCallback()
			 { }
    virtual		~SIM_PointForceCallback()
			 { }

    /// This callback is called for each point in a geometry. This callback
    /// is executed by SIM_Force::getPointForces(). The returned force
    /// and torque are in either geometry space or world space depnding
    /// on the value of the forcesinworldspace flag passed to the
    /// getPointForces function.
    virtual void	 forceCallbackOffset(GA_Offset ptoff,
				       const UT_Vector3 &force,
				       const UT_Vector3 &torque) = 0;
};

/// This is the definition of the SIM_Force interface. The only function that
/// needs to be overridden (besides any standard SIM_Data overrides) is the
/// getForceSubclass() function.
class SIM_API SIM_Force : public SIM_Data
{
public:
    typedef enum
    {
	SIM_FORCE_SAMPLEPOINT,
	SIM_FORCE_SAMPLECIRCLE,
	SIM_FORCE_SAMPLESPHERE
    } SIM_ForceSample;
    
    /// Returns the force and torque applied by this force to a point.
    /// This function calls getForceSubclass().
    void		 getForce(const SIM_Object &object,
				const UT_Vector3 &position,
				const UT_Vector3 &velocity,
				const UT_Vector3 &angvel,
				const fpreal mass,
				UT_Vector3 &force,
				UT_Vector3 &torque) const;
    /// Returns the forces and torques applied by this force to a list
    /// of points.  This function calls getForceSetSubclass().
    /// This function is available so that subclasses that implement
    /// getForceSetSubclass can make use of multithreading for a
    /// set of force calculations.  Returns true if ok, false otherwise.
    /// (We use this return value to determine whether or not a subclass
    /// actually implements this function)
    bool		 getForceSet(const SIM_Object &object,
				const UT_Vector3Array &positions,
				const UT_Vector3Array &velocities,
				const UT_Vector3Array &angvelocities,
				const UT_FloatArray &masses,
				UT_Vector3Array &forces,
				UT_Vector3Array &torques) const;
    /// Creates a resolver to evaluate this force on the given object.
    /// The caller must delete the returned resolver.
    SIM_ForceResolver   *getForceResolver(const SIM_Object &object) const;

    /// Returns the force and torque applied by this force to a circle.
    /// This function calls getForceOnCircleSubclass().
    void		 getForceOnCircle(const SIM_Object &object,
				const UT_Vector3 &position,
				const UT_Vector3 &normal,
				const fpreal radius,
				const UT_Vector3 &velocity,
				const UT_Vector3 &angvel,
				const fpreal mass,
				UT_Vector3 &force,
				UT_Vector3 &torque) const;
    /// Returns the force and torque applied by this force to a sphere.
    /// This function calls getForceOnSphereSubclass().
    void		 getForceOnSphere(const SIM_Object &object,
				const UT_Vector3 &position,
				const fpreal radius,
				const UT_Vector3 &velocity,
				const UT_Vector3 &angvel,
				const fpreal mass,
				UT_Vector3 &force,
				UT_Vector3 &torque) const;

    /// Returns the derivatives of force & torque with respect
    /// to position and velocity.  The jacobians are 6x6 matrices
    /// which will be initialized as UT_Matrix(1, 6, 1, 6).
    void		getForceJacobian(const SIM_Object &object,
				const UT_Vector3 &position,
				const UT_Vector3 &velocity,
				const UT_Vector3 &angvel,
				const fpreal mass,
				UT_Matrix &dFdX,
				UT_Matrix &dFdV) const;
				

    /// Allows solvers to query what the optimal sampling type
    /// for this force would be.  Solvers do not have to respect this.
    /// This should be user overrideable so users can trade efficiency
    /// for accuracy.
    SIM_ForceSample	getOptimalForceSampling() const;

    /// This is a convenience function for applying a force to each
    /// point in a piece of geometry. For each point in the geometry,
    /// the force on that point is calculated with the getForce()
    /// function. The result of each call is passed to the
    /// SIM_PointForceCallback::forceCallback() function, along with
    /// the point number. The position, velocity, and mass for each
    /// point come from the point attributes (if they exist), modified
    /// by the SIM_Position or SIM_Motion data if they are passed in.
    void		 getPointForces(SIM_PointForceCallback &cb,
					const SIM_Object &object,
					const GU_ConstDetailHandle &gdh,
					const UT_DMatrix4 &geoxform,
					const SIM_Position *position,
					const SIM_Motion *motion,
					bool forcesinworldspace) const;

protected:
    explicit		 SIM_Force(const SIM_DataFactory *factory);
    virtual		~SIM_Force();

    /// Looks for a SIM_Noise subdata and multiplies the force or torque
    /// vector by the noise value at the specified position.
    void		 applyNoise(const UT_Vector3 &pos,
				    UT_Vector3 &forceortorque) const;
    /// Looks for a SIM_Noise subdata and multiplies the force and torque
    /// vector by the noise value at the specified position. This is
    /// equivalent to calling the alternate applyNoise function twice,
    /// but eliminates the need to look up the noise value twice.
    void		 applyNoise(const UT_Vector3 &pos,
				    UT_Vector3 &force,
				    UT_Vector3 &torque) const;
    /// The dFdX and dFdV jacobians should already be filled out
    /// with the jacobians of this force prior to noise.  They are
    /// then scaled to match the magnitude of the noise at this location.
    void		 applyNoiseJacobian(const UT_Vector3 &pos,
				    UT_Matrix &dFdX,
				    UT_Matrix &dFdV) const;

    /// Returns the force and torque applied by this force to a point. The
    /// outputs may be modified according to the information passed in about
    /// the object to which the force is being applied. The default
    /// implementation sets the force and torque to zero.
    virtual void	 getForceSubclass(const SIM_Object &object,
				const UT_Vector3 &position,
				const UT_Vector3 &velocity,
				const UT_Vector3 &angvel,
				const fpreal mass,
				UT_Vector3 &force,
				UT_Vector3 &torque) const;
    /// Creates a force resolver to evaluate this force in a threadsafe
    /// manner.
    virtual SIM_ForceResolver *getForceResolverSubclass(const SIM_Object &object) const;
    /// Returns the forces and torques applied by this force to a list of
    /// points.  Returns true if ok, false otherwise.  This return value
    /// is used to determine whether or not subclasses actually implement
    /// this function.
    /// If this is implemented, it is the implementations responsibility
    /// to resize the forces & torque arrays appropriately.
    virtual bool	 getForceSetSubclass(const SIM_Object &object,
				const UT_Vector3Array &positions,
				const UT_Vector3Array &velocities,
				const UT_Vector3Array &angvelocities,
				const UT_FloatArray &masses,
				UT_Vector3Array &forces,
				UT_Vector3Array &torque) const;
    /// Returns the force and torque applied by this force to a circle. The
    /// outputs may be modified according to the information passed in about
    /// the object to which the force is being applied. The default
    /// implementation calls getForce() and assumes this is the force on
    /// a unit area. So the force and torque are then scaled by the area
    /// of the circle.
    virtual void	 getForceOnCircleSubclass(const SIM_Object &object,
				const UT_Vector3 &position,
				const UT_Vector3 &normal,
				const fpreal radius,
				const UT_Vector3 &velocity,
				const UT_Vector3 &angvel,
				const fpreal mass,
				UT_Vector3 &force,
				UT_Vector3 &torque) const;
    /// Returns the force and torque applied by this force to a sphere. The
    /// outputs may be modified according to the information passed in about
    /// the object to which the force is being applied. The default
    /// implementation calls getForce() and assumes that the result
    /// is a force per unit volume.  The force and torque are then
    /// scaled by the volume of the sphere.
    virtual void	 getForceOnSphereSubclass(const SIM_Object &object,
				const UT_Vector3 &position,
				const fpreal radius,
				const UT_Vector3 &velocity,
				const UT_Vector3 &angvel,
				const fpreal mass,
				UT_Vector3 &force,
				UT_Vector3 &torque) const;

    /// Returns the derivatives of force & torque with respect
    /// to position and velocity.  The jacobians are 6x6 matrices
    /// indexed from 1 (ie, dFdX(1,1) is first element).
    /// The default behaviour is to return 0 matrices.  Note that
    /// the matrices are already initialized to 0 when this is called.
    virtual void	getForceJacobianSubclass(const SIM_Object &object,
				const UT_Vector3 &position,
				const UT_Vector3 &velocity,
				const UT_Vector3 &angvel,
				const fpreal mass,
				UT_Matrix &dFdX,
				UT_Matrix &dFdV) const;

    /// Allows solvers to query what the optimal sampling type
    /// for this force would be.  Solvers do not have to respect this.
    /// This should be user overrideable so users can trade efficiency
    /// for accuracy.
    virtual SIM_ForceSample	getOptimalForceSamplingSubclass() const;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_Force, SIM_Data);
};

/// This class is used to create a usually threadsafe resolver
/// to evaluate forces.  It binds most of the data to cut the number
/// of parameter evaluations needed, caching where sensible.
/// Each thread should have its own resolver for thread-local caches,
/// so use the copy() method for each thread.
class SIM_API SIM_ForceResolver
{
public:
	     SIM_ForceResolver();
	     SIM_ForceResolver(const SIM_Force *force, const SIM_Object *obj);
	     SIM_ForceResolver(const SIM_ForceResolver &src);
    virtual ~SIM_ForceResolver();

    SIM_ForceResolver &operator=(const SIM_ForceResolver &src);

    virtual SIM_ForceResolver *copy() const = 0;

    /// Defaults to the resolver being threadsafe provided all
    /// of the noise resolvers are threadsafe.
    virtual bool threadsafe() const;

    virtual void getForce(const UT_Vector3 &position,
			  const UT_Vector3 &velocity,
			  const UT_Vector3 &angvel,
			  const fpreal mass,
			  UT_Vector3 &force,
			  UT_Vector3 &torque) = 0;

    /// Whether the getForceJacobian returns non-zero values.
    /// The default implementation is to always return a 0 jacobian,
    /// hence hasJacobian of false.
    virtual bool hasJacobian() const { return false; }
    virtual void getForceJacobian(const UT_Vector3 &position,
				const UT_Vector3 &velocity,
				const UT_Vector3 &angvel,
				const fpreal mass,
				UT_Matrix &dFdX,
				UT_Matrix &dFdV);

    /// Does componentwise multiplication of the given vectors
    /// with our noise resolvers.
    void applyNoise(const UT_Vector3 &pos,
			    UT_Vector3 &forceortorque);
    void applyNoise(const UT_Vector3 &pos,
			    UT_Vector3 &force,
			    UT_Vector3 &torque);
    void applyNoiseJacobian(const UT_Vector3 &pos,
			    UT_Matrix &dFdX,
			    UT_Matrix &dFdV);

protected:
    UT_ValArray<SIM_PropertyResolver *>	myNoiseResolvers;
};


class SIM_API SIM_ForceResolverGeneric : public SIM_ForceResolver
{
public:
	    SIM_ForceResolverGeneric(const SIM_Force *force,
				     const SIM_Object *object)
	    {
		myForce = force;
		myObject = object;
	    }

    virtual SIM_ForceResolver *copy() const 
    {
	return new SIM_ForceResolverGeneric(*this);
    }

    virtual bool threadsafe() const { return false; }

    virtual void getForce(const UT_Vector3 &position,
			  const UT_Vector3 &velocity,
			  const UT_Vector3 &angvel,
			  const fpreal mass,
			  UT_Vector3 &force,
			  UT_Vector3 &torque)
    {
	myForce->getForce(*myObject, position, velocity, angvel,
			    mass, force, torque);
    }
    virtual bool hasJacobian() const { return true; }
    virtual void getForceJacobian(const UT_Vector3 &position,
				const UT_Vector3 &velocity,
				const UT_Vector3 &angvel,
				const fpreal mass,
				UT_Matrix &dFdX,
				UT_Matrix &dFdV)
    {
	myForce->getForceJacobian(*myObject,
			    position, velocity, angvel,
			    mass,
			    dFdX, dFdV);
    }
protected:
    const SIM_Force	*myForce;
    const SIM_Object	*myObject;
};

#endif

