/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        Geometry Library (C++)
 *
 * COMMENTS:
 *
 *	This implements a graph where the graph abstracts trim loops on
 *	a surface.  Each vertex of the graph represents an intersection between
 *	two edges, and each edge of the graph represents a segment between
 *	two vertices on a trimLoop.
 *
 */

#ifndef __GD_TrimGraph_h__
#define __GD_TrimGraph_h__

#include "GD_API.h"
typedef int GD_VERTEX;
typedef int GD_EDGE;

enum GD_TrimGraphError
{
    GD_NONE = 0,
    GD_CANT_FIND_LOOP = 1,
    GD_NOT_TWO_SIDES = 2
};

class GD_TrimLoop;
class GD_TrimHitInfo;
class GD_TrimVertex;
class GD_TrimEdges;
class GD_TrimRegion;
class GD_Face;

class GD_API GD_TrimGraph
{
public:
	// clockwise indicated the direction of the graph.
	GD_TrimGraph(int clockwise, float tol=1e-4F);
	~GD_TrimGraph();

	// Adds a new loop to the graph.  After adding the the new loop, we
	// would try to find a closed loop.  If a closed loop is found, we 
	// return 1, otherwise 0.
	int		addTrimLoop(GD_Face* face, GD_TrimLoop *tloop);

	// Adds a new vertex to the graph.  It serves the purpose of having
	// one central place to store all the vertices so that it's convenient
	// to reference a vertex or to delete them at the end.
	GD_VERTEX 	addTrimVertex(GD_TrimVertex *vertex);

	// When a function finds a loop that's made up of two edges, we
	// call this function.  edge1 and edge2 are the indices of the
	// edges that make up the loop, the "hitinfo" is the infomation
	// of intersecting edge2 with edge1 [ edge1->intersect(edge2) ]
	void 		foundLoop(GD_EDGE edge1, GD_EDGE edge2, 
				  const UT_Array<GD_TrimHitInfo> &hitinfo);

	GD_TrimVertex* 	getVertex(GD_VERTEX n) const {return myVertices((unsigned)n);};
	GD_TrimEdges* 	getEdge(GD_EDGE n) const {return myLoops((unsigned)n);};
	int 		vertexCount() const {return (int)myVertices.entries();};

	// Returns a loop from the graph.  This is just a loop that was
	// identified when adding the loops to the graph.  Returns NIL if
	// no loop was found.  It is the caller's responsibility to delete
	// the returned result.
	GD_TrimLoop*		getLoop() const;
	GD_TrimGraphError	toProfile(GD_TrimRegion *region, 
					  const UT_BoundingRect &brect);
private:
	// Tries to find a loop, using "root" as the root of the tree.  This
	// should be able to detect and identify if a cycle exists and it's
	// connected to the root.  However, this makes no guarantee to which
	// cycle will be found.
	int findLoop(GD_VERTEX root);

	// Indicates that we have a loop that is already closed; makes life
	// a lot easier.
	void foundClosedLoop(GD_EDGE edge);

	// Writing out a single closed loop out to the region.  The closed loop
	// has a vertx associated with it that has exactly one vertex, with
	// the end points of the loop recorded on it.
	void 	toProfileClosedLoop(GD_TrimVertex *vertex, 
				    GD_TrimRegion *region) const;

	// Write out a cycle that consist of exactly two loops, (and thus
	// two vertices, curV and nextV) to the the region.
	// If region is nil, then we don't write it out.  If tloop is not nil,
	// then, we would return the loop also.  Make sure tloop has no
	// TrimPieces since we are just appending to it without checking.
	void 	toProfileTwoLoops(GD_TrimVertex *curV, 
					  GD_TrimVertex *nextV,
					  GD_TrimRegion *region, 
					  GD_TrimLoop *tloop = 0) const;

	void 	toProfileMultiLoops(GD_TrimVertex *nextV, 
					    GD_TrimRegion *region,
					    GD_TrimLoop *tloop = 0) const;

	int 				 myFoundLoop;
	UT_ValArray<GD_Face*>		 myFaces;
	UT_ValArray<GD_TrimEdges*>	 myLoops;
	UT_ValArray<GD_TrimVertex*>	 myVertices;
	int				 myClockwise;
	float				 myTol;
};

#endif

