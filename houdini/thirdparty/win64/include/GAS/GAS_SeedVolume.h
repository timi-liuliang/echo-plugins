/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_SeedVolume.h ( GAS Library, C++)
 *
 * COMMENTS: 	Used to initialize FLIP particles in an empty region
 * 		in the event that the simulation volume is moved.
 *		This can occur when tracking an object. The new particles
 *		will match the provided surface and velocity field.
 */

#ifndef __GAS_SeedVolume__
#define __GAS_SeedVolume__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

#include <GU/GU_Detail.h>
#include <GA/GA_SplittableRange.h>

#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_UniquePtr.h>

class GAS_API GAS_SeedVolume : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_SURFACE, SurfaceName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_S(GAS_NAME_VELOCITY, VelocityName);
    GET_DATA_FUNC_S("newvolume", VolumeName);

    GET_DATA_FUNC_S("velocityattribute", VelocityAttributeName);

    GETSET_DATA_FUNCS_I("particlespervoxel", ParticlesPerVoxel);
    GETSET_DATA_FUNCS_F("oversampling", Oversampling);
    GETSET_DATA_FUNCS_F("oversamplingbandwidth", OversamplingBandwidth);
    GETSET_DATA_FUNCS_B("oversamplebounds", OversampleBounds);

    GETSET_DATA_FUNCS_V3("volumesize", VolumeSize);
    GETSET_DATA_FUNCS_V3("volumecenter", VolumeCenter);

    GETSET_DATA_FUNCS_B("usewaterline", UseWaterline);
    GETSET_DATA_FUNCS_F("waterline", Waterline);
    GETSET_DATA_FUNCS_V3("waterlinedirection", WaterlineDirection);

    GET_DATA_FUNC_B("flagnewparticles", FlagNewParticles);
    GET_DATA_FUNC_S("newparticleattribute", NewParticleAttributeName);

    GETSET_DATA_FUNCS_B("pscaletoggle",PscaleToggle);
    GETSET_DATA_FUNCS_F("particleradius", ParticleRadius);
    GETSET_DATA_FUNCS_F(SIM_NAME_SEED, Seed);

    GETSET_DATA_FUNCS_I("slice", Slice);

protected:
	explicit	GAS_SeedVolume(const SIM_DataFactory *factory);
	virtual		~GAS_SeedVolume();

	virtual bool solveGasSubclass(SIM_Engine &engine,
					SIM_Object *obj,
		        		SIM_Time time,
					SIM_Time timestep);

	void seedVolume(SIM_Object *obj,
                            GU_Detail *gdp,
                            const SIM_RawField *surface,
                            const SIM_RawField *newvolume,
                            const SIM_RawIndexField *indexfield,
                            const SIM_VectorField *velocity) const;

    struct gas_SeedNewVolumeParms
    {
        fpreal              seed, particleradius;
        fpreal              oversampling, oversamplingcutoff;
        bool                oversampleatbounds;
        const SIM_RawField  *surface, *newvolume;
        const SIM_RawIndexField *indexfield;
        const UT_VoxelArrayF *volumevoxels;
        int                 particlespervoxel, slice;
        UT_Vector3I         orig, lowersim, uppersim;
        UT_Vector3          volorig, volend;

        SIM_BoundaryLine    waterline;

        UT_ValArray<UT_UniquePtr<UT_Vector3Array>> *ptslist;
    };

    THREADED_METHOD1_CONST(GAS_SeedVolume, parms.volumevoxels->numTiles() > 1, // Inequality bc there is no explicit "canMultiThread" method for UT_VoxelArray
                           seedNewVolume,
                           const gas_SeedNewVolumeParms &, parms)

    void seedNewVolumePartial(const gas_SeedNewVolumeParms &parms,
                                const UT_JobInfo &info) const;
private:

    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_SeedVolume,
			GAS_SubSolver,
			"Gas Seed Volume",
			getDopDescription());
};

#endif
