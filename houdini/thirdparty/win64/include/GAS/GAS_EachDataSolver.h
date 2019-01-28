/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_EachDataSolver.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_EachDataSolver__
#define __GAS_EachDataSolver__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GAS_API GAS_EachDataSolver : public GAS_SubSolver
{
public:
    GETSET_DATA_FUNCS_S("destdataname", DestDataName);
    GETSET_DATA_FUNCS_B("copydata", CopyData);
    GETSET_DATA_FUNCS_B("splitfields", SplitFields);

    /// We to change the way in which our timestep is used so we have
    /// to preempt the solveGasSubclass.
    virtual SIM_Result	solveObjectsSubclass(SIM_Engine &engine,
				SIM_ObjectArray &objects,
				SIM_ObjectArray &newobjects,
				SIM_ObjectArray &feedbacktoobjects,
				const SIM_Time &timestep);
    
protected:
    explicit		 GAS_EachDataSolver(const SIM_DataFactory *factory);
    virtual		~GAS_EachDataSolver();

    virtual bool	solveGasSubclass(SIM_Engine &engine,
			    SIM_Object *obj,
			    SIM_Time time,
			    SIM_Time timestep)
			{
			    return true;
			}

    void		doSubSolve(SIM_Engine &engine, SIM_Object *obj, const SIM_DataArray &subsolvers, SIM_Time timestep);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_EachDataSolver,
			GAS_SubSolver,
			"Gas Each Data Solver",
			getDopDescription());
};

#endif

