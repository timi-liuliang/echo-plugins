/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_SubStep.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_SubStep__
#define __GAS_SubStep__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GAS_API GAS_SubStep : public GAS_SubSolver
{
public:
    GETSET_DATA_FUNCS_I(SIM_NAME_PRIMARYSOLVER, PrimarySolver);
    GET_DATA_FUNC_S(GAS_NAME_VELOCITY, VelocityName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GETSET_DATA_FUNCS_I("substeps", Substeps);
    GETSET_DATA_FUNCS_I("minimumsubsteps", MinimumSubsteps);
    GETSET_DATA_FUNCS_F("cflcond", CFLCond);
    GET_DATA_FUNC_B("createsubstepdata",  CreateSubstepData);
    GET_DATA_FUNC_B("cflonce",  CFLOnce);
    GET_DATA_FUNC_B("quantize",  Quantize);

    GET_DATA_FUNC_S(GAS_NAME_TRACKERADDR, TrackerAddress);
    GET_DATA_FUNC_I(GAS_NAME_TRACKERPORT, TrackerPort);
    GET_DATA_FUNC_S(GAS_NAME_JOBNAME, JobName);
    GET_DATA_FUNC_I("slice", Slice);
    GET_DATA_FUNC_I("numslice", NumSlice);

    /// We to change the way in which our timestep is used so we have
    /// to preempt the solveGasSubclass.
    virtual SIM_Result	solveObjectsSubclass(SIM_Engine &engine,
				SIM_ObjectArray &objects,
				SIM_ObjectArray &newobjects,
				SIM_ObjectArray &feedbacktoobjects,
				const SIM_Time &timestep);
    
protected:
    explicit		 GAS_SubStep(const SIM_DataFactory *factory);
    virtual		~GAS_SubStep();

    virtual bool	solveGasSubclass(SIM_Engine &engine,
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
    static const SIM_DopDescription	*getDopDescription();

    /// Gets the solver to use for getting object properties.
    const SIM_Solver			*getCurrentSolver() const;

    /// Finds the new substep recommended by a solver that has indicated
    /// a substep needs to be repeated.
    SIM_Time				 getAdjustedSubStep(
					    SIM_ObjectArray &objects);

    SIM_Solver				*myCurrentSolver;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_SubStep,
			GAS_SubSolver,
			"Gas SubStep",
			getDopDescription());
};

#endif

