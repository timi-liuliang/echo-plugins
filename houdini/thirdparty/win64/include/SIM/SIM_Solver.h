/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Solver_h__
#define __SIM_Solver_h__

#include "SIM_API.h"
#include "SIM_DataUtils.h"
#include "SIM_PhysicalParms.h"

class SIM_Solver;
class SIM_Engine;
class SIM_ObjectArray;

typedef UT_ValArray<SIM_Solver *>	 SIM_SolverArray;
typedef UT_ValArray<const SIM_Solver *>	 SIM_ConstSolverArray;

/// This is the base class for all Solvers. Solvers are the classes that
/// perform the actual simulation for objects. The solver is passed a
/// set of objects with mutual affector relationships.
class SIM_API SIM_Solver : public SIM_Data
{
public:
    /// The possible return codes from a solve operation. These are kept in
    /// order of priority to make multi-solvers easier to write. Any result
    /// value from a subsolver that is greater than the previous highest
    /// subsolver result becmoes the new overall return value.
    typedef enum {
	SIM_SOLVER_SUCCESS,
	SIM_SOLVER_REPEAT,
	SIM_SOLVER_SUBSTEP,
	SIM_SOLVER_FAIL
    } SIM_Result;

    /// Solve for the given objects. This function is called for each timestep
    /// after the one where the objects are first created. This function calls
    /// solveObjectSubclass to perform the real work.
    SIM_Result		 solveObjects(SIM_Engine &engine,
				SIM_ObjectArray &objects,
				SIM_ObjectArray &newobjects,
				SIM_ObjectArray &feedbacktoobjects,
				const SIM_Time &timestep);

    SIM_Result		 postSolveObjects(SIM_Engine &engine,
				SIM_ObjectArray &objects,
				SIM_ObjectArray &newobjects,
				SIM_ObjectArray &feedbacktoobjects,
				const SIM_Time &timestep);

    /// Solves for a relationship.  Invoked if "Solver" is a subdata
    /// of a SIM_Relationship data.
    SIM_Result		 solveRelationship(SIM_Engine &engine,
				SIM_Relationship &relationship,
				const SIM_Time &timestep);

    /// Get the number of cached time steps we want to solve for these objects.
    /// This function calls getRequestedCacheSubclass to get the real value.
    int			 getRequestedCache() const;
    /// Gets the impulse mass matrix of an object for an impulse at the
    /// provided world space position. This function simply calls
    /// getImpulseMassMatrixSubclass().
    void		 getImpulseMassMatrix(const SIM_Object &object,
				const UT_Vector3 &impulseworldpos,
				UT_DMatrix3 &immatrix) const;
    /// Gets the impulse mass matrix of an object for an impulse at the
    /// provided point on the geometry of the object. This function simply
    /// calls getPointImpulseMassMatrixSubclass().
    void		 getPointImpulseMassMatrix(const SIM_Object &object,
				GA_Index ptnum, UT_DMatrix3 &immatrix) const;
    /// Builds a resolver to efficiently compute mass matrices.
    SIM_PointImpulseMassMatrixResolver *getPointImpulseMassMatrixResolver(
				const SIM_Object &object) const;
    /// Gets the value of some physical property for the supplied object
    /// at a given position in world space. This function simply calls
    /// getPropertyAtPositionSubclass().
    fpreal		 getPropertyAtPosition(const SIM_Object &object,
				const UT_Vector3 &worldpos,
				const SIM_Property &property) const;
    /// Gets the value of some physical property for the supplied object
    /// at a given point on the object's geometry. This function simply
    /// calls getPropertyAtPointSubclass().
    fpreal		 getPropertyAtPoint(const SIM_Object &object,
				GA_Index ptnum,
				const SIM_Property &property) const;

    /// Builds a resolver to efficiently compute property values
    SIM_PropertyResolver *getPropertyResolver(const SIM_Object &object,
				    const SIM_Property &property) const;
    /// Gets the default collider label for an object using this solver.
    void		 getDefaultColliderLabel(const SIM_Object &object,
				UT_String &label) const;
    /// Gets the default collider type for use on an object with a
    /// particular collider label. This information is used by
    /// SIM_Object::getCollider() to create a SIM_Collider to use
    /// for collision detection betweena pair of objects. The
    /// collidertype value returns the name of the SIM_Collider
    /// subclass to use, and the collidereverseobjectroles parameter
    /// returns whether the reverse object roles flag should be turned
    /// on for the new collider. See the ReverseObjectRoles flag in
    /// SIM_Collider for an explanation of this parameter.
    void		 getDefaultCollider(const SIM_Object &object,
				const UT_String &colliderlabel,
				UT_String &collidertype,
				bool &colliderreverseobjectroles) const;

    /// Returns the standard attribute name that corresponds to a given
    /// SIM_Property value.
    static const char	*getPropertyAttribName(const SIM_Property &property);
    /// Static method that implements a good standard approach to getting
    /// a property at a given position.
    static fpreal	 getPropertyAtPositionStatic(const SIM_Object &object,
				const UT_Vector3 &worldpos,
				const SIM_Property &property);
    /// Static method that implements a good standard approach to getting
    /// a property at a given point number.
    static fpreal	 getPropertyAtPointStatic(const SIM_Object &object,
				GA_Index ptnum, const SIM_Property &property);
    /// Static resolver for getting property values.
    static SIM_PropertyResolver *getPropertyResolverStatic(
				    const SIM_Object &object,
				    const SIM_Property &property);

    // Sets a temporary start time which may be used by this solver in
    // place of the global simulation time.  This is useful when
    // handling substeps or multi-phase integration schemes.  It is up
    // to each solver to use this start time.
    void		 setStartTime(const SIM_Time &starttime);

    // Returns true if this solver is a trivial solver, ie, does no
    // updates itself.  Is allowed to vary depending on which object
    // is being solved.
    bool		 isStatic(const SIM_Object *obj) const { return isStaticSubclass(obj); }

protected:
    /// Basic SIM_Solver constructor.
    explicit		 SIM_Solver(const SIM_DataFactory *factory);
    /// Basic SIM_Solver destructor.
    virtual		~SIM_Solver();

    // Gets the currently stored start time.  Returns false if the start time
    // is invalid and leaves time unchanged.
    bool		 getStartTime(SIM_Time &time);
    void		 clearStartTime();
 
    /// Gets an array of SIM_Solver subdata attached to this data.
    /// Useful for solvers which switch between or run other solvers.
    void		 getSolverSubdata(SIM_SolverArray &subsolvers,
					UT_StringArray *subsolvernames);
    /// Gets an array of const SIM_Solver subdata attached to this data.
    /// Useful for solvers which switch between or run other solvers.
    void		 getConstSolverSubdata(SIM_ConstSolverArray &subsolvers,
					UT_StringArray *subsolvernames) const;

    /// Determines if this solver does nothing on the object
    virtual bool	 isStaticSubclass(const SIM_Object *obj) const
    { return false; }

    /// This method solves for some objects. It performs whatever processing
    /// is necessary to take objects from their state at one time to another.
    /// The default implementation does nothing. The objects parameter holds
    /// all the objects that should be solved for this timestep. The
    /// newobjects parameter is a set of objects that were just created in
    /// this timestep, and so should in most cases not be solved on this
    /// timestep so that they maintain correct initial conditions.
    virtual SIM_Result	 solveObjectsSubclass(SIM_Engine &engine,
					SIM_ObjectArray &objects,
					SIM_ObjectArray &newobjects,
					SIM_ObjectArray &feedbacktoobjects,
					const SIM_Time &timestep) = 0;

    // return success?
    virtual SIM_Result	 postSolveObjectsSubclass(SIM_Engine &engine,
						  SIM_ObjectArray &objects,
						  SIM_ObjectArray &newobjects,
						  SIM_ObjectArray &feedbacktoobjects,
						  const SIM_Time &timestep){ return SIM_SOLVER_SUCCESS; };

    virtual SIM_Result	 solveRelationshipSubclass(SIM_Engine &engine,
					SIM_Relationship &relationship,
					const SIM_Time &timestep)
    { return SIM_SOLVER_SUCCESS; }

    /// Returns the amount of history required by this solver. Override
    /// this function if your solver requires past data to calculate the
    /// next time step. The default implementation returns 0.
    virtual int		 getRequestedCacheSubclass() const;
    /// Gets the impulse mass matrix of an object for an impulse at the
    /// provided world space position. The default implementation gets the
    /// closest point in the geometry and calls getPointImpulseMassMatrix().
    virtual void	 getImpulseMassMatrixSubclass(
				const SIM_Object &object,
				const UT_Vector3 &impulseworldpos,
				UT_DMatrix3 &immatrix) const;
    /// Gets the impulse mass matrix of an object for an impulse at the
    /// provided point on the geometry of the object. The default
    /// implementation finds the world space position of the point and
    /// calls getImpulseMassMatrix().
    virtual void	 getPointImpulseMassMatrixSubclass(
				const SIM_Object &object,
				int ptnum, UT_DMatrix3 &immatrix) const;
    /// Builds a resolver for evaluating mass matrices swiftly
    virtual SIM_PointImpulseMassMatrixResolver *getPointImpulseMassMatrixResolverSubclass(
				const SIM_Object &object) const;
    /// Gets the value of some physical property for the supplied object
    /// at a given position in world space.
    virtual fpreal	 getPropertyAtPositionSubclass(const SIM_Object &object,
				const UT_Vector3 &worldpos,
				const SIM_Property &property) const;
    /// Gets the value of some physical property for the supplied object
    /// at a given point on the object's geometry.
    virtual fpreal	 getPropertyAtPointSubclass(const SIM_Object &object,
				int ptnum,
				const SIM_Property &property) const;
    /// Builds a resolver for evaluating properties swiftly
    virtual SIM_PropertyResolver *getPropertyResolverSubclass(
				const SIM_Object &object, 
				const SIM_Property &property) const;
    /// Gets the default collider label for an object using this solver.
    virtual void	 getDefaultColliderLabelSubclass(
				const SIM_Object &object,
				UT_String &label) const;
    /// Gets the default collider type for use on an object with a
    /// particular collider label.
    virtual void	 getDefaultColliderSubclass(
				const SIM_Object &object,
				const UT_String &colliderlabel,
				UT_String &collidertype,
				bool &colliderreverseobjectroles) const;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_Solver, SIM_Data);

    // Holds the current time - ie. the time at the beginning
    // of the timestep currently being processed.  This can be
    // used (by using calls to setStartTime and getStartTime)
    // to inform solvers of when they should use a time other
    // than the global simulation time when processing a timestep
    SIM_Time			myCurrentStartTime;
    bool			myCurrentStartTimeValid;
 
};

#endif

