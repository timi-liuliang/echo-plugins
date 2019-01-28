/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Integrator.h ( GAS Library, C++)
 *
 * COMMENTS:	Used to integrate the forces in a particle fluid
 *		simulation, and to provide several integration
 *		methods.
 */

#ifndef __GAS_Integrator__
#define __GAS_Integrator__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

#include <UT/UT_ValArray.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_Vector.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_ThreadedAlgorithm.h>

class GU_Detail;

class SIM_GeometryCopy;
class SIM_Impacts;
class GAS_SPH;

// Definitions for number of steps in each integration type
#define GAS_RK_45_NUMSTEPS 6
#define GAS_RK_4_NUMSTEPS 4
#define GAS_RK_21_NUMSTEPS 3
#define GAS_RK_32_NUMSTEPS 4
#define GAS_EULER_NUMSTEPS 1
#define GAS_MIDPOINT_NUMSTEPS 2

class GAS_API GAS_Integrator : public GAS_SubSolver
{
public:
    GETSET_DATA_FUNCS_I(SIM_NAME_PRIMARYSOLVER, PrimarySolver);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_I("integratetype", IntegrateType);
    GET_DATA_FUNC_I("advecttype", AdvectType);
    GET_DATA_FUNC_F("xsphconstant", XsphConstant);
    GET_DATA_FUNC_B("docollision", DoCollision);
    GET_DATA_FUNC_B("resolvesdf", ResolveSDF);
    GET_DATA_FUNC_B("prestashpositions", PreStashPositions);
    GET_DATA_FUNC_B("poststashpositions", PostStashPositions);
    GET_DATA_FUNC_B("collisionuv", CollisionUV);
    GET_DATA_FUNC_B("docollisionfeedback", DoCollisionFeedback);
    GET_DATA_FUNC_B("addimpacts", AddImpacts);
    GET_DATA_FUNC_I("scaletype", ScaleType);
    GET_DATA_FUNC_B("doincremental", IncrementalForce);
    GET_DATA_FUNC_B("integrateorientation", IntegrateOrientation);
    GET_DATA_FUNC_B("integratevelocity", IntegrateVelocity);
    GET_DATA_FUNC_F("errortolerance", ErrorTolerance);
    GET_DATA_FUNC_F("repetitiontolerance", RepTolerance);
    GET_DATA_FUNC_F("minsubstep", MinSubstep);
    GET_DATA_FUNC_F("maxsubstep", MaxSubstep);
    GET_DATA_FUNC_F("defaultpscale", DefaultPScale);
    GET_DATA_FUNC_B("usepscaleforsdf", UsePScaleForSDF);
    GET_DATA_FUNC_B("pscaleisradius", PScaleIsRadius);
    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

    // Available integration types
    enum IntegrateMethod {
	EULER,
	MIDPOINT,
	RUNGE_KUTTA,
#if 0
	NEW_RUNGE_KUTTA,
#endif
	RUNGE_KUTTA_FEHLBERG,
	RUNGE_KUTTA_21,
	RUNGE_KUTTA_32,
	LEAPFROG
    };

    // Different ways of scaling the forces
    enum ScaleMethod {
	MASS,
	DENSITY
    };

    // Different advection methods
    enum AdvectionMethod {
	STANDARD = 0,
	XSPH
    };

    enum ObjectState {
	OBJECTSTATE_INITIAL = 0,
	OBJECTSTATE_CURRENT,
	OBJECTSTATE_TOTAL
    };

    /// Asks all inputs to compute their forces and integrates
    /// them for the given timestep.  We may do this several times
    /// if a higher-order timestep is used.
    virtual SIM_Result	 solveObjectsSubclass(SIM_Engine &engine,
				SIM_ObjectArray &objects,
				SIM_ObjectArray &newobjects,
				SIM_ObjectArray &feedbacktoobjects,
				const SIM_Time &timestep);

    bool		 shouldMultiThread() { return true; }

protected:
    explicit		 GAS_Integrator(const SIM_DataFactory *factory);
    virtual		~GAS_Integrator();

    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep)
			 {
			     return true;
			 }

    virtual void	 getImpulseMassMatrixSubclass(
				const SIM_Object &object,
				const UT_Vector3 &impulseworldpos,
				UT_DMatrix3 &immatrix) const;
    virtual void	 getPointImpulseMassMatrixSubclass(
				const SIM_Object &object,
				int ptnum, UT_DMatrix3 &immatrix) const;
    virtual SIM_PointImpulseMassMatrixResolver *getPointImpulseMassMatrixResolverSubclass(
				const SIM_Object &object) const;

private:
    /// A struct used to store intermediate information about an object
    /// during multi-step integration methods.
    struct ObjectInfo {
	UT_Vector3Array	 myInitialPositions;
	UT_Vector3Array	 myInitialVelocities;
	UT_ValArray<UT_Quaternion>	 myInitialOrientations;
	UT_Vector3Array  myInitialAngularVelocities;

	UT_Vector3Array	 myCurrentPositions;
	UT_Vector3Array	 myCurrentVelocities;
	UT_ValArray<UT_Quaternion>	 myCurrentOrientations;
	UT_Vector3Array  myCurrentAngularVelocities;

	UT_Vector3Array	 myTotalPositions;
	UT_Vector3Array	 myTotalVelocities;
	UT_ValArray<UT_Quaternion>	 myTotalOrientations;
	UT_Vector3Array  myTotalAngularVelocities;
    };

    typedef UT_ValArray<UT_Quaternion>	 gas_QuatArray;

    /// This data structure is used to store all intermediate information
    /// used by Runge-Kutta style integrators
    struct RungeKuttaDataTable {
	/// We store two arrays of vector arrays - one for positions and
	/// the other for velocities.  Each position/velocity array pair
	/// represents one step in the particular RK method.  Similarly,
	/// we store orientations/angular velocities as well.
	UT_Array<UT_Vector3Array>	 myPositionStepData;
	UT_Array<UT_Vector3Array>	 myVelocityStepData;
	UT_Array<UT_Vector3Array>	 myOrientationStepData;
	UT_Array<UT_Vector3Array>	 myAngVelocityStepData;
	int				 myNumSteps;

	RungeKuttaDataTable() : myNumSteps(0) {}

	/// Accessor function for each of the step arrays
	UT_Vector3Array			&getPositionStep(int i)
					 {
					     return myPositionStepData(i);
					 }

	UT_Vector3Array			&getVelocityStep(int i)
					 {
					     return myVelocityStepData(i);
					 }

	UT_Vector3Array			&getOrientationStep(int i)
					 {
					     return myOrientationStepData(i);
					 }

	UT_Vector3Array			&getAngVelocityStep(int i)
					 {
					     return myAngVelocityStepData(i);
					 }

	void				 resizeIfNeeded(int npts, bool doangular = false)
					 {
					     for (int i = 0; i < myNumSteps; i++)
					     {
						 myPositionStepData(i).entries(npts);
						 myVelocityStepData(i).entries(npts);
						 // Only set these array sizes if doangular is true
						 if (doangular)
						 {
						     myOrientationStepData(i).entries(npts);
						     myAngVelocityStepData(i).entries(npts);
						 }
					     }
					 }

	void				 addStepsIfNeeded(int numsteps)
					 {
					     while (myNumSteps < numsteps)
					     {
						 myPositionStepData.append();
						 myVelocityStepData.append();
						 myOrientationStepData.append();
						 myAngVelocityStepData.append();

						 myNumSteps++;
					     }
					 }
    };

    /// Solves the intermediate stages for the specified runge-kutta
    /// integration scheme for each object in 'objects'.  The number
    /// of integration steps is specified in numsteps.  'coefficients'
    /// is a matrix specifying the runge-kutta coefficients used in
    /// each integration step.  The dimension of coefficients is
    /// (numsteps-1) x (numsteps-1) since the first step in any RK
    /// scheme is always the same.  'timecoefficients' are the
    /// coefficients necessary for incrementing the time at each step.
    /// This array also has only (numsteps-1) entries, since the first
    /// of these entries would always be 0 if it had more.
    void		 solveRungeKuttaSteps(SIM_Engine &engine,
					      SIM_ObjectArray &objects,
					      SIM_ObjectArray &newobjects,
					      SIM_ObjectArray &feedbacktoobjects,
					      const SIM_Time &timestep,
					      int numsteps,
					      const fpreal64 *coefficients,
					      const fpreal64 *timecoefficients,
					      bool cplUpdate=true);

    /// This helper function is used to reinitialize each object's
    /// particle system after a runge-kutta integration step.  It
    /// determines the change from the last step and stores this
    /// in the appropriate runge-kutta data table.  It then sets
    /// up the system in preparation for the next step.
    /// 'coefficients' stores the RK coefficients needed to set
    /// up the state for the next step.
    void		 reinitializeSystems(SIM_ObjectArray &objects,
					     int numsteps, int stepnum,
					     const SIM_Time &timestep,
					     const fpreal64 *coefficients,
					     bool cplUpdate=true);

    class rkInfoParms
    {
    public:
	int numsteps, stepnum;
	SIM_Time timestep;
	const fpreal64 *coefficients;
	bool cplUpdate;
    };
    /// Called by the above function to reinitialize a specific
    /// object's particle system.
    THREADED_METHOD6(GAS_Integrator, gdp->getNumPoints() > 2048,
			reinitializeSystem,
			GU_Detail *, gdp,
			const GA_PointGroup *, ptgrp,
			RungeKuttaDataTable &, data,
			ObjectInfo &, inf,
			const rkInfoParms &, parms,
			int, npts)
    void		 reinitializeSystemPartial(GU_Detail *gdp,
					    const GA_PointGroup *ptgrp,
					    RungeKuttaDataTable &data,
					    ObjectInfo &inf,
					    const rkInfoParms &parms,
					    int npts,
					    const UT_JobInfo &info);
    
    /// This helper function is used to compute the final positions,
    /// velocities, etc. of all particles using the already-computed
    /// runge-kutta step data.
    void		 computeFinalStates(SIM_ObjectArray &objects,
				    int numsteps, const fpreal64 *coefficients);

    /// Called by the above function to compute the final state for
    /// a specific object's particle system.
    THREADED_METHOD7(GAS_Integrator, gdp->getNumPoints() > 2048,
		    computeFinalState,
		    GU_Detail *, gdp,
		    const GA_PointGroup *, ptgrp,
		    RungeKuttaDataTable &, data,
		    ObjectInfo &, inf, 
		    int, numsteps,
		    const fpreal64 *, coefficients,
		    int, npts)
			
    void		 computeFinalStatePartial(GU_Detail *gdp,
				    const GA_PointGroup *ptgrp,
				    RungeKuttaDataTable &data,
				    ObjectInfo &inf, int numsteps,
				    const fpreal64 *coefficients,
				    int npts,
				    const UT_JobInfo &info);

    /// This is a helper function used to reset everything back to its initial
    /// state.
    void		 resetInitialStates(SIM_ObjectArray &objects);

    /// Copies the ObjectInfo's current, total, or initial data
    /// into the geometry data
    void		 objectStateToGeo(GU_Detail *gdp,
				const GA_PointGroup *ptgrp,
				ObjectInfo &inf,
				ObjectState state);
    THREADED_METHOD5(GAS_Integrator, gdp->getNumPoints() > 2048,
			objectStateToGeo,
			GU_Detail *, gdp,
			const GA_PointGroup *, ptgrp,
			ObjectInfo &, inf,
			ObjectState, state,
			GA_Size, npts)
    void		 objectStateToGeoPartial(GU_Detail *gdp,
				    const GA_PointGroup *ptgrp,
				    ObjectInfo &inf,
				    ObjectState state,
				    GA_Size npts,
				    const UT_JobInfo &info);

    /// Returns the maximum number of points to safely iterate over.
    /// Adds a warning if the point count chaned.
    GA_Size		 determinePointCount(const GU_Detail *gdp, ObjectInfo &inf);

    /// This function just caches the states of all systems in their
    /// respective object info arrays.
    void		 cacheSystemStates(SIM_ObjectArray &objects);

    /// Copies the gdp's data into the given object state arrays.
    void		 geoToObjectState(const GU_Detail *gdp,
				const GA_PointGroup *ptgrp,
				ObjectInfo &inf,
				ObjectState state);

    THREADED_METHOD5(GAS_Integrator, gdp->getNumPoints() > 2048,
			geoToObjectState,
			const GU_Detail *, gdp,
			const GA_PointGroup *, ptgrp,
			ObjectInfo &, inf,
			ObjectState, state,
			GA_Size, npts)
    void		 geoToObjectStatePartial(const GU_Detail *gdp,
				    const GA_PointGroup *ptgrp,
				    ObjectInfo &inf,
				    ObjectState state,
				    GA_Size npts,
				    const UT_JobInfo &info);

    /// Determines the "distance" between the velociteis currently stored
    /// in the object geometries and the velocities stored in our object
    /// info array.  We just take the maximum distance for any system.
    fpreal64		 getMaxDistance(SIM_ObjectArray &objects);

    /// Finds the "distance" described above for the given system.
    fpreal64		 getDistance(const GU_Detail *gdp,
					const GA_PointGroup *ptgrp, 
					ObjectInfo &inf);

    /// This function is used for embedded methods to determine whether
    /// or not an iteration succeeded (ie. if the two
    /// approximations made by the method agree to a specified accuracy).
    /// This is done by comparing the myCurrentVelocities array in
    /// the system's ObjectInfo struct to the current velocity of
    /// each particle in the system.
    bool		 embeddedRKsucceeded(SIM_ObjectArray &objects,
				    const SIM_Time &timestep,
				    fpreal64 toleranceCoefficient);

    /// Called by the above function on an individual system.  This will
    /// also place the timestep recommended by the embedded method in to
    /// an attribute on the gdp so that it can be used by GAS_SubStep.
    bool		 embeddedRKsucceeded(GU_Detail *gdp, 
				    const GA_PointGroup *ptgrp,
				    ObjectInfo &inf,
				    const SIM_Time &timestep, fpreal tolerance,
				    fpreal reptolerance, fpreal minsubstep,
				    fpreal maxsubstep,
				    fpreal64 toleranceCoefficient);

    /// This function is used for standard integrators to determine
    /// whether or not an iteration succeeded (ie. if the velocity
    /// difference (acceleration) does not exceed an error condition)
    bool		 integrationSucceeded(SIM_ObjectArray &objects,
    				    const SIM_Time &timestep);

    /// Called by the above function on an individual system.  This
    /// will also place the recommended timestep in to an attribute
    /// on the gdp so that it can be used by GAS_SubStep
    bool		 integrationSucceeded(GU_Detail *gdp, 
				    const GA_PointGroup *ptgrp,
				    ObjectInfo &inf,
    				    const SIM_Time &timestep, fpreal tolerance,
				    fpreal reptolerance, fpreal minsubstep,
				    fpreal maxsubstep);

    /// Integrates the system for the given timestep using the
    /// Euler method.
    SIM_Result		 doEuler(SIM_Engine &engine,
				    SIM_ObjectArray &objects,
				    SIM_ObjectArray &newobjects,
				    SIM_ObjectArray &feedbacktoobjects,
				    const SIM_Time &starttime,
				    const SIM_Time &endtime,
				    const SIM_Time &timestep,
				    const SIM_Time &scaledtime);

    /// Integrates the system for the given timestep using the
    /// Midpoint method.
    SIM_Result		 doMidpoint(SIM_Engine &engine,
				    SIM_ObjectArray &objects,
				    SIM_ObjectArray &newobjects,
				    SIM_ObjectArray &feedbacktoobjects,
				    const SIM_Time &starttime,
				    const SIM_Time &endtime,
				    const SIM_Time &timestep,
				    const SIM_Time &scaledtime);

    /// Integrates the system for the given timestep using the
    /// Runge-Kutta method.
    SIM_Result		 doRungeKutta(SIM_Engine &engine,
				    SIM_ObjectArray &objects,
				    SIM_ObjectArray &newobjects,
				    SIM_ObjectArray &feedbacktoobjects,
				    const SIM_Time &starttime,
				    const SIM_Time &endtime,
				    const SIM_Time &timestep,
				    const SIM_Time &scaledtime);

    /// Integrate the system using the Runge-Kutta-Fehlberg method.
    /// We also return a flag indicating if this failed, and a new
    /// recommended substep value.
    SIM_Result		 doRungeKuttaFehlberg(SIM_Engine &engine,
				    SIM_ObjectArray &objects,
				    SIM_ObjectArray &newobjects,
				    SIM_ObjectArray &feedbacktoobjects,
				    const SIM_Time &starttime,
				    const SIM_Time &endtime,
				    const SIM_Time &timestep,
				    const SIM_Time &scaledtime);

    /// Integrate the system using an embedded Runge-Kutta 2(1) method.
    /// We also return a flag indicating if this failed, and a new
    /// recommended substep value.
    SIM_Result		 doRungeKutta21(SIM_Engine &engine,
				    SIM_ObjectArray &objects,
				    SIM_ObjectArray &newobjects,
				    SIM_ObjectArray &feedbacktoobjects,
				    const SIM_Time &starttime,
				    const SIM_Time &endtime,
				    const SIM_Time &timestep,
				    const SIM_Time &scaledtime);

    /// Integrate the system using an embedded Runge-Kutta 3(2) method.
    /// We also return a flag indicating if this failed, and a new
    /// recommended substep value.
    SIM_Result		 doRungeKutta32(SIM_Engine &engine,
				    SIM_ObjectArray &objects,
				    SIM_ObjectArray &newobjects,
				    SIM_ObjectArray &feedbacktoobjects,
				    const SIM_Time &starttime,
				    const SIM_Time &endtime,
				    const SIM_Time &timestep,
				    const SIM_Time &scaledtime);

    /// Leapfrog integrator - a second order integrator requiring
    /// only a single force evaluation per time step.
    SIM_Result		 doLeapfrog(SIM_Engine &engine,
    				    SIM_ObjectArray &objects,
				    SIM_ObjectArray &newobjects,
				    SIM_ObjectArray &feedbacktoobjects,
				    const SIM_Time &starttime,
				    const SIM_Time &endtime,
				    const SIM_Time &timestep,
				    const SIM_Time &scaledtime);

    // Helpfer functions for the Leapfrog method.  The first updates
    // particle positions according to the leapfrog method, while the
    // second updates velocities.
    void		 leapfrogAdvectObjs(SIM_ObjectArray &objects,
    				    const SIM_Time &timestep);

    void		 leapfrogAdvect(SIM_Object *obj, GU_Detail *gdp,
				    const GA_PointGroup *ptgrp,
    				    const SIM_Time &timestep);

    void		 leapfrogIntegrateObjs(SIM_ObjectArray &objects,
    				    const SIM_Time &timestep);

    void		 leapfrogIntegrate(GU_Detail *gdp,
				    const GA_PointGroup *ptgrp,
    				    const SIM_Time &timestep);

    /// Helper functions for the Leapfrog method.  These apply
    /// the position and velocity symplectic shear operators
    /// to the system.
    void		 leapfrogPositionUpdate(SIM_ObjectArray &objects,
    				    const SIM_Time &timestep,
				    fpreal scale);

    void		 leapfrogPositionUpdate(SIM_Object *obj,
				    GU_Detail *gdp,
				    const GA_PointGroup *ptgrp,
    				    const SIM_Time &timestep,
				    fpreal scale);

    void		 leapfrogVelocityUpdate(SIM_Engine &engine,
    				    SIM_ObjectArray &objects,
				    SIM_ObjectArray &newobjects,
				    SIM_ObjectArray &feedbacktoobjects,
    				    const SIM_Time &timestep,
				    fpreal scale);

#if 0
    /// FIXME
    /// Integrate the system with the runge-kutta 4 method using
    /// our new setup
    ///
    /// Note: solveRungeKuttaSteps now defaults to cplUpdate 
    ///       instead of Taylor updates, this is now better
    ///        
    void		 doNewRungeKutta(SIM_Engine &engine,
				    SIM_ObjectArray &objects,
				    SIM_ObjectArray &newobjects,
				    SIM_ObjectArray &feedbacktoobjects,
				    const SIM_Time &timestep);
#endif

    /// Helper function used to initialize our stored object info.
    /// It is also used to intialize Runge Kutta data tables.
    void		 initObjectInfoArray(SIM_ObjectArray &objects,
				    int numRungeKuttaSteps = 0);

    /// Applies all of our subsolvers to the current geometry.  This
    /// function exists so that this can easily be done multiple times
    /// during a multi-step method.
    void		 applyAllSolvers(SIM_Engine &engine,
				    SIM_ObjectArray &objects,
				    SIM_ObjectArray &newobjects,
				    SIM_ObjectArray &feedbacktoobjects,
				    const SIM_Time &timestep,
				    fpreal64 timescale,
				    bool incremental = false);

#if 0
    /// Applies the position solver to each object using the
    /// given timestep.
    void		 applyPositionSolver(SIM_Engine &engine,
				    SIM_ObjectArray &objects,
				    SIM_ObjectArray &newobjects,
				    SIM_ObjectArray &feedbacktoobjects,
				    const SIM_Time &timestep,
				    fpreal64 timescale);
#endif

    /// Clears the force attribute in all suplied data objects.
    void		 clearAllForces(SIM_ObjectArray &objects);

    /// Integrates the current forces acting on the system for one
    /// timestep using the standard Euler method.
    ///	    currentscale:   the amount by which to scale the deltas
    ///			    when adding to the initial values for
    ///			    the particle.
    ///	    totalscale:	    the amount by which to scale the deltas
    ///			    when adding them to the total velocity
    ///			    change.
    void		 applyCurrentDeltas(GU_Detail *gdp,
				    const GA_PointGroup *ptgrp,
				    const SIM_Time &timestep,
				    ObjectInfo &inf,
				    fpreal64 currentscale,
				    fpreal64 totalscale,
				    fpreal64 currentvscale,
				    fpreal64 totalvscale);

    /// Iterates through each object and calls the above function
    /// on it.
    void		 applyDeltasToObjs(SIM_ObjectArray &objects,
				    const SIM_Time &timestep,
				    fpreal64 currentscale,
				    fpreal64 totalscale,
				    fpreal64 currentvscale,
				    fpreal64 totalvscale);

    /// Applies changes to the current velocity of particles in the
    /// geometry based on the contents of the force attribute.  Also
    /// applies the necessary changes to particle positions, depending
    /// on the value of predict.  If predict is false, then particle
    /// positions are integrated based on the starting velocity (before
    /// applying forces).  Otherwise, they are integrated using the
    /// post-force velocity.
    void		 applyVelocityChanges(SIM_Object *obj, GU_Detail *gdp,
				    const GA_PointGroup *ptgrp,
				    const SIM_Time &timestep,
				    ObjectInfo &inf,
				    fpreal64 posscale = 1.0,
				    bool predict = true,
				    fpreal64 velscale = 1.0);

    void		 applyVelocityChangesToObjs(SIM_ObjectArray &objects,
				    const SIM_Time &timestep,
				    fpreal64 posscale = 1.0,
				    bool predict = true,
				    fpreal64 velscale = 1.0);

    THREADED_METHOD7(GAS_Integrator, gdp->getNumPoints() > 2048,
				    computeVelocityChanges,
				    GU_Detail *, gdp,
				    const GA_PointGroup *, ptgrp,
				    SIM_Time,  timestep,
				    UT_Vector3Array &, velocities,
				    float,  posscale,
				    bool,  predict,
				    float,  velscale)
    void		 computeVelocityChangesPartial(
				    GU_Detail *gdp,
				    const GA_PointGroup *ptgrp,
				    SIM_Time timestep,
				    UT_Vector3Array &velocities,
				    float posscale,
				    bool predict,
				    float velscale,
				    const UT_JobInfo &info);

    /// Applies an incremental velocity change to all
    /// objects, and adjusts the positions of particles
    /// accordingly.
    void		 applyVelocityIncrementsToObjs(SIM_ObjectArray &objects,
				    const SIM_Time &timestep);

    /// Advects the particles in the given geometry using the
    /// given array of "velocity" vectors.  We use an input
    /// array here to allow for modifications to the velocity
    /// (ie. to incorporate instantaneous changes due to acceleration
    /// etc.).  Using this method will allow us to support more
    /// sophisticated advection techniques.  
    /// This method uses the AdvectionTYpe to determine how to act.
    void		 advectParticles(SIM_Object *obj,
				    GU_Detail *gdp,
				    const GA_PointGroup *ptgrp,
    				    const SIM_Time &timestep,
    				    const UT_Vector3Array &velocities);

    /// Performs a simple pos += v * ts update
    THREADED_METHOD4(GAS_Integrator, gdp->getNumPoints() > 2048,
			advectParticlesEuler,
			GU_Detail *, gdp,
			const GA_PointGroup *, ptgrp,
			SIM_Time, timestep,
			const UT_Vector3Array &, velocities)
    void		 advectParticlesEulerPartial(GU_Detail *gdp,
				const GA_PointGroup *ptgrp,
				SIM_Time timestep,
				const UT_Vector3Array &velocities,
				const UT_JobInfo &info);

    /// Performs a euler update on the attributes
    /// Simplistic Euler update with no temp arrays.
    void		 directIntegratePosObjs(SIM_ObjectArray &objects,
				    const SIM_Time &timestep);
    THREADED_METHOD3(GAS_Integrator, gdp->getNumPoints() > 2048,
			directIntegratePos,
			GU_Detail *, gdp,
			const GA_PointGroup *, ptgrp,
			SIM_Time, timestep)
    void		 directIntegratePosPartial(GU_Detail *gdp,
				const GA_PointGroup *ptgrp,
				SIM_Time timestep,
				const UT_JobInfo &info);

    THREADED_METHOD3(GAS_Integrator, gdp->getNumPoints() > 2048,
			directIntegrateOrient,
			GU_Detail *, gdp,
			const GA_PointGroup *, ptgrp,
			SIM_Time, timestep)
    void		 directIntegrateOrientPartial(GU_Detail *gdp,
				const GA_PointGroup *ptgrp,
				SIM_Time timestep,
				const UT_JobInfo &info);

    /// Computes the xsph velocity given the specified velocity array
    THREADED_METHOD6(GAS_Integrator, gdp->getNumPoints() > 2048,
			computeXSPHVel,
			GU_Detail *, gdp,
			const GA_PointGroup *, ptgrp,
			const GAS_SPH &, sph,
			const UT_Vector3Array &, refvel,
			UT_Vector3Array &, xsphvel,
			float, xsphConstant)
    void 		 computeXSPHVelPartial(GU_Detail *gdp,
			    const GA_PointGroup *ptgrp,
			    const GAS_SPH &sph,
			    const UT_Vector3Array &refvel,
			    UT_Vector3Array &xsphvel,
			    float xsphConstant,
			    const UT_JobInfo &info);

    /// Resets positions and velocities back to their current values.
    void		 resetObjectStates(SIM_ObjectArray &objects);

    /// Sets the final value of velocity to the total velocity
    /// accumulated for each point, and sets each point's position
    /// back to its initial value.
    void		 setFinalValues(SIM_ObjectArray &objects);

    /// Responds to impact information attached to a particle by
    /// bouncing the particle from the impact point.
    /// Returns true if anything was changed.
    bool		 bounceParticleFromImpact(GU_Detail *gdp,
				    const SIM_Time &starttime,
				    const SIM_Time &endtime,
				    const SIM_Time &timestep,
				    const UT_DMatrix4 &xform,
				    const UT_DMatrix4 &ixform,
				    const SIM_Impacts *impacts,
				    int impactidx, GA_Index ptnum);

    /// Responds to all impacts for a given object.
    void		 respondToCollisions(SIM_Engine &engine,
				    SIM_Object &object,
				    const SIM_ObjectArray &affectors,
				    const UT_DMatrix4 &xform,
				    const UT_DMatrix4 &ixform,
				    const UT_DMatrix4 &posxform,
				    const SIM_Time &starttime,
				    const SIM_Time &endtime,
				    const SIM_Time &timestep,
				    const char *impactdataname,
				    bool clearimpacts = false);

    /// Applies impacts to the particles stored in object.
    void		 handleObjectImpacts(SIM_Engine &engine,
				    SIM_Object &object,
				    const SIM_Time &starttime,
				    const SIM_Time &endtime,
				    const SIM_Time &timestep,
				    bool clearimpacts = false);

    /// Applies impacts to all objects.
    void		 handleAllImpacts(SIM_Engine &engine,
				    SIM_ObjectArray &objects,
				    const SIM_Time &starttime,
				    const SIM_Time &endtime,
				    const SIM_Time &timestep,
				    bool clearimpacts = false);

    /// Do any post processing after a timestep
    void                 doPostSolveUpdates( SIM_Engine &engine,
					     SIM_ObjectArray &objects,
					     SIM_ObjectArray &newobjects,
					     SIM_ObjectArray &feedbacktoobjects,
					     const SIM_Time &timestep );

    static const SIM_DopDescription	*getDopDescription();

    /// Gets the solver to use for getting object properties.
    const SIM_Solver			*getCurrentSolver() const;

    /// We store an array of object info objects to keep track of any
    /// intermediate object states during multi-step integration
    /// methods.
    UT_Array<ObjectInfo>		 myObjectInfoArray;

    /// We store a persistent array of runge-kutta data tables, each
    /// of which stores intermediate RK integration steps for a
    /// different object.  We store this permanently so that we don't
    /// have to repeatedly allocate the (undoubedly large amount of)
    /// memory required for this data structure on each substep.
    UT_Array<RungeKuttaDataTable>	 myRungeKuttaData;

    /// Store a pointer to an engine so that we can add errors
    /// as necessary.
    SIM_Engine				*myEngine;

    // Store matrices of values for each type of Runge-Kutta integration
    // we want to do
    static const fpreal64		 RK_45_COEFS[][GAS_RK_45_NUMSTEPS-1];
    static const fpreal64		 RK_45_TIME_COEFS[GAS_RK_45_NUMSTEPS-1];
    static const fpreal64		 RK_45_TOTAL_COEFS4[GAS_RK_45_NUMSTEPS];
    static const fpreal64		 RK_45_TOTAL_COEFS5[GAS_RK_45_NUMSTEPS];

    static const fpreal64		 RK_4_COEFS[][GAS_RK_4_NUMSTEPS-1];
    static const fpreal64		 RK_4_TIME_COEFS[GAS_RK_4_NUMSTEPS-1];
    static const fpreal64		 RK_4_TOTAL_COEFS[GAS_RK_4_NUMSTEPS];

    static const fpreal64		 RK_21_COEFS[][GAS_RK_21_NUMSTEPS-1];
    static const fpreal64		 RK_21_TIME_COEFS[GAS_RK_21_NUMSTEPS-1];
    static const fpreal64		 RK_21_TOTAL_COEFS1[GAS_RK_21_NUMSTEPS];
    static const fpreal64		 RK_21_TOTAL_COEFS2[GAS_RK_21_NUMSTEPS];
    static const fpreal64		 RK_21_TOLERANCE_COEF;

    static const fpreal64		 RK_32_COEFS[][GAS_RK_32_NUMSTEPS-1];
    static const fpreal64		 RK_32_TIME_COEFS[GAS_RK_32_NUMSTEPS-1];
    static const fpreal64		 RK_32_TOTAL_COEFS2[GAS_RK_32_NUMSTEPS];
    static const fpreal64		 RK_32_TOTAL_COEFS3[GAS_RK_32_NUMSTEPS];

    static const fpreal64		 EULER_TOTAL_COEFS[GAS_EULER_NUMSTEPS];

    static const fpreal64		 MIDPOINT_COEFS[][GAS_MIDPOINT_NUMSTEPS-1];
    static const fpreal64		 MIDPOINT_TIME_COEFS[GAS_MIDPOINT_NUMSTEPS-1];
    static const fpreal64		 MIDPOINT_TOTAL_COEFS[GAS_MIDPOINT_NUMSTEPS];

    SIM_Solver				*myCurrentSolver;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Integrator,
			GAS_SubSolver,
			"GAS Integrator",
			getDopDescription());
};

#endif

