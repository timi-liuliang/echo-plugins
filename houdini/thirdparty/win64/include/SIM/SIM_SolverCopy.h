/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SolverCopy_h__
#define __SIM_SolverCopy_h__

#include "SIM_API.h"
#include "SIM_SingleSolver.h"
#include "SIM_OptionsUser.h"

/// This class is an implementation of a solver which copies SIM_Data
/// from one name to another, possibly from a different object.
class SIM_API SIM_SolverCopy : public SIM_SingleSolver,
			       public SIM_OptionsUser
{
public:
    GETSET_DATA_FUNCS_B("srcisme", SourceIsMyself);
    GETSET_DATA_FUNCS_S("srcobject", SourceObject);
    GETSET_DATA_FUNCS_S(SIM_NAME_SOURCEDATA, SourceData);
    GETSET_DATA_FUNCS_S(SIM_NAME_DESTINATIONDATA, DestinationData);

protected:
    explicit		 SIM_SolverCopy(const SIM_DataFactory *factory);
    virtual		~SIM_SolverCopy();

    /// Our solver function override that does nothing.
    virtual SIM_Result	 solveSingleObjectSubclass(SIM_Engine &engine,
					SIM_Object &object,
					SIM_ObjectArray &feedbacktoobjects,
					const SIM_Time &timestep,
					bool newobject);

private:
    static const SIM_DopDescription	*getSolverCopyDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SolverCopy,
			SIM_SingleSolver,
			"Copy Data Solver",
			getSolverCopyDopDescription());
};

#endif

