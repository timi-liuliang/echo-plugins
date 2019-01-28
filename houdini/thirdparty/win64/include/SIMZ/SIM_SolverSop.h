/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SolverSop_h__
#define __SIM_SolverSop_h__

#include "SIMZ_API.h"
#include <SIM/SIM_OptionsUser.h>
#include <SIM/SIM_SingleSolver.h>
#include "SIMZ_Utils.h"

#include <SOP/SOP_Compiled.h>

class SIM_GeometryCopy;

class SIMZ_API SIM_SolverSop : public SIM_SingleSolver,
			       public SIM_OptionsUser
{
public:
    // Access functions for our specific data.
    GETSET_DATA_FUNCS_B("usesoppath", UseSopPath);
    GETSET_DATA_FUNCS_S(SIM_NAME_SOPPATH, SopPath);
    GETSET_DATA_FUNCS_S(SIM_NAME_DATAPATH, DataPath);
    GETSET_DATA_FUNCS_S("primaryinputname", PrimaryInputName);
    GETSET_DATA_FUNCS_B("doinvoke", DoInvoke);
    GETSET_DATA_FUNCS_I("numinputs", NumInputs);

    void	getInputName(UT_String &value, int inputno) const
    { UT_WorkBuffer buf;  buf.sprintf("inputname%d", inputno); return getOptions().getOptionS(buf.buffer(), value); }
    exint	getInputSource(int inputno) const
    { UT_WorkBuffer buf;  buf.sprintf("inputsource%d", inputno); return getOptions().getOptionI(buf.buffer()); }
    void	getInputDataName(UT_String &value, int inputno) const
    { UT_WorkBuffer buf;  buf.sprintf("inputdataname%d", inputno); return getOptions().getOptionS(buf.buffer(), value); }
    void	getInputObjectName(UT_String &value, int inputno) const
    { UT_WorkBuffer buf;  buf.sprintf("inputobjectname%d", inputno); return getOptions().getOptionS(buf.buffer(), value); }
    void	getInputRelationship(UT_String &value, int inputno) const
    { UT_WorkBuffer buf;  buf.sprintf("inputrel%d", inputno); return getOptions().getOptionS(buf.buffer(), value); }
    void	getInputColliderLabel(UT_String &value, int inputno) const
    { UT_WorkBuffer buf;  buf.sprintf("inputcolliderlabel%d", inputno); return getOptions().getOptionS(buf.buffer(), value); }
    exint	getInputTimeMode(int inputno) const
    { UT_WorkBuffer buf;  buf.sprintf("inputtimemode%d", inputno); return getOptions().getOptionI(buf.buffer()); }

    GETSET_DATA_FUNCS_I("numoutputs", NumOutputs);
    void	getOutputName(UT_String &value, int inputno) const
    { UT_WorkBuffer buf;  buf.sprintf("outputname%d", inputno); return getOptions().getOptionS(buf.buffer(), value); }
    void	getOutputDataName(UT_String &value, int inputno) const
    { UT_WorkBuffer buf;  buf.sprintf("outputdataname%d", inputno); return getOptions().getOptionS(buf.buffer(), value); }
    void	getOutputSopPath(UT_String &value, int inputno) const
    { UT_WorkBuffer buf;  buf.sprintf("outputsoppath%d", inputno); return getOptions().getOptionS(buf.buffer(), value); }

    GETSET_DATA_FUNCS_B(SIM_NAME_TRANSFORMSOPOUTPUT, TransformSopOutput);
    GETSET_DATA_FUNCS_B(SIM_NAME_SOLVEINITFRAME, SolveInitFrame);
    GETSET_DATA_FUNCS_B("usetimestep", UseTimeStep);
    GETSET_DATA_FUNCS_F("timescale", TimeScale);

    static PRM_Template	 myTemplateList[];
    static PRM_Template	 myTemplateList2[];

protected:
    explicit		 SIM_SolverSop(const SIM_DataFactory *factory);
    virtual		~SIM_SolverSop();
    /// myCompiled aware copy constructor.
    virtual void	 makeEqualSubclass(const SIM_Data *source);

    virtual SIM_Result	 solveSingleObjectSubclass(SIM_Engine &engine,
					SIM_Object &object,
					SIM_ObjectArray &feedbacktoobjects,
					const SIM_Time &timestep,
					bool newobject);
    virtual SIM_Result	 solveRelationshipSubclass(SIM_Engine &engine,
					SIM_Relationship &relation,
					const SIM_Time &timestep);


private:
    SIM_Result	 solveRootObject(SIM_Engine &engine,
					SIM_RootData &rootdata,
					const SIM_Time &timestep,
					bool newobject);

    static const SIM_DopDescription	*getSolverSopDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SolverSop,
			SIM_SingleSolver,
			"SOP Solver",
			getSolverSopDopDescription());

    // These should all be bundled into a single structure to represent
    // a compiled chunk of nodes.
    SOP_Compiled	*myCompiled;
    SOP_Compiled::TaskStateArray	*myTaskStates;
};

#endif

