/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_CollisionDetect.h ( GAS Library, C++)
 *
 * COMMENTS:	Detects collisions between points and objects
 */

#ifndef __GAS_CollsionDetect__
#define __GAS_CollsionDetect__

#include "SIMZ_API.h"

#include <GAS/GAS_SubSolver.h>
#include <GAS/GAS_Utils.h>

class SIMZ_API GAS_CollisionDetect : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_S(GAS_NAME_PTGROUP, PointGroupName);

    GET_DATA_FUNC_I("collider", Collider);
    GET_DATA_FUNC_S("relationship", RelationshipName);
    GET_DATA_FUNC_S("colliderlabel", ColliderLabel);

    GET_DATA_FUNC_S("soppath", SopPath);
    GET_DATA_FUNC_S("dopobjects", DopObjects);

    GET_DATA_FUNC_B("animategeo", UseDeformingGeo);

    GET_DATA_FUNC_B("resethitnum", ResetHitNum);

    GET_DATA_FUNC_B("addhitnum", AddHitNum);
    GET_DATA_FUNC_B("addhitpos", AddHitPos);
    GET_DATA_FUNC_B("addhitnml", AddHitNml);
    GET_DATA_FUNC_B("addhitv", AddHitVel);
    GET_DATA_FUNC_B("addhittime", AddHitTime);
    GET_DATA_FUNC_B("addhitpath", AddHitPath);
    GET_DATA_FUNC_B("addhitprim", AddHitPrim);
    GET_DATA_FUNC_B("addhituv", AddHitUV);

    GET_DATA_FUNC_F("defaultpscale", DefaultPScale);
    GET_DATA_FUNC_B("usepscaleforsdf", UsePScaleForSDF);
    GET_DATA_FUNC_B("pscaleisradius", PScaleIsRadius);

    bool		 shouldMultiThread() { return true; }

protected:
    explicit		 GAS_CollisionDetect(const SIM_DataFactory *factory);
    virtual		~GAS_CollisionDetect();

    void		 computeSopAtTimeOffset(SIM_Engine &engine,
						SOP_Node *sop,
						GU_ConstDetailHandle *gdh,
						UT_DMatrix4 &xform,
						SIM_Time timeoffset,
						SIM_Time timestep);

    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_CollisionDetect,
			GAS_SubSolver,
			"GAS Collision Detect",
			getDopDescription());
};

#endif

