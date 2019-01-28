/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library.
 *
 * COMMENTS:	Class that does the work for the PolyKnit sop.
 *
 */
#ifndef __GU_PolyKnit_h__
#define __GU_PolyKnit_h__

#include "GU_API.h"
#include <UT/UT_IntArray.h>
#include <UT/UT_Array.h>
#include <GEO/GEO_Closure.h>
#include <GEO/GEO_PointClassifier.h>

class UT_BitArray;
class GEO_PrimPoly;
class GU_Detail;
class gu_WeightPathPair;
class gu_WeightPriorityQueue;

// The kinds of polygons that can be built with PolyKnit.  Meta polygons are
// really many polygons that are composed of a path along the connected
// geometry.
enum GU_PolyKnitType
{
    GU_POLYKNIT_TRIANGLE = 0,
    GU_POLYKNIT_QUAD,
    GU_POLYKNIT_META_TRIANGLE,
    GU_POLYKNIT_META_QUAD
};

/// Parameters for PolyKnit
class GU_API GU_PolyKnitParms
{
public:
    bool    myUnique;
    bool    myFlipNormals;
    bool    myRemoveDegenerates;
    bool    myCollapseQuads;
};

class GU_API GU_PolyKnit
{
public:
    GU_PolyKnit(GU_Detail *gdp);

    ~GU_PolyKnit() {}

    /// Builds the polygons in destgdp form the arrays of points and types
    /// Returns true if completely successful, false otherwise
    bool buildPolygons(GU_Detail *destgdp,
	 	       const UT_IntArray &ptarray,
		       const UT_IntArray &typearray,
		       const GU_PolyKnitParms &parms,
		       GA_PrimitiveGroup *resultprims);


    // Finds the shortest path from srcpt to destpt and returns true.
    // NOTE: This function assumes that the two points are connected
    bool getShortestPath(GA_Offset srcpt, GA_Offset destpt,
			 UT_Array<GA_Offset> &ptlist);

    /// Returns true if the two points are connected, and false otherwise.
    /// It uses and adds to our point connector
    bool arePointsConnected(GA_Offset srcpt, GA_Offset destpt);

private:
    // Recalculates the new weights and paths from this new point (srcpt)
    void	     updateWeights(GA_Offset srcpt, float src_w,
				   UT_Array<GA_Offset> &src_path,
				   gu_WeightPriorityQueue &pq);

    void	     updateWeightsForPoint(GA_Offset srcpt,
					   GA_Offset nextpt,
					   float src_w,
					   UT_Array<GA_Offset> &src_path,
					   gu_WeightPriorityQueue &pq,
					   UT_BitArray &visited);

    // Helper functions for buildPolygons.  They all return true if the
    // succeed and false otherwise
    bool	     buildTriangle(GU_Detail *gdp, const UT_IntArray &ptarray,
			    const UT_Array<GA_Offset> &newptarray,
			    int i, bool reverse_poly, bool remove_degenerates,
			    bool unique, GEO_PrimPoly *&poly);
    bool	     buildQuad(GU_Detail *gdp, const UT_IntArray &ptarray,
			    const UT_Array<GA_Offset> &newptarray,
			    int i, bool reverse_poly, bool remove_degenerates,
			    bool unique, bool collapse_quads,
			    GEO_PrimPoly *&poly);
    bool	     buildMetaTriangle(GU_Detail *gdp,
			    const UT_IntArray &ptarray,
			    UT_Array<GA_Offset> &newptarray,
			    int idx, bool reverse_poly, bool remove_degenerates,
			    bool unique, GA_PrimitiveGroup *resultprims);
    bool	     buildMetaQuad(GU_Detail *gdp, const UT_IntArray &ptarray,
			    UT_Array<GA_Offset> &newptarray,
			    int idx, bool reverse_poly, bool remove_degenerates,
			    bool unique, bool collapse_quads, 
			    GA_PrimitiveGroup *resultprims);

    // Takes the points in srcptlist and copies them to the gdp, making note
    // of it in newptarray
    void	     createNewPoints(GU_Detail *gdp,
			 const UT_Array<GA_Offset> &srcptlist,
			 UT_Array<GA_Offset> &newptarray);

    // Re-orders the given points so that the two source points are connected
    // and the two destination points are connected.  If we can't do that
    // we return false, otherwise we return true.  Order is also important,
    // so we try to keep the order (srcpt0 doesn't change)
    bool	     reorderPoints(GA_Offset srcpt0, GA_Offset &srcpt1,
				   GA_Offset &destpt0, GA_Offset &destpt1);
    // This version tries to make sure that destpt0 and destpt1 are connected
    bool	     reorderPoints(GA_Offset &srcpt, GA_Offset &destpt0,
				   GA_Offset &destpt1);

    // Just build a triangle/quad with the given point numbers, the unique flag
    // says where to grab the points from (the new point list or the gdp),
    // and the reverse flag says whether or not the point orders should be
    // reversed
    GEO_PrimPoly     *createTriangle(int i0, int i1, int i2,
				     bool unique, bool reverse, GU_Detail *gdp,
				     UT_Array<GA_Offset> &newptarray);
    GEO_PrimPoly     *createQuad(int i0, int i1, int i2, int i3, bool unique,
				 bool reverse, GU_Detail *gdp,
				 UT_Array<GA_Offset> &newptarray);


    GU_Detail		*myGdp;
    GEO_Closure		 myClosure;
    GEO_PointClassifier	 myPointClassifier;
};

#endif
