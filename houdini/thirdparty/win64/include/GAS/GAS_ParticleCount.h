/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:    GAS_ParticleCount.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ParticleCount__
#define __GAS_ParticleCount__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"
#include <UT/UT_ThreadedAlgorithm.h>

#include <openvdb/tools/PointIndexGrid.h>

class SIM_RawField;

class GAS_API GAS_ParticleCount : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELDDEST, FieldDestName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);

protected:
    explicit		GAS_ParticleCount(const SIM_DataFactory *factory);
    virtual     	~GAS_ParticleCount();

    /// Constructs a signed distance field by skinning the set
    /// of particles.
    virtual bool	solveGasSubclass(SIM_Engine &engine,
					SIM_Object *obj,
					SIM_Time time,
					SIM_Time timestep);

    void		countParticles(const GU_Detail *gdp,
					SIM_RawField *dst);

			THREADED_METHOD2(GAS_ParticleCount,
				dst->shouldMultiThread(),
				countParticles,
				SIM_RawField *, dst,
				const openvdb::tools::PointIndexGrid *, grid)
    void         	countParticlesPartial(
				SIM_RawField *dst,
				const openvdb::tools::PointIndexGrid *ptgrid,
				const UT_JobInfo &info);

private:
    static const SIM_DopDescription *getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ParticleCount,
                        GAS_SubSolver,
                        "Gas Particle Count",
                        getDopDescription());
};


#endif

