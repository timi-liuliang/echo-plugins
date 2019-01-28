/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_VorticleForces.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_VorticleForces__
#define __GAS_VorticleForces__

#include "GAS_API.h"

#include <SIM/SIM_VectorField.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

#include <UT/UT_ThreadedAlgorithm.h>

class SIM_ScalarField;

class GAS_API GAS_VorticleForces : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_VELOCITY, VelocityName);
    GET_DATA_FUNC_S("particlefield", ParticleField);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GETSET_DATA_FUNCS_F("confinement", ConfinementStrength);
    GET_DATA_FUNC_B("computev", ComputeVelocity);

    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

    bool		 shouldMultiThread() { return true; }

protected:
    explicit		 GAS_VorticleForces(const SIM_DataFactory *factory);
    virtual		~GAS_VorticleForces();

    /// Applies a force to the velocity field proportional
    /// to the vorticle values.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    THREADED_METHOD5(GAS_VorticleForces, velocity->getField(axis)->shouldMultiThread(),
		    addVorticleForces,
		    SIM_VectorField *, velocity,
		    const GU_Detail *, vorticles,
		    const GA_PointGroup *, vptgrp,
		    fpreal, timestep,
		    int, axis)
    void		 addVorticleForcesPartial(SIM_VectorField *velocity,
				    const GU_Detail *vorticles,
				    const GA_PointGroup *vptgrp,
				    fpreal timestep,
				    int axis,
				    const UT_JobInfo &info);

    THREADED_METHOD5(GAS_VorticleForces, shouldMultiThread(),
				calculateVorticleForces,
				const GU_Detail *, vorticles,
				const GA_PointGroup *, vptgrp,
				GU_Detail *, gdp, fpreal, timestep,
				fpreal, confinement)
    void		 calculateVorticleForcesPartial(
				const GU_Detail *vorticles, 
				const GA_PointGroup *vptgrp,
				GU_Detail *gdp,
				fpreal timestep, fpreal confinement,
				const UT_JobInfo &info);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_VorticleForces,
			GAS_SubSolver,
			"Gas Vorticle Forces",
			getDopDescription());
};

#endif

