/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_RawField.h ( SIM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SIM_RawField__
#define __SIM_RawField__

#include "SIM_API.h"

#include <GEO/GEO_PointTree.h>
#include <UT/UT_VoxelArray.h>
#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_Vector.h>
#include <UT/UT_FloatArray.h>
#include <UT/UT_Wavelet.h>

class SIM_VectorField;
class SIM_RawField;
class SIM_RawIndexField;
class GEO_PrimVolume;
class GEO_PrimVDB;
class GEO_PrimVolumeXform;
class GU_Detail;
class CE_Grid;

class gu_sdf_qelem;

// The order of this enum is important.  It is a 3 bit array
// where each bit determines if the center value or corner value
// should be used on that axis.  0 means center, 1 means node.
// The least significant bit is x axis, then y, then z.
enum SIM_FieldSample
{
    SIM_SAMPLE_CENTER = 0,
    SIM_SAMPLE_FACEX = 1,
    SIM_SAMPLE_FACEY = 2,
    SIM_SAMPLE_EDGEXY = 3,
    SIM_SAMPLE_FACEZ = 4,
    SIM_SAMPLE_EDGEXZ = 5,
    SIM_SAMPLE_EDGEYZ = 6,
    SIM_SAMPLE_CORNER = 7
};

enum SIM_FieldBoundary
{
    SIM_BOUNDARY_NONE,	// Unchanged, free to change.
    SIM_BOUNDARY_SAME,	// Same as adjacent voxels.
    SIM_BOUNDARY_ZERO,	// Set to constant value, myBoundaryValue
    SIM_BOUNDARY_NEGATE,	// Opposite sign of adjacent voxels.
    SIM_BOUNDARY_FIXED,	// Unaffected by enforcement, but should be treated
			// as a fixed boundary by projection methods.
};

enum SIM_FieldAdvection
{
    SIM_ADVECT_SINGLE = 0,	// Take a single step
    SIM_ADVECT_TRACE = 1,	// Trace according to velocity field
    SIM_ADVECT_MIDPOINT = 2,	// Trace using midpoint updates
    SIM_ADVECT_HJWENO = 3,	// Don't trace, but use HJWENO update
    SIM_ADVECT_UPWIND = 4,	// First order upwind.
    SIM_ADVECT_RK3 = 5,		// Third order TVD-RK3
    SIM_ADVECT_RK4 = 6		// Fourth order TVD-RK4
};

//
// The boundary of the field can be set to be "open" above a
// line using the following struct. The height value specifies
// the dividing line between the open and closed boundary with 
// respect to the plane positioned at the origin and oriented with
// planeNormal. This is useful for letting liquid splash
// out of a closed boundary without the expense of explicitly
// creating a solid boundary.
//

struct SIM_BoundaryLine
{
    SIM_BoundaryLine()
        : activated(false)
        , height(0.)
        , planeNormal(UT_Vector3(0.))
        {}
    bool activated;
    fpreal height;
    UT_Vector3 planeNormal;
};


// Type definition for a velocity function
typedef UT_Vector3 (*sim_PointVelocity)(const UT_Vector3 &, int);

class SIM_API SIM_RawField
{
public:
		SIM_RawField();
    virtual    ~SIM_RawField();

    /// Copy constructor:
		SIM_RawField(const SIM_RawField &src);

    /// Assigment operator:
    const SIM_RawField	&operator=(const SIM_RawField &src);		

    /// Initializes the field.
    /// The resolution given is in terms of voxels, the actual dimensions
    /// of this field may be slightly different due to the sampling
    /// choice.
    void	init(SIM_FieldSample sample, 
		     const UT_Vector3 &orig, const UT_Vector3 &size,
		     int xres, int yres, int zres);
    void	init(SIM_FieldSample sample, 
		     const UT_Vector3 &orig, const UT_Vector3 &size,
		     int xres, int yres, int zres, 
		     const UT_Vector3 &voxelsize);

    /// Initializes the field.
    /// Will gain ownership of the given voxel array.
    void	init(SIM_FieldSample sample,
		     const UT_Vector3 &orig, const UT_Vector3 &size,
		     UT_VoxelArrayF *voxels);
    void	init(SIM_FieldSample sample,
		     const UT_Vector3 &orig, const UT_Vector3 &size,
		     UT_VoxelArrayF *voxels,
		     const UT_Vector3 &voxelsize);

    /// Initializes the voxel iterator to only run over a subset
    /// of tiles that correspond to the appropriate range for
    /// the jobinfo.
    void	getPartialRange(UT_VoxelArrayIteratorF &vit,
			    const UT_JobInfo &info) const;

    /// Returns true if this should be multithreaded.
    bool	shouldMultiThread() const
		{
#ifdef CELLBE
		    return false;
#else
		    return field()->numTiles() > 1;
#endif
		}

    /// Initializes this to be the same dimensions, sampling pattern,
    /// etc, of the given field.
    /// The values of this may be reset to zero.
    void	match(const SIM_RawField &src);
    void	match(const SIM_RawIndexField &src);

    /// Initializes this field to a constant value.
    void	makeConstant(float cval);

    /// Initializes this to be the same as the given field.  The
    /// values of this will be found by resampling the other field
    /// with the new sampling pattern.
    void	resample(SIM_FieldSample sample, const SIM_RawField *src);

private:
    /// This helper function does the actual work for resample(...).
    THREADED_METHOD2(SIM_RawField, shouldMultiThread(), resampleHelper,
                     SIM_FieldSample, sample,
                     const SIM_RawField*, src);
    void resampleHelperPartial(SIM_FieldSample sample,
                               const SIM_RawField* src,
                               const UT_JobInfo& info);
public:

    /// Returns the set of samples in *this* field which correspond
    /// to the given location & sampling pattern.
    /// ix, iy, and iz should be size 8.
    /// If you want the deltas for the sampling pattern, call with
    /// x, y, z zero and clamp to false.
    void	getSamplePattern(SIM_FieldSample sample, int x, int y, int z,
			    int &numsample, int *ix, int *iy, int *iz,
			    bool clamp) const;

    /// Returns an offset, in voxel coords, to sample *this* field giving
    /// a voxel based address in a field with sampling pattern sample.
    /// The offset will be 0.5, 0, or -0.5 in each axis.
    UT_Vector3	getSampleVoxelOffset(SIM_FieldSample sample) const;

    /// Extrapolates the values of this field to all locations which are
    /// on the wrong side of the isocontour.  dir == 1 means greater than
    /// isocontour is wrong, dir == -1 is less than.
    /// maxdist, if not negative, represents the greatest *absolute*
    /// distance to extrapolate to.  Values outside of this are left
    /// unaffected, unless clamp is set, in which case they are set to
    /// clampval.
    void	extrapolate(const SIM_RawField *depths,
			    const SIM_RawField *valid,
			    fpreal isocontour, 
			    fpreal dir, fpreal maxdist,
			    bool clamp, fpreal clampval);

    struct sim_extrapolateFromIndexParms
    {
        const SIM_RawField *src;
        const SIM_RawField *depths;
	const SIM_RawField *valid;
	const SIM_RawField *collision;
        const SIM_RawIndexField *index;
        const GU_Detail *gdp;
        UT_DMatrix4 xform;
        fpreal isocontour;
	fpreal isotol;
        fpreal dir;
        fpreal maxdist;
        bool clamp;
        fpreal clampval;
    };

    struct sim_extrapolateCacheElem
    {
	sim_extrapolateCacheElem(fpreal v, fpreal w, const UT_Vector3 &p):
			    sampleval(v), samplew(w), ptpos(p) {}
	sim_extrapolateCacheElem() : sampleval(0), samplew(0) {}
	fpreal 		sampleval;
	fpreal 		samplew;
	UT_Vector3 	ptpos;
    };

    /// Extrapolates the values of this field to all locations which are
    /// on the wrong side of the isocontour.  dir == 1 means greater than
    /// isocontour is wrong, dir == -1 is less than.
    /// maxdist, if not negative, represents the greatest *absolute*
    /// distance to extrapolate to.  Values outside of this are left
    /// unaffected, unless clamp is set, in which case they are set to
    /// clampval.  The extrpolated values are found by interpolating at the
    /// position of the closest points in the provided GU_Detail, as stored
    /// in the index field by point index.
    void extrapolateFromIndex(const SIM_RawField *depths,
                              const SIM_RawIndexField *index,
			      const SIM_RawField *collision,
			      const SIM_RawField *valid,
                              const GU_Detail *gdp,
                              const UT_DMatrix4 &xform,
                              fpreal isocontour,
			      fpreal isotol,
                              fpreal dir,
                              fpreal maxdist,
                              bool clamp,
                              fpreal clampval);

    THREADED_METHOD1(SIM_RawField, shouldMultiThread(),
                     extrapolateFromIndexInternal,
                     sim_extrapolateFromIndexParms &,parms);

    void extrapolateFromIndexInternalPartial(sim_extrapolateFromIndexParms &p,
                                             const UT_JobInfo &info);

    /// Advances source field along the field's curvature.
    /// Takes a given b to act as the movement coefficient.
    /// Does a single euler step.
    /// this is filled in with the resulting field.  this cannot be source.
    /// This solves:
    ///  dphi/dt = b * K * |grad(phi)|
    THREADED_METHOD3(SIM_RawField, shouldMultiThread(), moveAlongCurvature,
		    fpreal, b_val,
		    const SIM_RawField &, source,
		    fpreal, timestep)
    void	 moveAlongCurvaturePartial(fpreal b_val, 
			    const SIM_RawField &source,
			    fpreal timestep,
			    const UT_JobInfo &jobinfo);

    /// Advances this field along the field's curvature.
    /// Is given a total amount to move and will calculate the appropriate
    /// timestep according to the given cfl condition & min/max iterations.
    void	 moveAlongCurvature(fpreal amount,
			    fpreal cflcond,
			    int miniter, int maxiter);

    /// Advances source field along the field's normal direction.
    /// Takes a matching field which defines the speed to move in the
    /// normal direction on a per cell basis.
    /// Past the bandwidth assumes a valid SDF and skips gradient estimates.
    /// Does a single euler step.
    /// this is filled in with the resulting field.  this cannot
    /// be source.
    /// This solves:
    ///  dphi/dt + speed * |grad(phi)| = 0
    THREADED_METHOD4(SIM_RawField, shouldMultiThread(), moveAlongNormal,
		     const SIM_RawField &, speed,
		     const SIM_RawField &, source,
		     fpreal, timestep,
		     fpreal, bandwidth)
    void	moveAlongNormalPartial(const SIM_RawField &speed, 
				const SIM_RawField &source, 
				fpreal timestep, fpreal bandwidth,
				const UT_JobInfo &info);

    /// Uses the midpoint method to do a second order temporal
    /// update of the moveAlongNormal algorithm.
    void	moveAlongNormalMidpoint(const SIM_RawField &speed,
				const SIM_RawField &source,
				fpreal timestep, fpreal bandwidth);

    /// Performs the reinitialization equation.
    /// This solves for:
    ///  dphi/dt + S(phi0) * (|grad(phi)| - 1) = 0
    /// at steady state.
    ///  S(phi0) = phi0 / sqrt(phi0^2 + dx^2), a smeared sign function.
    /// It is held constant for the reinitialization.
    /// Usemidpoint indicates using midpoint integration vs. Euler.
    /// Any parts of the SDF past the bandwidth are assumed to already
    /// be a valid SDF and not require reinitialization.
    void	reinitializeSignedDistance(int maxiter,
					   bool usemidpoint,
					   fpreal bandwidth);

    /// Negates all values in this field
    THREADED_METHOD(SIM_RawField, shouldMultiThread(), negate)
    void	negatePartial(const UT_JobInfo &info);

    /// Scales all values in this field
    THREADED_METHOD1(SIM_RawField, shouldMultiThread(), scale,
		    fpreal, scale)
    void	scalePartial(fpreal scale, const UT_JobInfo &info);

    /// Makes this field the minimum or maximum 
    /// of this field and the other field.  The other field
    /// is not assumed to be matching.
    THREADED_METHOD1(SIM_RawField, shouldMultiThread(), maximum,
			const SIM_RawField *, other)
    void	maximumPartial(const SIM_RawField *other, const UT_JobInfo &info);

    THREADED_METHOD1(SIM_RawField, shouldMultiThread(), minimum, 
		    const SIM_RawField *, other)
    void	minimumPartial(const SIM_RawField *other, const UT_JobInfo &jobinfo);

    /// Averages this with the other field.  Result in this.  Assumes
    /// fields match.
    THREADED_METHOD1(SIM_RawField, shouldMultiThread(), average,
		    const SIM_RawField &, other)
    void	averagePartial(const SIM_RawField &other, const UT_JobInfo &jobinfo);	

    /// Sums the field into separate summation lists, one per component
    /// of the comp index field.  Negative components ignored, the given
    /// arrays should already be sized to fit the maxindex.
    THREADED_METHOD3_CONST(SIM_RawField, shouldMultiThread(), sumPerComponent,
		    UT_DoubleArray &, result,
		    UT_Int64Array &, activevoxels,
		    const SIM_RawIndexField *, comp)
    void	sumPerComponentPartial(UT_DoubleArray &result,
		    UT_Int64Array &activevoxels,
		    const SIM_RawIndexField *comp,
		    const UT_JobInfo &info) const;

    THREADED_METHOD5_CONST(SIM_RawField, shouldMultiThread(), sumComponent,
		    double &, result,
		    int64 &, activevoxels,
		    int, compnum,
		    const SIM_RawIndexField *, comp,
		    const SIM_RawField *, pressureboundary)
    void	sumComponentPartial(double &result,
		    int64 &activevoxels,
		    int compnum,
		    const SIM_RawIndexField *comp,
		    const SIM_RawField *pressureboundary,
		    const UT_JobInfo &info) const;

    /// Adds to this field the value given per component.  Negative components
    /// neglected.
    THREADED_METHOD2(SIM_RawField, shouldMultiThread(), addValuePerComponent,
		const UT_DoubleArray &, valuelist,
		const SIM_RawIndexField *, comp)
    void	addValuePerComponentPartial(const UT_DoubleArray &valuelist,
		    const SIM_RawIndexField *comp,
		    const UT_JobInfo &info);

    THREADED_METHOD4(SIM_RawField, shouldMultiThread(), addValueToComponent,
		double, value,
		int, compnum,
		const SIM_RawIndexField *, comp,
		const SIM_RawField *, pressureboundary)
    void	addValueToComponentPartial(double value,
		    int compnum,
		    const SIM_RawIndexField *comp,
		    const SIM_RawField *pressureboundary,
		    const UT_JobInfo &info);

    /// Sets this to A + s*B.  A & B match this.  This can be either
    /// A or B, or neither.
    THREADED_METHOD3(SIM_RawField, shouldMultiThread(), setScaleAdd, 
		    const SIM_RawField &, A,
		    fpreal, scale,
		    const SIM_RawField &, B)
    void	setScaleAddPartial(const SIM_RawField &A,
			    fpreal scale,
			    const SIM_RawField &B, const UT_JobInfo &jobinfo);

    /// Sets this to the smeared sign function of the given sdf.
    ///  this = sdf / sqrt(sdf*sdf + bandwidth*bandwidth)
    /// this must already match sdf.
    THREADED_METHOD2(SIM_RawField, shouldMultiThread(), smearedSign,
			const SIM_RawField &, sdf,
			fpreal, bandwidth)
    void	smearedSignPartial(const SIM_RawField &sdf,
				    fpreal bandwidth,
				    const UT_JobInfo &info);

    /// Sets this to the heaviside function of itself.
    /// this = clamp(-this/diam+0.5, 0, 1)
    THREADED_METHOD(SIM_RawField, shouldMultiThread(), convertToHeaviside)
    void	convertToHeavisidePartial(const UT_JobInfo &info);

    /// Perform heaviside & inverse heaviside in a consistent fashion.
    static fpreal	toHeaviside(fpreal val, fpreal diam);
    static fpreal	fromHeaviside(fpreal val, fpreal diam);
    /// Defines width as maxcomponent of our voxels.
    fpreal		toHeaviside(fpreal val) const;
    fpreal		fromHeaviside(fpreal val) const;

    /// Types of reductions supported by reduceOp.
    enum REDUCE_NAMES
    {
	REDUCE_MAX,
	REDUCE_MIN,
	REDUCE_AVERAGE,
	REDUCE_SUM,
	REDUCE_SUMABS,
	REDUCE_SUMSQUARE,
	REDUCE_RMS,
	REDUCE_MEDIAN,
	REDUCE_MAXABS,
	REDUCE_MINABS
    };
    
    /// Perform reduction on the field using the given method
    /// The internal methods are helpers to deal with the threading
    /// collating.
    fpreal	reduceOp(REDUCE_NAMES op) const;

    /// Prebuilt versions.
    fpreal	average() const { return reduceOp(REDUCE_AVERAGE); }

    /// Performs a localized reduction on the field.
    /// Stores in this the result of the reduction.
    /// Radius is in world coordinates.
    void	localReduceOp(REDUCE_NAMES op, const UT_Vector3 &radius);

    /// Inplace box blur.
    /// Radius is in world coordinates.
    void	boxBlur(float radius) 
		{ 
		    UT_Vector3		r(radius, radius, radius);
		    localReduceOp(REDUCE_AVERAGE, r); 
		}

    /// Performs the reduction only including parts of the field
    /// that match the given mask.  If mask is null, falls through
    /// to normal reduction.
    fpreal	reduceMaskedOp(REDUCE_NAMES op, 
			const SIM_RawField *mask, bool maskissdf) const;

    /// Sorts the valid voxels (as defined by optional mask) and returns
    /// the given percentile voxel.  0.5 means median. 0.25 first quartile,
    /// etc.
    fpreal	findProportionalValue(fpreal position, 
			const SIM_RawField *mask, bool maskissdf) const;

    /// Returns true if the given field and this one match in terms
    /// of number of voxels and bounding box size.
    /// This means the voxel cells match - not necessarily the sample
    /// points!
    bool	isMatching(const SIM_RawField *field) const;
    bool	isMatching(const SIM_RawIndexField *field) const;

    /// Returns true if the two fields are precisely aligned.  This
    /// means that samples are matched so a given integer index
    /// into either field would give the same result.
    bool	isAligned(const SIM_RawField *field) const;
    bool	isAligned(const SIM_RawIndexField *field) const;
    bool	isAligned(const GEO_PrimVolume *vold) const;

    /// Returns true if our field has any NANs
    bool	hasNan() const { return field()->hasNan(); }

    /// Tests for nans, outputs text and asserts if has any
    /// Only runs if the test environment variable is set.
    void	testForNan() const;

    /// Fetches the raw field.
    const UT_VoxelArrayF	*field() const { if (myFieldOutOfDate) { updateFieldFromGrid(); } return myField; }
    UT_VoxelArrayF		*fieldNC() const { if (myGrid) { clearGrid(); } return myField; }

    /// Fetches the compute grid, returns 0 if not present.
    CE_Grid			*grid() const;

    /// Fetches the compute grid but throws cl::Error() on failure.
    CE_Grid			*requireGrid() const;

    /// Mark the field as out of date, but only if we have a valid grid.
    void			 markGridAsChanged() {if (myGrid) myFieldOutOfDate = true;}
    void			 clearGrid() const;

    /// Copies our field into the grid.
    void	 updateGridFromField() const;

    /// Copies our grid into our field.
    void	 updateFieldFromGrid() const;

    /// Steals the voxel array, leaving this pointing to a 0 constant
    /// array
    UT_VoxelArrayF		*steal();

    const UT_Vector3		&getOrig() const { return myOrig; }
    const UT_Vector3		&getSize() const { return mySize; }
    const UT_Vector3		&getBBoxOrig() const { return myBBoxOrig; }
    const UT_Vector3		&getBBoxSize() const { return myBBoxSize; }
    const UT_Vector3		&getVoxelSize() const { return myVoxelSize; }
    void			 setVoxelSize(const UT_Vector3 &voxelsize)
				 { myVoxelSize = voxelsize; myVoxelDiameter = voxelsize.length(); }
    fpreal			 getVoxelDiameter() const { return myVoxelDiameter; }
    fpreal			 getVoxelVolume() const
				 { return myVoxelSize.x() 
					* myVoxelSize.y() 
					* myVoxelSize.z(); }

    SIM_FieldSample		 getSample() const { return mySample; }

    int64			 getMemoryUsage() const;

    /// Returns the resolution of the voxel grid that we are sampling.
    /// This is a count of voxels, so may differ for our different
    /// sampling methods.
    void		 getVoxelRes(int &xres, int &yres, int &zres) const;

    /// Returns the actual number of samples in each resolution.
    /// Preferred over field()->getXRes() as it doesn't require a
    /// copy of the CE fields.
    exint		 getXRes() const { return myField->getXRes(); }
    exint		 getYRes() const { return myField->getYRes(); }
    exint		 getZRes() const { return myField->getZRes(); }

    /// Functions to resolve quantities about the field.

    /// Sample the field at the given world space location.
    fpreal		 getValue(UT_Vector3 pos) const;

    /// Returns an averaged value for the center of the given voxel.
    fpreal		 getCellValue(int x, int y, int z) const;

    /// Adds the value to the given voxel cell, averaging out among
    /// adjacent samples if we aren't sampled evenly.
    void		 addToCell(int x, int y, int z, fpreal v);

    /// Ensures the given voxel cell has the given value.  This
    /// will set all of the adjacent samples if we aren't sampled
    /// evenly.
    void		 setCellValue(int x, int y, int z, fpreal v);

    /// Return the gradient of the field at the given world space location.
    /// Uses central differencing with a sample spacing of the voxelsize.
    UT_Vector3		 getGradient(UT_Vector3 pos) const;

    /// Returns the gradient at the given voxel index.
    /// Uses central differencing.
    UT_Vector3		 getGradientAtIndex(int x, int y, int z) const;

    /// Computes the laplacian of the field at the world space
    /// coordinate using interpolation of getLaplacianAtIndex
    fpreal64		 getLaplacian(UT_Vector3 pos) const;

    /// Computes the laplacian of the field at the specific voxel index
    /// specified
    fpreal64		 getLaplacianAtIndex(int x, int y, int z) const;

    /// Returns the curvature of the field at a given world space location.
    /// Uses interpolation of index based getCurvatureAtIndex.
    fpreal64		 getCurvature(UT_Vector3 pos) const;

    /// Computes the curvature at the given voxel index.
    /// Uses central differencing.
    /// The resulting curvature is clamped according to the resolution
    /// of the field to avoid abnormally large values in noisy data.
    fpreal64		 getCurvatureAtIndex(int x, int y, int z) const;

    /// Computes the curvature according to a 3^3 voxel probe
    static fpreal64	 getCurvatureAtProbe(UT_VoxelProbeCubeF &probe, const UT_Vector3 &invvoxelsize);

    /// Computes K |grad(phi|), the curvature times the length of the
    /// gradient.  By folding the two operations, we can return a non-zero
    /// value where grad goes to zero (but curvature doesn't) using
    /// L'Hospital's rule.  This also does not clamp the curvature.
    fpreal64		 getCurvatureTimesGradAtIndex(int x, int y, int z) const;

    /// Giving the relevent divided differences, compute the HJWENO
    /// approxmiation of the derivative.
    fpreal64		 calculateHJWENO(fpreal64 v1, fpreal64 v2, fpreal64 v3, fpreal64 v4, fpreal64 v5) const;

    /// Calculate the derivitive along the specified axis using
    /// an HJ WENO method at the given index.  The boolean
    /// determines whether the derivitive in the positive or negative
    /// direction is computed.
    fpreal64		 calculateDerivative(int x, int y, int z,
				    int axis, bool positivegradient) const;

    /// Converts the boundary line information from world space to local index space.
    /// This applies an inverse transform to the plane normal and regular transform
    /// to the height value to put both into index space.
    SIM_BoundaryLine	boundaryLineToIndex(const SIM_BoundaryLine &worldbline) const;

    /// Convert indices to world coordinates and vice-versa.  Note this uses
    /// this field's indices which change depending on sampling.
    bool		 indexToPos(int x, int y, int z, UT_Vector3 &pos) const;

    /// Converts a worldspace position into an integer index.
    bool		 posToIndex(UT_Vector3 pos, int &x, int &y, int &z) const;

    bool		posToIndex(UT_Vector3 wpos, UT_Vector3 &ipos) const;

    /// Converts worldspace position into an integer index + the lerp values
    /// required to interpolate.
    /// Lerp with (1-dx) * (x,y,z) + dx * (x+1,y,z)
    bool		 posToIndex(UT_Vector3 pos, int &x, int &y, int &z,
				    fpreal &dx, fpreal &dy, fpreal &dz) const;
    /// Convert voxel cell indices to world coordinates and vice-versa.
    /// Returns values at cell centers.  Is equivalent to the indexToPos
    /// style functions only when sampling is CENTER.
    bool		 cellIndexToPos(int x, int y, int z, UT_Vector3 &pos) const;
    bool		 posToCellIndex(UT_Vector3 pos, int &x, int &y, int &z) const;

    /// Verbs that can be performed on these fields.

    /// Advect a point in space according to an array of velocity
    /// fields.
    static void		 advect(UT_Vector3 &pos, 
				const SIM_RawField *velx, 
				const SIM_RawField *vely, 
				const SIM_RawField *velz, float time,
				const SIM_RawField *collision = 0, 
				float cfl = 1.0F);
    static void		 advect(UT_Vector3 &pos,
				sim_PointVelocity getVelocity,
				float time, float voxelsize,
				int jobnum = 0,
				const SIM_RawField *collision = 0,
				float cfl = 1.0F);

    /// Advect a point with the midpoint method.
    static void		advectMidpoint(UT_Vector3 &pos,
				const SIM_RawField *velx, 
				const SIM_RawField *vely, 
				const SIM_RawField *velz, float time,
				const SIM_RawField *collision = 0,
				float cfl = 1.0F);
    static void		advectMidpoint(UT_Vector3 &pos,
				sim_PointVelocity getVelocity,
				float time, float voxelsize,
				int jobnum = 0,
				const SIM_RawField *collision = 0,
				float cfl = 1.0F);

    /// Advect a point with TVD-RK3 method.
    static void		advectRK3(UT_Vector3 &pos,
				const SIM_RawField *velx,
				const SIM_RawField *vely,
				const SIM_RawField *velz, float time,
				const SIM_RawField *collision = 0,
				float cfl = 1.0F);

    /// Advect a point with TVD-RK4 method.
    static void		advectRK4(UT_Vector3 &pos,
				const SIM_RawField *velx,
				const SIM_RawField *vely,
				const SIM_RawField *velz, float time,
				const SIM_RawField *collision = 0,
				float cfl = 1.0F);

    /// Move a point to the given isooffset.
    /// Returns false if fails to complete the move in the given time.
    bool		movePtToIso(UT_Vector3 &pos,
				    fpreal goaliso,
				    fpreal maxtime,
				    fpreal tol = 1e-4) const;

    /// Advect the source field by the given set of velocity fields,
    /// setting this to the result.  Source cannot be this.
    /// this and source are assumed to match.
    void		 advect(const SIM_RawField *source,
				const SIM_RawField *velx, 
				const SIM_RawField *vely, 
				const SIM_RawField *velz, 
				float time,
				const SIM_RawField *collision,
				SIM_FieldAdvection advectmethod,
				float cfl);

    void		advectMinMax(const SIM_RawField *source,
				SIM_RawField *minf, SIM_RawField *maxf,
				const SIM_RawField *velx, 
				const SIM_RawField *vely, 
				const SIM_RawField *velz, 
				float time,
				const SIM_RawField *collision,
				SIM_FieldAdvection advectmethod,
				float cfl);

    struct sim_advectParms
    {
	const SIM_RawField *source;
	const SIM_RawField *velx, *vely, *velz;
	float		    time;
	const SIM_RawField *collision;
	float		    cfl;
	SIM_RawField	   *minf, *maxf;
	SIM_FieldAdvection advectmethod;
    };

    THREADED_METHOD7(SIM_RawField, shouldMultiThread(), advectSingle,
		    const SIM_RawField *, source,
		    const SIM_RawField *, velx,
		    const SIM_RawField *, vely,
		    const SIM_RawField *, velz,
		    float, time,
		    const SIM_RawField *, collision,
		    float, cfl
		    )
    void		 advectSinglePartial(const SIM_RawField *source,
				const SIM_RawField *velx, 
				const SIM_RawField *vely, 
				const SIM_RawField *velz, 
				float time,
				const SIM_RawField *collision,
				float cfl,
				const UT_JobInfo &info);

    THREADED_METHOD1(SIM_RawField, shouldMultiThread(), advectSingleMinMax,
		    const sim_advectParms &, parms)
    void		 advectSingleMinMaxPartial(const sim_advectParms &parms,
				const UT_JobInfo &info);

    THREADED_METHOD7(SIM_RawField, shouldMultiThread(), advectTrace,
		    const SIM_RawField *, source,
		    const SIM_RawField *, velx,
		    const SIM_RawField *, vely,
		    const SIM_RawField *, velz,
		    float, time,
		    const SIM_RawField *, collision,
		    float, cfl
		    )
    void		 advectTracePartial(const SIM_RawField *source,
				const SIM_RawField *velx, 
				const SIM_RawField *vely, 
				const SIM_RawField *velz, 
				float time,
				const SIM_RawField *collision,
				float cfl,
				const UT_JobInfo &info);

    THREADED_METHOD1(SIM_RawField, shouldMultiThread(), advectTraceMinMax,
		    const sim_advectParms &, parms)
    void		 advectTraceMinMaxPartial(const sim_advectParms &parms,
				const UT_JobInfo &info);

    THREADED_METHOD8(SIM_RawField, shouldMultiThread(), advectMultistep,
		    const SIM_RawField *, source,
		    const SIM_RawField *, velx,
		    const SIM_RawField *, vely,
		    const SIM_RawField *, velz,
		    float, time,
		    const SIM_RawField *, collision,
		    float, cfl,
		    SIM_FieldAdvection, advectmethod
		    )
    void		 advectMultistepPartial(const SIM_RawField *source,
				const SIM_RawField *velx, 
				const SIM_RawField *vely, 
				const SIM_RawField *velz, 
				float time,
				const SIM_RawField *collision,
				float cfl,
				SIM_FieldAdvection advectmethod,
				const UT_JobInfo &info);

    THREADED_METHOD1(SIM_RawField, shouldMultiThread(), advectMultistepMinMax,
		    const sim_advectParms &, parms)
    void		 advectMultistepMinMaxPartial(const sim_advectParms &parms,
				const UT_JobInfo &info);

    /// Advects the field using HJWENO
    THREADED_METHOD5(SIM_RawField, shouldMultiThread(), advectHJWENO,
			const SIM_RawField &, source,
			const SIM_RawField *, velx,
			const SIM_RawField *, vely,
			const SIM_RawField *, velz,
			fpreal, timestep
			)
    void 		advectHJWENOPartial(const SIM_RawField &source,
				const SIM_RawField *velx, 
				const SIM_RawField *vely, 
				const SIM_RawField *velz, 
				fpreal timestep,
				const UT_JobInfo &info);

    /// Advects the field using upwind
    THREADED_METHOD5(SIM_RawField, shouldMultiThread(), advectUpwind,
			const SIM_RawField &, source,
			const SIM_RawField *, velx,
			const SIM_RawField *, vely,
			const SIM_RawField *, velz,
			fpreal, timestep
			)
    void 		advectUpwindPartial(const SIM_RawField &source,
				const SIM_RawField *velx, 
				const SIM_RawField *vely, 
				const SIM_RawField *velz, 
				fpreal timestep,
				const UT_JobInfo &info);


    THREADED_METHOD4(SIM_RawField, shouldMultiThread(), 
			buoyancy,
			const SIM_RawField *, temperature,
			fpreal, up, 
			fpreal, Tamb,
			fpreal, buoyancy)
    void		 buoyancyPartial(const SIM_RawField *temperature,
				  fpreal up, fpreal Tamb,
				  fpreal buoyancy,
				  const UT_JobInfo &info);

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

    /// Performs the requires mixing.
    static fpreal	 mixValues(MIX_NAMES mixtype,
                               fpreal d, fpreal s)
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

    struct sim_particleToFieldParms
    {
	float		threshold, bandwidth;
	bool		extrapolate, usemaxextrapolate;
	float		maxextrapolatedist;
	float		d_preadd, d_premul;
	float		s_preadd, s_premul;
	float		postadd, postmul;
	float		uniformrad;
	MIX_NAMES	calctype;

	bool		useattrib;
	const char	*attribname;
	int		offset;
	UT_DMatrix4	xform;
	GEO_PrimVolumeXform	*fieldxform;
	bool		useaffine;
	const char	*affinename;
    };

    static fpreal applyParticleToFieldParms(fpreal srcval, fpreal dstval,
                                    const sim_particleToFieldParms &parms)
    {
        // srcval is our source value from the particle-defined field.
        // dstval is original value from destination field.
        // Apply our calculation method, and return.
        srcval *= parms.s_premul;
        srcval += parms.s_preadd;
        dstval *= parms.d_premul;
        dstval += parms.d_preadd;
        dstval = mixValues(parms.calctype, dstval, srcval);

        dstval *= parms.postmul;
        dstval += parms.postadd;
        return dstval;

    }

    THREADED_METHOD3(SIM_RawField, shouldMultiThread(),
			    applyParticles,
			    const GU_Detail *, particles,
			    GEO_PointTreeGAOffset *, pttree,
			    sim_particleToFieldParms &, parms)
    void		 applyParticlesPartial(
				    const GU_Detail *particles,
				    GEO_PointTreeGAOffset *pttree,
				    sim_particleToFieldParms &parms,
				    const UT_JobInfo &info);

    void		 accumulateParticles(const GU_Detail *particles,
					    sim_particleToFieldParms &parms,
					    const GA_PointGroup *ptgrp = NULL);

    THREADED_METHOD6(SIM_RawField, shouldMultiThread(), advect2,
		    const SIM_RawField *, source,
		    sim_PointVelocity, getVelocity,
		    float, time, float, voxelsize,
		    const SIM_RawField *, collision,
		    float, cfl)
    void		advect2Partial(const SIM_RawField *source,
				sim_PointVelocity getVelocity,
				float time, float voxelsize,
				const SIM_RawField *collision,
				float cfl,
				const UT_JobInfo &info);

    /// Advect this field by the given velocity fields.  Invokes
    /// advect but handles the creation of the intermediate field.
    void		 advectSelf(const SIM_RawField *velx, 
				const SIM_RawField *vely, 
				const SIM_RawField *velz, 
				float time,
				const SIM_RawField *collision,
				SIM_FieldAdvection advectmethod,
				float cfl);
    void		 advectSelf(sim_PointVelocity getVelocity,
				float time, float voxelsize,
				const SIM_RawField *collision,
				float cfl);

    /// Like advectSelf, but also generates min/max fields.
    void		 advectMinMaxSelf(SIM_RawField *minfield,
				SIM_RawField *maxfield,
				const SIM_RawField *velx, 
				const SIM_RawField *vely, 
				const SIM_RawField *velz, 
				float time,
				const SIM_RawField *collision,
				SIM_FieldAdvection advectmethod,
				float cfl);
    /// Solve the diffusion equation over this field according to
    /// the given diffusion rate.  One likely wants to roll the
    /// desired timestep into the diffusion rate.
    void		 diffuse(fpreal diffrate,
				int numiter,
				const SIM_RawField *collision=0);

    /// Raw diffuse algorithm, exposed only for external performance tests
    static void		 diffuse(float *dstdata, const float *srcdata[3][3][3],
				 float b, float ivsx, float ivsy, float ivsz,
				 int tx, int ty,
				 int max_xtile, int max_ytile,
				 int max_xvox, int max_yvox);

    /// Determine all components connected according to < 0 semantic
    void		 computeConnectedComponents(UT_VoxelArray<int64> &comp, int &numcomponent) const;

    /// Computes the divergence of the face-centered velocity field
    /// and stores the result in this.  this must match the velocity field.
    THREADED_METHOD2(SIM_RawField, shouldMultiThread(), buildDivergenceFace,
			const SIM_VectorField *, vel,
			const SIM_RawField *, surface)
    void		 buildDivergenceFacePartial(const SIM_VectorField *vel,
				    const SIM_RawField *surface,
				    const UT_JobInfo &info);

    enum PCG_METHOD
    {
	PCG_NONE,
	PCG_JACOBI,
	PCG_CHOLESKY,
	PCG_MIC
    };

    /// Solves the pressure field that would eliminate the given
    /// divergence field.
    void		 solvePressure(const SIM_RawField *divergence,
					const SIM_RawField *collision,
					int numiter = 20);
    void		 solvePressurePCG(const SIM_RawField &divergence,
				        SIM_RawIndexField &index,
					SIM_VectorField *vel,
					const SIM_RawIndexField *comp = 0,
					const UT_IntArray *expandable = 0,
					const SIM_RawField *surface = 0,
					bool variational = true,
					bool ghostfluid = true,
					PCG_METHOD pcgmethod = PCG_MIC);

    // For each voxel v in *this and corresponding voxel b in *B
    // sv = sum of 6-way neighbours of v
    // v' = (sv * sumweight + b) * weight
    // This is only applied to the voxels whose parity (x^y^z) matches
    // parity, so should be called twice to complete a single iteration.
    THREADED_METHOD4(SIM_RawField, shouldMultiThread(),
	    gaussSeidelIteration,
	    const SIM_RawField *, B,
	    fpreal32,  weight,
	    fpreal32,  sumweight,
	    int,  parity)
    void gaussSeidelIterationPartial(
	    const SIM_RawField * B,
	    fpreal32  weight,
	    fpreal32  sumweight,
	    int  parity,
	    const UT_JobInfo &info);

    // Note that in the flat case the destination field is passed
    // parameter, as opposed to the previous function where it is
    // the B field that is a parameter.
    THREADED_METHOD4_CONST(SIM_RawField, shouldMultiThread(),
	    gaussSeidelIterationFlat,
	    fpreal32 *, A,
	    fpreal32,  weight,
	    fpreal32,  sumweight,
	    int,  parity)
    void gaussSeidelIterationFlatPartial(
	    fpreal32 * A,
	    fpreal32  weight,
	    fpreal32  sumweight,
	    int  parity,
	    const UT_JobInfo &info) const;

    // Note that in the flat case the destination field is passed
    // parameter, as opposed to the previous function where it is
    // the B field that is a parameter.
    // This does a 4-way neighbours.
    THREADED_METHOD4_CONST(SIM_RawField, shouldMultiThread(),
	    gaussSeidelIterationFlat2D,
	    fpreal32 *, A,
	    fpreal32,  weight,
	    fpreal32,  sumweight,
	    int,  parity)
    void gaussSeidelIterationFlat2DPartial(
	    fpreal32 * A,
	    fpreal32  weight,
	    fpreal32  sumweight,
	    int  parity,
	    const UT_JobInfo &info) const;

    /// Enforces the boundary conditions on this field.
    /// Each axis can have its own conditions
    /// The boundary line is expected to be in index space
    void		 enforceBoundary(SIM_FieldBoundary collisionboundary = SIM_BOUNDARY_NONE, 
					const SIM_RawField *collision=0,
					const SIM_RawField *cvalue = 0,
                                        const SIM_RawField *boundary = 0,
					const SIM_BoundaryLine &indexbline = SIM_BoundaryLine());
    void		 enforceCollisionBoundary(SIM_FieldBoundary boundary,
					const SIM_RawField *collision,
					const SIM_RawField *cvalue = 0);
    void		 enforceSideBoundary(int axis, int side,
					SIM_FieldBoundary bound,
					fpreal boundaryvalue,
					const SIM_BoundaryLine &indexbline = SIM_BoundaryLine(),
                                        const SIM_RawField *boundaryfield = 0);

    THREADED_METHOD3(SIM_RawField, shouldMultiThread(),
			enforceCollisionBoundaryInternal,
			SIM_FieldBoundary, boundary,
			const SIM_RawField *, collision,
			const SIM_RawField *, cvalue)
    void		 enforceCollisionBoundaryInternalPartial(
				SIM_FieldBoundary boundary,
				const SIM_RawField *collision,
				const SIM_RawField *cvalue,
				const UT_JobInfo &info);

    /// Enforces the boundary conditions onto a flat array
    /// using the dimensions of this.
    /// Does SAME boundaries for collision objects.
    /// Index field is where to copy from for each collision voxel.
    void		enforceBoundaryFlat(fpreal32 *values,
				const SIM_RawIndexField *collision_lookup);
    THREADED_METHOD2(SIM_RawField, shouldMultiThread(), 
			enforceCollisionBoundaryFlat,
			fpreal32 *, values,
			const SIM_RawIndexField *, collision_lookup)

    void		enforceCollisionBoundaryFlatPartial(fpreal32 *values,
				const SIM_RawIndexField *collision_lookup,
				const UT_JobInfo &info);
    void		enforceSideBoundaryFlat(fpreal32 *values,
				int axis, int side,
				SIM_FieldBoundary bound,
				fpreal boundval);

    /// These boundary conditions do *not* apply to reading outside
    /// of the valid field range.  The native UT_VoxelArray boundary
    /// condition is used for that.
    /// Instead, they are used for the behaviour of enforceBoundary
    /// and by various places where we want to distinguish open 
    /// velocity fields from closed.

    void		 setBoundary(int axis, int side, SIM_FieldBoundary bound)
				    { myBoundary[axis][side] = bound; }
    SIM_FieldBoundary	 getBoundary(int axis, int side) const 
				    { return myBoundary[axis][side]; }
    void		 setBoundaryValue(int axis, int side, fpreal v)
				    { myBoundaryValue[axis][side] = v; }
    fpreal		 getBoundaryValue(int axis, int side) const
				    { return myBoundaryValue[axis][side]; }

    /// Mark this field as being an extrapolated field.  Out of bound
    /// voxels will read the clamped value.  The difference between
    /// the clamped position and the real position is then dot producted
    /// with the given scale factor and added to the resulting value.
    /// This allows you to have rest fields that extrapolate meaningfully.
    void		 setAsExtrapolatedField(UT_Vector3 scale);

    /// These adjust the native UT_VoxelArray border values that *are*
    /// used for reading outside the valid range.
    UT_VoxelBorderType	 getBorder() const;
    float		 getBorderValue() const;
    void		 setBorder(UT_VoxelBorderType border, float bval);

    /// These adjust the native UT_VoxelArray comrpression options.
    void         setCompressionOptions(const UT_VoxelCompressOptions &options);
    const UT_VoxelCompressOptions &getCompressionOptions() const;

    /// These adjust the native UT_VoxelArray comrpression tolerance..
    void         setCompressionTolerance(fpreal tol);
    fpreal       getCompressionTolerance() const;

    /// Transform turns this into a packed set of wavelet coeffecients
    /// from the scalar data in field.  Inverse unpacks and generates
    /// a scalar field into this.
    ///
    /// Using a smooth edge conditon we assume that
    /// in case of an odd sized field, the ultimate row has
    /// zero diff coefficients.   We thus store the extra
    /// column in the averages side of the matrix, for
    /// ceil(n/2) in the averages and floor(n/2) in the diffs.
    /// Note this causes 3d fields that are actually 2d
    /// to properly decompose.
    void		 waveletTransform(UT_Wavelet::WAVELET_NAMES wavelettype,
					const SIM_RawField *field,
					int maxpasses = -1);
    void		 waveletInverseTransform(
					UT_Wavelet::WAVELET_NAMES wavelettype,
					const SIM_RawField *wavelet,
					int maxpasses = -1);

    /// Computes the sum of squares of the given level's detail vector.
    void		 waveletComputePSD(const SIM_RawField *wavelet,
					    int level);

    /// Extracts the given component from a packed wavelet array.
    void		 waveletExtractComponent(const SIM_RawField *wavelet,
					    int level,
					    int component);

    /// Builds from either a GEO_PrimVolume or a GEO_PrimVDB.
    /// The provided volidx is the index inside of a vector volume
    /// to use for the building.
    void		 buildFromPrim(const GEO_Primitive *vol,
				      int volidx,
				      const UT_DMatrix4 &xform,
				      fpreal scale);

    /// Builds from a GEO_PrimVolume
    void		 buildFromGeo(const GEO_PrimVolume *vol,
				      const UT_DMatrix4 &xform,
				      fpreal scale);

    /// Compute fractional volume weights representing the amount the voxel
    /// surrounding each sample point is inside the provided SDF.  This method
    /// just subsamples the voxel space according to the samplesperaxis
    /// parameter.


    void computeSDFWeightsSampled(const SIM_RawField *sdf,
                                         int samplesperaxis,
                                         bool invert,
                                         fpreal minweight,
                                         fpreal dilatedist = 0)
    { 
        computeSDFWeightsSampledInternal(sdf,samplesperaxis,invert,
                                            minweight, dilatedist);
    }

private:    
    THREADED_METHOD5(SIM_RawField, shouldMultiThread(),
                     computeSDFWeightsSampledInternal,
                     const SIM_RawField *, sdf,
                     int, samplesperaxis,
                     bool, invert,
                     fpreal, dilatedist,
                     fpreal, minweight);

    void computeSDFWeightsSampledInternalPartial(const SIM_RawField *sdf,
                                         int samplesperaxis,
                                         bool invert,
                                         fpreal minweight,
                                         fpreal dilatedist,
                                         const UT_JobInfo &info);
public:
    /// Compute fractional volume weights representing the amount the voxel
    /// surrounding each sample point is inside the provided SDF.  This method
    /// uses the normalized area of the square that is in the plane given by
    /// the axis parameter as an approximation.
    THREADED_METHOD4(SIM_RawField, shouldMultiThread(),
                     computeSDFWeightsFace,
                     const SIM_RawField *, sdf,
                     int, axis,
                     bool, invert,
                     fpreal, minweight);

    void computeSDFWeightsFacePartial(const SIM_RawField *sdf,
                                      int axis,
                                      bool invert,
                                      fpreal minweight,
                                      const UT_JobInfo &info);

    /// Compute fractional volume weights representing the amount the voxel
    /// surrounding each sample point is inside the provided SDF.  This method
    /// uses accurate volume fractions.
    THREADED_METHOD3(SIM_RawField, shouldMultiThread(),
                     computeSDFWeightsVolumeFraction,
                     const SIM_RawField *, sdf,
                     bool, invert,
                     fpreal, minweight);

    void computeSDFWeightsVolumeFractionPartial(const SIM_RawField *sdf,
                                      bool invert,
                                      fpreal minweight,
                                      const UT_JobInfo &info);


    /// Compute fractional volume weights for the voxel specified by x, y, z.
    /// This method uses the normalized area of the square that is in the
    /// plane given by the axis parameter as an approximation.
    fpreal computeVoxelSDFWeightFace(int x, int y, int z,
                                     const SIM_RawField *sdf,
                                     int axis) const;

    /// Scale this field by s * B / C or set to zero if scaled value is lower than
    /// given threshold
    THREADED_METHOD4(SIM_RawField, shouldMultiThread(),
                     setScaleDivideThreshold,
                     fpreal, scale,
                     const SIM_RawField *, B,
                     const SIM_RawField *, C,
                     fpreal, threshold)
    void    setScaleDivideThresholdPartial(fpreal scale,
                                     const SIM_RawField *B,
                                     const SIM_RawField *C,
                                     fpreal threshold,
                                     const UT_JobInfo &jobinfo);

protected:
    void		 waveletRebuildFromVoxelArray(
				UT_VoxelArrayF *array,
				float scale);


    static fpreal	 applyBoundary(SIM_FieldBoundary bound, fpreal v,
					fpreal boundval);

    THREADED_METHOD2_CONST(SIM_RawField, shouldMultiThread(), reduceOpInternal,
		    fpreal64 *, sum,
		    REDUCE_NAMES, op)
    void	reduceOpInternalPartial(fpreal64 *sum, REDUCE_NAMES op, const UT_JobInfo &info) const;

    THREADED_METHOD5_CONST(SIM_RawField, shouldMultiThread(), reduceMaskedOpInternal,
		    fpreal64 *, sum,
		    fpreal64 *, masktotal,
		    REDUCE_NAMES, op,
		    const SIM_RawField *, mask,
		    bool, maskissdf)
    void	reduceMaskedOpInternalPartial(fpreal64 *sum, fpreal64 *masktotal, REDUCE_NAMES op, const SIM_RawField *maskfield, bool maskissdf, const UT_JobInfo &info) const;

    void		sortAllVoxels(UT_FloatArray &voxelvalues) const;
    void		sortAllVoxelsMasked(UT_FloatArray &voxelvalues, 
					    const SIM_RawField *maskfield, 
					    bool maskissdf) const;

    /// Methods for extrapolation
    THREADED_METHOD8(SIM_RawField, shouldMultiThread(), buildExtrapList,
			UT_Array<UT_ValArray<gu_sdf_qelem *> > &, lists,
			const SIM_RawField *, depths,
			const SIM_RawField *, valid,
			fpreal, isocontour,
			fpreal, dir,
			fpreal, maxdist,
			bool, clamp,
			fpreal, clampval)
    void		 buildExtrapListPartial(
				UT_Array<UT_ValArray<gu_sdf_qelem *> > &lists,
				const SIM_RawField *depths,
				const SIM_RawField *valid,
				fpreal isocontour,
				fpreal dir, fpreal maxdist,
				bool clamp, fpreal clampval,
				const UT_JobInfo &info);

    /// Reduction by reducing each axis in turn.
    /// This *will* change field and dst != field is required.
    void		 localReduceByAxis(REDUCE_NAMES op,
					    UT_VoxelArrayF &dst,
					    UT_VoxelArrayF &field,
					    UT_Vector3 radius) const;

    /// Reduce along a given axis by the specified radius in voxels.
    template <int AXIS, REDUCE_NAMES OP>
    void		 localReduceAxis(UT_VoxelArrayF &dstfield,
					    const UT_VoxelArrayF &field,
					    float radius,
					    const UT_JobInfo &info) const;
    template <int AXIS, REDUCE_NAMES OP>
    void		 localMinMaxAxis(UT_VoxelArrayF &dstfield,
					    const UT_VoxelArrayF &field,
					    float radius,
					    const UT_JobInfo &info) const;

    template <int AXIS>
    void		 localReduceAxisOp(REDUCE_NAMES op,
					    UT_VoxelArrayF &dstfield,
					    UT_VoxelArrayF &field,
					    float radius,
					    const UT_JobInfo &info) const;
    /// Again a triple specialization to engage threading.
    THREADED_METHOD4_CONST(SIM_RawField,
		    field.getTileRes(1)*field.getTileRes(2) > 1,
		    localReduceAxisX,
		    REDUCE_NAMES, op,
		    UT_VoxelArrayF &, dst,
		    UT_VoxelArrayF &, field,
		    float, radius)
    void		localReduceAxisXPartial(
				REDUCE_NAMES op,
				UT_VoxelArrayF &dst,
				UT_VoxelArrayF &field,
				float radius,
				const UT_JobInfo &info) const
    { localReduceAxisOp<0>(op, dst, field, radius, info); }

    THREADED_METHOD4_CONST(SIM_RawField,
		    field.getTileRes(0)*field.getTileRes(2) > 1,
		    localReduceAxisY,
		    REDUCE_NAMES, op,
		    UT_VoxelArrayF &, dst,
		    UT_VoxelArrayF &, field,
		    float, radius)
    void		localReduceAxisYPartial(
				REDUCE_NAMES op,
				UT_VoxelArrayF &dst,
				UT_VoxelArrayF &field,
				float radius,
				const UT_JobInfo &info) const
    { localReduceAxisOp<1>(op, dst, field, radius, info); }

    THREADED_METHOD4_CONST(SIM_RawField,
		    field.getTileRes(0)*field.getTileRes(1) > 1,
		    localReduceAxisZ,
		    REDUCE_NAMES, op,
		    UT_VoxelArrayF &, dst,
		    UT_VoxelArrayF &, field,
		    float, radius)
    void		localReduceAxisZPartial(
				REDUCE_NAMES op,
				UT_VoxelArrayF &dst,
				UT_VoxelArrayF &field,
				float radius,
				const UT_JobInfo &info) const
    { localReduceAxisOp<2>(op, dst, field, radius, info); }

    THREADED_METHOD3(SIM_RawField, shouldMultiThread(), buildFromGeoSampled,
		    const GEO_PrimVolume *, vol,
		    const UT_DMatrix4 &, xform,
		    fpreal, scale)
    void		 buildFromGeoSampledPartial(const GEO_PrimVolume *vol,
						    const UT_DMatrix4 &xform,
						    fpreal scale,
						    const UT_JobInfo &info);

    THREADED_METHOD4(SIM_RawField, shouldMultiThread(), buildFromVDBSampled,
		    const GEO_PrimVDB *, vdb,
		    int, vdbidx,
		    const UT_DMatrix4 &, xform,
		    fpreal, scale)
    void		 buildFromVDBSampledPartial(const GEO_PrimVDB *vdb,
						    int vdbidx,
						    const UT_DMatrix4 &xform,
						    fpreal scale,
						    const UT_JobInfo &info);
    
    SIM_FieldSample		 mySample;

    /// We always have myField at our current resolution.
    /// myGrid only exists if it matches myField.  If myField
    /// changes, myGrid will be destroyed.
    /// If something updates the grid the field is out of date
    /// so is flagged.
    UT_VoxelArrayF		*myField;
    mutable CE_Grid		*myGrid;
    mutable bool		 myFieldOutOfDate;

    // This is the size and offset which converts our UT_VoxelArray into
    // world coordinates.
    UT_Vector3			 myOrig, mySize;
    
    // This is our actual official size.
    UT_Vector3			 myBBoxOrig, myBBoxSize;

    UT_Vector3			 myVoxelSize;
    fpreal			 myVoxelDiameter;

    SIM_FieldBoundary		 myBoundary[3][2];
    fpreal			 myBoundaryValue[3][2];
};


///
/// Allows you to iterate over the voxel cells of a RawField.  This
/// is different than iterating over the voxel samples!
/// The idea of getting or setting the raw value doesn't apply
/// here as we are not necessarily alligned with the sample grid.
///
class SIM_API SIM_RawFieldCellIterator
{
public:
	     SIM_RawFieldCellIterator() 
	     { 
		 myRes[0] = myRes[1] = myRes[2] = 0;
		 rewind(); 
	     }
    virtual ~SIM_RawFieldCellIterator()
	     {
	     }
    
    void	setArray(const SIM_RawField *field)
		{
		    field->getVoxelRes(myRes[0], myRes[1], myRes[2]);
		    rewind();
		}

    void	rewind() 
		{ 
		    myIdx[0] = 0; 
		    myIdx[1] = 0; 
		    myIdx[2] = 0; 
		}

    bool	atEnd() const 
		{ 
		    return myIdx[2] >= myRes[2];
		}

    void	advance() 
		{ 
		    myIdx[0]++;
		    if (myIdx[0] >= myRes[0])
		    {
			myIdx[0] = 0;
			myIdx[1]++;
			if (myIdx[1] >= myRes[1])
			{
			    myIdx[1] = 0;
			    myIdx[2]++;
			    if (myIdx[2] >= myRes[2])
			    {
				// All done!
				myIdx[2] = myRes[2];
			    }
			}
		    }
		}

    int		x() const { return myIdx[0]; }
    int		y() const { return myIdx[1]; }
    int		z() const { return myIdx[2]; }
    int		idx(int axis) const { return myIdx[axis]; }
    
    /// Returns true if we are at the start of a new tile.
    /// Since we don't work on tile basis, we define a single
    /// x pass to be a tile.
    bool	isStartOfTile() const
		{
		    return myIdx[0] == 0;
		}

protected:
    int					 myIdx[3];
    int					 myRes[3];
};

//
// These macros create the CALL_VOXELPROBE macro
//
// CALL_VOXELPROBE(const SIM_RawField *src, 
//		   SIM_RawField *dst, 
//		   float defaultvalue,
//		   callFunc(dst, theprobe))
//
// callFunc should be
//
// template <typename P>
// void
// callFunc(SIM_RawField *dst, P &probe)
//
// dst->isMatching(src) must be true.
//
// The callFunc will receive in the parameter "theprobe" a
// UT_VoxelProbeAverage properly templated against the difference
// between the source and dest fields.
// theprobe.setIndex(x, y, z);
// will cause theprobe.getValue() to return the interpolated value
// in source of the destination's voxel sample (x, y, z).  This takes
// into account the different sampling patterns.
//
#define CALL_PROBEXY(XSTEP, YSTEP, src, dst, command)	\
{							\
    if ((dstsample ^ srcsample) & 4)			\
    {							\
	if (dstsample & 4)	/* ZStep -1 */		\
	{						\
	    UT_VoxelProbeAverage<fpreal32, XSTEP, YSTEP, -1> theprobe;	\
	    theprobe.setArray(src->field());		\
	    command;					\
	}						\
	else			/* ZStep 1 */		\
	{						\
	    UT_VoxelProbeAverage<fpreal32, XSTEP, YSTEP, 1> theprobe;	\
	    theprobe.setArray(src->field());		\
	    command;					\
	}						\
    }							\
    else			/* ZStep 0 */		\
    {							\
	UT_VoxelProbeAverage<fpreal32, XSTEP, YSTEP, 0> theprobe;	\
	theprobe.setArray(src->field());		\
	command;					\
    }							\
}

#define CALL_PROBEX(XSTEP, src, dst, command)		\
{							\
    if ((dstsample ^ srcsample) & 2)			\
    {							\
	if (dstsample & 2)	/* YStep -1 */		\
	{						\
	    CALL_PROBEXY(XSTEP, -1, src, dst, command);	\
	}						\
	else			/* YStep 1 */		\
	{						\
	    CALL_PROBEXY(XSTEP, 1, src, dst, command);	\
	}						\
    }							\
    else			/* YStep 0 */		\
    {							\
	CALL_PROBEXY(XSTEP, 0, src, dst, command);	\
    }							\
}

#define CALL_VOXELPROBE(src, dst, defval, command) 	\
{							\
    if (!src)						\
    {							\
	UT_VoxelProbeConstant<fpreal32> theprobe;	\
	theprobe.setValue(defval);			\
	command;					\
    }							\
    else						\
  {							\
    int		dstsample, srcsample;			\
    dstsample = dst->getSample();			\
    srcsample = src->getSample();			\
    if ((dstsample ^ srcsample) & 1)			\
    {							\
	if (dstsample & 1)	/* XStep -1 */		\
	{						\
	    CALL_PROBEX(-1, src, dst, command);		\
	}						\
	else			/* XStep 1 */		\
	{						\
	    CALL_PROBEX(1, src, dst, command);		\
	}						\
    }							\
    else			/* XStep 0 */		\
    {							\
	CALL_PROBEX(0, src, dst, command);		\
    }							\
  }							\
}

#endif
