/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SolverStatic_h__
#define __SIM_SolverStatic_h__

#include "SIM_API.h"
#include "SIM_SingleSolver.h"

/// This class is an implementation of a solver which does nothing.
/// It causes the object to remain stationary and unchanged regardless
/// of the situation.
class SIM_API SIM_SolverStatic : public SIM_SingleSolver
{
protected:
    explicit		 SIM_SolverStatic(const SIM_DataFactory *factory);
    virtual		~SIM_SolverStatic();

    /// Our solver function override that does nothing.
    virtual SIM_Result	 solveSingleObjectSubclass(SIM_Engine &engine,
					SIM_Object &object,
					SIM_ObjectArray &feedbacktoobjects,
					const SIM_Time &timestep,
					bool newobject);

    /// Hint to other solvers that we do nothing.
    virtual bool	 isStaticSubclass(const SIM_Object *) const 
    {	return true; }


private:
    static const SIM_DopDescription	*getSolverStaticDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SolverStatic,
			SIM_SingleSolver,
			"Static Solver",
			getSolverStaticDopDescription());
};

#endif

