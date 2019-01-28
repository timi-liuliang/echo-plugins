/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ReduceLocal.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ReduceLocal__
#define __GAS_ReduceLocal__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_VoxelArray.h>
#include <GU/GU_Detail.h>

#include <SIM/SIM_RawField.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;

class GAS_API GAS_ReduceLocal : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    GETSET_DATA_FUNCS_F("radius", Radius);
    GETSET_DATA_FUNCS_V3("voxelradius", VoxelRadius);
    GET_DATA_FUNC_B("usevoxelradius", UseVoxelRadius);
    GET_DATA_FUNC_B("opencl", UseOpenCL);
    GET_DATA_FUNC_I("reduction", Reduce);
    GET_DATA_FUNC_F("percentile", Percentile);
    GET_DATA_FUNC_I("scaleby", ScaleBy);

    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

    // Reduce methods
    enum REDUCTION_NAMES
    {
	REDUCE_MAX = 0,
	REDUCE_MIN,
	REDUCE_MAXABS,
	REDUCE_MINABS,
	REDUCE_AVERAGE,
	REDUCE_MEDIAN,	// Yeah, expensive :>
	REDUCE_SUM,
	REDUCE_SUMABS,
	REDUCE_SUMSQUARE,
	REDUCE_RMS,
	NUM_REDUCE
    };
    bool		 shouldMultiThread(UT_VoxelArrayF *field) const 
			 {
			     return field->numTiles() > 1;
			 }

protected:
    explicit		 GAS_ReduceLocal(const SIM_DataFactory *factory);
    virtual		~GAS_ReduceLocal();

    /// Reduces fields by gathering local windows and applying the
    /// reduction operator.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);


    void 		 doReduction(SIM_RawField *field, SIM_Time timestep);
    void doReductionCL(SIM_RawField *field, SIM_Time timestep);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ReduceLocal,
			GAS_SubSolver,
			"Gas Reduce Local",
			getDopDescription());
};

#endif
