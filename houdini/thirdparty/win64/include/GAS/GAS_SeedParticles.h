/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_SeedParticles.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_SeedParticles__
#define __GAS_SeedParticles__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <GU/GU_Detail.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;
class SIM_ScalarField;

class GAS_API GAS_SeedParticles : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_SURFACE, SurfaceName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);

    GETSET_DATA_FUNCS_I("particlepervoxel", ParticlePerVoxel);
    GETSET_DATA_FUNCS_F("particlesize", ParticleSize);

protected:
    explicit		 GAS_SeedParticles(const SIM_DataFactory *factory);
    virtual		~GAS_SeedParticles();

    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    /// Creates new particles inside the surface with random positions.
    /// Ensures they are all at least the particle size away from the
    /// surface.  Note that particle size is scaled by the voxel
    /// size
    void		 seedParticles(GU_Detail *gdp, 
				    const SIM_ScalarField *surface);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_SeedParticles,
			GAS_SubSolver,
			"Gas Seed Particles",
			getDopDescription());
};

#endif

