/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_SandForces.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_SandForces__
#define __GAS_SandForces__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_VectorField;
class SIM_ScalarField;

class GAS_API GAS_SandForces : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_SURFACE, SurfaceName);
    GET_DATA_FUNC_S(GAS_NAME_PRESSURE, PressureName);
    GET_DATA_FUNC_S(GAS_NAME_STRAIN, StrainName);
    GET_DATA_FUNC_S(GAS_NAME_VELOCITY, VelocityName);

    GETSET_DATA_FUNCS_F("frictionangle", FrictionAngle);
    GETSET_DATA_FUNCS_F("cohesion", Cohesion);
    
    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

protected:
    explicit		 GAS_SandForces(const SIM_DataFactory *factory);
    virtual		~GAS_SandForces();

    /// Compute the amount of stress to add to each voxel according
    /// to the given strain amounts.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    /// Using the given strain field, update the velocity
    /// field to take into account the relevant strains.
    void		 addSandForces(SIM_VectorField *velocity,
				    SIM_ScalarField *surface,
				    SIM_MatrixField *strain,
				    SIM_ScalarField *pressure,
				    fpreal timestep);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_SandForces,
			GAS_SubSolver,
			"Gas Sand Forces",
			getDopDescription());
};

#endif
