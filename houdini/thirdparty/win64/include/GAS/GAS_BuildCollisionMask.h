/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_BuildCollisionMask.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_BuildCollisionMask__
#define __GAS_BuildCollisionMask__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <GU/GU_Detail.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;
class SIM_VectorField;
class SIM_IndexField;

class GAS_API GAS_BuildCollisionMask : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    GET_DATA_FUNC_S(GAS_NAME_COLLISION, CollisionName);
    GET_DATA_FUNC_S(GAS_NAME_COLLISIONVELOCITY, CollisionVelocityName);
    GET_DATA_FUNC_S(GAS_NAME_INDEX, IndexFieldName);
    GETSET_DATA_FUNCS_B("usepointvelocity", UsePointVelocity);
    GETSET_DATA_FUNCS_B("usevolumevelocity", UseSDFVelocity);
    GETSET_DATA_FUNCS_B("allownonsdf", AllowNonSDF);

    GET_DATA_FUNC_B("usebandwidth", UseBandwidth);
    GET_DATA_FUNC_F("bandwidth", Bandwidth);
    GET_DATA_FUNC_B("clamptobandwidth", ClampToBandwidth);

    GET_DATA_FUNC_F("particlebandwidth", ParticleBandwidth);
    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);
protected:
    explicit		 GAS_BuildCollisionMask(const SIM_DataFactory *factory);
    virtual		~GAS_BuildCollisionMask();

    /// Builds the collision mask for all collider affectors.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void		 buildCollisionMask(SIM_Object *obj,
				    SIM_ScalarField *collisionmask,
				    SIM_VectorField *collisionvel,
				    SIM_IndexField *index,
				    SIM_Time time,
				    fpreal velscale);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_BuildCollisionMask,
			GAS_SubSolver,
			"Gas BuildCollisionMask",
			getDopDescription());
};

#endif
