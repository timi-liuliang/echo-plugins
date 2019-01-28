/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Trimmed patch conversion tesselator.
 *
 */

#ifndef __GU_TrimConvertPatch_h__
#define __GU_TrimConvertPatch_h__

#include "GU_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_Vector4Array.h>
#include <GA/GA_AttributeRefMap.h>
#include <GA/GA_WorkVertexBuffer.h>
#include "GU_TrimBasePatch.h"

class GU_Detail;
class GEO_TPSurf;
class GU_PrimPoly;
class GU_TrimConvertPatch;

//
// Class for tesselating on trim surfaces.
//
class GU_API GU_TrimConvertTesselator : public GU_TrimBaseTesselator
{
public:
    GU_TrimConvertTesselator(GU_Detail *gdp, GU_TrimConvertPatch *sourcepatch);
    virtual ~GU_TrimConvertTesselator();

    // Prepare for tesselation: (Called after trimming done)
    virtual void	reset();

    // Add points in counter clockwise order
    virtual void	startPoly();
    virtual void	endPoly();
    virtual void	cancelPoly();
    virtual void	addPolyPoint(GU_TrimBasePoint *point);

    // Add points two at a time:
    virtual void	startTStrip();
    virtual void	endTStrip();
    virtual void	addTStripPoints(GU_TrimBasePoint *p1,
					GU_TrimBasePoint *p2);

    // Marks whether we want to to preserve groups with newly created
    // primitives or not.
    void		setPreserveGroups(bool preservegroups)
			    { myPreserveGroups = preservegroups; }

private:
    GA_Offset getPoint(int index);

    GU_Detail		*myGDP;
    GU_PrimPoly		*myPrim;		// Current primitive.
    GA_Offset		myP1, myP2;		// Prev tstrip points.
    GU_TrimConvertPatch *myPatch;		// Patch being tesselated

    // First/most recent points on polygon to ensure duplicate
    // points are avoided.
    GU_TrimBasePoint	*myFirstPolyPoint, *myLastPolyPoint;

// We do not own these:
    GA_WorkVertexBuffer *myVertexBuffer;
    GA_Offset           *myPoints;
    GA_AttributeRefMap  *myHandles;
    UT_IntArray		*myPointIndices;
    int			 myCurveEnd;	// Index of last curve point
    bool		 myPreserveGroups;
};


class GU_API GU_TrimConvertPatch : public GU_TrimBasePatch
{
public:
    GU_TrimConvertPatch(GU_Detail *gdp, GEO_TPSurf *surface);
    virtual ~GU_TrimConvertPatch();

    // Sets all arrays so tesselator can get points directly
    // without going through extra layer.
    void		 getPointArrays(
				GA_WorkVertexBuffer *&vertices,
				GA_Offset *&points,
				UT_IntArray *&ptindices,
				GA_AttributeRefMap *&hlist,
				int &curveend);

    // Updates all points lying in certain cell boundaries.
    virtual void	 refreshCells(GEO_AttributeHandleList *hlist,
				const GEO_TPSurf *, int, int, int, int) {}

    GEO_TPSurf		*getSurf() const	{ return mySurf; }

private:

protected:
    // Initializes worlddata of given point:
    virtual void	 fillPoint(GU_TrimPatchPoint *p);

    // Calls fill point for all normal curve values
//    virtual void	 fillCurvePoints();

    // Does fwd differencing of corner values:
    virtual void	 fillCornerPoints();

    // Does fwd differencing of edge values (As along isoparms, can
    // optimize...)
    virtual void	 fillEdgePoints();

protected:
    GEO_TPSurf              *mySurf;
    GU_Detail               *myGDP;
    GA_WorkVertexBuffer     *myVertexBuffer;
    UT_ValArray<GA_Offset>   myPoints;
    UT_IntArray             *myPointIndices;
    GA_AttributeRefMap       myHandles;
    int                      myCurveEnd; // Index of last curve point

private:
};
#endif

