/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_NetFieldBorderExchange.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_NetFieldBorderExchange__
#define __GAS_NetFieldBorderExchange__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_VectorField;
class SIM_ScalarField;

class GAS_API GAS_NetFieldBorderExchange : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    
    GET_DATA_FUNC_S(GAS_NAME_TRACKERADDR, TrackerAddress);
    GET_DATA_FUNC_I(GAS_NAME_TRACKERPORT, TrackerPort);
    GET_DATA_FUNC_S(GAS_NAME_JOBNAME, JobName);

    GET_DATA_FUNC_I("blend", Blend);

protected:
    explicit		 GAS_NetFieldBorderExchange(const SIM_DataFactory *factory);
    virtual		~GAS_NetFieldBorderExchange();

    /// Moves the fields given by FieldName by the given velocity
    /// field.  Moves the points given by GeometryName likewise.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_NetFieldBorderExchange,
			GAS_SubSolver,
			"Gas Net Field Border Exchange",
			getDopDescription());
};

#endif

