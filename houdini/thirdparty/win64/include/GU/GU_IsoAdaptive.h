/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_IsoAdaptive.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_IsoAdaptive__
#define __GU_IsoAdaptive__

#include "GU_API.h"
#include <GEO/GEO_PrimList.h>
#include <UT/UT_Array.h>
#include <UT/UT_FloatArray.h>

class GU_Detail;
class GEO_PrimVolume;
class GEO_PrimPoly;

class gu_isotrisoup;
class gu_isotri;
class gu_isopt;
class gu_isoedge;

///
/// This sampler base class is used by the adaptive sampler
/// to query the implicit function.  Because some operations may
/// be accelerated with special knowledge of your implicit function,
/// some higher level tools are present here as virtuals.
///
class GU_API GU_IsoAdaptiveSampler
{
public:
    GU_IsoAdaptiveSampler();
    virtual ~GU_IsoAdaptiveSampler() {}

    /// Returns the value of the implicit function at a given point
    /// negative is inside, positive outside.
    virtual float sample(const UT_Vector3 &pos) = 0;

    /// Computes the gradient of the implicit function
    virtual UT_Vector3 gradient(const UT_Vector3 &pos) = 0;

    /// Settles the given point onto the iso surface.  The base class
    /// does a search along the normal until an opposite signed
    /// value is found, then a brent search is done from there.
    /// Returns false if failed to settle
    virtual bool 	settle(UT_Vector3 &pos, float tol);

    /// Does a hinge settle.  
    /// First we run a normal settle, but if it fails or is too far
    /// we switch to a search on the circle
    /// described by the direction, normal, and distance.
    virtual bool	hingeSettle(UT_Vector3 &result, UT_Vector3 center, UT_Vector3 stepdir, UT_Vector3 normal, float stepsize, float tol);

    /// Reports the ideal step size for any location in space.
    float		stepsize(const UT_Vector3 &pos);

    /// The subclass stepsize will be multiplied by the scale and clamped
    /// to the given min/max.
    void		setStepScale(float minstep, float maxstep, float stepscale);

    /// Overloads the curvature based step calcuation with another sampler,
    /// it will invoke the given sampler to determine the step size.
    void		setStepSizeSampler(GU_IsoAdaptiveSampler *sampler);

    /// To compute the curvature the gradient is sampled at nearby
    /// points.  This controls the distance to do the sampling.
    /// Default is 0.01
    void		setCurvatureTestDist(float querydist);


    /// Returns true if pos is inside the active region.
    virtual bool	isInside(const UT_Vector3 &pos);

protected:
    /// Computes the ideal step size at this location.  Step
    /// size should be a smooth function over space.  It must be fine
    /// enough to capture any sudden changes in direction or thin
    /// connections.
    /// The default will use a curvature computation by computing
    /// the gradient at small offsets in the non-normal directions.
    /// The result is scaled by the global stepsize scale and clamped
    /// by the min/max step sizes.
    virtual float	stepsizeSubclass(const UT_Vector3 &pos);

    float		myMinStep, myMaxStep, myStepScale;
    float		myQueryDist;

    GU_IsoAdaptiveSampler	*myStepSizeSampler;
};

///
///  This sampler works with a hierarchy of SDF volumes defined by
/// GEO_PrimVolume
///
class gu_VolumeHierarchy;

class GU_API GU_IsoAdaptiveSamplerVolume : public GU_IsoAdaptiveSampler
{
public:
    GU_IsoAdaptiveSamplerVolume(const UT_Array<const GEO_PrimVolume *> &vollist, float offset, bool invert, float overlap, bool usesmallest, bool strict);
    virtual ~GU_IsoAdaptiveSamplerVolume();

    virtual float	sample(const UT_Vector3 &pos);
    virtual UT_Vector3	gradient(const UT_Vector3 &pos);

    /// Returns true if pos is inside the active region.
    virtual bool	isInside(const UT_Vector3 &pos);
protected:
    gu_VolumeHierarchy	*myHierarchy;
    const GEO_PrimVolume	*myRefVolume;
    float		myOffset;
    // -1 for flipping, 1 for no flip.
    float		mySide;
    // If true, only the finest resolution is used.
    bool		myUseSmallest;
    // If true, use a strict is inside on the volumes.
    bool		myStrict;
};


///
/// GU_IsoAdaptive is a verb class to do an adaptive surfacing of
/// an iso surface.
///
class GU_API GU_IsoAdaptive
{
public:
    GU_IsoAdaptive();
    virtual ~GU_IsoAdaptive();

    void	isosurface(GU_Detail *gdp,
			    GU_IsoAdaptiveSampler &sampler,
			    UT_Vector3 seedpos,
			    float tol,
			    // Maximum change in step size per triangle,
			    // ie, 0.5 to insist new triangle is between
			    // 0.5 and 2 distance.
			    float maxchange,
			    bool closegaps,
			    bool flipedges,
			    bool computenml,
			    bool stopatvolume);
protected:
    /// Searchs outwards from seedpos to build an equaliteral triangle
    /// settled to the surface
    gu_isotri	*buildSeedTri(UT_Vector3 seedpos);

    /// Given an edge grows it into an equaliteral triangle scaled
    /// to the surface.
    /// Returns false if fails, and does nothing then.
    bool	 growEdge(gu_isoedge *edge, bool stopatvolume);

    /// Checks if the given point is within an overlap of another
    /// triangle.  The maxedge length of the triangles in question
    /// will be scaled by the searchscale
    bool	 checkOverlap(gu_isotri *ntri, UT_Vector3 testpos,
			      gu_isotri *ignore, float searchscale);

    /// Looks at this edge and its successive edge to see if
    /// they are close enough together to form an ear to cut off.
    bool	 cutEar(gu_isoedge *edge);
    

private:
    gu_isotrisoup	*mySoup;
    GU_IsoAdaptiveSampler	*mySampler;
    float		 myTol;
    float		 myMaxShrink, myMaxGrow;
};


#endif
