/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Extrapolate.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Extrapolate__
#define __GAS_Extrapolate__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GAS_API GAS_Extrapolate : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_SURFACE, SurfaceName);
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);

    GET_DATA_FUNC_F("startiso",	StartIso);
    GET_DATA_FUNC_B("posdir", PositiveDir);

    GET_DATA_FUNC_B("usemaxdist", UseMaxDist);
    GET_DATA_FUNC_F("maxdist", MaxDist);
    GET_DATA_FUNC_B("usemaxcells", UseMaxCells);
    GET_DATA_FUNC_F("maxcells", MaxCells);
    GET_DATA_FUNC_B("usemaxspeed", UseMaxSpeed);
    GET_DATA_FUNC_F("maxspeedscale", MaxSpeedScale);
    GET_DATA_FUNC_B("useclamping", UseClamping);
    GET_DATA_FUNC_F("clampvalue", ClampValue);
    GET_DATA_FUNC_F("tol", Tolerance);

protected:
    explicit		 GAS_Extrapolate(const SIM_DataFactory *factory);
    virtual		~GAS_Extrapolate();

    /// Adds to the velocity field the effect of all forces at that point.
    /// Unit density is currently assumed.
    /// Only incorpreates the external simulation forces.  Internal
    /// gas forces are handled elsewher.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Extrapolate,
			GAS_SubSolver,
			"Gas Extrapolate",
			getDopDescription());
};

#endif

