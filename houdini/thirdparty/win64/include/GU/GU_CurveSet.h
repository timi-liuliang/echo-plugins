/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Describes a set of space curves.  Creates nodes
 *	whereever the space curves intersect, recording which
 *	curves and where they intersected.  Curves are
 *	also given an index when they are added, allowing
 *	easy cross referencing to other data structures.
 *
 */

#ifndef __GU_CurveSet_h__
#define __GU_CurveSet_h__

#include "GU_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Vector3.h>

// Node iterator:
#define FOR_ALL_NODES(curveset, node) \
    for (node = curveset.getHeadNode(); node; node = node->next())

class GD_TrimLoop;
class GU_CurveSet;
class GU_CurveNode;
class GEO_Face;
class GU_Detail;

class GU_API GU_CurveSetInfo
{
public:
    // Not a very intelligent == operator, something is required for 
    // NT build though.
    int		operator==(const GU_CurveSetInfo &curveInfo)
		{ return curve == curveInfo.curve; }

    GEO_Face 			*curve;
    UT_ValArray<GU_CurveNode *>	 nodes;

    void	removeNode(const GU_CurveNode *node);
};

class GU_API GU_CurveNodeInfo
{
public:
    int		operator==(const GU_CurveNodeInfo & /*ni*/)
			{ return 1; }		// Req'd for NT

    // data
    int		 curveidx;	// The curve that hits this node.
    float	 u;		// Where that curve isects.
    float	 d2;		// Distance of curve to this node
};

class GU_API GU_CurveNode
{
public:
    void 		 addCurve(GU_CurveSetInfo &curveinfo, int curveidx, 
				    float u, float d2, const UT_Vector3 &pt);

    GU_CurveNode	*next() const { return myNext; }
    int			 degree() const { return myDegree; }
    GU_CurveNodeInfo	&operator()(int i) { return myEdges(i); }
    GU_CurveNodeInfo	 operator()(int i) const { return myEdges(i); }
    UT_Vector3		&point() { return myPt; }
    UT_Vector3		 point() const { return myPt; }
    UT_Vector3		&normal() { return myNormal; }
    UT_Vector3		 normal() const { return myNormal; }

public:
    // data
    UT_Vector3			 myPt;		// Where isection is
    UT_Vector3			 myNormal;	// Orientation of node
    int				 myDegree;	// # curves isecting here
    UT_Array<GU_CurveNodeInfo> myEdges;	// All edges hitting here.
    GU_CurveNode		*myNext;	// Next in chain.
    float			 myD2;		// Dist of isect to point.
};

class GU_API GU_CurveSet
{
public:
    GU_CurveSet(int useEnds = 0);
    ~GU_CurveSet();

    // Merges two curvesets.  Curve indices are changed:
    void		merge(GU_CurveSet &curveset);

    // Adds new curve.  Returns its index
    // Not const as intersection info is generated.
    int 		merge(GEO_Face &curve);
    
    int  		isEmpty() const 
				{ return (myCurves.entries()) ? 0 : 1; }
    int			numCurves() const { return myCurves.entries(); }

    // Clears all local information without destroying the refarray.
    void		reset();

    GU_CurveSetInfo	&operator()(int i) { return myCurves(i); }
    GU_CurveSetInfo 	operator()(int i) const { return myCurves(i); }

    // Not a very intelligent == operator, something is required for 
    // NT build though.
    int		operator==(const GU_CurveSet &curveSet)
		{ return myCurves == curveSet.myCurves; }
    
    // Builds topological info of set.
    void		 buildNodeList(float tol = 1e-3);

    GU_CurveNode	*getHeadNode() const { return myNodes; }

private:
    // Eliminates topological information of set.
    void		 clearNodeList();

    GU_CurveNode	*findNode(const UT_Vector3 &p, 
				    float tol = 1e-3) const;
    GU_CurveNode	*createNode(const UT_Vector3 &p);

    // List of all curves that are present.
    UT_Array<GU_CurveSetInfo>	 myCurves;
    // Head of the node list.
    GU_CurveNode			*myNodes;
    // True if we only use ends of curves.
    int					 myUseEnds;
};

#endif

