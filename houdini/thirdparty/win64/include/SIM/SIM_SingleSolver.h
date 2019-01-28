/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SingleSolver_h__
#define __SIM_SingleSolver_h__

#include "SIM_API.h"
#include "SIM_Solver.h"

class SIM_Object;

/// This class is a specialized version of SIM_Solver for solvers that
/// handle all objects in mutual affector relationships independently.
/// This class implements the solveObjectsSubclass() and
/// initializeObjectsSubclass() functions by calling the new virtuals
/// solveSingleObjectSubclass() and initializeSingleObjectSubclass()
/// for each object.
class SIM_API SIM_SingleSolver : public SIM_Solver
{
public:
    /// Solve for the given object. This function is called for each timestep
    /// after the one where the objects are first created. This function calls
    /// solveObjectSubclass to perform the real work.
    SIM_Result		 solveSingleObject(SIM_Engine &engine,
					SIM_Object &object,
					SIM_ObjectArray &feedbacktoobjects,
					const SIM_Time &timestep,
					bool newobject);

protected:
    /// Basic SIM_SingleSolver constructor.
    explicit		 SIM_SingleSolver(const SIM_DataFactory *factory);
    /// Basic SIM_SingleSolver destructor.
    virtual		~SIM_SingleSolver();

    /// Override this method to call solveSingleObject().
    virtual SIM_Result	 solveObjectsSubclass(SIM_Engine &engine,
					SIM_ObjectArray &objects,
					SIM_ObjectArray &newobjects,
					SIM_ObjectArray &feedbacktoobjects,
					const SIM_Time &timestep);

    /// This method is called for each object passed to
    /// solveObjectsSubclass().
    virtual SIM_Result	 solveSingleObjectSubclass(SIM_Engine &engine,
					SIM_Object &object,
					SIM_ObjectArray &feedbacktoobjects,
					const SIM_Time &timestep,
					bool newobject) = 0;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_SingleSolver, SIM_Solver);
};

#endif

