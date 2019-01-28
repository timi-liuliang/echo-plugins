/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Trimmed patch tesselator.
 *
 */

#ifndef __GU_TrimPatch_h__
#define __GU_TrimPatch_h__

#include "GU_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Convex.h>
#include "GU_TrimBasePatch.h"

class GD_TrimLoop;
class GU_TPSurfCache;
class GEO_TPSurf;

//
// Classes for storing polys/strips
//
class GU_API GU_TrimTriangle
{
public:
    int		operator==( const GU_TrimTriangle &tri )
		{
		    return (myP[0] == tri.myP[0] &&
			    myP[1] == tri.myP[1] &&
			    myP[2] == tri.myP[2]);
		}
    int 	myP[3];
};

class GU_API GU_TrimStrip
{
public:
    int		operator==( const GU_TrimStrip &strip )
		{
		    return (myP == strip.myP);
		}
    UT_IntArray		myP;
};


//
// Class for collecting tesselated surfaces & convexing the polys.
//
class GU_API GU_TrimTesselator : public GU_TrimBaseTesselator,
				 protected UT_Convex {
public:
    GU_TrimTesselator();
    virtual ~GU_TrimTesselator();

    virtual int64 getMemoryUsage(bool inclusive) const;

// These are required by the TrimBaseTesselator:
    virtual void reset();

    // Add points in counter clockwise order
    virtual void startPoly();
    virtual void endPoly();
    virtual void cancelPoly();
    virtual void addPolyPoint(GU_TrimBasePoint *point);

    // Add points two at a time:
    virtual void startTStrip(); 
    virtual void endTStrip(); 
    virtual void addTStripPoints(GU_TrimBasePoint *p1, 
				 GU_TrimBasePoint *p2);

// And these by the convexer:
    virtual int	 getPointCount() const;
    virtual void getPoint(int num, float &x, float &y) const;

    virtual void beginPolygon(int npoint, bool lastone);
    virtual void polyVertex(int num);
    virtual void endPolygon();

// And these by ourselves:
    // Restart the retrieval of polygons
    void	 resetRetrieval();

    // Retrieve triangles from this list:  Returns 0 if non left.
    // Returns the indices.  Invoke the cache to translate into
    // points.
    int		 getTriangle(int &p1, int &p2, int &p3);

    // Returns strips.
    int		 getStrip();
    int		 getStripPointCount();
    int		 getStripPoints(int &p1, int &p2);
    void	 getStripPoint(int idx, int &p1, int &p2);
    int		 nextStrip();
protected:
    // Where we collect points:
    UT_ValArray<GU_TrimBasePoint *>	 myPolyPoints;
    GU_TrimBasePoint			*myLastPolyPoint, *myFirstPolyPoint;

    // Where we collect triangles:
    UT_Array<GU_TrimTriangle>	 myTriangles;
    UT_Array<GU_TrimStrip>		 myStrips;

    // Where to fetch next quad/triangle from
    int					 myTriIdx, myStripIdx;
    int					 myStripOffset;
    // Used by the convexer to track state:
    int					 myCurVertex, myCurPoly;
};

//
// Tesselating a base patch.
//
class GU_API GU_TrimPatch : public GU_TrimBasePatch
{
public:
    GU_TrimPatch(GU_TPSurfCache *parent, const GEO_TPSurf *surf);
    virtual ~GU_TrimPatch();

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += GU_TrimBasePatch::getMemoryUsage(false);
        return mem;
    }

    // Updates all points lying in certain cell boundaries.
    virtual void	 refreshCells(GEO_AttributeHandleList *hlist,
				const GEO_TPSurf *surf,
				int umin, int umax, int vmin, int vmax);

    // Allows wire frame renderer to pull isoparms:
    void		 startUIsoWalk(int uiso);
    int			 getUIsoSpan(int &trimtype);
    int			 getUIsoSpanPoint(int &point);
    
    void		 startVIsoWalk(int viso);
    int			 getVIsoSpan(int &trimtype);
    int			 getVIsoSpanPoint(int &point);

    void		 startTrimCurveWalk();
    int			 getTrimCurveSpan();
    int			 getTrimCurveSpanPoint(int &point);

protected:
    // Initializes worlddata of given point:
    virtual void	 fillPoint(GU_TrimPatchPoint *p);

    // Calls fill point for all normal curve values
    // virtual void	 fillCurvePoints();

    // Does fwd differencing of corner values:
    virtual void	 fillCornerPoints();

    // Does fwd differencing of edge values (As along isoparms, can
    // optimize...)
    // virtual void	 fillEdgePoints();

protected:
    // Parent cache:
    GU_TPSurfCache	*myParent;
    const GEO_TPSurf		*mySurf;

    // State variables for isoparm walker:
    int			 myCurIsoIdx, myCurIsoOffset, myCurIsoTrim;
    int			 mySpanStart, myLastWasEnd;
    GU_TrimPatchPoint	*myCurIsoPoint;
    // State variables used by trim curve walker:
    GU_TrimPatchPoint	*myTrimCurve, *myTrimCurvePoint;
    // This is the actual start, minus exterior points.
    GU_TrimPatchPoint	*myCurTrimCurveStart;
};

#endif

