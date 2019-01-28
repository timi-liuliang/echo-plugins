/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_LineTree.h (C++)
 *
 * COMMENTS:
 *
 *	This implements an PMR-Tree/octree variant designed for sorting
 * line segments in 3-space for easy access.
 *
 */

#ifndef __UT_LineTree_h__
#define __UT_LineTree_h__

#include "UT_API.h"
#include "UT_VectorTypes.h"
#include "UT_BoundingBox.h"
#include "UT_Array.h"


class utLineTreeNode;
class utLeafNode;

class UT_API UT_LineTree 
{
public:
    UT_LineTree();
    virtual ~UT_LineTree();

    // add a line segment, returns index of line segment
    int addLine( const UT_Vector3 &pt1, const UT_Vector3 &pt2 );

    // return the number of lines in the tree
    int size() const { return myLineList.size(); }

    // build the tree using the line segments added with addLine
    // returns FALSE if tree is already built or no lines have been added, 
    // 
    int buildTree();
    
    // removes all line segments from tree
    void clear();

    // finds the nearest line id to the given point.
    // if dist2 != NULL, then it is stored the squared distance from the
    // found line segment and the point pt.
    // returns -1 if pt is not within our bounding box or the tree is not
    // build yet
    int findNearestLineSegment( const UT_Vector3 &pt, float *dist2 = NULL ) const;

    // finds the k nearest line segments to the given point.
    // the results are stored into the buffers given by the caller
    // return value is the number of actual line segments found
    unsigned findKNearestLineSegments(	const UT_Vector3 &pt, 
					int line_id[],
					float line_dist2[],
					unsigned max_lines ) const;

    // set the threshold for splitting up; 
    // will only take effect when starting a new tree!
    void requestThreshold( unsigned threshold )
	    { myRequestedThreshold = threshold; }

    // retrieves the added line segment by id 
    // returns TRUE if found, otherwise FALSE
    int getLineById( int id, UT_Vector3 &pt1, UT_Vector3 &pt2 );

    // reverse the positions of the given line segment
    void reverseLine(int id)
	    { myLineList(id).reverse(); }

    const UT_Vector3& linePos1(int id) const { return myLineList(id).myP1; }
    const UT_Vector3& linePos2(int id) const { return myLineList(id).myP2; }

    // outputs the tree to stdout for debugging
    void dumpTree(bool verbose = true) const;

private:
    struct Line {
	Line() {}
	Line(const UT_Vector3 &p1, const UT_Vector3 &p2)
	    : myP1(p1), myP2(p2)
	{
	    myIDir = 1.0F / (myP2 - myP1);
	}
	void reverse()
	{
	    UT_Vector3 p = myP1;
	    myP1 = myP2;
	    myP2 = p;
	    myIDir = 1.0F / (myP2 - myP1);
	}

	UT_Vector3	myP1;
	UT_Vector3	myP2;
	UT_Vector3	myIDir;
    };

    class IsInside {
    public:
	IsInside(const UT_BoundingBox &box,
		  const UT_Array<Line> &list)
	    : myBox(box)
	    , myLineList(list) {}
	bool operator()(int idx) const
	{
	    return myBox.isLineInside(myLineList(idx).myP1,
				      myLineList(idx).myIDir);
	}
    private:
	const UT_BoundingBox	&myBox;
	const UT_Array<Line>	&myLineList;
    };

    utLineTreeNode	*buildTree(const UT_BoundingBox &box,
				   int *lines, int nlines, int depth);
    void		 dumpTree(const utLineTreeNode *node,
				  bool verbose,
				  unsigned &num_octnodes,
				  unsigned &num_leafnodes,
				  double &num_leaflines,
				  unsigned &max_depth,
				  unsigned depth = 0) const;

    static utLineTreeNode  *newLeafNode( 
				unsigned max_lines, 
				const UT_Vector3 &corner1,
				const UT_Vector3 &corner2 );

    void getKClosestLine( 
		    utLeafNode *tree, const UT_Vector3 &pt,
		    int line_idx[], float mindist2[],
		    unsigned &num_lines, unsigned max_lines ) const;

private: 
    UT_Array<Line>	 myLineList;
    utLineTreeNode	*myRoot;
    unsigned		 myRequestedThreshold;
    unsigned		 myThreshold;
};

#endif // __UT_LineTree_h__
