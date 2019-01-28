/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VGEO_Volume.h ( VGEO Library, C++)
 *
 * COMMENTS:	Procedurally defined volumetric rendering data
 */

#ifndef __VGEO_Volume__
#define __VGEO_Volume__

#include "VGEO_API.h"
#include <UT/UT_VectorTypes.h>
#include <UT/UT_BoundingBox.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_AtomicInt.h>

template <class T> class UT_Array;
template <bool> class VGEO_KDTree;

// We have to include voxel array as we can't forward declare
// UT_VoxelArrayF, I think.
#include <UT/UT_VoxelArray.h>
#include <UT/UT_IntrusivePtr.h>
#include <GA/GA_Types.h>

class UT_StringArray;
class UT_Filter;
class VEX_LateBinding;
class GEO_Primitive;
class GEO_Vertex;
class GU_Detail;
class VGEO_HitList;
class VGEO_RayState;
class VGEO_ShadeObject;
class VGEO_Ray;

/// Base class for volume primitives in mantra
class VGEO_API VGEO_Volume
    : public UT_IntrusiveRefCounter<VGEO_Volume>
{
public:
	     VGEO_Volume();
    virtual ~VGEO_Volume();

    /// @{
    /// Return the geometric primitive or vertex associated with the
    /// volume.  These can be used to fill in primitive and detail
    /// attributes associated with the volume.
    virtual GA_Offset		 getGeoPrimitive() const
				 { return GA_INVALID_OFFSET; }
    virtual GA_Offset		 getGeoVertex() const
				 { return GA_INVALID_OFFSET; }
    /// @}

    /// Return the object space step size to use when the volume quality is
    /// 1 and there is no object-level scale.  For primitives based on
    /// voxel grids, this method is usually implemented by returning the
    /// voxel size.
    virtual float	 getNativeStepSize() const = 0;
    
    /// Return a set of bounding boxes that define the structure of the
    /// volumetric data.  The boxes are used to accelerate ray intersection
    /// testing with the volume and to cull empty space for micropolygon
    /// rendering.  They need not be an exact representation of the volume,
    /// though more accurate boxes will lead to more efficient renders.
    /// Specifically, the union of the boxes returned by this method should
    /// fully enclose the region of the volume to be rendered.  If there is
    /// volumetric data outside all boxes, the render may contain
    /// artifacts.
    ///
    /// Boxes should be defined locally, and need to incorporate the
    /// displacement bound.
    /// @param radius Filter radius in voxels
    /// @param dbound Displacement bound in local space
    /// @param zerothreshold Threshold value under which the volume is considered empty.
    virtual void	 getBoxes(UT_Array<UT_BoundingBox> &boxes,
				  float radius,
				  float dbound,
				  float zerothreshold) const = 0;

    /// Return a list of attributes present in this volume.  These
    /// attributes will be bound to surface shader variables when the
    /// volume is rendered.  The "idx" passed into evaluation routines will
    /// be the array index returned by this operation.
    /// - sizes: number of floats in each attribute
    virtual void	 getAttributeBinding(UT_StringArray &names,
					     UT_IntArray &sizes) const = 0;

    /// Evaluate a volume attribute at a given position.
    /// @param pos Evaluation position
    /// @param filter Filter function
    /// @param radius Filter radius in @b voxels
    /// @param time Evaluation time (0-1)
    /// @param idx Attribute index (based on getAttributeBinding())
    /// @param data Storage for evaluation data
    /// @see UT_Filter
    virtual void	 evaluate(const UT_Vector3 &pos,
		    		  const UT_Filter &filter,
				  float radius, float time,
				  int idx, float *data) const = 0;

    /// Evaluate multiple volume attribute at given positions.
    /// @param pos Evaluation positions
    /// @param filter Filter function
    /// @param radius Filter radius in @b voxels
    /// @param time Evaluation times (0-1)
    /// @param idx Attribute index (based on getAttributeBinding())
    /// @param data Storage for evaluation data
    /// @param size Number of positions to evaluate
    /// @param stride Data size for an individual evaluation, this should match the size returned by getAttributeBinding() for the given index.
    /// @see UT_Filter
    virtual void	 evaluateMulti(const UT_Vector3 *pos,
				       const UT_Filter &filter,
				       float radius,
				       const float *time,
				       int idx, float *data,
				       int size, int stride) const;

    /// Evaluate the minimum and maximum values for an attribute within a
    /// given box.  If this operation is not supported, the caller will
    /// assume that the interval is unbounded.
    /// @param box Evaluation interval
    /// @param filter Filter function
    /// @param radius Filter radius in @b voxels
    /// @param time Evaluation time (0-1)
    /// @param idx Attribute index (based on getAttributeBinding())
    /// @param minval Storage for evaluation data (minimum over box)
    /// @param maxval Storage for evaluation data (maximum over box)
    /// @see UT_Filter
    virtual bool	 evaluateInterval(const UT_BoundingBox &box,
					  const UT_Filter &filter,
					  float radius,
					  float time, int idx,
					  float *minval,
					  float *maxval) const
			 {
			     return false;
			 }

    /// A class to transfer ray intersection hit information back to the
    /// renderer from ray tracing callbacks.  The VGEO_HitList,
    /// VGEO_RayState, and VGEO_ShadeObject classes are internal to mantra
    /// and are only accessible to plugins as forward declarations.
    class HitList {
    public:
	/// Constructor (used internally)
	HitList(VGEO_HitList &hits,
		VGEO_RayState &state,
		VGEO_ShadeObject *prim,
		float zoff,
		float tmin,
		float tmax,
		float stepsize)
	    : myHits(hits)
	    , myState(state)
	    , myPrim(prim)
	    , myZOff(zoff)
	    , myTMin(tmin)
	    , myTMax(tmax)
	    , myStepSize(stepsize) {}

	/// Return the minimum ray parameter for hit insertion
	float	tmin() const		{ return myTMin; }
	/// Return the maximum ray parameter for hit insertion. When
	/// t >= tmax(), intersection tests can be terminated.
	float	tmax() const		{ return myTMax; }
	/// Return the stratified sample offset for this hit list. This
	/// value is premultiplied by stepsize().
	float	zoff() const		{ return myZOff; }
	/// Return the volume step size for ray marching algorithms
	float	stepsize() const	{ return myStepSize; }

	/// Insert a ray intersection hit. This method may decrease the
	/// value returned by tmax().
	/// @param t Ray parameter for the intersection
	/// @param pos Intersection position in local space
	/// @param backface Whether the hit is backfacing
	void	addHit(float t, const UT_Vector3 &pos, bool backface);

	/// Dump the hist
	void	dump() const;

	/// Size of the hit list
	exint	entries() const;

    private:
	VGEO_HitList		&myHits;
	VGEO_RayState		&myState;
	VGEO_ShadeObject	*myPrim;
	float			 myZOff;
	float			 myTMin;
	float			 myTMax;
	float			 myStepSize;
    };

    /// Find the intersection of a ray against the isosurface defined by a
    /// scalar field.  The default implementation uses ray marching and the
    /// evaluateMulti() method to find a zero-crossing via sampling.
    /// @param ray Ray for intersection testing
    /// @param filter Filter function
    /// @param radius Filter radius in @b voxels
    /// @param time Evaluation time (0-1)
    /// @param idx Attribute index (based on getAttributeBinding())
    /// @param threshold Isosurface threshold value
    /// @param hits Callback object for insertion of ray intersection results
    virtual void	 intersectIsosurface(const VGEO_Ray &ray,
					     const UT_Filter &filter,
					     float radius,
					     float time,
					     int idx,
					     float threshold,
					     HitList &hits) const;

    /// Perform ray marching through a volume.  The default implementation
    /// finds occupied intervals of the volume based on the boxes returned
    /// by getBoxes(), and inserts evenly spaced samples within these
    /// intervals.
    /// @param ray Ray for intersection testing
    /// @param hits Callback object for insertion of ray intersection results
    virtual void	 intersectVolume(const VGEO_Ray &ray,
					 HitList &hits) const;

    /// Evaluate the gradient of a volume attribute at a given position.
    /// @param pos Evaluation position
    /// @param filter Filter function
    /// @param radius Filter radius in @b voxels
    /// @param time Evaluation time (0-1)
    /// @param idx Attribute index (based on getAttributeBinding())
    /// @see UT_Filter
    virtual UT_Vector3	 gradient(const UT_Vector3 &pos,
		    		  const UT_Filter &filter,
				  float radius, float time,
				  int idx) const;

    /// Returns a local distance for use in default gradient calculations.
    virtual float	 getGradientDelta() const	{ return 1e-3F; }

    /// Returns the volume attribute index to be used for normal vector
    /// calculations.
    virtual int		 getNormalAttribute() const	{ return 0; }

    /// Returns the voxel size in the same space as 'pos' in evaluate.
    virtual float	 getVoxelSize() const		{ return 0; }

    /// Returns true when the volume data should be interpreted as an SDF
    virtual bool	 isSDF() const			{ return false; }

    /// Utility class to perform numerically safe ray marching
    class DiscreteRayMarcher {
    public:
	DiscreteRayMarcher(float stepsize, float zoff)
	    : myStepSize(stepsize)
	    , myZOff(zoff) {}

	// tmin must be >= 0 for this to work correctly
	float init(float tmin)
	{
	    UT_ASSERT(tmin >= 0);
	    mySteps = exint((myStepSize - myZOff + tmin) / myStepSize);
	    float tval = myZOff + mySteps*myStepSize;

	    if (tval <= tmin)
	    {
		mySteps++;
		tval = myZOff + mySteps*myStepSize;
	    }

	    // If tval < tmin, the volume is invalid (i.e. 0 sized?)
	    UT_ASSERT(tval > tmin && "0 sized volume?");
	    return tval;
	}

	float step()     { mySteps++; return myZOff + mySteps*myStepSize; }
	float stepback() { mySteps--; return myZOff + mySteps*myStepSize; }

    private:
	float	myStepSize;
	float	myZOff;
	exint	mySteps;
    };

public:
    /// @private Function used internally to get an octree
    VGEO_KDTree<false>	*getTree(float radius, float zerothreshold);
    /// @private Function used internally to get displaced octree
    VGEO_KDTree<false>	*getDisplaceTree(float radius,
					 float dbound,
					 float zerothreshold);

    /// @private Function used internally to create a late binding
    void		 fillBinding(VEX_LateBinding &bind) const;

    /// Utility method to create bounding boxes for every nonzero voxel in
    /// the voxel array.
    static void		 addVoxelBoxes(const UT_VoxelArrayF &voxels,
				       UT_Array<UT_BoundingBox> &boxes,
				       float radius,
				       float dbound,
				       float zerothreshold);

    /// Utility method to create an isosurface from a voxel array
    static GU_Detail	*getVoxelIsosurface(const UT_VoxelArrayF &voxels,
				       float radius,
				       float dbound,
				       float zerothreshold);

private:
    VGEO_KDTree<false>	*myTree;
    VGEO_KDTree<false>	*myDisplaceTree;
};

using VGEO_VolumePtr = UT_IntrusivePtr<VGEO_Volume>;

#endif
