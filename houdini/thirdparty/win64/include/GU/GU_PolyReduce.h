/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Polygonal reduction tool (C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PolyReduce_h__
#define __GU_PolyReduce_h__

#include "GU_API.h"
#include <UT/UT_Vector3.h>
class GA_EdgeGroup;
class GEO_Detail;

// Description of a single edge collapse
class GU_API GU_EdgeCollapseNode
{
public:
    int				 myIv1, myIv2;	// Source indices to reduce.
    int				 myNewIv;	// Resulting index.
    int				 myDegenTri;	// Tris removed
    UT_Vector3			 myV;		// Resulting position.
    int				 myNewPt;	// New point number
    
private:
    friend class GU_EdgeCollapseList;
    GU_EdgeCollapseNode		*myNext, *myPrev;
};

// A list of edge collapses:
class GU_API GU_EdgeCollapseList
{
public:
    GU_EdgeCollapseList() { myHead = 0; myTail = 0; }
    ~GU_EdgeCollapseList();

    GU_EdgeCollapseNode		*next(GU_EdgeCollapseNode *node);
    void			 remove(GU_EdgeCollapseNode *node);
    void			 append(GU_EdgeCollapseNode *node);
    int				 empty() const { return myHead ? 0 : 1; }
    GU_EdgeCollapseNode		*first() const { return myHead; }

private:
    GU_EdgeCollapseNode		*myHead, *myTail;
};

class GU_API GU_PolyReduceParms
{
public:
    GU_PolyReduceParms();
    ~GU_PolyReduceParms();

    int		numpolys;	// Number of polys to reduce to.
    float	percentage;	// Percentage to reduce
    int		usepercent;	// Which method to use.
    bool	useedgelimit;	// Constrain number of edges to collapse
    int		numedgecollapse;// Max edges to collapse.
    int		meshinvert;	// Test for mesh inversion?
    float	optimization;	// 1 for optimized point positions
				// 0 for just mid points
    float	borderweight;	// Weighting to add to border planes
    float	lengthweight;	// Weighting to add to the length of sides
    int		keepedges;	// Never collapse the edges
    int		snap;		// Snap to existing vertices

    // Feature edgse:
    GEO_Detail   *gdpf;			// Detail containing feature edges
    const GA_EdgeGroup *features;	// Feature edges
    const GA_EdgeGroup *forcecollapse;	// Edges to collapse.
    float	  featureweight;	// The weight thereof

    int		altergeo;	// Do we change the geometry at all?

    GU_EdgeCollapseList	*edgelist;	// Where we accumulate all ecols.
    int		uniquevertex;	// Number unique vertex
    int		initialtris;	// Number stris started with

    float	attribweight;	// Importance of the weight attribute.
    float	topologicalweight;	// Penalty factor for high valences.
};
    

#endif


