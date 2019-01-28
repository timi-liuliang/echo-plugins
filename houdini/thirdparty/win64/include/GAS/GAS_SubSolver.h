/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_SubSolver.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_SubSolver__
#define __GAS_SubSolver__

#include "GAS_API.h"

#include <PRM/PRM_Name.h>
#include <PRM/PRM_ChoiceList.h>

#include <UT/UT_StringArray.h>
#include <SIM/SIM_Data.h>
#include <SIM/SIM_DataUtils.h>
#include <SIM/SIM_Utils.h>
#include <SIM/SIM_OptionsUser.h>
#include <SIM/SIM_Solver.h>
#include <SIM/SIM_Time.h>
#include <SIM/SIM_RawField.h>

#include "GAS_Utils.h"

class SIM_Engine;
class SIM_Object;
class SIM_Geometry;
class SIM_Position;
class SIM_GeometryCopy;
class SIM_ScalarField;
class SIM_VectorField;
class SIM_MatrixField;
class SIM_IndexField;
class SIM_RawIndexField;
class GU_SDF;
class GAS_SPH;
class GOP_Manager;

///
/// Captures all OCL errors during the life of this object.
/// When it is done, dumps any reported to the specified solver/object.
///
class GAS_SubSolver;

GAS_API void
GASfillRelationshipMenu(void *, PRM_Name *names, int size,
			const PRM_SpareData *, const PRM_Parm *);

class GAS_API GAS_ScopedOCLErrorSink
{
public:
    GAS_ScopedOCLErrorSink(SIM_Object *obj, GAS_SubSolver *solver);
    ~GAS_ScopedOCLErrorSink();

    void		addError(const char *msg,
				 UT_ErrorSeverity severity);

private:
    SIM_Object			*myObj;
    GAS_SubSolver		*mySolver;
    UT_StringArray	 	myErrors;
    UT_Array<UT_ErrorSeverity>	mySeverities;
};

class GAS_API GAS_SubSolver : public SIM_Solver,
			      public SIM_OptionsUser
{
public:
    /// Merely calls solve on each object.
    virtual SIM_Result	solveObjectsSubclass(SIM_Engine &engine,
				SIM_ObjectArray &objects,
				SIM_ObjectArray &newobjects,
				SIM_ObjectArray &feedbacktoobjects,
				const SIM_Time &timestep);

    /// Merely calls postSolve on each object.
    virtual SIM_Result	postSolveObjectsSubclass(SIM_Engine &engine,
				SIM_ObjectArray &objects,
				SIM_ObjectArray &newobjects,
				SIM_ObjectArray &feedbacktoobjects,
				const SIM_Time &timestep);

    /// Adds the flags to the description suitable for microsolvers.
    /// pure apply data, bypass data is a multisolver, and unique
    /// datanames.
    static void setGasDescription(SIM_DopDescription &descr);

    /// Applies this subsolver for a single timestep.
    /// Returns true on success
    virtual bool	 solveGas(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    /// Retrieves the SIM_DATA specified by the
    /// *option* given by the name.  Ie, look up in  our options,
    /// find the data name, look up a data matching the name,
    /// return resulting data
    /// Reports message if a field is missing unless silent is set.
    const SIM_Geometry	*getGeometry(const SIM_Object *obj, const char *name, bool silent=false);
    SIM_Geometry	*getGeometryNonConst(SIM_Object *obj, const char *name, bool silent=false);

    const GA_PointGroup *getPointGroup(const GU_Detail *gdp, GOP_Manager &mgr, const char *parmname = GAS_NAME_PTGROUP);
    SIM_GeometryCopy	*getGeometryCopy(SIM_Object *obj, const char *name, bool silent=false);
    SIM_GeometryCopy	*getGeometryCopyByDataName(SIM_Object *obj, const char *dataname, bool silent=false);
    SIM_ScalarField	*getScalarField(SIM_Object *obj, const char *name, bool silent=false);
    SIM_VectorField	*getVectorField(SIM_Object *obj, const char *name, bool silent=false);
    SIM_MatrixField	*getMatrixField(SIM_Object *obj, const char *name, bool silent=false);
    SIM_IndexField	*getIndexField(SIM_Object *obj, const char *name, bool silent=false);
    const SIM_ScalarField	*getConstScalarField(const SIM_Object *obj, const char *name, bool silent=false);
    const SIM_VectorField	*getConstVectorField(const SIM_Object *obj, const char *name, bool silent=false);
    const SIM_MatrixField	*getConstMatrixField(const SIM_Object *obj, const char *name, bool silent=false);
    const SIM_IndexField	*getConstIndexField(const SIM_Object *obj, const char *name, bool silent=false);

    const SIM_Position		*getConstPosFromField(const SIM_Object *obj, const char *name);

    /// Builds the transform mapping world -> geo
    void		 getWorldToGeometry(UT_DMatrix4 &xform, SIM_Object *obj, const char *geopath);
    /// Builds the transform mapping geo -> world
    void		 getGeometryToWorld(UT_DMatrix4 &xform, SIM_Object *obj, const char *geopath);

    /// Returns the matching SIM_Data.  If it doesn't exist,
    /// creates a new one.
    /// The resulting data is not guaranteed to exist - a bad name
    /// could end up prohibitting creation.
    SIM_GeometryCopy	*getOrCreateGeometry(SIM_Object *obj, const char *name);
    SIM_ScalarField	*getOrCreateScalarField(SIM_Object *obj, const char *name);
    SIM_VectorField	*getOrCreateVectorField(SIM_Object *obj, const char *name);
    SIM_MatrixField	*getOrCreateMatrixField(SIM_Object *obj, const char *name);
    SIM_IndexField	*getOrCreateIndexField(SIM_Object *obj, const char *name);

    /// Retrieves all of the SIM_DATAs that match the pattern stored
    /// in the option given by name.
    void		 getMatchingData(SIM_DataArray &data,
				    SIM_Object *obj, const char *name, bool silent=false);
    void		 getMatchingData(SIM_DataArray &data,
				    UT_StringArray &datanames,
				    SIM_Object *obj, const char *name, bool silent=false);
    void		 getMatchingDataByName(SIM_DataArray &data,
				    SIM_Object *obj, const char *name, bool silent=false);
    void		 getMatchingDataByName(SIM_DataArray &data,
				    UT_StringArray &datanames,
				    SIM_Object *obj, const char *name, bool silent=false);
    void		 getMatchingConstData(SIM_ConstDataArray &data,
				    UT_StringArray &datanames,
				    SIM_Object *obj, const char *name, bool silent=false);

    /// Fetches matching data, but promotes any SIM_Geometry into 
    /// SIM_GeometryCopy
    void		 getMatchingGeoCopy(SIM_DataArray &data,
				    SIM_Object *obj, const char *name, bool silent=false);
    void		 getMatchingGeoCopyByName(SIM_DataArray &data,
				    SIM_Object *obj, const char *name, bool silent=false);


    /// Makes the given field match the reference field in terms of
    /// bounding box & voxel count.
    /// A no-op if reffield or field is null.
    void		 matchField(SIM_ScalarField *field, const SIM_ScalarField *reffield);
    void		 matchField(SIM_VectorField *field, const SIM_ScalarField *reffield);
    void		 matchField(SIM_MatrixField *field, const SIM_ScalarField *reffield);
    void		 matchField(SIM_IndexField *field, const SIM_ScalarField *reffield);

    /// Builds a relationship field, storing distance to each object
    /// in the destination field, along with the OBJID and object velocity
    /// in the optional index and vector fields.
    /// The negate flag controls the sign of the generated fields,
    /// collision have "inside object" as negative so have a negate of true.
    /// Mask should already be initialized as we only write to
    /// new values that are more-in the relevant object.
    /// To trigger a bandwidth, initialize mask to the max dist you
    /// want to write.
    /// The given fields must all be aligned.
    /// Bandwidth is how many voxels to border particles by for our
    /// lookup.
    void		 buildRelationshipField(
				SIM_ScalarField *mask,
				SIM_VectorField *vel,
				SIM_IndexField *index,
				UT_DMatrix4 masktoworld,
				const SIM_Object *srcobj,
				bool usepoint, bool usesdf,
				bool allownonsdf,
				bool negate,
				fpreal particlebandwidth,
				fpreal bandwidth,
				fpreal velscale = 1);

    /// Looks up the data referred to by the parameter (parameter name,
    /// NOT data name here!) and finds its bounding box.  Handles
    /// fields and geometry.  xform is set to the toworld transform
    /// of the box, the box is in local space.
    /// Returns false if no reference found.  The size & center
    /// will be left alone in that case.
    /// The groupparmname will be referenced if it isn't null and
    /// it is a geometry - then it will be a point group to use
    /// for the limiting.
    bool		 findReferenceBBox(
				const SIM_Object *,
				const char *parmname,
				const char *groupparmname,
				UT_Vector3 &center,
				UT_Vector3 &size,
				UT_DMatrix4 &xform);

    void		 reportCLError(SIM_Object *obj, int err, const char *msg) const;
   
protected:
    explicit		 GAS_SubSolver(const SIM_DataFactory *factory);
    virtual		~GAS_SubSolver();

    /// Applies this subsolver for a single timestep.
    /// Returns true on success
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep) = 0;

    // Applies postSolve tasks if exists
    virtual bool	 postSolveGasSubclass(SIM_Engine &engine,
					      SIM_Object *obj,
					      SIM_Time time,
					      SIM_Time timestep){ return true; };

    /// Get a property value by looking at the Physical Parms of an object.
    fpreal		getPropertyFromState(const SIM_Object &object,
				const SIM_Property &property) const;

    /// Evaluates property at a given position
    virtual fpreal	 getPropertyAtPositionSubclass(const SIM_Object &object,
				const UT_Vector3 &worldspacepos,
				const SIM_Property &property) const;
    virtual fpreal	 getPropertyAtPointSubclass(const SIM_Object &object,
				int ptnum, const SIM_Property &property) const;

    virtual SIM_PropertyResolver *getPropertyResolverSubclass(const SIM_Object &object, const SIM_Property &property) const;

    /// Determines the appropriate timestep dependent on the velocity
    /// field. If provided, maxvel will contain the maximum velocity on output.
    fpreal		 calculateTimestep(SIM_Engine &engine, 
					   const SIM_VectorField *velocity, 
					   fpreal cflcond,
					   UT_Vector3 *maxvel = 0) const;
    
    /// Determines the appropriate timestep dependent on the geometry
    /// data.  The size of the particles is used as the limitting factor
    /// for CFL condition.
    /// If no particle scale or velocity exists, returns infinite.
    /// If provided, maxspeed and minradius will contain the maximum speed
    /// and minimum radius on output.
    fpreal		 calculateTimestep(GU_ConstDetailHandle gdh,
					   fpreal cflcond,
					   fpreal *maxspeed = 0,
					   fpreal *minradius = 0) const;

    struct RelationshipParms
    {
	SIM_RawField	*mask;
	SIM_VectorField	*vel;
	SIM_PropertyResolver *vel_lookup;
	SIM_RawIndexField *index;
	int64 id;

	const SIM_RawField *surface;
	bool isdensity;
	const GEO_PrimVolume *volume;
	UT_DMatrix4	tolocal;
	UT_DMatrix4	masktoworld;
	UT_DMatrix4	worldtomask;
	GEO_PointTree	*pttree;
	const GU_Detail *gdp;
	const GU_SDF	*sdf;

	bool		 restricttorange;
	UT_BoundingBox	 activerange;
	bool negate;
	fpreal maxdist;
	fpreal particlebandwidth;
	fpreal velscale;
    };
    /// Builds a relationship field after most stuff has been setup
    THREADED_METHOD1(GAS_SubSolver, parms.mask->shouldMultiThread(),
			buildRelationshipFieldInternal,
			const RelationshipParms &, parms)
    void		 buildRelationshipFieldInternalPartial(
				    const RelationshipParms &parms, 
				    const UT_JobInfo &info);

public:
    enum MIX_NAMES {
	MIX_COPY,
	MIX_ADD,
	MIX_SUB,
	MIX_MUL,
	MIX_DIV,
	MIX_MAX,
	MIX_MIN,
	MIX_AVERAGE,
	NUM_MIX
    };
    
    enum TIMESCALE_NAMES
    {
	TIMESCALE_NONE,	
	TIMESCALE_ADD,
	TIMESCALE_MULT,
	TIMESCALE_BOTH,
	TIMESCALE_DIVIDE,
	TIMESCALE_SCALEMULT,
	NUM_TIMESCALE
    };

    enum LENGTHSCALE_NAMES
    {
	LENGTHSCALE_NONE,
	LENGTHSCALE_LENGTH,
	LENGTHSCALE_AREA,
	LENGTHSCALE_VOLUME,
	NUM_LENGTHSCALE
    };

    /// Performs the requires mixing.
    SYS_STATIC_FORCE_INLINE float mixValues(MIX_NAMES mixtype,
				    float d, float s)
    {
	switch (mixtype)
	{
	    case MIX_COPY:
		d = s;
		break;
	    case MIX_ADD:
		d += s;
		break;
	    case MIX_SUB:
		d -= s;
		break;
	    case MIX_MUL:
		d *= s;
		break;
	    case MIX_DIV:
		d = SYSsafediv(d, s);
		break;
	    case MIX_MAX:
		d = SYSmax(d, s);
		break;
	    case MIX_MIN:
		d = SYSmin(d, s);
		break;
	    case MIX_AVERAGE:
		d = d + s;
		d *= 0.5f;
		break;
	    case NUM_MIX:
		UT_ASSERT(!"Invalid mix value");
		break;
	}

	return d;
    }

    /// Scales value exponentially for timestep t.
    /// v' = e ^ (ln(v) * t)
    static float 	applyTimeScalePow(float value, float t);

    /// Applies the effect of TIMESCALE_NAMES to the additive
    /// and multiplicative values.
    static void		applyTimeScale(float &add, float &mul,
			    float timestep, int timescale);

    /// Applies the effect of the given LENGTHSCALE_NAMES to
    /// the quantitity given
    static float	applyLengthScale(float val, float width,
			    int scaletype);


    /// Zeros out the force and optionaly torque attributes.
    void		 clearForces(GU_Detail *gdp, bool cleartorque = false) const;
    void		 clearForces(GU_Detail *gdp, const GA_PointGroup *grp, bool cleartorque) const;


    /// Integrates the force into the velocity/angvel.  Optionally
    /// also adjusts the position by the effect of the acceleration
    /// (*not* the effect of the velocity!)
    THREADED_METHOD6_CONST(GAS_SubSolver, gdp->getNumPoints() > 2048,
				applyForces,
				GU_Detail *, gdp,
				const GA_PointGroup *, ptgrp,
				SIM_Time, timestep,
				bool, densityscale,
				bool, doorient,
				bool, updatepos)
    void		 applyForcesPartial(GU_Detail *gdp,
				const GA_PointGroup *ptgrp,
				SIM_Time timestep,
				bool densityscale,
				bool doorient,
				bool updatepos,
				const UT_JobInfo &info) const;


    /// Builds the GAS_SPH data structure, either directly
    /// from the given SIM_Geometry or by using any SIM_PointNeighbourList
    /// attached as NeighbourList.
    /// The object version looks for Geometry/NeighbourList
    /// Returns false if failed, either due to null geometry or missing
    /// vital attributes.
    bool		 buildSPH(GAS_SPH &sph, const SIM_Geometry *geo) const;
    bool		 buildSPH(GAS_SPH &sph, const SIM_Object *obj) const;

protected:

    /// Holds the parm names for our mix methods.
    static PRM_Name	 	ourMixMethods[NUM_MIX+1];
    static PRM_ChoiceList	ourMixMethodMenu;

    static PRM_Name		ourTimeScaleMethods[NUM_TIMESCALE+1];
    static PRM_ChoiceList	ourTimeScaleMenu;

    static PRM_Name		ourLengthScaleMethods[NUM_LENGTHSCALE+1];
    static PRM_ChoiceList	ourLengthScaleMenu;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(GAS_SubSolver, SIM_Solver);
};

#endif

