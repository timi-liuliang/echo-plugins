/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ParticleSeparate.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ParticleSeparate__
#define __GAS_ParticleSeparate__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"
#include <GU/GU_Detail.h>
#include <GA/GA_SplittableRange.h>
#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_HashGrid.h>
#include <openvdb/tools/PointIndexGrid.h>

class GU_Detail;

class GAS_API GAS_ParticleSeparate : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_I("iter", Iterations);
    GET_DATA_FUNC_F("amount", Amount);
    GET_DATA_FUNC_F("searchscale", SearchScale);
    GET_DATA_FUNC_F("distscale", DistScale);
    GET_DATA_FUNC_B("uniformradius", UniformRadius);

protected:
    explicit		 GAS_ParticleSeparate(const SIM_DataFactory *factory);
    virtual		~GAS_ParticleSeparate();

    /// Moves the fields given by FieldName by the given velocity
    /// field.  Moves the points given by GeometryName likewise.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    bool		 shouldMultiThread() { return true; }

protected:
    THREADED_METHOD7(GAS_ParticleSeparate, shouldMultiThread(),
				separateParticles,
				GU_Detail *, gdp, 
				const GA_SplittableRange &, range,
				const UT_HashGrid<GA_Offset> &, grid,
				fpreal, amount,
				fpreal, searchscale,
				fpreal, distscale,
				const UT_FloatArray &, radlist)
    void		 separateParticlesPartial(GU_Detail *gdp,
				const GA_SplittableRange &range,
				const UT_HashGrid<GA_Offset> &grid,
				fpreal amount,
				fpreal searchscale,
				fpreal distscale,
				const UT_FloatArray &radlist,
				const UT_JobInfo &info);

    THREADED_METHOD8(GAS_ParticleSeparate, range.canMultiThread(),
				separateParticlesUniform,
				GU_Detail *, gdp,
				const GA_SplittableRange &, range,
				const openvdb::tools::PointIndexGrid *, ptgrid,
				fpreal, amount,
				fpreal, searchscale,
				fpreal, distscale,
				float, rad,
				const UT_Vector3Array &, curpos)
    void		 separateParticlesUniformPartial(GU_Detail *gdp,
				const GA_SplittableRange &range,
				const openvdb::tools::PointIndexGrid *ptgrid,
				fpreal amount,
				fpreal searchscale,
				fpreal distscale,
				float rad,
				const UT_Vector3Array &curpos,
				const UT_JobInfo &info);

    bool		separateUniform(SIM_Object *obj, GU_Detail *gdp,
					const GA_PointGroup *ptgrp,
					GA_ROHandleF &rad_h);
private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ParticleSeparate,
			GAS_SubSolver,
			"Gas Particle Separate",
			getDopDescription());
};

#endif
