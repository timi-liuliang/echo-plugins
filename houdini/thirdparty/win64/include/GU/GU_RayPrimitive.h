/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Ray info - cached information for each primitive type
 *
 */

#ifndef __GU_RayPrimInfo_h__
#define __GU_RayPrimInfo_h__

#include "GU_API.h"
#include "GU_NURBPyramid.h"
#include "GU_RayRBezCurve.h"
#include "GU_RayRBezSurf.h"
#include <GEO/GEO_Detail.h>
#include <GEO/GEO_PrimitiveP.h>
#include <GEO/GEO_PrimNURBCurve.h>
#include <GEO/GEO_PrimNURBSurf.h>
#include <GEO/GEO_PrimRBezCurve.h>
#include <GEO/GEO_PrimRBezSurf.h>
#include <GEO/GEO_PrimVolume.h>
#include <GA/GA_KnotVector.h>
#include <GA/GA_NUBBasis.h>
#include <GA/GA_Types.h>
#include <TS/TS_Expression.h>

#include <UT/UT_BoundingBox.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_ThreadSpecificValue.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_VDBUtils.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4Array.h>
#include <UT/UT_VoxelArray.h>
#include <SYS/SYS_Inline.h>

#ifdef OPENVDB_ENABLED
#include <openvdb/openvdb.h>
#endif

class GU_Detail;
class GU_MinInfo;
class GU_RayInfo;
class GU_RayIntersect;
class GEO_Primitive;
class GEO_PrimRBezSurf;

class GU_API GU_Ray
{
public:
    GU_Ray(const UT_Vector3 &o, const UT_Vector3 &d, const UT_Matrix4 *mat = 0);

    float		globalDist(float t) const	{ return t / tscale; }
    float		localDist(float t) const	{ return t * tscale; }

    UT_Vector3		org, dir;
    float		tscale;
};

// Finds intersection of ray & quad.
int
GU_getBilinearQuadIntersection(const UT_Vector3 &org, 
			    const UT_Vector3 &dir,
			    const UT_Vector3 &A, const UT_Vector3 &B,
			    const UT_Vector3 &C, const UT_Vector3 &D,
			    const UT_Vector3 &norm,
			    float &t, float &u, float &v, float &d2);
//________________________________________________________
//
//  Base class for primitive info
//________________________________________________________
//
class GU_API GU_RayPrimInfo
{
public:
	     GU_RayPrimInfo(GEO_ConstPrimitiveP prim, int serialidx);

    // TODO: Make pure virtual when compiler allows us
    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo)
			{
			    return 0;
			}
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo)
			{
			    return 0;
			}

    // NB: "t" value is position on this, (u,v,w) position
    // on passed in primitive.
    virtual int		intersectPrim(
			    const GEO_Detail &geo,
			    const GEO_Detail &prim_geo,
			    GU_RayPrimInfo *prim,
			    GU_RayInfo &hitinfo)
			{
			    return -1;
			}
    virtual int		minimumPrim(
			    const GEO_Detail &geo,
			    const GEO_Detail &prim_geo,
			    GU_RayPrimInfo *prim,
			    GU_MinInfo &mininfo)
			{
			    return 0;
			}

    virtual UT_Vector3  getNormal(const UT_Vector3 &uvw) const
    {
        return UT_Vector3(0,0,0);
    }

    void		bumpRefCount(int dir);
    static void		getXYProjection(const UT_Vector3 &nml, int &x, int &y);

    // Checks the dirty bit, if set, initializes.
    void		ensureClean(const GEO_Detail &geo);
    // Initializes.  
    virtual void	init(const GEO_Detail &geo)
			{
			}

    SYS_FORCE_INLINE
    GEO_ConstPrimitiveP	primP(const GEO_Detail &geo) const
			{
			    return GEO_ConstPrimitiveP(&geo, myPrimOffset);
			}

    /// Report approximate memory usage.
    virtual int64 getMemoryUsage(bool inclusive) const = 0;

protected:
    void		 initBBox(GEO_ConstPrimitiveP prim);

public:
    GA_Offset		 myPrimOffset;
    UT_BoundingBox	 bbox;
    float		 sortValue;
    int			 mySerialIdx;		// Our index into a serial table

    SYS_AtomicCounter    refCount;
    // it is cute how we carefully conserve bits here, when you consider
    // things like the UT_BoundingBox and what shows up in the 
    // subclasses!
    bool		 degenerate:1;
    //bool		 subclassflag:1;
    volatile int 	 myDirty;

protected:
    virtual ~GU_RayPrimInfo();
};

//________________________________________________________
//
//  Quadric ray intersection class
//________________________________________________________
//
class GU_API GU_RayQuadricInfo : public GU_RayPrimInfo
{
public:
	     GU_RayQuadricInfo(GEO_ConstPrimitiveP prim, int serialidx);
    virtual ~GU_RayQuadricInfo();

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

public:
    static int		circle(
			    GEO_ConstPrimitiveP prim,
			    const GU_Ray &ray,
			    GU_RayInfo &hi,
			    UT_Vector3 &nml);
    static int		sphere(
			    GEO_ConstPrimitiveP prim,
			    const GU_Ray &ray,
			    GU_RayInfo &hi,
			    UT_Vector3 &nml);
    static int		tube(
			    GEO_ConstPrimitiveP prim,
			    const GU_Ray &ray,
			    GU_RayInfo &hi,
			    UT_Vector3 &nml);

    static void		minCircle(const UT_Vector3 &p, UT_Vector3 &minpt);
    static void		minSphere(const UT_Vector3 &p, UT_Vector3 &minpt);
    static void		minTube(
			    GEO_ConstPrimitiveP prim,
			    const UT_Vector3 &p,
			    UT_Vector3 &minpt,
			    float &u,
			    float &v);

    UT_Matrix4		xform;		// Ray transform
    UT_Matrix3		nmlXform;	// Normal transform
};

//________________________________________________________
//
//  Triangle ray intersection class
//________________________________________________________
//
template<bool istet=false>
class GU_API GU_RayTriangleInfo : public GU_RayPrimInfo
{
public:
	     GU_RayTriangleInfo(GEO_ConstPrimitiveP prim, int serialidx,
				 GA_Offset p0, GA_Offset p1,
				 GA_Offset p2, float w=0.0f);
    virtual ~GU_RayTriangleInfo();

    virtual void	init(const GEO_Detail &geo);

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);

    virtual UT_Vector3  getNormal(const UT_Vector3 &uvw) const
    {
        return nml;
    }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

public:
    GA_Offset		 myV[3];	// Three corner vertices
    UT_Vector3		 nml;		// Surface normal
    float		 D;		// Plane eqn constant
    int			 x, y;		// Oriented to projected plane
    const float          myW;
};

// These are hardened triangles:
template<bool istet=false>
class GU_API GU_RayHTriangleInfo : public GU_RayPrimInfo
{
public:
	     GU_RayHTriangleInfo(GEO_ConstPrimitiveP prim, int serialidx,
				 const UT_Vector3 &p1, const UT_Vector3 &p2,
				 const UT_Vector3 &p3, float w=0.0f);
    virtual ~GU_RayHTriangleInfo();

    virtual void	init(const GEO_Detail &geo);

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);

    virtual UT_Vector3  getNormal(const UT_Vector3 &uvw) const
    {
        return nml;
    }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

public:
    UT_Vector3		 myV[3];	// Three corner verices
    UT_Vector3		 nml;		// Surface normal
    float		 D;		// Plane eqn constant
    int			 x, y;		// Oriented to projected plane
    const float          myW;
};

//________________________________________________________
//
//  Tetrahedron ray intersection class
//________________________________________________________
//

/// Checks if query is inside the tet specified by ps.
/// If the tet is inverted and it's inside, negated will be set to true.
/// It computes normals, areas, and sub tet volumes already, so that they
/// can be used afterward.
GU_API bool
GUisPointInsideTet(
    const UT_Vector3 &query,
    const UT_Vector3 *ps,
    UT_Vector3 n[4],
    bool &negated,
    float dots[4],
    float A[4]) noexcept;

/// Computes the uvw coordinates of a query point inside a tet, based on
/// the values computed by GUisPointInsideTet.
GU_API UT_Vector3
GUfindPointInsideTet(
    float dots[4],
    const float A[4]) noexcept;

class GU_API GU_RayTetInfo : public GU_RayPrimInfo
{
public:
	     GU_RayTetInfo(GEO_ConstPrimitiveP prim, int serialidx,
				 GA_Offset p0, GA_Offset p1,
				 GA_Offset p2, GA_Offset p3);
    virtual ~GU_RayTetInfo();

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

public:
    GA_Offset		 myV[4];	// Four corner vertices
};

// These are hardened triangles:
class GU_API GU_RayHTetInfo : public GU_RayPrimInfo
{
public:
	     GU_RayHTetInfo(GEO_ConstPrimitiveP prim, int serialidx,
				 const UT_Vector3 &p1, const UT_Vector3 &p2,
				 const UT_Vector3 &p3, const UT_Vector3 &p4);
    virtual ~GU_RayHTetInfo();

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

public:
    UT_Vector3		 myV[4];	// Four corner verices
};

//___________________________________________________________________________
//
//  Quad ray intersection classes - HQuad is for hardened points (i.e. NURBS)
//___________________________________________________________________________
//
class GU_API GU_RayQuadInfo : public GU_RayPrimInfo
{
public:
	     GU_RayQuadInfo(GEO_ConstPrimitiveP prim, int serialidx,
				 GA_Offset p0, GA_Offset p1,
				 GA_Offset p2, GA_Offset p3,
				 float ustart = 0.0F, float ulength = 1.0F,
				 float vstart = 0.0F, float vlength = 1.0F,
                                 float w=0.0f);
    virtual ~GU_RayQuadInfo();

    virtual void	init(const GEO_Detail &geo);

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);

    virtual UT_Vector3  getNormal(const UT_Vector3 &uvw) const
    {
        return nml;
    }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

public:
    GA_Offset		 myV[4];	// Four corner vertices
    UT_Vector3		 nml;		// Surface normal

    //float		 D;			// Plane eqn constant
    //int		 x, y;			// Oriented to projected plane
    float		 myUStart, myULength;
    float		 myVStart, myVLength;
    const float          myW;
};

class GU_API GU_RayHQuadInfo : public GU_RayPrimInfo
{
public:
	     GU_RayHQuadInfo(GEO_ConstPrimitiveP prim, int serialidx,
				 const UT_Vector3 &p0, const UT_Vector3 &p1,
				 const UT_Vector3 &p2, const UT_Vector3 &p3,
				 float ustart = 0.0f, float ulen = 1.0f,
				 float vstart = 0.0f, float vlen = 1.0f,
				 bool revnml = false, float w=0.0f);
    virtual ~GU_RayHQuadInfo();

    virtual void	init(const GEO_Detail &geo);

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);

    virtual UT_Vector3  getNormal(const UT_Vector3 &uvw) const
    {
        return nml;
    }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

public:
    UT_Vector3		 myV[4];		// Four corner vertices
    UT_Vector3		 nml;			// Surface normal
    bool		 myRevNml;	// Do we reverse the normal?

    // These numbers aren't needed by the bilinear code.
    //float		 D;		// Plane eqn constant
    //int		 x, y;		// Oriented to projected plane

    float		 myUStart, myULength;
    float		 myVStart, myVLength;
    const float          myW;
};

//________________________________________________________
//
//  General Face ray intersection class
//________________________________________________________
//
class GU_API GU_RayPolyInfo : public GU_RayPrimInfo
{
public:
	     GU_RayPolyInfo(GEO_ConstPrimitiveP prim, int serialidx,
			    const UT_Array<GA_Offset> &ipts, float w=0.0f);
    virtual ~GU_RayPolyInfo();

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);

    virtual UT_Vector3  getNormal(const UT_Vector3 &uvw) const
    {
        return nml;
    }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

public:
    UT_Vector3		nml;
    float		D;
    int			x, y;
    const UT_Array<GA_Offset> pts;
    const float         myW;
};

class GU_API GU_RayHPolyInfo : public GU_RayPrimInfo
{
public:
			GU_RayHPolyInfo(
			    GEO_ConstPrimitiveP prim,
			    int serialidx,
			    const UT_Vector4Array &p,
			    float w = 0.0f);
			GU_RayHPolyInfo(
			    GEO_ConstPrimitiveP prim,
			    int serialidx,
			    const UT_Array<UT_Vector3> &p,
			    float w = 0.0f);
    virtual		~GU_RayHPolyInfo();
    
    template<typename ARRAY_TYPE>
    void initPoly(GEO_ConstPrimitiveP p, const ARRAY_TYPE &pts);
    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);

    virtual UT_Vector3  getNormal(const UT_Vector3 &uvw) const
    {
        return nml;
    }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        if (xval)
            mem += npts * sizeof(xval[0]);
        if (yval)
            mem += npts * sizeof(yval[0]);
        return mem;
    }

public:
    using FloatPtrArray = UT_UniquePtr<float[]>;

    int			 npts;
    FloatPtrArray	 xval;
    FloatPtrArray	 yval;
    UT_Vector3		 nml;
    float		 D;
    int			 x, y;
    const float          myW;
};

//________________________________________________________
//
//  Meta-surface ray intersection class
//________________________________________________________
//
class GU_API GU_RayMetaInfo : public GU_RayPrimInfo
{
public:
	     GU_RayMetaInfo(const TS_MetaExpressionPtr &expr, int serialidx);
    virtual ~GU_RayMetaInfo();

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);

    virtual int64 getMemoryUsage(bool inclusive) const;

public:
    mutable UT_ThreadSpecificValue<TS_MetaExpressionPtr>	myMetaExprCopies;
    TS_MetaExpressionPtr	myExprRef;
};

//________________________________________________________
//
//  VDB intersection class
//________________________________________________________
class GU_API GU_RayVDBInfo : public GU_RayPrimInfo
{
public:
	     GU_RayVDBInfo(GEO_ConstPrimitiveP vdb, int serialidx);
    virtual ~GU_RayVDBInfo();

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);

    virtual int64 getMemoryUsage(bool inclusive) const;

public:
    GEO_PrimVolumeXform		myXform;
    float			myStepSize;
    UT_ThreadSpecificValue<void *> myCache;
    bool			myFailed;
    UT_VDBType			myVDBType;
#ifdef OPENVDB_ENABLED
    const openvdb::GridBase	*myGridRawPtr;
#endif

};

//________________________________________________________
//
//  HeightField intersection class
//________________________________________________________
class GU_API GU_RayHeightFieldInfo : public GU_RayPrimInfo
{
public:
	    GU_RayHeightFieldInfo(const GEO_PrimVolume *vdb, int serialidx);
    virtual ~GU_RayHeightFieldInfo();

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);

    virtual int64 getMemoryUsage(bool inclusive) const;

public:
    int				myAxis, myA1, myA2;
    int				myDim[3];
    GEO_PrimVolumeXform		myXform;
    bool			myFailed;
    float			myScale, myInvScale;
    const UT_VoxelArrayF	*mySamples;
    UT_VoxelArrayF		myMinMap, myMaxMap;
    UT_VoxelMipMapF		myMinMM, myMaxMM;
};

//________________________________________________________
//
//  Packed primitive intersection class
//________________________________________________________

class GU_API GU_RayPackedInfo : public GU_RayPrimInfo
{
public:
    GU_RayPackedInfo(GEO_ConstPrimitiveP prim, int serialidx, int picking,
                     int polyline, int harden, bool solidtet);
    virtual ~GU_RayPackedInfo();

    virtual void	init(const GEO_Detail &geo);

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);

    virtual int64	getMemoryUsage(bool inclusive) const;

    void		unpack(const GEO_Detail &geo);

private:
    const int myPicking;
    const int myPolyLine;
    const int myHarden;
    const int mySolidTet;

    UT_UniquePtr<GEO_Detail> myUnpackedGdp;
    UT_UniquePtr<GU_RayIntersect> myUnpackedIntersect;
};

//________________________________________________________
//
//  Polyline intersection class
//________________________________________________________
//
class GU_API GU_RayPolyLineInfo : public GU_RayPrimInfo
{
public:
			GU_RayPolyLineInfo(
			    GEO_ConstPrimitiveP prim,
			    int serialidx,
			    int span, int len);
    virtual		~GU_RayPolyLineInfo();

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);
    virtual int		intersectPrim(
			    const GEO_Detail &geo,
			    const GEO_Detail &prim_geo,
			    GU_RayPrimInfo *prim,
			    GU_RayInfo &hitInfo);
    virtual int		minimumPrim(
			    const GEO_Detail &geo,
			    const GEO_Detail &prim_geo,
			    GU_RayPrimInfo *prim,
			    GU_MinInfo &mininfo);

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

public:
    int			mySpan;		// What part of the poly
    int			myLen;		// we are responsible for
};

//________________________________________________________
//
//  Curve intersection class
//
//  Note: working in homogeneous coordinate
//________________________________________________________
//

class GU_API GU_RayCurveInfo : public GU_RayPrimInfo
{
public:
	     GU_RayCurveInfo(
		     GEO_ConstPrimitiveP curve,
		     int serialidx,
		     bool is_first_span,
		     bool is_last_span)
		: GU_RayPrimInfo(curve, serialidx)
		, myFirstSpanFlag(is_first_span)
		, myLastSpanFlag(is_last_span)
	     {
	     }
    virtual ~GU_RayCurveInfo() {}

protected:
    // checks the proximity of the ray to the end points of the curve, 
    // and compares it with the tolerance given in hit_info,
    // but only for these endpoints
    // of the curve for which first/last span flags (the member data) are true.
    // If ray passes within the given tolerance, in the hemispherical cup
    // at the end of the curve, that "seals" the cylidrical zone around
    // the curve, it is considered as a hit, and 1 is returned.
    // INPUTS:
    //   org, dir - the origin and direction of the ray
    //   curve    - the curve endpoints we investigate
    //   hit_info - its tolerance is treated as a radius of a hemisphere at the 
    //		    endpoints, within which to test the ray's presence
    // OUTPUT
    //   hit_info - if hit, a hit is added
    // RETURN:
    //  1 if hit: ray passes within tolerance radius of endpoint(s), otherwise 0
    int		intersectEndPointZone(
			const GEO_Detail &geo,
			const UT_Vector3 &org,
			const UT_Vector3 &dir,
			const GU_RayRBezCurve* curve,
			GU_RayInfo &hit_info) const;

public:
    // usually a curve primitive is subdivided into several spans, whose
    // info is encoded in GU_RayRBezCurveInfo and GU_RayNURBCurveInfo. 
    // Following flags indicate if this span is first or last on the curve.
    bool		myFirstSpanFlag : 1, // 1 if it's firsrt span on curve
			myLastSpanFlag  : 1; // 1 if it's last span on curve
};

class GEO_PrimRBezCurve;

class GU_API GU_RayRBezCurveInfo : public GU_RayCurveInfo
{
public:
	     GU_RayRBezCurveInfo(
		     const GEO_PrimRBezCurve *curve,
		     int serialidx,
		     bool is_first_span,
		     bool is_last_span,
		     int span,
		     int nvtx,
		     int ord);
    virtual ~GU_RayRBezCurveInfo();

    static int count(const GEO_PrimRBezCurve *bezcurve)
    {
        const GA_KnotVector &knots = bezcurve->getBasis()->getVector();
        return knots.size() - 1;
    }
    template<typename FUNCTOR>
    static void build(const FUNCTOR &functor, const GEO_PrimRBezCurve *bezcurve, int *pinfonum=nullptr)
    {
        const int ord = bezcurve->getOrder();
        const GA_KnotVector &knots = bezcurve->getBasis()->getVector();
        const int last = knots.size() - 2;
        const int s = bezcurve->getVertexCount();

        for (int j = last; j >= 0; --j) 
        {
            const bool is_first = (j == 0);
            const bool is_last = (j == last);
            functor(new GU_RayRBezCurveInfo(bezcurve, (pinfonum ? *pinfonum : 0),
                is_first, is_last, j, s, ord));
            if (pinfonum)
                ++(*pinfonum);
        }
    }

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);
    virtual int		intersectPrim(
			    const GEO_Detail &geo,
			    const GEO_Detail &prim_geo,
			    GU_RayPrimInfo *prim,
			    GU_RayInfo &hitInfo);
    virtual int		minimumPrim(
			    const GEO_Detail &geo,
			    const GEO_Detail &prim_geo,
			    GU_RayPrimInfo *prim,
			    GU_MinInfo &mininfo);

    virtual void	init(const GEO_Detail &geo);
    GU_RayRBezCurve    *buildCurve(const GEO_Detail &geo);
    
    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        if (myCurve)
            mem += myCurve->getMemoryUsage(true);
        return mem;
    }

public:
    int				    mySpan;
    UT_UniquePtr<GU_RayRBezCurve>   myCurve;
};

class GU_API GU_RayNURBCurveInfo : public GU_RayCurveInfo
{
public:
	     GU_RayNURBCurveInfo(
		     GEO_ConstPrimitiveP curve,
		     int serialidx,
		     UT_Interval u,
		     const UT_RefMatrix<UT_Vector4> &patch_cv,
		     int ord,
		     bool is_rational);
    virtual ~GU_RayNURBCurveInfo();

    template<typename FUNCTOR>
    static void build(const FUNCTOR &functor, const GEO_PrimNURBCurve *nurbcurve, int *pinfonum=nullptr)
    {
        const GA_NUBBasis *ub = (const GA_NUBBasis *)nurbcurve->getBasis();
        const int ord = ub->getOrder();
        GA_PwHandleRO h(nurbcurve->getDetail());
        GU_NURBPyramid<0> pyramid(ub, nurbcurve, h);
        const bool rational = nurbcurve->isRational();

        bool first = true;
        for (pyramid.rewind(ub); !pyramid.atEnd(); pyramid.advance())
        {
            GU_RayNURBCurveInfo *nurbcurveinfo = new GU_RayNURBCurveInfo(
                nurbcurve, (pinfonum ? *pinfonum : 0),
                pyramid.getInterval(), pyramid.getResult(),
                ord, rational);
            nurbcurveinfo->myFirstSpanFlag = first;
            functor(nurbcurveinfo);

            if (pinfonum)
                ++(*pinfonum);

            first = false;
        }
    }

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);
    virtual int		intersectPrim(
			    const GEO_Detail &geo,
			    const GEO_Detail &prim_geo,
			    GU_RayPrimInfo *prim,
			    GU_RayInfo &hitInfo);
    virtual int		minimumPrim(
			    const GEO_Detail &geo,
			    const GEO_Detail &prim_geo,
			    GU_RayPrimInfo *prim,
			    GU_MinInfo &mininfo);
    
    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myCurve.getMemoryUsage(false);
        return mem;
    }

public:
    GU_RayRBezCurve	myCurve;
};


//________________________________________________________
//
//  Surface intersection class
//
//  Note: working in homogeneous coordinate
//________________________________________________________
//
class GU_API GU_RayRBezSurfInfo : public GU_RayPrimInfo
{
public:
	     GU_RayRBezSurfInfo(
		const GEO_PrimRBezSurf *surf, int serialidx,
		int u_span, int v_span,
		int u_ord, int v_ord,
		float u_start, float u_length,
		float v_start, float v_length,
		int u_beg, int v_beg,
		int u_num, int v_num,
		bool is_rational);
    virtual ~GU_RayRBezSurfInfo();

    static exint count(const GEO_PrimRBezSurf *bezsurf)
    {
        const GA_KnotVector &uknots = bezsurf->getUBasis()->getVector();
        const GA_KnotVector &vknots = bezsurf->getVBasis()->getVector();
        return exint(vknots.size()-1)*(uknots.size()-1);
    }

    template<typename FUNCTOR>
    static void build(const FUNCTOR &functor, const GEO_PrimRBezSurf *bezsurf, int *pinfonum=nullptr)
    {
        const int uord = bezsurf->getUOrder();
        const int vord = bezsurf->getVOrder();
        const GA_KnotVector &uknots = bezsurf->getUBasis()->getVector();
        const GA_KnotVector &vknots = bezsurf->getVBasis()->getVector();
        const int cols = bezsurf->getNumCols();
        const int rows = bezsurf->getNumRows();
        const bool rational = bezsurf->isRational();

        for (int r = vknots.size()-2; r >= 0; r--) 
        {
            const int r1 = (vord-1)*r;
            for (int c = uknots.size()-2; c >= 0; c--)
            {
                const int c1 = (uord-1)*c;
                functor(new GU_RayRBezSurfInfo(
                    bezsurf, (pinfonum ? *pinfonum : 0),
                    c, r,
                    uord, vord,
                    uknots(c), uknots(c+1)-uknots(c),
                    vknots(r), vknots(r+1)-vknots(r),
                    c1, r1,
                    cols, rows,
                    rational));

                if (pinfonum)
                    ++(*pinfonum);
            }
        }
    }

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);

    virtual int		intersectPrim(
			    const GEO_Detail &geo,
			    const GEO_Detail &prim_geo,
			    GU_RayPrimInfo *prim,
			    GU_RayInfo &hitinfo);
    virtual int		minimumPrim(
			    const GEO_Detail &geo,
			    const GEO_Detail &prim_geo,
			    GU_RayPrimInfo *prim,
			    GU_MinInfo &hitinfo);

    virtual void	init(const GEO_Detail &geo);
    virtual GU_RayRBezSurf *buildSurf(const GEO_Detail &geo);

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        if (mySurf)
            mem += mySurf->getMemoryUsage(true);
        return mem;
    }

public:
    int				 myUSpan, myVSpan;
    UT_UniquePtr<GU_RayRBezSurf> mySurf;
};

class GU_API GU_RayNURBSurfInfo : public GU_RayPrimInfo
{
public:
	     GU_RayNURBSurfInfo(
		     GEO_ConstPrimitiveP surf, int serialidx,
		     int u_ord, int v_ord,
		     UT_Interval u, UT_Interval v,
		     const UT_RefMatrix<UT_Vector4> &patch_cv,
		     bool is_rational);
    virtual ~GU_RayNURBSurfInfo();

    template<typename FUNCTOR>
    static void build(const FUNCTOR &functor, const GEO_PrimNURBSurf *nurbsurf, int *pinfonum=nullptr)
    {
        const GA_NUBBasis *ub = (const GA_NUBBasis *)nurbsurf->getUBasis();
        const GA_NUBBasis *vb = (const GA_NUBBasis *)nurbsurf->getVBasis();
        const int uOrd = ub->getOrder();
        const int vOrd = vb->getOrder();
        GA_PwHandleRO h(nurbsurf->getDetail());
        GU_NURBPyramid<0> uPyramid(ub, nurbsurf, h);
        GU_NURBPyramid<1> vPyramid(vb, &uPyramid.getResult());
        const bool rational = nurbsurf->isRational();

        for (uPyramid.rewind(ub); !uPyramid.atEnd(); uPyramid.advance())
        {
            const UT_Interval &u = uPyramid.getInterval();
            for (vPyramid.rewind(vb); !vPyramid.atEnd(); vPyramid.advance())
            {
                functor(new GU_RayNURBSurfInfo(nurbsurf,
                    (pinfonum ? *pinfonum : 0),
                    uOrd, vOrd,
                    u, vPyramid.getInterval(),
                    vPyramid.getResult(),
                    rational));

                if (pinfonum)
                    ++(*pinfonum);
            }
        }
    }

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);
    virtual int		minimum(
			    const GEO_Detail &geo,
			    const UT_Vector3 &p,
			    GU_MinInfo &mininfo);
    virtual int		intersectPrim(
			    const GEO_Detail &geo,
			    const GEO_Detail &prim_geo,
			    GU_RayPrimInfo *prim,
			    GU_RayInfo &hitinfo);
    virtual int		minimumPrim(
			    const GEO_Detail &geo,
			    const GEO_Detail &prim_geo,
			    GU_RayPrimInfo *prim,
			    GU_MinInfo &hitinfo);

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += mySurf.getMemoryUsage(false);
        return mem;
    }

public:
    GU_RayRBezSurf	mySurf;
};

//___________________________________________________________________________
//
//  Moving triangle ray intersection classes 
//      - HTriangle is for hardened points
//___________________________________________________________________________
//
class GU_API GU_RayMovingTriangleInfo : public GU_RayPrimInfo
{
public:
	     GU_RayMovingTriangleInfo(
				 GEO_ConstPrimitiveP prim0, int serialidx,
				 GA_Offset p00, GA_Offset p01,
				 GA_Offset p02,
				 GEO_ConstPrimitiveP prim1,
				 GA_Offset p10, GA_Offset p11,
				 GA_Offset p12);
    virtual ~GU_RayMovingTriangleInfo();

    virtual void	init(const GEO_Detail &geo);

    virtual int		intersect(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo);

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

private:
    int			insertHit(
			    const GEO_Detail &geo,
			    const UT_Vector3 &org,
			    const UT_Vector3 &dir,
			    GU_RayInfo &hitinfo,
			    float t,
			    float nt);

public:
    GEO_ConstPrimitiveP  myPrim2;
    GA_Offset		 myV0[3];	// Three corner vertices at t = 0
    GA_Offset		 myV1[3];	// Three corner vertices at t = 1

    double		 a2, a1, a0;	// Coefficients of x comp of normal
    double		 b2, b1, b0;	// Coefficients of y comp of normal
    double		 c2, c1, c0;	// Coefficients of z comp of normal
};


#endif
