/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_SPHDensity.h ( GAS Library, C++)
 *
 * COMMENTS:
 *	Recalculates the density for each particle according
 *	to its mass & sph values.
 */

#ifndef __GAS_SPHDensity__
#define __GAS_SPHDensity__

#include "GAS_API.h"

#include <GU/GU_Detail.h>

#include <UT/UT_ThreadedAlgorithm.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GAS_SPH;

class GU_Detail;

class GAS_API GAS_SPHDensity : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY,	GeometryName);

    bool		 shouldMultiThread() { return true; }

protected:
    explicit		 GAS_SPHDensity(const SIM_DataFactory *factory);
    virtual		~GAS_SPHDensity();

    /// Moves the fields given by FieldName by the given velocity
    /// field.  Moves the points given by GeometryName likewise.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

protected:
    THREADED_METHOD2(GAS_SPHDensity, shouldMultiThread(), calculateDensity,
				GU_Detail *, gdp,
				const GAS_SPH &, sph)
    void		 calculateDensityPartial(GU_Detail *gdp,
				const GAS_SPH &sph,
				const UT_JobInfo &info);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_SPHDensity,
			GAS_SubSolver,
			"Gas SPH Density",
			getDopDescription());
};

#endif
