/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Advect.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Advect__
#define __GAS_Advect__

#include "GAS_API.h"

#include <UT/UT_ValArray.h>
#include <UT/UT_ThreadedAlgorithm.h>

#include <GU/GU_Detail.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GAS_SPH;
class GU_Detail;
class SIM_VectorField;
class UT_Lock;

class GAS_API GAS_Advect : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELD, FieldName);
    GET_DATA_FUNC_S("minfield", MinFieldName);
    GET_DATA_FUNC_S("maxfield", MaxFieldName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_S(GAS_NAME_PTGROUP, PointGroupName);
    GET_DATA_FUNC_S(GAS_NAME_COLLISION, CollisionName);
    GET_DATA_FUNC_S(GAS_NAME_VELOCITY, VelocityName);
    GET_DATA_FUNC_B("usegeometryvelocity", UseGeometryVelocity);
    GET_DATA_FUNC_B("useneighbour", UseNeighbour);
    GET_DATA_FUNC_B("updatep", UpdatePosition);
    GETSET_DATA_FUNCS_S("attribute", Attribute);
    GET_DATA_FUNC_B("updatev", UpdateVelocity);
    GETSET_DATA_FUNCS_S("velattribute", VelAttribute);
    GETSET_DATA_FUNCS_F("cflcond", CFLCond);

    GET_DATA_FUNC_I("advectmethod", AdvectMethod);

    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

    /// Public velocity evaluation function.
    static UT_Vector3	 getVelocity(const UT_Vector3 &p, int jobnum = 0);

protected:
    explicit		 GAS_Advect(const SIM_DataFactory *factory);
    virtual		~GAS_Advect();

    /// Moves the fields given by FieldName by the given velocity
    /// field.  Moves the points given by GeometryName likewise.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    /// Moves the markers according to the velocity field given.
    THREADED_METHOD8(GAS_Advect, gdp->getNumPoints() > 2048, advectPoints,
		    GU_Detail *, gdp,
		    const GA_PointGroup *, grp,
		    const UT_DMatrix4 &, xform_togeo,
		    const UT_DMatrix4 &, xform_toworld,
		    const SIM_VectorField *, velocity,
		    fpreal, timestep,
		    const GA_RWHandleV3 &, p_h,
		    const GA_RWHandleV3 &, v_h)
    void		 advectPointsPartial(GU_Detail *gdp,
				const GA_PointGroup *grp,
				const UT_DMatrix4 &xform_togeo,
				const UT_DMatrix4 &xform_toworld,
				const SIM_VectorField *velocity, 
				fpreal timestep,
				const GA_RWHandleV3 &p_h,
				const GA_RWHandleV3 &v_h,
				const UT_JobInfo &info);

    THREADED_METHOD8(GAS_Advect, gdp->getNumPoints() > 2048, advectPointsGeo,
		    GU_Detail *, gdp, 
		    const GA_PointGroup *, grp,
		    const UT_DMatrix4 &, xform_togeo,
		    const UT_DMatrix4 &, xform_toworld,
		    fpreal, timestep, 
		    fpreal, voxelsize,
		    const GA_RWHandleV3 &, p_h,
		    const GA_RWHandleV3 &, v_h)
    void		 advectPointsGeoPartial(GU_Detail *gdp, 
				const GA_PointGroup *grp,
				const UT_DMatrix4 &xform_togeo,
				const UT_DMatrix4 &xform_toworld,
				fpreal timestep,
				fpreal voxelsize, 
				const GA_RWHandleV3 &p_h,
				const GA_RWHandleV3 &v_h,
				const UT_JobInfo &info);

    // Advect the points against their own velocities.
    THREADED_METHOD6(GAS_Advect, gdp->getNumPoints() > 2048, advectPointsVel,
		    SIM_Object *, obj,
		    GU_Detail *, gdp,
		    const GA_PointGroup *, ptgrp,
		    fpreal, timestep,
		    const GA_RWHandleV3 &, p_h,
		    const GA_ROHandleV3 &, v_h)
    void		 advectPointsVelPartial(SIM_Object *obj, 
						GU_Detail *gdp, 
						const GA_PointGroup *grp,
						fpreal timestep,
						const GA_RWHandleV3 &p_h,
						const GA_ROHandleV3 &v_h,
						const UT_JobInfo &info);

private:
    static const SIM_DopDescription	*getDopDescription();

    /// Store a list of SPH data structures.  We dynamically resize
    /// this list during velocity queries to adapt to the number of
    /// threads being run.
    static UT_ValArray<GAS_SPH *>	 ourSPHlist;

    /// Also store the gdp we're currently using, so that we can
    /// initialize sph structures as needed.
    static GU_Detail			*ourVelocityGdp;

    /// Finally, keep track of whether or not to do nearest
    /// neighbour searching, since new SPH structures will need
    /// to know this.
    static bool				 ourDoNearestNeighbour;

    /// Store a lock so that we can resize our list of SPH
    /// structures in a multithreaded way.
    static UT_Lock			 ourSPHlock;

    static GAS_SPH			*ourSPH;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Advect,
			GAS_SubSolver,
			"Gas Advect",
			getDopDescription());
};

#endif
