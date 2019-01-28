/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ExternalForces.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ExternalForces__
#define __GAS_ExternalForces__

#include "GAS_API.h"

#include <SIM/SIM_VectorField.h>
#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_ForceResolver;

class GAS_API GAS_ExternalForces : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_SURFACE, SurfaceName);
    GET_DATA_FUNC_S(GAS_NAME_DENSITY, DensityName);
    GET_DATA_FUNC_S(GAS_NAME_VELOCITY, VelocityName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_S(GAS_NAME_PTGROUP, PointGroupName);
    GET_DATA_FUNC_S("forcenames", ForceNames);
    GET_DATA_FUNC_B("scalebysurface", ScaleBySurface);
    GET_DATA_FUNC_B("scalebydensity", ScaleByDensity);
    GET_DATA_FUNC_I("scaletype", ScaleType);
    GET_DATA_FUNC_B("computev", ComputeVelocity);
    GET_DATA_FUNC_B("stabledrag", StableDrag);

    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

    // Different ways of scaling the forces.  Needed so that forces
    // can be scaled using density rather than mass in SPH simulations.
    enum ScaleMethod {
	MASS,
	DENSITY,
	NONE
    };

protected:
    explicit		 GAS_ExternalForces(const SIM_DataFactory *factory);
    virtual		~GAS_ExternalForces();

    /// Adds to the velocity field the effect of all forces at that point.
    /// Unit density is currently assumed.
    /// Only incorpreates the external simulation forces.  Internal
    /// gas forces are handled elsewher.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

protected:
    void		 applyForcesToField(SIM_Object *obj,
                                const SIM_ConstDataArray &forces,
                                SIM_VectorField *velocity,
				const SIM_ScalarField *surface,
				const SIM_ScalarField *density,
				SIM_Time timestep);

    struct applyParms
    {
	SIM_Object *obj;
	UT_DMatrix4 toworldpoint;
	UT_DMatrix3 toselfvector;
	SIM_VectorField *velocity;
	SIM_RawField	*vel[3];
	const SIM_RawField *surface;
	const SIM_RawField *density;
	SIM_Time timestep;
    };

    THREADED_METHOD2(GAS_ExternalForces, parms.velocity->getXField()->shouldMultiThread(),
		    applyForceResolversToField,
		    const applyParms &, parms,
		    const UT_ValArray<SIM_ForceResolver *> &, forceresolvers)
    void		 applyForceResolversToFieldPartial(
				const applyParms &parms,
				const UT_ValArray<SIM_ForceResolver *> &forceresolvers,
				const UT_JobInfo &info);


    void		 applyForcesToGeo(SIM_Object *obj,
                                const SIM_ConstDataArray &forces,
				GU_Detail *gdp,
				const GA_PointGroup *ptgrp,
				SIM_Time timestep);

    THREADED_METHOD5(GAS_ExternalForces, gdp->getNumPoints() > 2048,
		    applyForceResolversToGeo,
		    SIM_Object *, obj,
		    GU_Detail *, gdp,
		    const GA_PointGroup *, ptgrp,
		    SIM_Time, timestep,
		    const UT_ValArray<SIM_ForceResolver *> &, forceresolvers)
    void	applyForceResolversToGeoPartial(SIM_Object *obj,
		    GU_Detail *gdp,
		    const GA_PointGroup *ptgrp,
		    SIM_Time timestep,
		    const UT_ValArray<SIM_ForceResolver *> &forceresolvers,
		    const UT_JobInfo &info);

    THREADED_METHOD2(GAS_ExternalForces,
		    parms.velocity->getXField()->shouldMultiThread(),
		    addBackField,
		    const applyParms &, parms,
		    int, axis);
    void	 addBackFieldPartial(const applyParms &parms,
			int axis,
			const UT_JobInfo &info);
    
private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ExternalForces,
			GAS_SubSolver,
			"Gas External Forces",
			getDopDescription());
};

#endif

