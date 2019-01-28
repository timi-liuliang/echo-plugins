/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Ray intersection for GU library (not necessarily as fast as
 *		RAY library)
 *
 */

#ifndef __GU_RayIntersect_h__
#define __GU_RayIntersect_h__

#include "GU_API.h"
#include <GEO/GEO_PrimitiveP.h>
#include <UT/UT_Defines.h>
#include <UT/UT_Array.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_ThreadSpecificValue.h>
#include <TS/TS_Expression.h>

class GA_PrimitiveGroup;
class GA_Range;
namespace GU { class BVH; }
class GU_Detail;
class guRayTree;
class GEO_Primitive;
class GU_RayPrimInfo;
class GU_IsectCurveSet;

enum GU_RayFindType
{
    GU_FIND_CLOSEST	= 0,
    GU_FIND_FURTHEST	= 1,
    GU_FIND_ALL		= 2
};

class GU_API GU_RaySerial
{
public:
    GU_RaySerial()
    {
	mySerial = 1;
    }

    ~GU_RaySerial()
    {
    }

    void	bumpSerial()
    {
	mySerial++;
	if (mySerial < 0)
	{
	    // We wrapped, reset.
	    myList.constant(0);
	    mySerial = 1;
	}
    }

    int		getSerial() const
    {
	return mySerial;
    }

    // Tests if the given index has been seen in this pass.  If not,
    // returns true as it is the first time.
    // In any case, the primitive will now be marked up to date.
    bool	firstTimeWithReset(int idx)
    {
	if (myList(idx) == mySerial)
	    return false;

	myList(idx) = mySerial;
	return true;
    }
    // Returns if this has not been reset on this pass, but does
    // not affect its state.
    bool	firstTime(int idx) const
    {
	if (myList(idx) == mySerial)
	    return false;
	return true;
    }
    // Resets this idx to be marked as seen.
    void	reset(int idx)
    {
	myList(idx) = mySerial;
    }

    void	ensureListExists(int length)
    {
	if (myList.entries() != length)
	{
	    myList.entries(length);
	    myList.constant(0);
	}
    }

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myList.getMemoryUsage(false);
        return mem;
    }

private:
    UT_IntArray		 myList;
    int			 mySerial;
};

class GU_API GU_RayInfoHit
{
public:
    int operator==(const GU_RayInfoHit &x) const
    {
	return (t==x.t && u == x.u && v == x.v && w == x.w && d2 == x.d2 && prim == x.prim);
    }
    float t, u, v, w, d2;  // Intersection point.  May add cache here.
    GEO_ConstPrimitiveP prim;
};

class GU_API GU_RayInfoCache
{
public:
    GU_RayInfoCache( )
    {
	valid = false;
	priminfo = 0;
	data = 0;
    }

    GU_RayPrimInfo 	*priminfo;
    void 		*data;
    bool		valid;
};

class GU_API GU_RayInfo
{
public:
    GU_RayInfo(float max = 1E18f, float min=0.0f,
		GU_RayFindType type = GU_FIND_CLOSEST, float tol = 1e-1F)
    {
	myHitList = 0;
	myCurveSet = 0;
	myTtol = 1E-2f;		// Points closer than this in "t" will not
				//	be double counted.
	myIgnoreTrims = true;	// Do we pay attention to trimmed out regions?
	myUseAlgebraic = false;	// Use algebraic pruning where available?
	myIgnoreMeta = false;	// Include metaballs.
	myExpandPolygon = false;
	myTValid = false;
	myDomainTol = 1E-2f;
	mySteps = 100;
	myMaxHits = 0;
	myTimeChange = 0.0F;
	myUnitTimeOffset = 0.0F;
	myUnitTimeUpperTol = 0.0F;
	mySerialList = 0;
	init(max, min, type, tol);
    }
    ~GU_RayInfo();

    GU_RaySerial	*getSerialList() const { return mySerialList; }
    void		setSerialList(GU_RaySerial *seriallist)
			{
			    mySerialList = seriallist;
			}
    bool		firstTime(int serialidx) const
			{ return mySerialList->firstTime(serialidx); }
    bool		firstTimeWithReset(int serialidx) const
			{ return mySerialList->firstTimeWithReset(serialidx); }
    void		resetSerial(int serialidx) 
			{ mySerialList->reset(serialidx); }

    void		setFindType(GU_RayFindType type);
    void		init(float max = 1E18f, float min=0.0f,
				GU_RayFindType type = GU_FIND_CLOSEST,
				float tolerance = 1e-1F, int ignoretrims=1,
				int usealgebraic = 0);

    bool		testHit(float t)
			{
			    return (t >= myTmin && t <= myTmax);
			}

    bool		insertHit(float nt, float nu, float nv, float nw,
				  float nd2, GEO_ConstPrimitiveP prim);

    void		addCurveSet(GU_IsectCurveSet &curveset);

    void		invalidateCache() { myCache.valid = false; }
    bool		isCacheValid()	  { return( myCache.valid ); }

    GU_RayFindType	getFindType() const	{ return myFindType; }

    void		reset();

    GEO_ConstPrimitiveP		 myPrim;
    UT_Vector3			 myNml;
    float			 myTmin;
    float			 myTmax;
    float			 myT;
    float 			 myU, myV, myW;	// Hit position on primitive
    float			 myD2;		// Squared error.
    float			 myTol;		// Max error to consider a hit.
    float			 myTtol;	// min dist on ray between
						// two points
    float			 myDomainTol;	// Tolerance in domain
    int				 mySteps;	// Marching size
    bool			 myIgnoreTrims;	// Pay attention to trim loops?
    bool			 myUseAlgebraic;// Use algebraic pruning?
    bool			 myExpandPolygon; // If true, you can have 
					// hits outside of 0,1 on quads.  
					// Prevents leaks with non-planar.
    bool			 myIgnoreMeta;	// Ignore metaballs.
    bool			 myTValid;	// Is myT set to a legit hit?
    int				 myMaxHits;	// Number of hits to stop 
						// searching at, if 0, is 
						// infinite.
    GU_RayInfoCache		 myCache;	// Last successful hit.
    UT_Array<GU_RayInfoHit>	*myHitList;
    GU_IsectCurveSet		*myCurveSet;	// Set of isect curves.

    float			 myTimeChange;   //Only for moving collision
    float			 myUnitTimeOffset;//Only for moving collision
    float			 myUnitTimeUpperTol; //Only for moving collision
private:
    GU_RayFindType		 myFindType;	// Only way to set is thru init
    GU_RaySerial		*mySerialList;	// Cache of serial numbers
};

// This is the version of hitinfo used for minimum finding.
// It is similar, but sufficiently different to have its own
// structure.  In particular, more u/v values are needed and
// caching is not.
// NB: All distances are actually distance squareds.
// As soon as a distance smaller than dmin is found, the minimum
// finder will terminate with that value.  This allows early
// exit with collison detection.
class GU_API GU_MinInfo
{
public:
    GU_MinInfo(float max = 1E18F, float min=1e-14F, int naccurate = 1)
    {
	dmin = min;
	dmax = max;
	accurate = naccurate;
	u1 = v1 = w1 = u2 = v2 = w2 = 0.0f;
	prim = 0;
	mySerialList = 0;
    }

    GU_RaySerial	*getSerialList() const { return mySerialList; }
    void		setSerialList(GU_RaySerial *seriallist)
			{
			    mySerialList = seriallist;
			}
    bool		firstTime(int serialidx) const
			{ return mySerialList->firstTime(serialidx); }
    bool		firstTimeWithReset(int serialidx) const
			{ return mySerialList->firstTimeWithReset(serialidx); }
    void		resetSerial(int serialidx) 
			{ mySerialList->reset(serialidx); }

    void		init(float max = 1E18F, float min=1e-14F)
			{
			    dmin = min;
			    dmax = max;
			}

    bool		addMin(float dist)
			{
			    if (dist >= dmax)
                                return false;

			    // Only closest makes sense with minimum
			    dmax = dist;
			    return true;
			}

    void		insertMin(float nd2, float nu1, float nv1, float nw1,
				  float nu2, float nv2, float nw2,
				  GEO_ConstPrimitiveP p)
			{
			    d = nd2;	// Recall: Distance squared.
			    u1 = nu1;
			    v1 = nv1;
			    w1 = nw1;
			    u2 = nu2;
			    v2 = nv2;
			    w2 = nw2;
			    prim = p;
			}

    void		swapOrder()
			{
			    float tmp;

			    tmp = u1;
			    u1 = u2;
			    u2 = tmp;

			    tmp = v1;
			    v1 = v2;
			    v2 = tmp;

			    tmp = w1;
			    w1 = w2;
			    w2 = tmp;
			}

    int operator==( const GU_MinInfo &other ) const
    {
	return(    dmin == other.dmin
		&& dmax == other.dmax
		&& d == other.d
		&& u1 == other.u1
		&& v1 == other.v1
		&& w1 == other.w1
		&& u2 == other.u2
		&& v2 == other.v2
		&& w2 == other.w2
		&& accurate == other.accurate
		&& prim == other.prim );
    }

public:
    float		dmin, dmax;	// Minimum & maximum squared distances.
    float		d;		// Current squared distance.
    float		u1, v1, w1;	// Current minimum on source.
    float		u2, v2, w2;	// Current minimum on dest.
    int			accurate;	// Do we polish sol'n?
    GEO_ConstPrimitiveP	prim;		// What we are close to.

private:
    GU_RaySerial	*mySerialList;
};

class GU_API GU_ProximityHelper
{
public:
    GU_ProximityHelper()	    {}
    virtual ~GU_ProximityHelper()   {}

    virtual float   getMaxRadius( int prim_num ) const = 0;
    virtual float   getRadius( int prim_num,
			       float u_real, float v_real ) const = 0;
};

class GU_API GU_RayIntersect
{
public:
     GU_RayIntersect();

     // Flag 'picking' should be set to 1.  When set to 0, curves and
     //   surfaces will be polygonalized.  Polygonalized curves have
     //   a different parameterization than the original curve, so the
     //   u coordinates will no longer be valid if picking == 0.
     // Flag 'polylines' indicates to build polylines instead of polygons.
     //   Normally, any closed polygon is treated as a surface.  If polyline
     //   is set to 1, all polygons are treated as wires.
     // Flag 'harden' indicates to build hard geometry - ie: the cached values
     //   will not update with those of the GDP.  This is usually a good thing,
     //   except it requires more memory.
     //   This also means that NURBs and Bezier surfaces will be changed
     //   into meshes, which may result in a faster intersect.
     // Flag 'usevisibility' indicates that it will look for a 3d visibility
     //   group.  If one is found, primitives will only be used if they
     //   are in that group.
     // Flag 'solidtet' will create solid tetrahedrons for a better minimum
     //   distance behaviour.
     GU_RayIntersect(const GU_Detail *gdp, const GA_PrimitiveGroup *group = nullptr, 
		     bool picking = false, bool polyline = false, bool harden = false,
		     bool usevisibility = false,
		     bool solidtet = false);

     // assume picking for a single primitive
     GU_RayIntersect(const GU_Detail *gdp, const GEO_Primitive *prim,
		     bool polyline = false, bool solidtet=false);

     // This version of the constructor is used for a special case intersector
     // against moving geometry.  Currently it only supports triangles.
     // Unlike the standard rays that are cast against the geometry, when
     // using this version of the intersector, the ray has to have a "speed"
     // associated with it.
     GU_RayIntersect(const GU_Detail *gdp0, const GU_Detail *gdp1);

     // This version of the constructor is like the one that takes a group
     // except the group is from a different gdp that has the same topology
     // (ie. prim counts are the sames). Note that if usevisibility is true,
     // then the visibility from the gdp is used (NOT limit_gdp).
     GU_RayIntersect(const GU_Detail *gdp,
		     const GU_Detail *limit_gdp,
		     const GA_PrimitiveGroup *vis_group,
		     bool picking = false, bool polyline = false, bool harden = false,
		     bool usevisibility = false);

    ~GU_RayIntersect();

    // Returns whether the supplied detail can be used with the deforming
    // geometry code (i.e. it is all triangles).
    static bool	validForDeformingGeometry(const GU_Detail *gdp0,
					  const GU_Detail *gdp1);

    void	init(const GU_Detail *gdp, const GA_PrimitiveGroup *group = nullptr,
		     bool picking = false, bool polyline = false, bool harden = false,
                     bool usevisibility = false, bool solidtet = false);
    void	init(const GU_Detail *gdp, const GEO_Primitive *prim,
		     bool polyline = false, bool solidtet = false);
    void	init(const GU_Detail *gdp,
		     const GU_Detail *limit_gdp,
		     const GA_PrimitiveGroup *limit_group, 
		     bool picking = false, bool polyline = false, bool harden = false,
		     bool usevisibility = false);

    bool        init() const { return myTree != 0; }

    // Reset to the default constructed state.
    void	clear();

    // Returns whether this instance is up-to-date for the specified
    // detail or whether it needs to be rebuilt.
    bool	validForDetail(const GU_Detail *gdp) const;

    const GU_Detail *
		detail() const { return myGdp; }

    // Send a ray into the gdp between tmin & tmax.  Returns 0 if miss,
    //	or the number of hits.  
    // If hit, the information for the hit is returned in
    //	the hitinfo.
    // Returns -1 if intersection was interrupted.
    int		sendRay(const UT_Vector3 &org, const UT_Vector3 &dir,
			GU_RayInfo &hitinfo) const;
    // Find minimum distance between point and gdp between tmin and tmax.
    // Note t = square of distance between point & primitive, so tmin
    // and tmax should be squares of the distance as desired.
    int		minimumPoint(const UT_Vector3 &p, GU_MinInfo &mininfo,
			     const GEO_Primitive *prim = 0) const;
    // Find all primitives in the gdp that are within a given radius of the
    // given point. Returns a list with u,v values sorted by primitive id in
    // hit_list.
    // If helper is non-NULL, then instead of using radius, we use it to
    // determine our radius.
    int		proximityPoint( UT_Array<GU_MinInfo> &hit_list,
				const UT_Vector3 &p,
				bool normalize_weights,
				const GU_ProximityHelper *helper,
				float radius = 0.0f ) const;

    // Intersects with a rayprimitive, used internally primarily:
    // Note it operates in opposite direction of intersectCaches!
    int		intersectPrim(const GEO_Detail &prim_geo,
			      GU_RayPrimInfo &prim, GU_RayInfo &hitinfo);
    
    // Intersect two raycaches of primitives.  Returns number of hits
    // found.  Only defined for some primitive types (Curves and surfaces)
    int		intersectCaches(GU_RayIntersect &intersect,
				GU_RayInfo &hitinfo);
    // Find minimum between two raycaches of primitives.  Returns 1 if
    // minimum found.  t value is square of distance between them, as
    // is d2.
    int		minimumCaches(const GU_RayIntersect &intersect,
			      GU_MinInfo &mininfo) const;

    int         isInside(const UT_Vector3 &pt, bool ignoretrim = true,
			  float tol=0.1) const;

    int		isInsideWinding(const UT_Vector3 &pt, bool ignoretrim = true,
				float tol = 0.1) const;

    /// Returns true if there are no primitives to intersect against.
    bool isEmpty() const;

    // Gets this thread's serial list.
    GU_RaySerial			*getSerialList() const;

    /// Report approximate memory usage.
    int64 getMemoryUsage(bool inclusive) const;

    // GU_RayIntersect usually returns real coordinates, not unit coordinates.
    // Usually you want unit coordinates so evaluateInteriorPoint can
    // be used.  This handles the conversion.  Usually it is a case
    // of realToUnitPair, but there are a few exceptions such as open
    // polylines.
    static void	fixBrokenUVs(const GEO_Primitive *prim, float &u, float &v);

protected:
    void	destroyTrees();

private:
    void	initDetailTracking(const GU_Detail *gdp);
    void	initMetaExpr(const GU_Detail *gdp,
                             const GA_Range &primrange);

    int         determineState(const UT_Vector3 &pt,
			       GEO_ConstPrimitiveP &prim,
			       int n, uint seed = 1, bool ignoretrim = true,
			       float tol = 0.1) const;
    int         determineStateWinding(const UT_Vector3 &pt,
			       GEO_ConstPrimitiveP &prim,
			       int n, uint seed = 1, bool ignoretrim = true,
			       float tol = 0.1) const;

private:
    const GU_Detail *myGdp;

    guRayTree	*myTree;
    mutable UT_ThreadSpecificValue<GU_RaySerial *>	mySerialList;

    /// NOTE: We can't have this as a direct member instead of a pointer,
    ///       because including GU_BVH.h without including GEO_BVHImpl.h
    ///       seems to cause compile errors, due to the compiler trying to
    ///       instantiate all of the outlined functions from GEO::BVH,
    ///       even though they aren't called in GU_BVH.h.
    ///       I tried to solve this with "extern template" statements,
    ///       but the compiler seemed to ignore that.
    UT_UniquePtr<GU::BVH> myBVH;

    /// This is only used for density() calls.
    TS_MetaExpressionPtr	myMetaExpr;

    // Keep track of some details about the detail we were initialized
    // with so users can query whether we're still valid.
    int		 myDetailUniqueId;
    int		 myDetailCacheCount;
};

#endif
