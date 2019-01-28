/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_SeedMarkers.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_SeedMarkers__
#define __GAS_SeedMarkers__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

#include <GU/GU_Detail.h>

#include <GA/GA_SplittableRange.h>
#include <GA/GA_PageHandle.h>
#include <GA/GA_PageIterator.h>

#include <SIM/SIM_VectorField.h>
#include <SIM/SIM_IndexField.h>

#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_ParallelUtil.h>
#include <UT/UT_UniquePtr.h>

#include <openvdb/tools/PointIndexGrid.h>

class GU_Detail;
class SIM_ScalarField;
class GU_PrimParticle;

class GAS_API GAS_SeedMarkers : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_SURFACE, SurfaceName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_S(GAS_NAME_SOURCE, SourceName);
    GET_DATA_FUNC_S("sourcevelocityfield", SourceVelocityName);
    GET_DATA_FUNC_B("applynewsourceonly", ApplyNewSourceOnly);

    GET_DATA_FUNC_B("usesourceattrib", UseSourceAttribute);
    GET_DATA_FUNC_S("sourceattrib", SourceAttribute);

    GET_DATA_FUNC_S(GAS_NAME_SINK, SinkName);
    GET_DATA_FUNC_S("velocityfield", BoundaryVelocityName);
    GET_DATA_FUNC_S("boundaryfield", BoundaryName);
    
    GETSET_DATA_FUNCS_I("surfelpervoxel", SurfelPerVoxel);
    GETSET_DATA_FUNCS_F("birththreshold", BirthThreshold);
    GETSET_DATA_FUNCS_F("deaththreshold", DeathThreshold);
    GETSET_DATA_FUNCS_F("oversampling", Oversampling);
    GETSET_DATA_FUNCS_F("oversamplingbandwidth", OversamplingBandwidth);
    GETSET_DATA_FUNCS_B("oversamplebounds", OversampleBounds);
    GETSET_DATA_FUNCS_B("movetoiso", MoveToIso);

    GETSET_DATA_FUNCS_B("outsidesurfels", OutsideSurfels);
    GETSET_DATA_FUNCS_B("insidesurfels", InsideSurfels);

    GETSET_DATA_FUNCS_B("addparticlesystem", AddParticleSystem);
    GETSET_DATA_FUNCS_B("killoutside", KillOutside);
    GETSET_DATA_FUNCS_B("copynearest", CopyNearest);

    GETSET_DATA_FUNCS_B("useboundary", UseBoundary);
    GETSET_DATA_FUNCS_V3("lowerboundarythickness", LowerBoundaryThickness);
    GETSET_DATA_FUNCS_V3("upperboundarythickness", UpperBoundaryThickness);

    GETSET_DATA_FUNCS_V3("volumesize", VolumeSize);
    GETSET_DATA_FUNCS_V3("volumecenter", VolumeCenter);

    GET_DATA_FUNC_B("useinterpvel", UseInterpVel);

    GETSET_DATA_FUNCS_B("usewaterline", UseWaterline);
    GETSET_DATA_FUNCS_F("waterline", Waterline);
    GETSET_DATA_FUNCS_V3("waterlinedirection", WaterlineDirection);

    GETSET_DATA_FUNCS_B("pscaletoggle",PscaleToggle);
    GETSET_DATA_FUNCS_F("particleradius", ParticleRadius); 

    GET_DATA_FUNC_S("interpattribs", InterpolateAttributes);

    GETSET_DATA_FUNCS_F("minrad", MinRad);
    GETSET_DATA_FUNCS_F("maxrad", MaxRad);

    GETSET_DATA_FUNCS_B("usebandwidth", UseBandwidth);
    GETSET_DATA_FUNCS_F("birthbandwidth", BirthBandwidth);
    GETSET_DATA_FUNCS_F("deathbandwidth", DeathBandwidth);
    GETSET_DATA_FUNCS_F("tol", Tolerance);
    GETSET_DATA_FUNCS_F(SIM_NAME_SEED, Seed);

    GETSET_DATA_FUNCS_I("slice", Slice);

protected:
    explicit		 GAS_SeedMarkers(const SIM_DataFactory *factory);
    virtual		~GAS_SeedMarkers();

    /// Moves the fields given by FieldName by the given velocity
    /// field.  Moves the points given by GeometryName likewise.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    /// Creates new marker particles so that there are sufficient
    /// near the iso contour.  Deletes any particles that are now
    /// too far away.
    void         seedMarkers(SIM_Object *obj,
    			    GU_Detail *gdp,
    			    const GA_PointGroup *ptgrp,
    			    const SIM_RawField *surface,
    			    const SIM_RawField *source,
    			    const SIM_RawField *sink,
    			    const SIM_RawField *collisionmask,
                            const SIM_RawField *boundarylayer,
                            const SIM_RawIndexField *indexfield,
                            const SIM_VectorField *sourcevelocity,
                            const SIM_VectorField *boundaryvelocity,
                            bool usewaterline);

    struct gas_SeedMarkersParms
    {
        fpreal			rmin, rmax, deathcutoff, birthcutoff;
        fpreal			birththreshold, deaththreshold;
        fpreal			oversampling, oversamplingcutoff, isotol;
        fpreal			seed, particleradius;
        bool			oversampleatbounds, movetoiso;
        const SIM_RawField	*surface, *source, *sink, *collision;
        const SIM_RawField  *boundarylayer;
        const SIM_RawIndexField *indexfield;
        const GU_Detail		*gdp;
        int             surfelpervoxel;
        bool			outsidesurfels, insidesurfels;
        bool			copynearest, usebandwidth;
        int             slice;
        UT_Vector3I     surfaceboundlower;
        UT_Vector3I     surfaceboundupper;

        SIM_BoundaryLine waterline;

        UT_ValArray<UT_UniquePtr<UT_Vector3Array>> *boundptslist;
        UT_ValArray<UT_UniquePtr<UT_Vector3Array>> *sourceptslist;
        UT_ValArray<UT_UniquePtr<UT_Vector3Array>> *newptslist;
        UT_ValArray<UT_UniquePtr<GA_OffsetArray>>  *closeptslist;
        UT_ValArray<UT_UniquePtr<GA_OffsetArray>>  *delptslist;
        UT_ValArray<UT_UniquePtr<UT_FprealArray>>  *radii;
        UT_ValArray<UT_UniquePtr<UT_FprealArray>>  *distances;

        const openvdb::tools::PointIndexGrid	*ptgridvdb;
    };

    THREADED_METHOD1_CONST(GAS_SeedMarkers, parms.surface->shouldMultiThread(),
                           reseedParticles,
                           const gas_SeedMarkersParms &, parms)
    void reseedParticlesPartial(const gas_SeedMarkersParms &parms,
                                const UT_JobInfo &info) const;
    THREADED_METHOD4_CONST(GAS_SeedMarkers, gdp->getNumPoints() > 2048,
                           updateRadius,
                           const SIM_RawField *, surface,
                           GU_Detail *, gdp,
                           fpreal, rmin,
                           fpreal, rmax)
    void updateRadiusPartial(const SIM_RawField *surface,
                             GU_Detail *gdp,
                             fpreal rmin, fpreal rmax,
                             const UT_JobInfo &info) const;

    THREADED_METHOD5_CONST(GAS_SeedMarkers, range.canMultiThread(),
                           interpolateAttributes,
			   const GU_Detail *, gdp,
			   const GA_SplittableRange &, range,
			   const GA_AttributeRefMap&, map,
			   const GA_RWHandleF &, rad_h,
			   const openvdb::tools::PointIndexGrid *, grid)
    void interpolateAttributesPartial(const GU_Detail *gdp,
				      const GA_SplittableRange &range,
				      const GA_AttributeRefMap& map,
				      const GA_RWHandleF &rad_h,
				      const openvdb::tools::PointIndexGrid *grid,
				      const UT_JobInfo &info) const;

private:

    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_SeedMarkers,
			GAS_SubSolver,
			"Gas Seed Markers",
			getDopDescription());
};


//
//
// Shared operations between GAS_SeedMarkers and GAS_SeedVolume
//
//

// Return whether a sample at this position should be oversampled.  Also
// returns the miniminum distance to an oversampling boundary (either the
// surface or volume boundary) for computing percentage of voxel that
// should be oversampled.
static bool
gas_needsOversampling(const UT_Vector3 &pos,
                      fpreal absdistvalue, fpreal cutoff,
                      const UT_Vector3 &surforig, const UT_Vector3 &surfend,
                      bool oversampleatbounds)
{
    bool needsoversample = false;
    if (absdistvalue < cutoff)
    {
        needsoversample = true;
    }
    if (oversampleatbounds)
    {
        UT_Vector3 offlo = pos - surforig;
        UT_Vector3 offhi = surfend - pos;
        fpreal bounddist = SYSmin(offlo.minComponent(), offhi.minComponent());
        // Reject sample if outside the boundaries.
        if (bounddist < 0)
            return false;
        if (bounddist < cutoff)
        needsoversample = true;
    }
    return needsoversample;
}


// Create a new array of type T if the local one is not empty, then
// swap in the local one's content, replacing the localarray with an emty
// array of the same capacity.
template <typename T>
static void
gas_swapLocal(T &localarray, int curtile, UT_ValArray<UT_UniquePtr<T>> &tilearrays)
{
    if (localarray.entries())
    {
        tilearrays(curtile) = UTmakeUnique<T>(localarray.capacity());
        tilearrays(curtile)->swap(localarray);
    }
}

struct gas_setVelocityHelper
{
    gas_setVelocityHelper(const GU_Detail &gdp,
                const GA_RWHandleV3 &vel_h,
                const SIM_VectorField &velocity,
                UT_Interrupt *boss)
                : gdp(gdp)
                , velhandle(vel_h)
                , surffield(NULL)
                , velfield(velocity)
                , boss(boss)
                , usebound(false)
                , lowerbound(UT_Vector3(0,0,0))
                , upperbound(UT_Vector3(0,0,0))
                {}

    gas_setVelocityHelper(const GU_Detail &gdp,
                const GA_RWHandleV3 &vel_h,
                const SIM_RawField *surface,
                const SIM_VectorField &velocity,
                UT_Interrupt *boss)
                : gdp(gdp)
                , velhandle(vel_h)
                , surffield(surface)
                , velfield(velocity)
                , boss(boss)
                , usebound(false)
                , lowerbound(UT_Vector3(0,0,0))
                , upperbound(UT_Vector3(0,0,0))
                {}

    gas_setVelocityHelper(const GU_Detail &gdp,
                const GA_RWHandleV3 &vel_h,
                const SIM_VectorField &velocity,
                UT_Interrupt *boss,
                const UT_Vector3 lowerbound,
                const UT_Vector3 upperbound)
                : gdp(gdp)
                , velhandle(vel_h)
                , surffield(NULL)
                , velfield(velocity)
                , boss(boss)
                , usebound(true)
                , lowerbound(lowerbound)
                , upperbound(upperbound)
                {}

    void operator()(const GA_SplittableRange &r) const
    {
        GA_RWPageHandleV3 vel_ph(velhandle.getAttribute());

        if(boss->opInterrupt())
            return;

        GA_Offset start, end;

        for (GA_Iterator it(r); it.blockAdvance(start, end); )
        {
            vel_ph.setPage(start);

            for (GA_Offset ptoff = start; ptoff < end; ++ptoff)
            {
                UT_Vector3 pos = gdp.getPos3(ptoff);

                // If we're enforcing the boundary velocity to points
                // in the boundary, we can early exit on particles
                // not inside the boundary layer.
                if(usebound && !(pos.x() <= lowerbound.x() ||
                                pos.y() <= lowerbound.y() ||
                                pos.z() <= lowerbound.z() ||
                                pos.x() >= upperbound.x() ||
                                pos.y() >= upperbound.y() ||
                                pos.z() >= upperbound.z()))
                    continue;

                // If we're overwriting velocity inside of a surface
                // volume, we can early exit on points not inside
                // the surface volume
                if (surffield && surffield->getValue(pos) > 0)
                    continue;

                vel_ph.set(ptoff, velfield.getValue(pos));
            }
        }
    }

    const GU_Detail &gdp;
    const GA_RWHandleV3 &velhandle;
    const SIM_RawField *surffield;
    const SIM_VectorField &velfield;
    UT_Interrupt *boss;

    const UT_Vector3 lowerbound;
    const UT_Vector3 upperbound;
    bool usebound;
};

class gas_randHelper
{
public:
    gas_randHelper(fpreal32 seed, const UT_Vector3 &pos)
    {
        myHashVec[0] = seed;
        myHashVec[1] = pos.x();
        myHashVec[2] = pos.y();
        myHashVec[3] = pos.z();
        myHashVec[4] = 0;
    }

protected:
    fpreal32 myHashVec[5];
};

class gas_randInt : public gas_randHelper
{
public:
    gas_randInt(fpreal32 seed, const UT_Vector3 &pos, fpreal32 increment = 1)
    : gas_randHelper(seed,pos), myIncrement(increment){}

    int operator()(int n)
    {
        uint seed = SYSvector_hash(myHashVec, 5);
        int r = SYSfloor(SYSrandom(seed) * n);
        myHashVec[4] += myIncrement;
        return r;
    }
private:
    fpreal32 myIncrement;
};

class gas_randVec : public gas_randHelper
{
public:
    gas_randVec(fpreal32 seed, const UT_Vector3 &pos)
    : gas_randHelper(seed,pos){}
       
    SYS_FORCE_INLINE UT_Vector3 operator()(int n)
    {
        myHashVec[4] = n;
        uint seed = SYSvector_hash(myHashVec, 5);
        UT_Vector3 randvec;
        randvec.x() = SYSrandomZero(seed);
        randvec.y() = SYSrandomZero(seed);
        randvec.z() = SYSrandomZero(seed);
        return randvec;
    }
};




#endif

