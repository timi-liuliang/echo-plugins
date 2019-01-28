/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Diffuse.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Diffuse__
#define __GAS_Diffuse__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <GU/GU_Detail.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;
class SIM_RawField;
class GAS_SPH;

class GAS_API GAS_Diffuse : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    GETSET_DATA_FUNCS_F("diffuse", DiffuseRate);
    GET_DATA_FUNC_F("cflcond", CFLCond);
    GET_DATA_FUNC_I("maxiter", MaxIter);
    GET_DATA_FUNC_I("miniter", MinIter);
    GET_DATA_FUNC_B("relax", DoRelax);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, Geometry);
    GET_DATA_FUNC_S("attribute", Attribute);
    GET_DATA_FUNC_I("attribtype", AttribType);

    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

    bool		 shouldMultiThread() { return true; }

protected:
    // Geometry attribute types
    enum gas_AttribType {
	FLOAT1,
	FLOAT2,
	FLOAT3,
	FLOAT4,
	VECTOR
    };

    explicit		 GAS_Diffuse(const SIM_DataFactory *factory);
    virtual		~GAS_Diffuse();

    /// Diffuses scalar fields by moving along curvature or performing
    /// relaxation steps.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    /// Actual performs the diffusion on one specific field.
    void		 doDiffuse(SIM_RawField *field,
				    SIM_Time timestep);

    /// Performs point attribute diffusion on a geometry field.
    void		 doDiffuse(SIM_Object *obj, GU_Detail *gdp,
				    SIM_Time timestep,
				    const UT_String &attribname,
				    gas_AttribType type);

    /// Uses a CFL condition to calculate a diffusion timestep
    /// based on the computed delta values.
    fpreal		 calculateTimestep(const GU_Detail *gdp,
				    const gas_AttribType type,
				    const int attribSize,
				    const UT_Vector4Array &deltas,
				    fpreal cflCond,
				    fpreal mindivstep,
				    fpreal maxdivstep,
				    fpreal timeleft);

    /// A multi-threaded function used to calculate the rate of
    /// change of the given attribute value.
    THREADED_METHOD7(GAS_Diffuse, shouldMultiThread(), computeDeltas,
				    const GU_Detail *, gdp,
				    const GAS_SPH &, sph,
				    UT_Vector4Array &, deltas,
				    const fpreal, diffuseRate,
				    const gas_AttribType, type,
				    const int, attribSize,
				    const UT_String &, attribName)
    void		 computeDeltasPartial(const GU_Detail *gdp,
				    const GAS_SPH &sph,
				    UT_Vector4Array &deltas,
				    const fpreal diffuseRate,
				    const gas_AttribType type,
				    const int attribSize,
				    const UT_String &attribName,
				    const UT_JobInfo &info);

    /// A multi-threaded function used to apply diffusion changes
    /// to the given attribute value.
    THREADED_METHOD6(GAS_Diffuse, shouldMultiThread(), applyDeltas,
				    GU_Detail *, gdp,
				    const UT_Vector4Array &, deltas,
				    const gas_AttribType, type,
				    const int, attribSize,
				    const UT_String &, attribName,
				    SIM_Time, timestep)
    void		 applyDeltasPartial(GU_Detail *gdp,
				    const UT_Vector4Array &deltas,
				    const gas_AttribType type,
				    const int attribSize,
				    const UT_String &attribName,
				    SIM_Time timestep,
				    const UT_JobInfo &info);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Diffuse,
			GAS_SubSolver,
			"Gas Diffuse",
			getDopDescription());
};

#endif
