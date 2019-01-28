/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_VelocityStretch.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_VelocityStretch__
#define __GAS_VelocityStretch__

#include "GAS_API.h"

#include <SIM/SIM_RawField.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GAS_SPH;
class SIM_VectorField;
class SIM_ScalarField;
class SIM_MatrixField;

class GAS_API gas_VelInfo
{
public:
    const SIM_VectorField	*velocity;
    GAS_SPH		*sph;
    const GU_Detail	*gdp;
    fpreal		 timestep;
};

class GAS_API GAS_VelocityStretch : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_VELOCITY, VelocityName);
    GET_DATA_FUNC_S("vattrib", VelAttrib);

    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_S("attribs", Attributes);

    GET_DATA_FUNC_B("scalexforms", ScaleXforms);
    GET_DATA_FUNC_B("rotatexforms", RotateXforms);
    
    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

protected:
    explicit		 GAS_VelocityStretch(const SIM_DataFactory *factory);
    virtual		~GAS_VelocityStretch();

    /// Stretches the vorticle vorticities according to the velocity
    /// field
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void		 stretchByVelocity(GU_Detail *vorticles,
				    const gas_VelInfo &info);
    void		 stretchByVelocity(SIM_VectorField *field,
				    const gas_VelInfo &info);
    void		 stretchByVelocity(SIM_MatrixField *field,
				    const gas_VelInfo &info);

    THREADED_METHOD2(GAS_VelocityStretch, dst->shouldMultiThread(),
			stretchScalarField,
			SIM_RawField *, dst,
			const gas_VelInfo &, velinfo)
    void		 stretchScalarFieldPartial(
				SIM_RawField *dst,
				const gas_VelInfo &velinfo,
				const UT_JobInfo &info);

    THREADED_METHOD4(GAS_VelocityStretch, dst->shouldMultiThread(),
			stretchVectorField,
			SIM_RawField *, dst,
			int, axis,
			SIM_VectorField *, src,
			const gas_VelInfo &, velinfo)
    void		 stretchVectorFieldPartial(
				SIM_RawField *dst,
				int axis,
				const SIM_VectorField *src,
				const gas_VelInfo &velinfo,
				const UT_JobInfo &info);

    THREADED_METHOD5(GAS_VelocityStretch, dst->shouldMultiThread(),
			stretchMatrixField,
			SIM_RawField *, dst,
			int, a1,
			int, a2,
			SIM_MatrixField *, src,
			const gas_VelInfo &, velinfo)
    void		 stretchMatrixFieldPartial(
				SIM_RawField *dst,
				int a1, int a2,
				const SIM_MatrixField *src,
				const gas_VelInfo &velinfo,
				const UT_JobInfo &info);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_VelocityStretch,
			GAS_SubSolver,
			"Gas Velocity Stretch",
			getDopDescription());
};

#endif

