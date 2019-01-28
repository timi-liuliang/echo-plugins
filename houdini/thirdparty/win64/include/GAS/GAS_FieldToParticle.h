/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_FieldToParticle.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_FieldToParticle__
#define __GAS_FieldToParticle__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_RawField;
class GA_PointGroup;

class GAS_API GAS_FieldToParticle : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_S(GAS_NAME_PTGROUP, PointGroupName);

    GET_DATA_FUNC_S("attrib", Attribute);

    GET_DATA_FUNC_B("createattrib", CreateAttrib);

    GET_DATA_FUNC_B("preserveaffine", PreserveAffine);
    GET_DATA_FUNC_S("affine", Affine);

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
    explicit		 GAS_FieldToParticle(const SIM_DataFactory *factory);
    virtual		~GAS_FieldToParticle();

    /// Transfers the values of an attribute from the particle
    /// geoemtry onto the field.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    THREADED_METHOD7(GAS_FieldToParticle, (particles->getNumPoints()>5000),
		    fieldToParticle,
		    const SIM_RawField *, srcfield,
		    GU_Detail *, particles,
		    const GA_PointGroup *, ptgrp,
		    fpreal, timestep,
		    const char *, attributename,
		    int, offset,
		    const UT_Matrix4D &, xform)
    void		 fieldToParticlePartial(const SIM_RawField *srcfield,
				    GU_Detail *particles,
				    const GA_PointGroup *ptgrp,
				    fpreal timestep,
				    const char *attributename,
				    int offset,
				    const UT_Matrix4D &xform,
				    const UT_JobInfo &info);

    THREADED_METHOD8(GAS_FieldToParticle, (particles->getNumPoints()>5000),
		    fieldToParticleAffine,
		    const SIM_RawField *, srcfield,
		    GU_Detail *, particles,
		    const GA_PointGroup *, ptgrp,
		    fpreal, timestep,
		    const char *, attributename,
		    const char *, affinename,
		    int, offset,
		    const UT_Matrix4D &, xform)
    void		 fieldToParticleAffinePartial(const SIM_RawField *srcfield,
				    GU_Detail *particles,
				    const GA_PointGroup *ptgrp,
				    fpreal timestep,
				    const char *attributename,
				    const char *affinename,
				    int offset,
				    const UT_Matrix4D &xform,
				    const UT_JobInfo &info);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_FieldToParticle,
			GAS_SubSolver,
			"Gas Field To Particle",
			getDopDescription());
};

#endif

