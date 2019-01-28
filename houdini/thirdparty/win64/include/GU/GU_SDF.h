/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry library (C++)
 *
 * COMMENTS:	Builds a Signed Distance Function from a GU_Detail.
 *		This also supports implicit SDFs.  These lack any
 *		voxel representation.
 */

#ifndef __GU_SDF_H__
#define __GU_SDF_H__

#include "GU_API.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_Plane.h>
#include <UT/UT_VoxelArray.h>
#include <UT/UT_PriorityQueue.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Array.h>
#include <GA/GA_Handle.h>
#include <GA/GA_Types.h>
#include <GEO/GEO_PrimVolume.h>
#include "GU_DetailHandle.h"

class GU_Detail;
class GA_ROAttributeRef;
class GU_RayIntersect;
class GEO_PrimPoly;
class GEO_Primitive;
class GU_SDF;

///
/// gu_sdf_qelem
/// This is used as an element in our queue.
///
class GU_API gu_sdf_qelem
{
public:
    // Where we are in the queue.
    int		qpos;
    // Where we are in the ref array.
    int		refpos;
    // What our x/y/z cell position is.
    int		x, y, z;
    // What our value is.
    fpreal	dist;
};

///
/// gu_sdf_voxpos
/// This is used to store a specific element of our voxel.
///
class GU_API gu_sdf_voxpos
{
public:
    gu_sdf_voxpos() {}
    gu_sdf_voxpos(int px, int py, int pz)
    {
	x = px; y = py; z = pz;
    }
    int		x, y, z;
};

///
/// gu_sdf_comp
/// Comparator used by our queue.
///
class GU_API gu_sdf_comp
{
public:
    bool operator()(const gu_sdf_qelem &e1, const gu_sdf_qelem &e2) const
    {
	return SYSabs(e1.dist) > SYSabs(e2.dist);
    }
    bool operator()(const gu_sdf_qelem *e1, const gu_sdf_qelem *e2) const
    {
	return SYSabs(e1->dist) > SYSabs(e2->dist);
    }
};

///
/// gu_sdf_queue
/// This is a queue of voxels sorted by minimum distance.
/// We want to subclass to be able to track where we are in the
/// queue.  This lets us update the queue when we change our elements 
/// distance.
///
class GU_API gu_sdf_queue :
      public UT_PriorityQueue<gu_sdf_qelem *, gu_sdf_comp, true>
{
public:
    virtual void	changedPosition(gu_sdf_qelem *e, unsigned int idx) const
    {
	e->qpos = idx;
    }
};

///
/// GU_SDFParms
/// All the parameters to build an SDF can be set using this class
///
class GU_API GU_SDFParms
{
public:
	     GU_SDFParms();
    virtual ~GU_SDFParms();

    enum sdfMode
    {
	RAY_INTERSECT,
	METAFIELD,
	MINIMUM,
	POINT_CLOUD,
	IMPLICIT_BOX,
	IMPLICIT_SPHERE,
	IMPLICIT_PLANE,
	VOLUME,
	VOLUMESAMPLE,
	VOLUMESAMPLEANDREBUILD,
	HEIGHTFIELD
    };

    /// These set the general method by which the SDF is computed.
    void		setMode(sdfMode mode);
    sdfMode		getMode() const;

    /// This sets the axis used for planar SDFs.
    void		 setPlaneAxis(const UT_Vector3 &axis);
    const UT_Vector3	&getPlaneAxis() const;

    /// Sets the divisions used by the SDF.  The given values represent
    /// the number of cells that will be present in each direction.
    /// The SDF samples the center of the cells to determine their
    /// values.
    void		setDivisions(int divx, int divy, int divz);
    void		getDivisions(int &divx, int &divy, int &divz) const;

    /// Determines if we should invert the sense of the SDFs field.
    void		setInvert(bool invert);
    bool		getInvert() const;

    /// Sets the offset applied to the SDF's field.
    void		setOffset(fpreal offset);
    fpreal		getOffset() const;

    /// Sets the tolerance to be used for ray intersection
    /// This is multiplied by the bounding box size of the sdf to
    /// determine the final tolerance.
    void		setTol(fpreal tol);
    fpreal		getTol() const;

    /// Sets the volume used for building the sdf.
    /// This must match the resolution of the sdf's parameters.
    void		setVolume(UT_VoxelArrayF *vol);
    UT_VoxelArrayF 	*getVolume() const;

    /// Sets the bounding box which the SDF will be sampled in.
    /// Note that the bounding box should fully contain the geometry
    /// or settings like ForceBounds become meaningless.
    /// If this method is not called, the bounding box will be set
    /// to the bounding box of the GDP increased by the expandBounds
    /// function.
    void		setBBox(const UT_BoundingBox &bbox);
    bool		hasBBox() const;
    void		getBBox(UT_BoundingBox &bbox) const;

    /// LaserScan will consider only the most extreme intersections along
    /// each axis, allowing geometry with bad normals or holes to process.
    /// This is only used with ray-based methods.
    void		setLaserScan(bool laserscan);
    bool		getLaserScan() const;

    /// FixSigns uses consensus methods to recover from holes.
    void		setFixSigns(int fixsigns);
    int			getFixSigns() const;

    /// ForceBounds forces all of the outermost voxels to be
    /// marked as outside.  This only applies when FixSigns is set.
    void		setForceBounds(bool forcebounds);
    bool		getForceBounds() const;

    /// Sets the maximum distance to which the SDF will be evaluated.
    /// Theoritically, things outside this distance will be left
    /// at +/- infinitity.  In practice, this is still not used.
    void		setMaxDistance(fpreal dist);
    fpreal		getMaxDistance() const;

    /// Sets the number of neighbours inspected for doing MLS computation
    /// when constructing from a point cloud.
    void		setNumNeighbour(int numneighbour);
    int			getNumNeighbour() const;

    void		setSweepCount(int sweepcount) { mySweepCount = sweepcount;};
    int			getSweepCount() const { return mySweepCount; }

    void		setSweepThreshold(fpreal sweepthreshold) { mySweepThreshold = sweepthreshold; };
    fpreal		getSweepThreshold() const { return mySweepThreshold; }

    void		setIsoContour(fpreal iso) { myIsoContour = iso; }
    fpreal		getIsoContour() const { return myIsoContour; }

    void		setAlignedVolume(bool aligned) { myAlignedVolume = aligned; }
    bool		getAlignedVolume() const { return myAlignedVolume; }

    // If the SDF is a VOLUME, has correct signs, and max distance is set, we will want
    // to make sure the background voxels outside of the narrow band match the sign
    // of the source volume's voxels.
    void        setMatchSourceSign();
    bool        getMatchSourceSign() const;

    void        setVolumeBuildFIM(bool activator) { myVolumeBuildFIM = activator; }
    bool        getVolumeBuildFIM() const { return myVolumeBuildFIM; }

    void        setToleranceFIM(fpreal tol) { myToleranceFIM = tol; }
    fpreal      getToleranceFIM() const { return myToleranceFIM; }

    void        setIterationsFIM(int iters) { myIterationsFIM = iters; }
    int         getIterationsFIM() const { return myIterationsFIM; }

private:

    sdfMode		myMode;
    int			myDivX, myDivY, myDivZ;

    bool		myInvert;

    fpreal		myTol;

    UT_VoxelArrayF	*myVolume;

    bool		myHasBBox;
    UT_BoundingBox	myBBox;

    UT_Vector3		myPlaneAxis;

    bool		myLaserScan;
    int			myFixSigns;
    bool		myForceBounds;
    
    fpreal		myOffset;
    fpreal		myMaxDist;

    int			myNumNeighbour;

    int			mySweepCount;
    fpreal		mySweepThreshold;

    fpreal		myIsoContour;
    
    bool		myAlignedVolume;

    bool                myMatchSourceSign;

    bool        myVolumeBuildFIM;
    fpreal      myToleranceFIM;
    int         myIterationsFIM;
};

///
/// GU_SDFDelayedBuilder
/// This class marshalls the data to build a GU_SDF so it can be
/// done multithreaded.
///
class GU_API GU_SDFDelayedBuilder
{
public:
	     GU_SDFDelayedBuilder();
    virtual ~GU_SDFDelayedBuilder();

    void			 build();	

    // multithread build of sdfs.
    static void		buildList(UT_Array<GU_SDFDelayedBuilder> &buildlist);

    GU_SDFParms			 myParms;
    GU_SDF			*mySDF;
    GU_ConstDetailHandle	 myGDH;

private:
    THREADED_METHOD1(GU_SDFDelayedBuilder,
		     buildlist.entries() > 1,
		     doBuildList,
		     UT_Array<GU_SDFDelayedBuilder> &, buildlist)
    void doBuildListPartial(UT_Array<GU_SDFDelayedBuilder> &buildlist,
			    const UT_JobInfo &info);
};


///
/// GU_SDF
/// This class builds a signed distance function from a given GU_Detail.
/// Signed distance functions contain an approximate distance to the original
/// surface in each voxel cell.  If cell is inside the the geometry, it
/// has a negative distance.  Sidedness of geometry is determined by
/// surface normals.  The geometry should be relatively watertight.
///
class GU_API GU_SDF
{
public:
	     GU_SDF();
    virtual ~GU_SDF();

    enum sdfImplicitType
    {
	SDF_EXPLICIT,		// Not implicit.
	SDF_SPHERE,
	SDF_PLANE,
	SDF_BOX,
	SDF_HEIGHTFIELD	
    };

    /// Expands the given bounding box so it contains a two cell border
    /// on all sides.  This helps condition the SDF stuff by ensuring
    /// the outer cells are all outside.
    static void			 expandBounds(UT_BoundingBox &bbox,
					int xres, int yres, int zres);

    /// Build from the given gdp.
    void			 build(const GU_Detail *gdp, 
				       const GU_SDFParms &parms);

    /// Initializes an empty sdf of a given resolution and from a given
    /// bounding box.
    void			 initEmpty(const UT_BoundingBox &bbox,
					   int xres, int yres, int zres);


    UT_VoxelArrayF		*getFunction() const { return myVoxels; }

    /// Returns the transform of a heightfield SDF.
    const GEO_PrimVolumeXform   &getHeightXform() const
                                 { return myHeightXform; }

    /// Returns the vertical axis of a heightfield SDF.
    int                          getHeightAxis() const { return myHeightAxis; }

    /// Implicit SDFs have no array of voxels, instead defining themselves
    /// as a simple atomic shape.
    bool			 isImplicit() const 
				 { return myImplicitType != SDF_EXPLICIT; }

    bool			 isValid() const
    {
	return (isImplicit() || getFunction() != 0);
    }

    /// Returns whether the specified build mode is multithreaded.
    static bool			isMultithreaded(const GU_SDFParms &parms)
    {
	return (parms.getMode() == GU_SDFParms::VOLUMESAMPLE);
    }

    /// Returns the type of implicit surface, if any.
    sdfImplicitType		 getImplicitType() const
				 { return myImplicitType; }

    /// The normal definining the implicit function.  This is only
    /// meaninful for half-plane implicit functions.
    const UT_Vector3		&getImplicitNormal() const
				 { return myImplicitNormal; }

    /// Returns the divisions that the spaces is voxelized into.
    /// This is meaningful even for implicit surfaces, as they have
    /// a prefered division set for purposes like advection.
    void			 getDivisions(int &xdiv, int &ydiv, int &zdiv) const;

    /// Calculates the SDF field at the point.
    fpreal			 getDistance(const UT_Vector3 &pos) const;

    // This calculates the distance to the SDF using a faster, but
    // less accurate method.  The tol parameter is set to the
    // tolerance to which the distance was computed.
    // Basically, this will point sample the SDF, put the distance to
    // the point sample in the tol parameter, and return the point sample.
    // This involves 1/8th of the voxel lookup as a getDistance.
    fpreal			 getFastDistance(const UT_Vector3 &pos, fpreal &tol) const;

    /// Returns the gradient of the SDF at the point.
    UT_Vector3			 getGradient(const UT_Vector3 &pos) const;

    /// Returns the closest point on the iso surface to the given
    /// point.
    /// The Courant-Friedreichs-Lewy condition governs how carefully
    /// we step to find the solution.  0.9 is near optimal, 0.5 is
    /// conservative.
    UT_Vector3			 findClosest(const UT_Vector3 &pos, 
					     fpreal iso = 0.0,
					     fpreal cfl_cond = 0.9,
					     fpreal tol = 1e-3) const;

    /// Advects a point according to the SDF's gradient field.
    /// Advection continues until the point has moved the given distance.
    /// If normalize_gradient is false, it is moved until the given
    /// amount of *time* has passed.
    UT_Vector3			 advect(const UT_Vector3 &pos,
					fpreal dist,
					fpreal cfl_cond = 0.9f,
					bool normalize_gradient = true) const;

    /// Finds the smallest value in the SDF along the given line
    /// segment.  The value must be smaller than the cutoff value.
    /// Returns false if no close value found.
    bool			 findSmallestOnEdge(
					fpreal &minvalue,
					UT_Vector3 &result,
					const UT_Vector3 &a,
					const UT_Vector3 &b,
					fpreal cutoff = SYS_FPREAL_MAX) const;

    /// Finds the smallest value in the SDF inside the given triangle.
    /// The value must be smaller than the cutoff value.
    /// Returns false if no close value found.
    ///
    /// Note: this algorithm is far from perfect. It does only a very
    /// partial scan over the interior of the triangle, and it doesn't work
    /// well with implicit box SDFs.
    bool			 findSmallestOnTri(
					fpreal &minValue,
					UT_Vector2 &resultBary,
					const UT_Vector3 &p0,
					const UT_Vector3 &p1,
					const UT_Vector3 &p2,
					fpreal cutoff = SYS_FPREAL_MAX) const;
					

    /// Returns the point along a path where it first intersects the SDF.
    /// If the path doesn't intersect the SDF, this function returns false.
    /// If the start of the path is inside the SDF, the function returns
    /// true, and returns the starting point of the path as the intersection.
    bool			 findRayIntersection(
					UT_Vector3 &result,
					const UT_Vector3 &a,
					const UT_Vector3 &b,
					fpreal boundaryvalue = 0.0) const;
					

    const UT_Vector3		&getVoxelSize() const { return myVoxelSize; }
    fpreal			 getVoxelDiameter() const { return myVoxelDiameter; }
    const UT_Vector3		&getSize() const { return mySize; }
    const UT_Vector3		&getOrig() const { return myOrig; }

    void			 setOrig(const UT_Vector3 &o);
    void			 setSize(const UT_Vector3 &s);

    /// Computes the center of mass of the SDF.
    void		 	 computeCenterOfMass(
				    UT_Vector3 &centerofmass) const;

    /// Computes the volume of the SDF.
    fpreal			 computeVolume() const;
    
    /// Computes the inertial tensor.
    /// You provide the center of mass for it to use.
    /// Each sample point of the SDF is treated as a point mass.  The
    /// mass is 0 for cell-points outside of the object, 1 for those
    /// fully inside, and suitably scaled for others.
    void			 computeInertialTensor(UT_DMatrix3 &tensor, 
				    const UT_Vector3 &centerofmass) const;

    /// Returns the amount of memory used by this object.
    int64			 getMemoryUsage() const;

    /// These invert the sense of the SDF.  This means the getDistance()
    /// function will always return -getDistance(), effectively turning
    /// the object inside out.
    /// This does not affect the calculation of volumes, etc!
    /// (Technically, an inverted SDF has infinite volume, which isn't
    /// all that useful anyways)
    bool			 isInverted() const { return myInvert; }
    void			 setInverted(bool invert) { myInvert = invert; }

    /// These offset the zero isosurface of the SDF. This means that
    /// getDistance() will always return getDistance() - offset,
    /// effectively inflating (if offset is positive) or deflating (if
    /// offset is negative) the object.
    ///
    /// Offsetting happens before inversion.
    fpreal			 getOffset() const { return myOffset; }
    void			 setOffset(fpreal offset) { myOffset = offset; }

    /// Load and save this SDF.
    /// This occurs in binary format.
    void		save(std::ostream &os) const;
    bool		load(UT_IStream &is);

protected:
    /// This sends rays along all the major axes.  The cell distances
    /// are set to the distance along the axes to surfaces.
    void			 sendRays(const GU_Detail *gdp, 
					    bool laserscan,
					    bool usemetafield,
					    fpreal tol);

    /// This fixes the sign of the distance field by finding a
    /// consensus among neighbouring voxels.
    /// This requires myQueueIndices to be present.
    /// If forcebounds is set, it will make the boundary all positive
    /// in sign, making it easier to recover from holes.
    void			 fixSigns(bool forcebounds);

    /// This finds consensus among the neighbours of this point
    /// It may then flip this points sign.  If it does so, the
    /// point and it's neighbours are added to the list to
    /// be tested in the future.
    /// This uses myQueueIndices to track if a voxel has been added before.
    /// Returns 1 if a point flipped, 0 otherwise.
    int			 	findConsensus(int x, int y, int z,
				    int iteration,
				    fpreal alpha, fpreal beta,
				    UT_Array<gu_sdf_voxpos> &flippedlist);

    /// Performs a fast sweep along the given axis.  Any voxel pair
    /// that has a bad consensus will be raytested & flipped.
    /// The flip is then propagated.
    /// dir is +/-1 to represent a forward or back sweep.
    int			fastSweepCorrect(GU_RayIntersect *isect, 
					int axis, int dir, fpreal alpha,
					fpreal tol);
    
    /// Rasterizers...
    void			 rasterize(const GEO_Primitive *prim);
    void			 rasterizePoly(const GEO_PrimPoly *poly);
    
    /// This takes coordinates in cell space.
    void			 rasterizeRawTri(UT_Vector3 p1, UT_Vector3 p2, UT_Vector3 p3);

    /// This sets the given cell to a specific SDF value.  This will be
    /// considered a finalized value, so it will be written to the
    /// output voxel array, any QElem will be deleted, and any neighbouring
    /// cells will get tentative values.
    /// Propagation to neighbouring cells only occurs if dist < maxdist,
    /// unless maxdist < 0
    void			 setCellDist(int x, int y, int z, fpreal dist, 
					fpreal maxdist=-1.0);

    /// Sets a tentative value to a given cell.
    void			 setCellTentative(int x, int y, int z, 
						  fpreal tentative);

    /// Gets or creates the qelement at the given location.  Returns 0
    /// if the element has already been processed.
    gu_sdf_qelem		*getQElem(int x, int y, int z, 
					  bool create=true);
    
    /// If you change the weight of the qelem, this will update the queue.
    void			 updateQElem(gu_sdf_qelem *qelem);

    /// Calculates the distance to a cell by using the finalized
    /// distances to adjacent cells.
    /// The olddistance is used to determine sign.
    fpreal			 findMinDist(int x, int y, int z,
					     fpreal olddist) const;

    /// Propagate distances from all elements in the queue until maximum
    /// distance is reached.
    void			 propagateQueue(fpreal maxdist);

    /// Builds a shell of distance values around the geometry
    /// based on minimum distance values.
    /// We want to fill in the cell values 
    /// from (x,y,z) to (x+xw-1,y+yw-1,z+zw-1)
    /// inclusive.   We only fill values that are within a band about
    /// the surface, leaving the rest for FMM.  This is because min
    /// distance calls are expensive!
    void			buildFromMinDist(GU_RayIntersect &isect,
					int x, int y, int z,
					int xw, int yw, int zw);

    /// Builds a shell of distance values around the geometry
    /// from the point cloud data.
    /// If normals are present, they are used to calculate sidedness.
    /// Otherwise, the SDF becomes an unsigned distance-to-implicit surface.
    void			 buildFromPointCloud(const GU_Detail *gdp,
					const GU_SDFParms &parms);

    //
    // Fast Iterative Method additions
    //

    bool findZeroCrossing(UT_VoxelProbeCubeF &src_probe, fpreal &voxeldist, const fpreal isocontour);

    void buildFromVolumeFIM(const UT_VoxelArrayF &src, const fpreal isocontour, const fpreal maxdist,
                                const fpreal tolerance, const int iterations);
    
    THREADED_METHOD7(GU_SDF, activelist.entries() > 100, solveEikonalIterationFIM,
                        UT_VoxelArrayF&, dst,
                        UT_Array<UT_Array<UT_Vector3I>>&, newlist,
                        UT_Array<bool>&, tileoccupied,
                        const UT_VoxelArrayF&, olddst,
                        const UT_Array<UT_Vector3I>&, activelist,
                        const fpreal, maxdist, const fpreal, tolerance);

    void solveEikonalIterationFIMPartial(UT_VoxelArrayF &dst,
                                            UT_Array<UT_Array<UT_Vector3I>> &newlist,
                                            UT_Array<bool> &tileoccupied,
                                            const UT_VoxelArrayF &olddst,
                                            const UT_Array<UT_Vector3I> &activelist,
                                            const fpreal maxdist, const fpreal tolerance,
                                            const UT_JobInfo &info);

    THREADED_METHOD1(GU_SDF, activelist.entries() > 100, assignSolvableLabelsFIM,
                        const UT_Array<UT_Vector3I>&, activelist);

    void assignSolvableLabelsFIMPartial(const UT_Array<UT_Vector3I> &activelist,
                                        const UT_JobInfo &info);

    THREADED_METHOD1(GU_SDF, activelist.entries() > 100, removeSolvableLabelsFIM,
                        const UT_Array<UT_Vector3I>&, activelist);

    void removeSolvableLabelsFIMPartial(const UT_Array<UT_Vector3I> &activelist,
                                        const UT_JobInfo &info);

    THREADED_METHOD2(GU_SDF, myQueueIndices->numTiles() > 20, loadFinishedCellNeighboursFIM,
                        UT_Array<UT_Array<UT_Vector3I>>&, newlist,
                        UT_Array<bool>&, tileoccupied);

    void loadFinishedCellNeighboursFIMPartial(UT_Array<UT_Array<UT_Vector3I>> &list,
                                                UT_Array<bool> &tileoccupied,
                                                const UT_JobInfo &info);

    THREADED_METHOD3(GU_SDF, occupied.entries() > 100, uncompressActiveTilesFIM,
                        UT_VoxelArrayF&, olddst,
                        UT_VoxelArrayF&, dst,
                        const UT_Array<bool>&, occupied);

    void uncompressActiveTilesFIMPartial(UT_VoxelArrayF &olddst,
                                            UT_VoxelArrayF &dst,
                                            const UT_Array<bool> &occupied,
                                            const UT_JobInfo &info);

    THREADED_METHOD5(GU_SDF, src.numTiles() > 20, computeAndLabelZeroCrossingsFIM,
                        UT_VoxelArrayF&, dst,
                        UT_VoxelArrayF&, olddst,
                        const UT_VoxelArrayF&, src,
                        const fpreal, isocontour,
                        const fpreal, maxdist);

    void computeAndLabelZeroCrossingsFIMPartial(UT_VoxelArrayF &dst,
                                                UT_VoxelArrayF &olddst,
                                                const UT_VoxelArrayF &src,
                                                const fpreal isocontour,
                                                const fpreal maxdist,
                                                const UT_JobInfo &info);

    THREADED_METHOD5(GU_SDF, src.numTiles() > 20, buildVolumeZeroCrossings,
                        UT_Array<UT_Array<UT_Vector3I>> &, crossingindices,
                        UT_Array<UT_Array<fpreal>> &, crossingdists,
                        const UT_VoxelArrayF&, src,
                        const fpreal, isocontour,
                        const fpreal, maxdist);

    void buildVolumeZeroCrossingsPartial(UT_Array<UT_Array<UT_Vector3I>> &crossingindices,
                                            UT_Array<UT_Array<fpreal>> &crossingdists,
                                            const UT_VoxelArrayF &src,
                                            const fpreal isocontour,
                                            const fpreal maxdist,
                                            const UT_JobInfo &info);

    /// Builds a shell of distance values around the given iso-contour
    /// of the volume.  This allows one to then reinitialize an SDF function.
    void buildFromVolume(const UT_VoxelArrayF &src, const fpreal isocontour,
                                const fpreal maxdist);
    
    /// Takes a list of GEO_Points and finds the best fititng
    /// plane which interpolates them.  If noff is not -1, it
    /// will derive the normals from the point normals.
    UT_Plane			 findPlaneFromNeighbour(
				    const UT_Vector3 &sample,
                                    const GU_Detail *gdp,
				    const GA_OffsetArray &neighbour,
				    const GA_ROHandleV3 &normalattrib) const;


    THREADED_METHOD2(GU_SDF,
		     myVoxels->numTiles() > 16,
		     copyVolumeSamples,
		     const GU_Detail *, gdp,
		     const UT_ValArray<const GEO_Primitive *> &, volumes);

    void copyVolumeSamplesPartial(const GU_Detail *gdp, 
			    const UT_ValArray<const GEO_Primitive *> &volumes,
			    const UT_JobInfo &info);

    /// This is the actual explicit array of voxels.  It is not
    /// present if the SDF array is implicit. It does not incorporate the
    /// offset or inversion.
    UT_VoxelArrayF		*myVoxels;

    /// What sort of implicit representation we are using.
    /// SDF_EXPLICIT if we have none.
    sdfImplicitType		 myImplicitType;

    UT_Vector3			 myImplicitNormal;

    /// This flags whether we act as if our SDF had the opposite sign
    /// than it does.
    bool			 myInvert;
    /// This defines how far we should shift the zero isosurface of the SDF.
    fpreal			 myOffset;
    
    /// Real world coordinates are transformed into the voxel array's 0-1
    /// space by subtracting myOrig and dividing by mySize.
    UT_Vector3			 myOrig, mySize;

    GEO_PrimVolumeXform		 myHeightXform;
    int				 myHeightAxis;

    /// This stores the size of a voxel along each axis.
    UT_Vector3			 myVoxelSize;

    /// This caches the diameter of a voxel cell.  Ie, myVoxelSize.length()
    fpreal			 myVoxelDiameter;

    /// These are used in the building stage to rasterize to, and in the
    /// expansion stage as the priority queues.
    UT_VoxelArray<int>		*myQueueIndices;
    UT_ValArray<gu_sdf_qelem *>	 myQueueElements;
    UT_IntArray			 myQueueFreeList;
    gu_sdf_queue 		*myQueue;
};

#endif
