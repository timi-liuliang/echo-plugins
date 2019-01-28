/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_EnforceBoundary.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_EnforceBoundary__
#define __GAS_EnforceBoundary__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <GU/GU_Detail.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GU_Detail;
class SIM_VectorField;
class SIM_IndexField;

class GAS_API GAS_EnforceBoundary : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    GET_DATA_FUNC_S(GAS_NAME_COLLISION, CollisionName);
    GET_DATA_FUNC_S(GAS_NAME_COLLISIONVALUE, CollisionValueName);
    GET_DATA_FUNC_S("boundaryvalue", BoundaryValueName);
    GET_DATA_FUNC_B("velocityenforce", VelocityEnforce);
    GET_DATA_FUNC_I("veltype", VelocityType);
    GET_DATA_FUNC_B("usephysparms", UsePhysParms);
    GET_DATA_FUNC_B("variational", Variational);
    GET_DATA_FUNC_F("bandwidth", Bandwidth);
    GET_DATA_FUNC_B("forbidinterference", ForbidInterference);
    GET_DATA_FUNC_B(SIM_NAME_OPENCL, UseOpenCL);

    GET_DATA_FUNC_B("usewaterline", UseWaterline);
    GET_DATA_FUNC_F("waterline",Waterline);
    GET_DATA_FUNC_V3("waterlinedirection",WaterlineDirection);


    /// Returns the friction of the system at the given world position.
    void		 calculatePhysParms(const UT_Vector3 &pos,
				    fpreal &bounce, 
				    fpreal &friction, fpreal &dynfriction);

    /// Returns the velocity of the collision object at the given
    /// world pos
    UT_Vector3		 calculateVelocity(const UT_Vector3 &pos);

protected:
    explicit		 GAS_EnforceBoundary(const SIM_DataFactory *factory);
    virtual		~GAS_EnforceBoundary();

    /// Moves the fields given by FieldName by the given velocity
    /// field.  Moves the points given by GeometryName likewise.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);
    /// Runs the enforcement on the OpenCL device
    virtual bool	 enforceBoundaryCL(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    void		 enforceVelBoundary(SIM_Engine &engine,
				    SIM_Object *obj,
				    SIM_VectorField *vectorfield,
				    const SIM_Position *pos,
				    const SIM_ScalarField *collision,
                    const SIM_VectorField *collsionval,
				    const SIM_IndexField *colindexfield,
				    const SIM_BoundaryLine &worldbline = SIM_BoundaryLine());

private:
    static const SIM_DopDescription	*getDopDescription();

    bool		 myUsePhysParms;
    SIM_Object		*myObject;
    SIM_Engine		*myEngine;
    const SIM_VectorField   *myCollisionVel;
    const SIM_IndexField	*myCollisionIndex;
    bool		 myVelUseSDF, myVelUsePoint;
    bool		 myToWorldIdentity;
    UT_DMatrix4		 myToWorld;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_EnforceBoundary,
			GAS_SubSolver,
			"Gas Enforce Boundary",
			getDopDescription());
};

#endif
