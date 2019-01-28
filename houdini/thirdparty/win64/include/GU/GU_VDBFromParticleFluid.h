/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_VDBFromParticleFluid.h (GU Library, C++)
 *
 * COMMENTS:s
 */

#ifndef GU_VDBFROMPARTICLEFLUID_H_
#define GU_VDBFROMPARTICLEFLUID_H_

#include <openvdb/Types.h>
#include <openvdb/Grid.h>
#include <openvdb/tree/LeafManager.h>

#include <UT/UT_Array.h>
#include <UT/UT_ParallelUtil.h>
#include <GEO/GEO_PrimVDB.h>

#include "GU_Detail.h"
#include "GU_API.h"

#define USE_W2SUM_MASS		0
#define USE_DIST		0
#define USE_WEIGHTED_DIST	0
#define USE_VELOCITY		0


template<typename RT>
class GU_API GU_VDBFromParticleFluid
{
public:
    typedef RT RealT;
    typedef UT_Vector3T<exint> CoordT;
    typedef UT_Vector3T<RealT> Vector3R;

    enum SurfacingMethod
    {
        PHANTOM_PARTICLES = 0,
        REFINED_AVERAGE_POSITION
    };

    enum SamplingMethod
    {
        BUCKET_SAMPLE = 0,
        SMOOTH_SAMPLE,
        AP_SAMPLE
    };

    struct Parms
    {
        Parms() :
	    sampling_method(SMOOTH_SAMPLE),
	    surfacing_method(PHANTOM_PARTICLES),
	    particle_separation(0.1),
	    voxel_size(0.5),
	    influence_radius(3),
	    surface_distance(1),
	    resampling_iterations(-1),
	    resample_input(true),
	    tile_size(10),
	    additional_padding(0.5),
	    bandwidth_voxels(3),
	    memory_usage(-1),
	    time_step(0),
	    velocity_effect(0),
	    smoothing_iterations(0),
	    feature_preservation(0.0),
	    sheeting_scale(1.0),
	    feature_enhancement(0.3),
	    tile_grid_orig(Vector3R(0.0, 0.0, 0.0))
        {}

        SurfacingMethod	surfacing_method;
        SamplingMethod	sampling_method;

        // A base unit length, e.g. particle separation, based on which other
        // parameters are meseared.
        RealT		particle_separation;

        // Voxel size measured in particle_separations
        RealT		voxel_size;

        // Common influence radius of all particles measured in particle_separations
        RealT		influence_radius;

        // Default droplet size measured in particle_separations if pscale attribute
        // is not present or in (per-particle) pscales when there is a pscale point
        // attribute
        RealT   	surface_distance;

        // Maximum number of resampling iterations, -1 means unlimited
        int     	resampling_iterations;

        // Half the width of narrow band in voxels
        RealT		bandwidth_voxels;

        // Total allowed work buffer memory usage in KB.
        // Memory used to for hold the outputis excluded.
        // -1 means unlimited.
        exint		memory_usage;

        // Side length of a tile (excluding padding) measured as a multiple of
        // influence radius
        int		tile_size;

        // Extra padding (in addition of influence radius that is added by default)
        //  measured as a multiple of influence radius
        RealT		additional_padding;

        // The simulation time step (used in velocity based particle interation)
        RealT		time_step;

        // Degree of integration of velocity effect ranging between 0.0 (minimum) and 1.0 (maximum)
        RealT		velocity_effect;

        // If true, the input particle set is resamped for the given influence radius.
        // This substantially improves the performance and eliminates hideous artifacts
        // of irregularities in particle samling
        bool		resample_input;

        // experimental, currently ignored
	int 		smoothing_iterations;
	RealT 		feature_preservation;

	RealT 		sheeting_scale;
	RealT		feature_enhancement;

	Vector3R	tile_grid_orig;
    };


private:

    struct TileParms
    {
	TileParms() :
	    surfacing_method(PHANTOM_PARTICLES),
	    sampling_method(SMOOTH_SAMPLE),
	    world_orig(0.0, 0.0, 0.0),
	    world_orig_voxel(0, 0, 0),
	    voxel_size(-1.0),
	    tile_voxels(-1.0),
	    padding_voxels(-1.0),
	    influence_voxels(-1.0),
	    surface_distance(-1.0),
	    sampling_frequency(-1.0),
	    sampling_iterations(0),
	    time_step(-1.0),
	    smoothing_iterations(0),
	    feature_preservation(0.0),
	    sheeting_scale(1.0),
	    feature_enhancement(0.3)
	{}

	SurfacingMethod	surfacing_method;
	SamplingMethod	sampling_method;
	Vector3R 	world_orig;
	CoordT 		world_orig_voxel;
	RealT 		voxel_size;
	exint 		tile_voxels;
	exint 		padding_voxels;
	RealT 		influence_voxels;
	RealT 		surface_distance;
	RealT		particle_separation;
	RealT 		sampling_frequency;
	int 		sampling_iterations;
	RealT 		time_step;
	int		smoothing_iterations;
	RealT		feature_preservation;
	RealT 		sheeting_scale;
	RealT		feature_enhancement;
    };

    struct GridPoint
    {
	union {
	    RealT 	wsum;		// sum of weights
	    RealT	tmp;		// reused as tmp in smoothing
	};

	union {
	    RealT 	rad;		// weighted radus (surface distance) sum
	    RealT 	final;		// reused for final density values
	};

	Vector3R 	pos;		// wieighted position sum

	int16		iter;		// last iteration that updated the point

#if USE_VELOCITY
	RealT		vel;
#endif

#if USE_DIST
	RealT 		dist;
#endif

#if USE_WEIGHTED_DIST
	RealT		distwm;
#endif

	inline Vector3R
	center() 	{ return pos / wsum; }
	inline RealT
	density()	{ return (pos.length() - rad) / wsum; }
	inline RealT
	height()	{ return pos.length() / wsum; }
	inline RealT
	radius() 	{ return rad / wsum; }
    };


    struct SamplingGridPoint
    {
	int16		iter;		// last iteration to have updated the point
	RealT 		wsum;		// sum of weights

	RealT		mass;

#if USE_VELOCITY
	RealT		vel;
#endif
	Vector3R 	pos;		// wieighted sum of positions
	RealT 		rad;		// weighted sum of radii (surface distances)

	RealT		oldmass;
	Vector3R	oldpos;
    };

    typedef typename openvdb::math::Vec2<int> Vec2T;
    typedef typename openvdb::tree::Tree4<Vec2T, 5, 4, 3>::Type TileTreeT;
    typedef typename TileTreeT::LeafNodeType TileLeafT;
    typedef typename openvdb::Grid<TileTreeT> TileGridT;
    typedef typename TileGridT::Ptr TileGridPtr;
    typedef typename TileGridT::Accessor TileAccessorT;
    typedef typename TileGridT::ConstAccessor TileConstAccessorT;
    typedef typename openvdb::tree::LeafManager<TileTreeT> TileLeafManagerT;
    typedef typename TileLeafManagerT::LeafRange TileRangeT;
    typedef UT_Array<typename TileLeafT::ValueOnIter> TaskArrayT;

public:
    typedef typename openvdb::tree::Tree4<RealT, 5, 4, 3>::Type RealTreeT;
    typedef typename openvdb::Grid<RealTreeT> RealGridT;
    typedef typename RealGridT::Ptr RealGridPtr;
    typedef typename RealTreeT::LeafNodeType RealLeafT;
    typedef typename RealGridT::Accessor RealAccessorT;
    typedef typename RealGridT::ConstAccessor ConstRealAccessorT;
    typedef typename openvdb::tree::LeafManager<RealTreeT> RealLeafManagerT;
    typedef typename RealLeafManagerT::LeafRange RealRangeT;


    GU_VDBFromParticleFluid(const GEO_Detail *gdp, const Parms &);

    GU_VDBFromParticleFluid(const GU_VDBFromParticleFluid &other, tbb::split);

    ~GU_VDBFromParticleFluid();

    RealGridPtr getGrid() { return myOutputVDBGrid; }

    const std::string getErrorMessage()
    {
	return myErrorMessage.str();
    }

    int evaluate(RealGridPtr result_grid, bool threaded = true);
    void drawGuide(GU_Detail *gdp, RealT padding);
    void drawGuide1(GU_Detail *gdp);
    void drawGuide2(GU_Detail *gdp);



    void operator()(const UT_BlockedRange<exint>& range);
    void join(GU_VDBFromParticleFluid& other);

private:

    SYS_PACKED_STRUCT_HINT_BEGIN(GridIdxKey, 4)
    {
	exint grididx;
	int key;
    };SYS_PACKED_STRUCT_HINT_END

    class ut_isGridIdxLess
    {
    public:
	inline bool operator()(const GridIdxKey &a,
			       const GridIdxKey &b) const
	{
	    if (a.grididx == b.grididx)
		return a.key < b.key;
	    return a.grididx < b.grididx;
	}
    };


    struct SentinelVoxelsCleaner
    {
    	void run(RealGridPtr grid);
    	void operator()(const RealRangeT& r) const;
    private:
    	RealGridPtr myGrid;
    };

    RealT bytesPerVoxel(RealT sampling_frequency_in_voxels);

    // convert the coordinates of a tile into a linear index
    inline exint
    tileCoordIndex(exint i, exint j, exint k)
    {
	return i * myMaxJK + j * myMaxK + k;
    }

    // convert from linear index to a tile coordinate
    inline void indexTileCoord(exint idx, exint &i, exint &j, exint &k)
    {
	i = idx / myMaxJK;
	idx -= i * myMaxJK;
	j = idx / myMaxK;
	idx -= j * myMaxK;
	k = idx;
    }


    struct Task;
    struct ParticleInstancer;
    struct InstanceMerger;

    void instanceParticles(RealT, RealT, UT_Array<GridIdxKey> &);
    void buildTiles();

    class sop_IsTaskLess
    {
    public:
	inline bool operator()(const TileLeafT::ValueOnIter &a,
			       const TileLeafT::ValueOnIter &b) const
	{
	    Vec2T vala = a.getValue();
	    Vec2T valb = b.getValue();
	    return vala(1) - vala(0) < valb(1) - valb(0);
	}
    };


    // some utility functions
    inline Vector3R vecFloor(const Vector3R v)
    {
	return Vector3R(SYSfloor(v(0)), SYSfloor(v(1)), SYSfloor(v(2)));
    }

    inline Vector3R vecCeil(const Vector3R v)
    {
	return Vector3R(SYSceil(v(0)), SYSceil(v(1)), SYSceil(v(2)));
    }

    inline Vector3R vecRound(const Vector3R v)
    {
	return Vector3R(SYSfloor(v(0) + RealT(0.5)), SYSfloor(v(1) + RealT(0.5)), SYSfloor(v(2) + RealT(0.5)));
    }

    inline RealT maxComp(const Vector3R v)
    {
	return SYSmax(v(0), v(1), v(2));
    }

    inline RealT minComp(const Vector3R v)
    {
	return SYSmin(v(0), v(1), v(2));
    }

    class Tile
    {
    public:
	Tile(const GEO_Detail *gdp, const TileParms &parms, RealAccessorT& acc, UT_IntArray &keys);
	void position(exint start_idx, exint stop_idx, Vector3R tile_orig, CoordT tile_orig_voxel);
	void run(bool resample_input, int band_voxels);

    private:
	void reset();

	inline exint
	coordToIndex(CoordT v, exint grid_side_points, exint grid_side_points_sqr)
	{
	    return grid_side_points_sqr * v(0) +
		   grid_side_points * v(1) + v(2);
	}

	inline exint
	coordToIndex(exint i, exint j, exint k, exint grid_side_points,
	             exint grid_side_points_sqr)
	{
	    return grid_side_points_sqr * i + grid_side_points * j + k;
	}

	inline CoordT
	indexToCoord(exint index, exint grid_side_points, exint grid_side_points_sqr)
	{
	    exint i = index / grid_side_points_sqr;
	    index -= i * grid_side_points_sqr;
	    exint j = index / grid_side_points;
	    index -= j * grid_side_points;
	    return CoordT(i, j, index);
	}

	// p is in buffer coordinates (voxel size is unit)
	// r is the particle surface distance value in voxels
	inline void
	pasteParticle(const Vector3R &pos, RealT rad,
	              RealT mass, RealT squeeze = RealT(1.0));

	inline void
	pasteParticleToSamplingGrid(const Vector3R &pos, RealT rad,
		      RealT mass, RealT squeeze = RealT(1.0));


	// Trilinear estimation of density at an arbitrary location
	inline RealT evalDensity(Vector3R xyz);
	inline RealT evalIsoDensity(Vector3R xyz, RealT *dist = NULL);

	inline RealT adjustedHeight(exint voxel_index, RealT srad);

	void expandIndexList(UT_IntArray &idx_list, int num_voxels, exint grid_side_points);

	void markBandVoxels(int band_radius, bool mark_all = false);

#if CLIP_DEEP_PARTICLES
	void separateDeepAndShallowSamples();
	void floodIndexList(UT_IntArray &idx_list, bool ascending);
	void floodDeepVoxels();
#endif

	void finalizeDensities();
	void writeToVDB(int band_voxels);

	void collectNewSamples();


	inline void recordSample(const Vector3R &pos, RealT rad, RealT mass = RealT(1.0),
				 RealT vel = RealT(0.0), bool clean_voxel_only = true);

	inline int findSamplingVoxelCorners(const Vector3R &pos, exint *offsets, RealT *weights);

	void resampleInputParticles();
	exint addAllParticles();
	exint pasteNewSamples();


#if EXACT_NN
	void buildPointTree();
	int nearestParticle(Vector3R &xyz);
#endif


	inline RealT softClipAbove(RealT min, RealT max, RealT clip, RealT x)
	{
	    if (x < min)
		return x;
	    if (x > max)
		return clip;
	    return ((max - x) * x + (x - min) * clip)/(max - min);
	}

	inline RealT softClipBelow(RealT min, RealT max, RealT clip, RealT x)
	{
	    if (x < min)
		return clip;
	    if (x > max)
		return x;
	    return ((max - x) * clip + (x - min) * x) / (max - min);
	}

	void boxFilter(RealT rwidth, int iterations, RealT bias);
	void boxFilterV2(int width, int iterations, RealT bias);

    private:

	const GEO_Detail 	*myGdp;
	UT_IntArray 		&myKeys;
	RealAccessorT 		myAcc;

	exint 			myStartIdx, myStopIdx;

	RealT			myTimeStep;

	// Buffer Grid
	RealT 			myBufferVoxelSize;
	exint 			myBufferGridSidePoints;
	exint			myBufferGridSidePointsSqr;
	exint 			myBufferGridSize;

	UT_Array<GridPoint>	myBufferGrid;

	// Sampling Grid
	RealT			mySamplingVoxelSize;
	exint			mySamplingGridSidePoints;
	exint			mySamplingGridSidePointsSqr;
	exint 			mySamplingGridSize;
	UT_Array<SamplingGridPoint>	mySamplingGrid;

	UT_IntArray		myDirtySamplingIndices;
#if CLIP_DEEP_PARTICLES
	UT_IntArray		myDeepSamplingIndices;
#endif
	UT_IntArray		myLoadedBufferIndices;
	UT_IntArray		myDirtyBufferIndices;

	// Sampling buffer origin relative to the buffer (local) origin
	Vector3R		mySamplingOrig;

	// Origin of the tile in global position
	Vector3R 		myTileOrig;

	CoordT 			myTileOrigVoxel;
	Vector3R 		myWorldOrig;
	CoordT 			myWorldOrigVoxel;
	Vector3R 		myBufferOrig;
	CoordT 			myBufferOrigVoxel;

	exint 			myPaddingVoxels;
	RealT			mySamplingFrequency;
	int			mySamplingIterations;
	RealT 			mySurfaceDistance;
	RealT			myParticleSeparation;
	RealT			myInfluenceVoxels;
	RealT			myInfluenceVoxelsSqr;

	int 			mySmoothingIterations;
	RealT			myFeaturePreservation;

#if EXACT_NN
	GEO_PointTreeT<exint>	myPointTree;
	UT_Vector3Array		myPoints;
#endif

	int16			myCurrentIteration;

	RealT			mySheetingScale;
	RealT			myFeatureEnhancement;

	SamplingMethod		mySamplingMethod;
	SurfacingMethod		mySurfacingMethod;

	UT_Interrupt 		*myBoss;
    };

private:

    const GEO_Detail	*myGdp;

    RealT		myTimeStep;
    RealT 		myInfluenceRadius;
    RealT 		myInfluenceVoxels;
    RealT		mySamplingFrequency;
    int			mySamplingIterations;

    UT_IntArray		myHardKeys;
    UT_IntArray 	&myKeys;

    exint 		myMaxJK, myMaxI, myMaxJ, myMaxK;
    Vector3R 		myBoundingBoxOrig;
    CoordT		myBoundingBoxOrigVoxel;

    RealT 		myTilePadding;
    exint 		myTilePaddingVoxels;
    exint 		myTileSideVoxels;

    RealT 		myTileSideLength;
    Vector3R		myTileGridOrig;

    // VDB Grid storing for each tile the pair of its start and end indices
    TileGridPtr 	myTileIdxGrid;

    RealT 		myVoxelSize;
    RealGridPtr 	myOutputVDBGrid;

    bool		myResampleInput;
    int			myBandVoxels;
    RealT		mySurfaceDistance;
    RealT		myParticleSeparation;

    TaskArrayT		myHardTasks;
    TaskArrayT		&myTasks;
    int 		myNumTiles;
    int 		myMaxThreads;

    std::stringstream 	myErrorMessage;
    int			myError;

    int 		mySmoothingIterations;
    RealT		myFeaturePreservation;

    SamplingMethod	mySamplingMethod;
    SurfacingMethod	mySurfacingMethod;

    bool		myIsSlave;
    SYS_AtomicCounter 	*myNextTileToDo;
    SYS_AtomicCounter	*myCompletedParticles;

    RealT		mySheetingScale;
    RealT		myFeatureEnhancement;
};

#endif /* GU_VDBFROMPARTICLEFLUID_H_ */
