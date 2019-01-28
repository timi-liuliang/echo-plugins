/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_NetSliceBalance.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_NetSliceBalance__
#define __GAS_NetSliceBalance__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

#include <SIM/SIM_SlicePlane.h>

class SIM_GeometryCopy;
class GU_Detail;
class SIM_SlicePlane;

class GAS_API GAS_NetSliceBalance : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    
    GET_DATA_FUNC_S(GAS_NAME_TRACKERADDR, TrackerAddress);
    GET_DATA_FUNC_I(GAS_NAME_TRACKERPORT, TrackerPort);
    GET_DATA_FUNC_S(GAS_NAME_JOBNAME, JobName);
    GET_DATA_FUNC_I("slice", Slice);

    GET_DATA_FUNC_S("sliceattr", SliceAttr);
    GET_DATA_FUNC_S("sliceroot", SliceRoot);

    GET_DATA_FUNC_F("balanceweight", BalanceWeight);
    GET_DATA_FUNC_F("timeweight", TimeWeight);

protected:
    explicit		 GAS_NetSliceBalance(const SIM_DataFactory *factory);
    virtual		~GAS_NetSliceBalance();

    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

private:
    void		 doNetBalance(SIM_GeometryCopy *geo,
				    SIM_SlicePlane *sliceroot,
				    int64 delta,
				    const char *address,
				    int port,
				    const char *jobname);

    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_NetSliceBalance,
			GAS_SubSolver,
			"Gas Net Slice Balance",
			getDopDescription());
};

#endif

