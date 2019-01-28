/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ParticlePressure.h ( GAS Library, C++)
 *
 * COMMENTS:	This node is intended to produce a pressure-like
 *		force acting on the particles of a particle fluid
 *		simulation.  This force is fairly similar to the
 *		SPH style pressure calculation in GAS_SPHForces,
 *		but uses an idea referred to as "double density
 *		relaxation" (see paper repository).
 *
 */

#ifndef __GAS_ParticlePressure__
#define __GAS_ParticlePressure__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"
#include "GAS_SPH.h"

#include <UT/UT_ValArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_ThreadedAlgorithm.h>

class GU_Detail;
class GAS_SPH;

class GAS_API GAS_ParticlePressure : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_F("gasconstant",  GasConstant);
    GET_DATA_FUNC_F("repulsionstrength", RepulsionStrength);
    GET_DATA_FUNC_B("dopressure",   DoPressure);
    GET_DATA_FUNC_B("computev",	    ComputeVelocity);

    bool		 shouldMultiThread() { return true; }

protected:
    explicit		 GAS_ParticlePressure(const SIM_DataFactory *factory);
    virtual		~GAS_ParticlePressure();

    /// Computes a pressure force for the given particle field
    /// and either adjusts the velocity directly, or just
    /// adds to the force variable.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				    SIM_Object *obj,
				    SIM_Time time,
				    SIM_Time timestep);

protected:
    THREADED_METHOD6(GAS_ParticlePressure, shouldMultiThread(),
			calculatePressureForce,
			GU_Detail *, gdp,
			const GAS_SPH &, sph,
			fpreal, gasconstant,
			fpreal, repulsionstrength,
			SIM_Time, timestep,
			UT_Array<UT_Vector3Array> &, forcelists)
    void		 calculatePressureForcePartial(GU_Detail *gdp,
				    const GAS_SPH &sph, 
				    fpreal gasconstant,
				    fpreal repulsionstrength,
				    SIM_Time timestep,
				    UT_Array<UT_Vector3Array> &forcelists,
				    const UT_JobInfo &info);

    THREADED_METHOD2(GAS_ParticlePressure, shouldMultiThread(),
			writeBackForces,
			GU_Detail *, gdp,
			const UT_Array<UT_Vector3Array> &, forcelists)
    void		 writeBackForcesPartial(GU_Detail *gdp,
				const UT_Array<UT_Vector3Array> &forcelists,
				const UT_JobInfo &info);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ParticlePressure,
			GAS_SubSolver,
			"Gas Particle Pressure",
			getDopDescription());
};

#endif
