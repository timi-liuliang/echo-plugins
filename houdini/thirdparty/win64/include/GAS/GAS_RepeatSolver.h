/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_RepeatSolver.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_RepeatSolver__
#define __GAS_RepeatSolver__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GAS_API GAS_RepeatSolver : public GAS_SubSolver
{
public:
    GETSET_DATA_FUNCS_S("whilevalue", WhileDataName);
    GETSET_DATA_FUNCS_S("solvepassvalue", SolvePassDataName);
    GETSET_DATA_FUNCS_S("numsolvepassvalue", NumSolvePassDataName);
    GETSET_DATA_FUNCS_I("minpass", MinPass);
    GETSET_DATA_FUNCS_I("maxpass", MaxPass);
    GETSET_DATA_FUNCS_B("usemaxpass", UseMaxPass);
    GETSET_DATA_FUNCS_B("usenumsolvepass", UseNumSolvePass);
    GETSET_DATA_FUNCS_B("usewhilevalue", UseWhileValue);

    /// We to change the way in which our timestep is used so we have
    /// to preempt the solveGasSubclass.
    virtual SIM_Result	solveObjectsSubclass(SIM_Engine &engine,
				SIM_ObjectArray &objects,
				SIM_ObjectArray &newobjects,
				SIM_ObjectArray &feedbacktoobjects,
				const SIM_Time &timestep);
    
protected:
    explicit		 GAS_RepeatSolver(const SIM_DataFactory *factory);
    virtual		~GAS_RepeatSolver();

    virtual bool	solveGasSubclass(SIM_Engine &engine,
			    SIM_Object *obj,
			    SIM_Time time,
			    SIM_Time timestep)
			{
			    return true;
			}

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_RepeatSolver,
			GAS_SubSolver,
			"Gas Repeat Solver",
			getDopDescription());
};

#endif

