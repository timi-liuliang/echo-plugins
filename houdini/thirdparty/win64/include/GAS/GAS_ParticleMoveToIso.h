/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ParticleMoveToIso.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ParticleMoveToIso__
#define __GAS_ParticleMoveToIso__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

#include <UT/UT_ThreadedAlgorithm.h>

class SIM_RawField;
class SIM_VectorField;

class GAS_API GAS_ParticleMoveToIso : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);

    GET_DATA_FUNC_I("rule", Rule);
    GET_DATA_FUNC_F("goaliso", GoalIso);
    GET_DATA_FUNC_F("tol", Tolerance);
    GET_DATA_FUNC_B("usepscale", UseParticleScale);
    GET_DATA_FUNC_B("deleteunmoveable", DeleteUnmoveable);
    GET_DATA_FUNC_B("reflect", Reflect);
    GET_DATA_FUNC_B("removevel", RemoveNormalVelocity);
    GETSET_DATA_FUNCS_S("velattribute", VelAttribute);

protected:
    explicit		 GAS_ParticleMoveToIso(const SIM_DataFactory *factory);
    virtual		~GAS_ParticleMoveToIso();

    /// Moves particles to the corresponding iso contour of the field.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void		 particleMoveToIso(SIM_RawField *field,
				    GU_Detail *particles,
				    const SIM_VectorField *vel);


    THREADED_METHOD7(GAS_ParticleMoveToIso, gdp->getNumPoints() > 1000,
                     moveToIsoInternal,
                     SIM_RawField *, field,
                     GU_Detail *, gdp,
		     const GA_PointGroup *, ptgrp,
                     const GA_ROHandleF &, rad_h,
                     const GA_RWHandleV3 &, vel_h,
                     const SIM_VectorField *, vel,
                     UT_ValArray<GA_OffsetArray> *, delpts);
    void         moveToIsoInternalPartial(SIM_RawField *field,
                                          GU_Detail *gdp,
					  const GA_PointGroup *ptgrp,
                                          const GA_ROHandleF &rad_h,
                                          const GA_RWHandleV3 &vel_h,
                                          const SIM_VectorField *vel,
                                          UT_ValArray<GA_OffsetArray> *delpts,
                                          const UT_JobInfo &info);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ParticleMoveToIso,
			GAS_SubSolver,
			"Gas Move To Iso",
			getDopDescription());
};

#endif

