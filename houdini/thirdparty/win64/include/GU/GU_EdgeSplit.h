/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: Splits edges.
 *
 */

#ifndef __GU_EdgeSplit_h__
#define __GU_EdgeSplit_h__

#include "GU_API.h"
#include <GEO/GEO_PrimPoly.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_VectorTypes.h>

class   GA_EdgeGroup;
class   GA_PointGroup;
class   GA_PrimitiveGroup;
class	GA_ElementWranglerCache;
class	GA_AttributeRefMap;
class	GEO_Closure;
class   GU_Detail;
class   GU_SplitHistory;
class   GU_SplitHistoryNode;
class   GU_SplitLoc;
class   GU_SplitPath;
class   GU_SplitPathNode;
class   GU_SplitPathManager;
class   gu_PrimToUpdate;
class   gu_CrossEdge;


// This class defines a split location.  It represents a place where a split
// will occur.  A list of these things is what we need to make a complete
// cut.  
class GU_API GU_SplitLoc
{
public:
    GU_SplitLoc(GA_Detail *gdp = NULL,
                GA_Offset p0off = GA_INVALID_OFFSET,
		GA_Offset p1off = GA_INVALID_OFFSET,
	        float t = 0,
		GEO_PrimPoly *poly = NULL)
        : myP0Off(p0off)
	, myP1Off(p1off)
	, myT(t)
	, myT2(-1)
	, myPoly(poly)
	, myOldPoly(0)
	, myInsidePrim(false)
        , myGdp(gdp)
	    { }

    GU_SplitLoc(float u, float v, GEO_PrimPoly *poly)
	: myP0Off(GA_INVALID_OFFSET)
	, myP1Off(GA_INVALID_OFFSET)
	, myT(u)
	, myT2(v)
	, myPoly(poly)
	, myInsidePrim(true)
	, myOldPoly(poly)
        , myGdp(&poly->getDetail())
	    { }

    GU_SplitLoc   &operator=(const GU_SplitLoc &src)
		   {
		       myP0Off= src.myP0Off;
		       myP1Off= src.myP1Off;
		       myT    = src.myT;
		       myT2   = src.myT2;
		       myPoly = src.myPoly;
		       myOldPoly = src.myOldPoly;
                       myGdp = src.myGdp;
		       myInsidePrim = src.myInsidePrim;
		       return *this;
                   }
    bool           operator==(const GU_SplitLoc &src) const;
    bool           operator!=(const GU_SplitLoc &src) const
		   {
		       return !(operator==(src));
		   }

    UT_Vector3     getPos3() const;
    void	   print() const;	// TODO: debug only

    GA_Offset      myP0Off;
    GA_Offset      myP1Off;
    float	   myT;  //
    float	   myT2; // second uv value for face vertices
    bool	   myInsidePrim; // the point is not a vertex/edge cut
    GEO_PrimPoly  *myPoly;
    GEO_PrimPoly  *myOldPoly; // used to store references to old polygons
			      // for in-polygon cuts to save recalculating
			      // u, v values
    GA_Detail     *myGdp;
};
typedef UT_Array<GU_SplitLoc *> GU_SplitLocArray;



// The UpdatePrimList is the list kept of all the prims that must be
// updated once the split is over.  These prims are not involved in any
// split, but have been affected by a split (probably because they share
// an edge that was split.)
class GU_API gu_UpdatePrimList
{
public:
    ~gu_UpdatePrimList();

    UT_Array<gu_PrimToUpdate *>  myList;

    void    updatePrims(GU_SplitHistory &hist, GU_Detail &gdp,
			float tolerance,
			GA_ElementWranglerCache &wranglers,
			bool createDuplicate = false);

    GEO_PrimPoly * createDuplicatePoly(GEO_PrimPoly &poly,
					GU_SplitHistory &hist,
					GU_Detail &gdp,
					GA_ElementWranglerCache	&wranglers);

    // Adds to list primlist the polygons that will be getting point pt
    void    getPrimsUsingPoint(GA_Offset pt, UT_IntArray &primlist);
};

class gu_PrimDistance;


// EdgeSplit workhorse.
class GU_API GU_EdgeSplit
{
public:
     GU_EdgeSplit(GU_Detail &gdp, GA_EdgeGroup *outedges = 0, 
	     bool modifyGdp = true, UT_Fpreal32Array *outPositions = NULL);
    ~GU_EdgeSplit();

    void        doSplit(const GU_SplitLocArray &splitlocs, bool force,
			bool quadcut, float tolerance, bool quadcomplete,
			bool fixends);

private:

    // This method takes both inputs, converts them to an edge split location,
    // if necessary, and calls splitEdge
    // The parameters:
    //	    from	    - from location
    //	    to		    - to location
    //	    path	    - current path so far
    //	    quadcut	    - true if we are quadcuttin', false otherwise
    //	    findingtail	    - true if we are looking for our tail
    //	    pointpass	    - serial number of passing the responsibility to
    //			      other primitives and avoid infinite recursion.
    void	split(const GU_SplitLoc &from, const GU_SplitLoc &to,
	              GU_SplitPath *&path, bool quadcut, bool findingtail,
		      int pointpass);
    bool	simplify(GU_SplitLoc &from, GU_SplitLoc &to);

    void	splitPoly(GEO_PrimPoly &poly, int fromedge, int toedge,
	                  float fromt, float tot, 
			  GA_Offset newpt0, GA_Offset newpt1);

    void	addEdgeToPoly(GEO_PrimPoly &poly, int fromedge, float fromt,
				 GA_Offset newpt0, GA_Offset newpt1,
				 GEO_PrimPoly &oldPoly,
				 float u, float v);

    // Returns true if the node was added to the path
    bool 	addPathNode(const GU_SplitLoc &from, const GU_SplitLoc &to,
			    GU_SplitPath &path, bool quadcut);
    
    void	updateNeighbourPrims(const GU_SplitLoc &splitloc);

    bool	restructureSplit(GU_SplitHistoryNode *stn, GU_SplitLoc &loc);

    void	executePath(GU_SplitPath &path);
    void        getTransferCandidates(const GU_SplitLoc &from,
				      GA_PrimitiveGroup &grp);
    bool	transferLoc(GU_SplitLoc &loc, GEO_PrimPoly *poly);
    bool	inPath(GEO_PrimPoly *poly, const GU_SplitPath &path);
    void	pruneGroup(const GU_SplitLoc &fromloc, const GU_SplitLoc &toloc,
			   UT_IntArray &primlist, bool quadcut);

    void	getEdgeCandidates(const GU_SplitLoc &loc,
	                          const UT_Vector3 &src, const UT_Vector3 &dest,
				  UT_Array<gu_CrossEdge> &arr,
				  bool quadcut);

    void	sortEdgeCandidates(const GU_SplitLoc &loc,
				   const UT_Vector3 &dest,
				   UT_Array<gu_CrossEdge> &arr);

    void	getPrimCandidates(const GEO_PrimPoly *poly, GA_Offset pt0,
				  GA_Offset pt1,const GU_SplitPath &path,
				  UT_IntArray &candidates, bool quadcut);

    GA_Offset   getNewPoint(GA_Offset pt0, GA_Offset pt1, fpreal t);
    GA_Offset   getNewPoint(const GEO_PrimPoly &p, const UT_Vector3 &pos,
			    float u, float v);

    int    	buildFromPrimArray(const GU_SplitLoc &fromloc,
				   const GU_SplitLoc &toloc,
				   UT_IntArray &fromlist,
				   UT_Array<gu_PrimDistance *> &primarray,
				   bool quadcut);

    // Takes the path and fills it with the correctly interpolated biases for
    // quad cuts.  The quadclosed flag indicates whether or not the path is
    // a closed loop.  The findingtail flag indicates whether or not we were
    // trying to find our tail (single-click quadcut).
    void	quadProcessPath(GU_SplitPath &path, bool quadclosed,
				bool findingtail);

    void	getParticipatingPolys(const GU_SplitLoc &loc,
				      GEO_Closure &closure, UT_IntArray &prims);

    void	interpolateInteriorPoint(GA_Offset &result,
					 GA_AttributeRefMap &map,
					 const GEO_PrimPoly &poly,
					 fpreal u, fpreal v);

    // ensures that the first and last split locations in the given array are
    // not on faces, as the algorithm requires that they are. If they are not, 
    // add the closest vertex to the first/last point appropriately
    void	fixFirstLastPoints(GU_SplitLocArray &splitlocs);

    // returns a split location that is a vertex closest to the given splitloc
    // skip is a point that will not be considered. Use this if for example you
    // just came from a point and don't want to return there
    GU_SplitLoc *closestPoint(const GU_SplitLoc &splitpt,
			      GA_Offset skip = GA_INVALID_OFFSET) const;

    // attempts to fix any quads on the original geometry which have been split
    // into polygons that are not quads. It only works when the polygon has been
    // split into exactly two parts, one of which is a quad or a triangle
    void	completeQuads();

    // adds the point specified to myOutPositions
    void	addToMyOutPositions(GA_Offset pt0, GA_Offset pt1, float t);

    // Data Members
    GA_PrimitiveGroup		*myTempPrimGrp;

    // This group will keep track of the new points we add to the geometry
    // so that we don't add duplicate points (keeps things consolidated
    GA_PointGroup		*myNewPointsGrp;
    gu_UpdatePrimList   	 myUpdatePrimList;
    GEO_PrimPoly		*lastPoly;
    GU_SplitHistory		*myHistory;
    GU_Detail			&myGdp;
    GU_SplitPathManager		*mySplitPathMan;
    GA_EdgeGroup		*myOutEdges;
    GA_ElementWranglerCache	*myWranglers;
    bool			 myForceCut;
    float			 myTolerance;
    bool			 myHasInsideCut;
    // this prevents myGdp from being changed. It is used when we want to find
    // the split path for rendering purposes, but don't want to actually change
    // the given GU_Detail
    bool			 myModifyGdp;
    // we typically don't want to modify the gdp when we just want a list of
    // edges being modified/add for rendering purposes. We can't use myOutEdges
    // because that requires the points to be added to the gdp, so instead we
    // just use a UT_Fpreal32Array
    UT_Fpreal32Array		*myOutPositions;

};

#endif
