/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ParticleToField.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ParticleToField__
#define __GAS_ParticleToField__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

#include <SIM/SIM_RawField.h>

#include <GEO/GEO_PointTree.h>

#include <UT/UT_ThreadedAlgorithm.h>

#include <openvdb/tools/PointIndexGrid.h>

class SIM_RawIndexField;

class GAS_API GAS_ParticleToField : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);

    GET_DATA_FUNC_B("useattrib", UseAttribute);
    GET_DATA_FUNC_S("attrib", Attribute);
    GET_DATA_FUNC_B("useaffine", UseAffine);
    GET_DATA_FUNC_S("affine", Affine);
    GET_DATA_FUNC_F("threshold", Threshold);
    GET_DATA_FUNC_B("accumulate", Accumulate);
    GET_DATA_FUNC_B("scalebyvolume", ScaleByVoxelVolume);
    GET_DATA_FUNC_B("extrapolate", Extrapolate);
    GET_DATA_FUNC_B("usemaxextrapolate", UseMaxExtrapolate);
    GET_DATA_FUNC_F("maxextrapolate", MaxExtrapolate);
    GET_DATA_FUNC_B("usemaxextrapolatedist", UseMaxExtrapolateDist);
    GET_DATA_FUNC_F("maxextrapolatedist", MaxExtrapolateDist);
    GET_DATA_FUNC_F("bandwidth", Bandwidth);
    GET_DATA_FUNC_B("uniformradius", UniformRadius);

    GET_DATA_FUNC_I("calculationtype", CalculationType);
    GET_DATA_FUNC_F("dstpreadd", DstPreAdd);
    GET_DATA_FUNC_F("dstpremul", DstPreMul);
    GET_DATA_FUNC_F("srcpreadd", SrcPreAdd);
    GET_DATA_FUNC_F("srcpremul", SrcPreMul);
    GET_DATA_FUNC_F("postadd", PostAdd);
    GET_DATA_FUNC_F("postmul", PostMul);

    GET_DATA_FUNC_I("srctimescale", SrcTimeScale);
    GET_DATA_FUNC_I("dsttimescale", DstTimeScale);
    GET_DATA_FUNC_I("posttimescale", PostTimeScale);

    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

protected:
    explicit		 GAS_ParticleToField(const SIM_DataFactory *factory);
    virtual		~GAS_ParticleToField();

    /// Transfers the values of an attribute from the particle
    /// geoemtry onto the field.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void		 particleToField(SIM_Object *obj, 
				    SIM_RawField *destfield,
				    const SIM_RawIndexField *altclosept,
				    const GU_Detail *particles,
				    const GA_PointGroup *ptgrp,
				    GEO_PointTreeGAOffset *&pointtree,
				    openvdb::tools::PointIndexGrid::ConstPtr &ptgridvdb,
				    fpreal timestep,
				    const char *attributename,
				    int offset,
				    const UT_DMatrix4 &xform,
				    const UT_Vector3 &orig,
				    const UT_Vector3 &size,
				    int xres, int yres, int zres);

			THREADED_METHOD5(GAS_ParticleToField,
					dst->shouldMultiThread(),
					applyUniformParticles,
					const GU_Detail *, particles,
					SIM_RawField *, dst,
					const openvdb::tools::PointIndexGrid *, grid,
					SIM_RawField::sim_particleToFieldParms &, parms,
					SIM_RawIndexField *, closestpt)
			void         applyUniformParticlesPartial(
					const GU_Detail *particles,
					SIM_RawField *dst,
					const openvdb::tools::PointIndexGrid *grid,
					SIM_RawField::sim_particleToFieldParms &parms,
					SIM_RawIndexField *closestpt,
					const UT_JobInfo &info);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ParticleToField,
			GAS_SubSolver,
			"Gas Particle To Field",
			getDopDescription());
};

#endif

