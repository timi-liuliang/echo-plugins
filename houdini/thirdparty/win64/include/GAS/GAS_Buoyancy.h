/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Buoyancy.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Buoyancy__
#define __GAS_Buoyancy__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_VectorField;
class SIM_ScalarField;

class GAS_API GAS_Buoyancy : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_TEMPERATURE, TemperatureName);
    GET_DATA_FUNC_S(GAS_NAME_VELOCITY, VelocityName);
    
    GETSET_DATA_FUNCS_V3("dir", BuoyancyDir);
    GETSET_DATA_FUNCS_F("lift", BuoyancyLift);
    GETSET_DATA_FUNCS_F("ambient", Ambient);

    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);
    GET_DATA_FUNC_B(SIM_NAME_OPENCL, UseOpenCL);

protected:
    explicit		 GAS_Buoyancy(const SIM_DataFactory *factory);
    virtual		~GAS_Buoyancy();

    /// Moves the fields given by FieldName by the given velocity
    /// field.  Moves the points given by GeometryName likewise.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void		 addBuoyancyForces(SIM_Object *obj,
				    SIM_VectorField *velocity,
				    const SIM_ScalarField *temperature,
				    fpreal timestep);

    void         addBuoyancyForcesCL(SIM_Object *obj,
                    SIM_VectorField *velocity,
                    const SIM_ScalarField *temperature,
                    const UT_Vector3 &up,
                    fpreal scale,
                    fpreal Tamb);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Buoyancy,
			GAS_SubSolver,
			"Gas Buoyancy",
			getDopDescription());
};

#endif

