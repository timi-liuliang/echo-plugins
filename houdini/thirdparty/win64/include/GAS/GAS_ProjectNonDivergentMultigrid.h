/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ProjectNonDivergentMultigrid.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ProjectNonDivergentMultigrid_h__
#define __GAS_ProjectNonDivergentMultigrid_h__

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GAS_API GAS_ProjectNonDivergentMultigrid : public GAS_SubSolver
{
public:
    /// These macros are used to create the accessors
    /// getFieldDstName and getFieldSrcName functions we'll use
    /// to access our data options.
    GET_DATA_FUNC_S(GAS_NAME_FIELDDEST, FieldDstName);
    GET_DATA_FUNC_S(GAS_NAME_FIELDSOURCE, FieldSrcName);
    GET_DATA_FUNC_F("reltol", RelTol);
    GET_DATA_FUNC_F("abstol", AbsTol);
    GET_DATA_FUNC_I("miniter", MinIter);
    GET_DATA_FUNC_I("maxiter", MaxIter);
    GET_DATA_FUNC_B(SIM_NAME_OPENCL, UseOpenCL);
    GET_DATA_FUNC_B("internalfacesclosed", InternalFacesClosed);

protected:
    explicit		 GAS_ProjectNonDivergentMultigrid(const SIM_DataFactory *factory);
    virtual		~GAS_ProjectNonDivergentMultigrid();

    /// The overloaded callback that GAS_SubSolver will invoke to
    /// perform our actual computation.  We are giving a single object
    /// at a time to work on.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);
    
    /// Performs the projection using OpenCL devices
    bool	 	 projectOpenCL(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);
private:
    /// We define this to be a DOP_Auto node which means we do not
    /// need to implement a DOP_Node derivative for this data.  Instead,
    /// this description is used to define the interface.
    static const SIM_DopDescription	*getDopDescription();

    /// These macros are necessary to bind our node to the factory and
    /// ensure useful constants like BaseClass are defined.
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ProjectNonDivergentMultigrid,
			GAS_SubSolver,
			"Gas ProjectNonDivergent Multigrid",
			getDopDescription());
};


#endif

