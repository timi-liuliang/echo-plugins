/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: Interface for computing sliding of edges.
 */

#ifndef __GU_EdgeSlide_h__
#define __GU_EdgeSlide_h__

#include "GU_API.h"
#include "GU_Detail.h"
#include <UT/UT_Array.h>
#include <UT/UT_Map.h>
#include <UT/UT_Set.h>
#include <UT/UT_VectorTypes.h>

class GU_RayIntersect;

enum GU_SlideMode
{
	GU_SLIDE_ANCHOREDBLEND,
	GU_SLIDE_BLEND,
	GU_SLIDE_MATCHPROFILE,
	GU_SLIDE_MATCHOPPOSITEPROFILE,
	GU_SLIDE_UNIFORMDISTANCE,
	GU_SLIDE_NUM_MODES
};

class GU_API GU_EdgeSlide
{
public:
    typedef std::pair<GA_Index, GA_Index> Edge;

    // caller can optionally pass a GU_RayIntersect that they want the slide
    // points to be snapped onto
    GU_EdgeSlide(GU_RayIntersect* templateGeoRayIntersect = nullptr)
	: myTemplateGeoRayIntersect(templateGeoRayIntersect)
	, myIsLoop(false)
	, myIsValidEdge(false)
	, myMovingLeft(false)
	, myMode(GU_SLIDE_ANCHOREDBLEND)
	, myGrabEdge(Edge(-1,-1))
	, myLastEdge(Edge(-1,-1)) {}

    // Generates the list of slide points given a list of selected edges
    // according to the current detail.
    bool setSlidePoints(const GU_Detail &geo, const UT_Array<Edge> &selection);

    // Called every movement step (i.e mouse move), fills indices and positions
    // in same order.  Caller can optionnaly pass a map that gets filled with
    // which slide points hit their borders and should be snapped.
    void moveSlidePoints(UT_Array<GA_Index> &indices,
			 UT_Vector3Array &positions, const GU_Detail &geo,
			 const UT_Vector3 &dir, const UT_Vector3 &orig,
        bool staticShift,
        UT_Map<GA_Index, GA_Index> *snapped = nullptr);

    // called to "end" slide edit, sliding works based on intial position of the
    // slide edge, this will set all the initial positions to the current
    // positions
    void setCurPosToInitPos();

    // clears slide pints array
    void clearSlidePoints();

    // Fills points with ordered traversal of slide edge (from end to end).
    // Points will contain all VALID point indices according to where on the
    // selection myGrabEdge is.  Called can optionally get the UNORDERED set of
    // invalid points.
    void getSlidePoints(UT_Array<GA_Index> &points,
			UT_Set<GA_Index> *invalid=nullptr) const;

    void getSlidePointsSnaps(UT_Array<GA_Index> &snaps,
        UT_Set<GA_Index> *invalid = nullptr) const;

    // Fills points with ordered traversal of 'left' or 'right' side of current
    // slide edge.
    void getNeighbouringPoints(UT_Array<GA_Index> &points, bool left) const;

    // Fills points with ordered traversal of slide edge (from end to end).
    // Points will contain all point positions according to where on the
    // selection myGrabEdge is.
    void getSlidePointsPos(UT_Vector3Array &points) const;

    // Fills points with ordered traversal of 'left' or 'right' side of current
    // slide edge.
    void getNeighbouringPointsPos(UT_Vector3Array &points, bool left) const;

    inline bool isEmpty() const { return !mySlidePoints.entries(); }
    inline bool isLoop() const { return myIsLoop; }
    inline bool isValid() const { return myIsValidEdge; }
    inline bool isMovingLeft() const { return myMovingLeft; }
    inline GU_SlideMode getMode() const { return myMode; }
    inline void setMode(GU_SlideMode mode) { myMode = mode; }
    inline void setGrabEdge(GA_Index p0, GA_Index p1)
				{ myGrabEdge = std::make_pair(p0, p1); }
    inline Edge getGrabEdge() { return myGrabEdge; }
    inline GU_RayIntersect* getRayIntersect() const { return myTemplateGeoRayIntersect; }
    inline void setRayIntersect(GU_RayIntersect *rayintersect) { myTemplateGeoRayIntersect = rayintersect; }
private:
    /****** structs ******/

    struct SlidePoint
    {
	SlidePoint(GA_Index idx, const UT_Vector3 &pos)
	    : myLeft(GA_INVALID_INDEX)
	    , myRight(GA_INVALID_INDEX)
	    , myIndex(idx)
	    , mySnap(GA_INVALID_INDEX)
	    , myPos(pos)
	    , myInitialPos(pos)
	    , myCorner(false)
	    , myValid(false) {}

	// existing points to the respective 'left' and 'right' in terms of
	// winding
	GA_Index myLeft;
	GA_Index myRight;
	GA_Index myIndex;
	GA_Index mySnap;

	UT_Vector3 myPos;
	UT_Vector3 myInitialPos;
	UT_Vector3 myLeftPos;
	UT_Vector3 myRightPos;

	bool myCorner;
	bool myValid;
    };

    /****** functions ******/

    // Generates Slide Edge
    void decideSlideEdge(const GU_Detail &geo, const UT_Array<Edge> &edges);

    // Helper function for decideSlideEdge that assigns 'left' and 'right'
    // neighbours
    void assignNeighbours(const GU_Detail &geo,
        const UT_Array<GA_Index>& neighbours,
        UT_Map<GA_Index, exint>& slidePointRef,
			  GA_Index point1, GA_Index point2, bool flipped,
        int index);

    /****** members ******/
    GU_RayIntersect *myTemplateGeoRayIntersect;
    bool myIsLoop;
    bool myIsValidEdge;
    bool myMovingLeft;
    GU_SlideMode myMode;
    Edge myGrabEdge;
    Edge myLastEdge;
    // Indices for grabbed edge into mySlidePoints so that we don't need to
    // look them up every time.
    std::pair<exint, exint> myGrabEdgeIndices;
    UT_Array<SlidePoint> mySlidePoints;
};

#endif
