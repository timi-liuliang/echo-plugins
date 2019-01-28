/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_VolumeBreak.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_VolumeBreak__
#define __GU_VolumeBreak__

#include "GU_API.h"
#include <GEO/GEO_PrimList.h>
#include <GA/GA_Edge.h>
#include <GA/GA_EdgeMap.h>
#include <GA/GA_ElementWrangler.h>
#include <GA/GA_Types.h>
#include <UT/UT_Map.h>
#include <UT/UT_Array.h>
#include <UT/UT_FloatArray.h>

class GU_Detail;
class GEO_PrimVolume;
class GEO_PrimPoly;
class GA_PrimitiveGroup;

enum guBreakPolyType
{
    GU_INSIDE_POLY,
    GU_OUTSIDE_POLY,
    GU_UNKNOWN
};

enum guBreakPointStatus
{
    GU_INTERIOR_POINT,
    GU_VERTEX_POINT
};

struct guBreakPoly;

struct guBreakPoint
{
    // The break point is either on the edge interior or on a vertex.
    guBreakPointStatus	 status;

    // The point.
    GA_Offset pt;

    // If status is InteriorPoint: the adjacent points.
    // If status is VertexPoint:   the same point, i.e. pt1 = pt2.
    GA_Offset pt1;
    GA_Offset pt2;

    // The adjacent break points determined while splitting.
    guBreakPoint *prev;
    guBreakPoint *next;

    // 0 if pt==pt1; 1 if pt==pt2; else between 0 and 1
    fpreal lerpbias;

    // The associated polygons.
    UT_Array<guBreakPoly *> polys;
};

struct guBreakPoly
{
    // The poly is classified as inside or outside the SDF volume.
    guBreakPolyType	 type;

    // The polygon.
    GEO_PrimPoly	*poly;

    // The associated break points.
    UT_Array<guBreakPoint *> points;
};

struct guBreakPointPair
{
    guBreakPoint	*pt1;
    guBreakPoint	*pt2;

    bool		 reusable1;
    bool		 reusable2;
};

typedef UT_Array<guBreakPoint *> guBreakPointArray;
typedef UT_Array<guBreakPoly *> guBreakPolyArray;

enum guNeedSplitResult
{
    GU_YES_SPLIT,
    GU_NO_SPLIT,
    GU_NO_SPLIT_REDUNDANT_NONE,
    GU_NO_SPLIT_REDUNDANT_ALL
};

enum guLoopCloseMethod
{
    GU_NO_CLOSE,
    GU_FLAT_CLOSE,
    GU_APEX_CLOSE,
    GU_MULTIAPEX_CLOSE,
    GU_GRADIENT_CLOSE
};



class GU_API GU_VolumeBreak
{
public:
    GU_VolumeBreak();
    ~GU_VolumeBreak();

    void setGroups(const GA_PrimitiveGroup *agrp)	{ myGrp = agrp; }
    void setSnapDistance(fpreal snapdistance)	{ myTol = snapdistance; }
    void setKeepInside(bool inside)		{ myKeepInside = inside; }
    void setKeepOutside(bool outside)		{ myKeepOutside = outside; }
    void setCloseGeometry(bool closegeo)	{ myCloseGeo = closegeo; }
    void setCloseHoles(guLoopCloseMethod closemethod)
						{ myCloseMethod = closemethod; }

    void volumeBreak(GU_Detail &gdp, GEO_PrimVolume &volume,
	    GA_PrimitiveGroup *ingrp, GA_PrimitiveGroup *inclgrp,
	    GA_PrimitiveGroup *outgrp, GA_PrimitiveGroup *outclgrp);


private:
    GU_Detail	 	*myGdp;
    GEO_PrimVolume 	*myVolume;

    const GA_PrimitiveGroup	*myGrp;

    fpreal		 myTol;
    bool		 myKeepInside;
    bool		 myKeepOutside;
    bool		 myCloseGeo;
    guLoopCloseMethod	 myCloseMethod;

    // Hash of edges to be broken.  Used to maintain a unique list.
    GA_EdgeMap<guBreakPoint *>	myEdgeTable;

    // List of all break points.
    UT_Array<guBreakPoint *>	myPointList;

    // List of all polygons, including those with no break points.
    UT_Array<guBreakPoly *>	myPolyList;

    // Point distances indexed by point num.
    UT_FloatArray		myPointDistances;

    GA_ElementWranglerCache *myWranglers;

    void		clearPointList();
    void		clearPolyList();
    void		printPointList();
    void		printPolyList();


    //
    // -- Breakpoint finding --
    //

    // Add zero or more breakpoints given the edge (pa, pb).
    void		addBreakPoint(guBreakPoly *poly, const GA_Edge &edge);

    // Find the break point location on the edge given two points.
    GA_Offset		makeBreakPoint(GA_Offset pt1, GA_Offset pt2,
                                       fpreal &lerpbias,
                                       GA_PointWrangler &ptwrangler);

    // Find the position between in1 and in2 where the SDF is 0.  The initial
    // SDF values at in1 and in2 are given in indist1 and indist2.
    int			findRoot(const GEO_PrimVolume &volume,
				const UT_Vector3 &in1, fpreal indist1,
				const UT_Vector3 &in2, fpreal indist2,
				UT_Vector3 &out,
				int maxiters = 100);

    // Find the position along the ray (origin, dir) where the SDF is 0.
    int			findRoot(const GEO_PrimVolume &volume,
				const UT_Vector3 &origin,
				const UT_Vector3 &dir,
				UT_Vector3 &out,
				int maxiters = 100);

    // Removes duplicate adjacent breakpoints on the same poly
    void		removeDuplicateBreakpoints(guBreakPoly *poly);


    //
    // -- Edge finding and splitting --
    //

    // Iteratively call splitClosedPoly or splitOpenPoly.
    int		 	splitPoly(guBreakPoly *poly, GA_ElementWranglerCache &wranglers);

    // Split into one poly free of break points and a second remaining poly.
    // The resulting polygons are either all open or all closed.
    int		 	splitClosedPoly(guBreakPoly *poly, bool &finished, GA_ElementWranglerCache &wranglers);

    // Split into one poly free of break points and a second remaining poly.
    // The resulting polygons are open.
    int		 	splitOpenPoly(guBreakPoly *poly, bool &finished, GA_ElementWranglerCache &wranglers);

    // Sort the list of break points with respect to the given polygon.
    // This sorts each break point internally as well as the list itself.
    void	 	sortBreakPoints(guBreakPoly *poly);
    
    // Finds a pair of break points from the polygon that defines an
    // approximating edge of the volume surface.
    int			findSurfaceEdge(guBreakPoly *poly,
				guBreakPointPair &outpair);

    // Determines whether or not the walk from startpt to endpt along the
    // polygon perimeter occurs inside the volume.
    bool		isInsideWalk(guBreakPoly *poly,
				guBreakPoint *startpt, guBreakPoint *endpt);

    // Determines whether the given edge from startpt to endpt requires that
    // the polygon be split.
    guNeedSplitResult	needSplit(guBreakPoly *poly,
				guBreakPoint *startpt, guBreakPoint *endpt);

    // Split the given polygon along the given edge and pass the pointer to
    // the new polygon back.  The new polygon is formed by the vertices
    // traversed when walking from startpt to endpt.
    int			splitClosedPoly(guBreakPoly *poly,
				guBreakPoint *startpt, guBreakPoint *endpt,
				guBreakPoly *&outnewpoly, GA_ElementWranglerCache &wranglers);

    // Removes breakpoints according to rules in the pair.
    int			removeBreakPoints(guBreakPoly *poly,
				guBreakPointPair &pair);


    // 
    // -- Hole closing --
    //

    // Given a list of breakpoints, extracts loops of these.
    void		findLoops(UT_Array<guBreakPointArray> &loops,
				guBreakPointArray &breakpoints);

    // Given a list of breakpoint loops, generates geometry to fill each loop.
    int			closeLoops(UT_Array<guBreakPointArray> &loops,
				GU_Detail &gdp, GEO_PrimVolume &sdf,
				UT_Array<GEO_PrimPoly *> &closurepolys,
                                GA_ElementWranglerCache &wranglers);




    // Return the insideness of the given poly when it is absolutely certain
    // with a minimum distance tolerance.  When uncertain, returns GU_UNKNOWN.
    guBreakPolyType	classifyPolyConservative(GEO_PrimPoly *poly, float tol);

    // Return the insideness of the given polygon; does not return GU_UNKNOWN.
    guBreakPolyType	classifyPoly(guBreakPoly *poly);

    GEO_PrimPoly *	createPolygon(GU_Detail &gdp);

    GEO_PrimPoly *	createPolygon(GU_Detail &gdp,
                                      const UT_Array<GA_Offset> &vtxes,
                                      bool closed,
                                      GEO_PrimPoly *srcpoly,
                                      GA_PrimitiveWrangler &polywrangler,
                                      GA_VertexWrangler &vtxwrangler);

    // Links p1 with p2 in that order, opposite order if backward is true.
    int			linkBreakPoints(guBreakPoint *p1, guBreakPoint *p2,
				bool backward);

    bool		linkExistsInPoly(guBreakPoint *pt, guBreakPoly *p);
};

struct guPolyApplyData
{
    GU_VolumeBreak	*breaker;
    guBreakPoly		*poly;
    GA_PointWrangler *ptwrangler;
};



#endif

