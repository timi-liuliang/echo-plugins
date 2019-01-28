/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_SolverPython.h (DOP Library, C++)
 *
 * COMMENTS:	
 *
 */

#ifndef __DOP_SolverPython_h__
#define __DOP_SolverPython_h__

#include "DOP_API.h"
#include "DOP_AutoSolver.h"
#include <SIM/SIM_ObjectArray.h>
class DOP_ScriptOperator;
class OP_OTLDefinition;

class DOP_API DOP_SolverPython : public DOP_AutoSolver
{
public:
    static OP_Node *myConstructor(
	OP_Network *net, const char *name, OP_Operator *entry);

    static DOP_ScriptOperator *createScriptOperator(const OP_OTLDefinition &);

    // These methods are called from within Python solver DOPs to get access
    // to the solver data when copying from DOP parameters into solver data.
    const SIM_RootData *solverDataRootData() const
    { return mySolverDataRootData; }
    const char *solverDataPathFromRoot() const
    { return mySolverDataPathFromRoot; }

    bool isCopyingParametersToData() const
    { return mySolverDataRootData != NULL; }

protected:
    DOP_SolverPython(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~DOP_SolverPython();

    virtual void setAdditionalParametersOnDataSubclass(
	SIM_Data &data, SIM_RootData &root_data, const char *data_name,
	DOP_Engine &engine, int output_idx);

private:
    void runCodeToSetParmsOnData(
	SIM_RootData &root_data, const char *data_name, int for_output_idx,
	DOP_Engine &engine);

    SIM_RootData *mySolverDataRootData;
    const char *mySolverDataPathFromRoot;
    bool myIsRunning;
};

#endif
