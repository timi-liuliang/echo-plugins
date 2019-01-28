/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Trimmed subpatch tesselator.
 *
 */

#ifndef __GU_TrimBasePatch_h__
#define __GU_TrimBasePatch_h__

#include "GU_API.h"
#include <GA/GA_KnotVector.h>

class GD_TrimLoop;
class GEO_TPSurf;
class GU_TrimCells;
class GEO_AttributeHandleList;

// Notes on terminology:
// left/right is increasing u values.
// top/bot is increasing v values.
// +---+---+
// |   |   | has two u divisions.

class GU_API GU_TrimBasePoint
{
public:
    float 		 u, v;		// Parameteric value of point.
    union {
	int64		 myInt;
	fpreal64	 myReal;
	const void	*myPointer;
    };
    float		 dist(const GU_TrimBasePoint &point) const
			 { return SYSsqrt((u-point.u)*(u-point.u) + 
				        (v-point.v)*(v-point.v)); }
};

enum GU_TrimPatchPointType
{
    GU_TrimNormalPointType = 0,
    GU_TrimUXsectionPointType = 1,
    GU_TrimVXsectionPointType = 2,
    GU_TrimExteriorPointType = 3	// Outside of domain!
};

class GU_API GU_TrimPatchPoint : public GU_TrimBasePoint
{
public:
    GU_TrimPatchPoint 	*next;		// Next point along line.
    // These are specific to crossing points, and null for curve
    // tesselation points.
    GU_TrimPatchPoint	*above, *below; // prev/next on isoparm.
		    // Above is greater u/v, below is lower.
    int			 increasing;	// Whether crosses in increasing 
					// direction
    GU_TrimPatchPointType type;
    int			 marker;	// Last cell id to touch this.
};

class GU_API GU_TrimCornerPoint : public GU_TrimBasePoint
{
public:
};

//
// Base class for tesselating on trim surfaces.
//
class GU_API GU_TrimBaseTesselator
{
public:
    GU_TrimBaseTesselator() {}
    virtual ~GU_TrimBaseTesselator() {}

    // Prepare tesselator for tesselation:
    virtual void reset() = 0;

    // Add points in counter clockwise order
    virtual void startPoly() = 0;
    virtual void endPoly() = 0;
    virtual void cancelPoly() = 0;
    virtual void addPolyPoint(GU_TrimBasePoint *point) = 0;

    // Add points two at a time:
    virtual void startTStrip() = 0; 
    virtual void endTStrip() = 0; 
    virtual void addTStripPoints(GU_TrimBasePoint *p1, 
				 GU_TrimBasePoint *p2) = 0;
};

class GU_API GU_TrimBaseRegion
{
public:
    GU_TrimBaseRegion() {}
    virtual ~GU_TrimBaseRegion() {}

    // Begins a new curve.  Closed determines if we should insert the
    // first point or assume the curve will loop back there.
    virtual GU_TrimBaseRegion	*newCurve(GU_TrimPatchPoint *point, 
				        int closed = 0) = 0;
    // Add the segment point to point->next to any edge lists it crosses.
    virtual GU_TrimBaseRegion	*addSegment(GU_TrimPatchPoint *point) = 0;
    virtual GU_TrimBaseRegion	*addSegment(GU_TrimPatchPoint *point,
				    int newi, int newj) = 0;
    virtual int			 endCurve(GU_TrimPatchPoint *point, 
					int closed = 0) = 0;

    void			 setOtherRegion(GU_TrimBaseRegion *other);

    virtual void		 tesselate(GU_TrimBaseTesselator *tesselator)
						 = 0;

    virtual void		 getCurrentIJ(int &i, int &j) = 0; 
protected:
    virtual void		 setCurrentIJ(int i, int j) = 0;

    GU_TrimBaseRegion		*myOtherRegion;
    // Has current curve crossed a boundary line?
    int				 hasCrossedLine;
};


class GU_API GU_TrimCoreRegion : public GU_TrimBaseRegion
{
public:
    GU_TrimCoreRegion();
    virtual ~GU_TrimCoreRegion();

    virtual int64 getMemoryUsage(bool inclusive) const;

    // Begins a new curve.  Closed determines if we should insert the
    // first point or assume the curve will loop back there.
    virtual GU_TrimBaseRegion	*newCurve(GU_TrimPatchPoint *point, 
					  int closed = 0);
    // Add the segment point to point->next to any edge lists it crosses.
    virtual GU_TrimBaseRegion	*addSegment(GU_TrimPatchPoint *point);
    virtual GU_TrimBaseRegion	*addSegment(GU_TrimPatchPoint *point,
				    int forcei, int forcej);
    // Returns true if curve has crossed.
    virtual int			 endCurve(GU_TrimPatchPoint *point, 
					    int closed = 0);

    virtual void		 tesselate(GU_TrimBaseTesselator *tesselator);

    void			 init(GU_TrimPatchPoint **edges,
					 GU_TrimCornerPoint *corners,
					 float *uisos, float *visos,
					 int divu, int divv,
					 GD_TrimLoop *origloops);

    virtual void		 getCurrentIJ(int &i, int &j)  
							{ i = curI; j = curJ; }

    // For cells of type 0, 2, 3 determines whether trimmed in (1) or
    // trimmed out (0)
    int				 getCellState(int ucell, int vcell);
    // For the corner at the specified isoparms, determines if the corner
    // is trimmed in (1) or trimmed out (0)
    int				 getTrimStateAt(int uiso, int viso);
protected:
    virtual void		 setCurrentIJ(int i, int j);

private:
    // Adds intersection to specified edge list (U or V edges).  Position
    // is i/j, new isect point created & inserted between point & point->next.
    // Increasing set appropriately.
    void			 insertUIsection(int i, int j, 
					GU_TrimPatchPoint *point, float v,
					int uincrease = -1);
    void			 insertVIsection(int i, int j, 
					GU_TrimPatchPoint *point, float u,
					int vincrease = -1);

    // Returns intersection of point to point->next with i or jth isoparm.
    float			 findUIsection(int i, 
					 GU_TrimPatchPoint *point, 
					 float *d = 0);
    float			 findVIsection(int j, 
					 GU_TrimPatchPoint *point,
					 float *d = 0);

    // Handles tesselation of cell w/ edges present:
    // u/vtrimmed in set to 1 if bot/right has a present edge.
    void			 tesselateCell(
					GU_TrimBaseTesselator *tesselator,
					int topedge, int leftedge,
					int topleftcorner, int &utrimmedin,
					int &vtrimmedin, int i, int j);

    // Finds next inbound point not tagged with marker in counter clockwise
    // direction of first/side.  If first NULL, looks from the end.
    // If tesselator given, adds corner points to tesselator.
    // Sides: 0 top, 1: left, 2: bot, 3: right
    GU_TrimPatchPoint		*findInboundPoint(int marker,
					int topedge, int leftedge,
					int topleftcorner, int side = 0,
					GU_TrimPatchPoint *first = 0,
					GU_TrimBaseTesselator *tesselator = 0,
					int *utrimmed = 0, int *vtrimmed = 0);

    // top row of edges is at 0, bottom starts at vdiv*udiv.
    // So for edge on u isoparm i, and to right of v isoparm j,
    // i*udiv + j is appropriate entry.
    GU_TrimPatchPoint		**myUEdges;

    // top row of edges is at 0, bottom starts at (vdiv+1)*(udiv-1).
    // So for edge on v isoparm j, and below u isoparm i,
    // i*(udiv+1) + j is appropriate entry.
    GU_TrimPatchPoint		**myVEdges;

    // List of all corners, see base patch for layout.
    GU_TrimCornerPoint		*myCorners;

    int				 curI, curJ;	// Current cell number.
		    // u isoparm i, and v isoparm j give top/left edges.
    int				 myUDiv, myVDiv;

    // Our strips.
    GU_TrimCells		*myCells;

    // We do not own these:
    float			*myUIsoparms, *myVIsoparms;
    GD_TrimLoop			*myOriginalTrimLoops;
};

class GU_API GU_TrimBasePatch
{
public:
    GU_TrimBasePatch();
    virtual ~GU_TrimBasePatch();

    virtual int64 getMemoryUsage(bool inclusive) const;

    // Traces & preps for tesselation
    void	 	 trace( const GA_KnotVector &ubreaks, 
				const GA_KnotVector &vbreaks,
				int uinteriordiv, int vinteriordiv,
				float lodc = -1);

    // Performs tesselation
    void	 	 tesselate(GU_TrimBaseTesselator *tesselator);

    // Sets this' trim loops.  This owns these.
    int			 setTrimLoops(GD_TrimLoop *loops, float umin,
				float vmin, float umax, float vmax);
    // Updates all points lying in certain cell boundaries.
    virtual void	 refreshCells(GEO_AttributeHandleList *hlist,
				const GEO_TPSurf *surf, int umin, int umax, 
				int vmin, int vmax) = 0;
    
protected:
    // Initializes worlddata of given point:
    virtual void	 fillPoint(GU_TrimPatchPoint *p) = 0;

    // Calls fill point for all normal curve values
    virtual void	 fillCurvePoints();

    // Does fwd differencing of corner values:
    virtual void	 fillCornerPoints() = 0;

    // Does fwd differencing of edge values (As along isoparms, can
    // optimize...)
    virtual void	 fillEdgePoints();

    // Allocates / initializes corner points/edge arrays.
    void		 initCells();
    void		 freeCells();
    void		 freeCurves();

    // Follows a curve:
    void		 traceCurve(GD_TrimLoop *loop, float curvelod);

    GU_TrimCoreRegion	*myCoreRegion;

    // First ucorediv*(vcorediv+1) are edges along U isoparms,
    // next (ucorediv+1)*vcorediv are along V isoparms.  See TrimCoreRegion
    // for order within there.
    GU_TrimPatchPoint	**myEdgeListArray;

    // (vcorediv+1)*(ucorediv+1) points.  The intersection of the u isoparm
    // i with the v isoparm j is at index i*(ucorediv+1) + j
    // The actual values will be filled out by GU_TrimCoreRegion.
    GU_TrimCornerPoint	*myCornerPointArray;

    // Starting points of curves, linked via above pointers.
    GU_TrimPatchPoint	*myCurveStarts;

    // We own the following:
    float		*myUIsoparms, *myVIsoparms;

    GD_TrimLoop		*myTrimLoops;
    GD_TrimLoop		*myOriginalTrimLoops;

    int			 myUDiv, myVDiv;
    int			 myULod, myVLod;
};
#endif

