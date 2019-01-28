/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ParticleToSDF.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ParticleToSDF__
#define __GAS_ParticleToSDF__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

#include <SIM/SIM_RawIndexField.h>

class SIM_RawField;
class SIM_RawIndexField;

class GAS_API GAS_ParticleToSDF : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_SURFACE, SurfaceName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);

    GET_DATA_FUNC_F("bandwidth", Bandwidth);
    GET_DATA_FUNC_B("usemaxspeed", UseMaxSpeed);
    GET_DATA_FUNC_F("maxspeedscale", MaxSpeedScale);

    GET_DATA_FUNC_F("scalemultiplier", ScaleMultiplier);
    GET_DATA_FUNC_I("method", Method);
    GET_DATA_FUNC_B("rebuildsdf", RebuildSDF);

protected:
    explicit		 GAS_ParticleToSDF(const SIM_DataFactory *factory);
    virtual		~GAS_ParticleToSDF();

    /// Constructs a signed distance field by skinning the set
    /// of particles.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void         particleToSDF(SIM_Object *obj,
                               SIM_RawField *destfield,
                               SIM_RawIndexField *indexfield,
                               const GU_Detail *particles,
                               const GA_PointGroup *ptgrp,
                               const UT_DMatrix4 &xform,
			       SIM_Time timestep,
			       fpreal bandwidth
  			    );


    class applyParticleParms
    {
    public:
    SIM_RawField				*destfield;
    const GU_Detail				*gdp;
    GEO_PointTreeGAOffset			*pttree;
    const openvdb::tools::PointIndexGrid 	*ptgridvdb;
    fpreal					bandwidth;
    UT_DMatrix4					xform;
    fpreal					scalemultiplier;
    int						method;
    float					uniformradius;
    SIM_RawIndexField				*indexfield;
    };

    THREADED_METHOD1(GAS_ParticleToSDF, parms.destfield->shouldMultiThread(),
			applyParticleToSDF,
			applyParticleParms &, parms)
    void	applyParticleToSDFPartial(applyParticleParms &parms,
				    const UT_JobInfo &info);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ParticleToSDF,
			GAS_SubSolver,
			"Gas Particle To SDF",
			getDopDescription());
};

#endif

