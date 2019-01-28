/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_GeometryToSDF.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_GeometryToSDF__
#define __GAS_GeometryToSDF__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_RawField;

class GAS_API GAS_GeometryToSDF : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_SURFACE, SurfaceName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);

    GETSET_DATA_FUNCS_I(SIM_NAME_MODE, Mode);
    GETSET_DATA_FUNCS_B(SIM_NAME_LASERSCAN, LaserScan);
    GETSET_DATA_FUNCS_B(SIM_NAME_FIXSIGNS, FixSigns);
    GETSET_DATA_FUNCS_B(SIM_NAME_FORCEBOUNDS, ForceBounds);
    GETSET_DATA_FUNCS_F(SIM_NAME_OFFSET, Offset);
    GETSET_DATA_FUNCS_F(SIM_NAME_TOLERANCE, Tolerance);
    GETSET_DATA_FUNCS_B(SIM_NAME_INVERT, Invert);
    GETSET_DATA_FUNCS_I("numneighbour", NumNeighbour);
    GETSET_DATA_FUNCS_I("sweepcount", SweepCount);
    GETSET_DATA_FUNCS_F("sweepalpha", SweepThreshold);

protected:
    explicit		 GAS_GeometryToSDF(const SIM_DataFactory *factory);
    virtual		~GAS_GeometryToSDF();

    /// Constructs a signed distance field by skinning the set
    /// of particles.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void		 geometryToSDF(SIM_Object *obj,
				    SIM_RawField *destfield,
				    const GU_Detail *geo,
				    const UT_DMatrix4 &xform);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_GeometryToSDF,
			GAS_SubSolver,
			"Gas Geometry To SDF",
			getDopDescription());
};

#endif

