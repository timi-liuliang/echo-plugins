/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Describes a set of intersection curves on surfaces.
 *	Handles additions of new curves, points to curves, etc.
 *	Standard type is UT_Vector4, where:
 *	(x, y, z, w) ~ (u1, v1, u2, v2)
 *
 */

#ifndef __GU_IsectCurveSet_h__
#define __GU_IsectCurveSet_h__

#include "GU_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_Vector4.h>
#include <UT/UT_BoundingRect.h>

class GA_PrimitiveGroup;
class GEO_Face;
class GEO_Profiles;
class GEO_TPSurf;
class GD_TrimLoop;
class GU_IsectNode;
class GU_IsectCurveSet;
class GU_IsectPoint;
class GU_Detail;

class GU_API GU_IsectCurve
{
public:
    GU_IsectCurve();
    GU_IsectCurve(int surface, GD_TrimLoop *loop);
    ~GU_IsectCurve();

    // Add a point to the curve.
    void 	append(const UT_Vector4 &point);
    // Append another curve:
    // These all steal the poinstfrom the provided curve!!!
    void 	append(GU_IsectCurve &curve);
    void 	appendReverse(GU_IsectCurve &curve);
    // Same, but prepend:
    void 	prepend(GU_IsectCurve &curve);
    void 	prependReverse(GU_IsectCurve &curve);
    // Adds curve, but copies points.
    void 	appendCopy(GU_IsectCurve &curve);

    // Reparameterizes all the points in the curve from 0,1 into
    // start, start+length:
    void 	 reparameterize(const UT_Vector4 &start, 
				const UT_Vector4 &length);

    // Accumulates into area based upon dir.
    void	 getArea(float &areaxy, float &areazw, int dir) const;

    // Accumulates into length based upon euclidean length
    void	 getLength(float &lengthxy, float &lengthzw) const;
				
    // Returns euclidean length of curve along specified surface
    // (In terms of domain space though!)
    float	 getLength(int surface) const;
				
    int		 numPoints() const { return myNumPts; }

    // Adds this curve to the set, making the appropriate nodes, etc.
    void	 linkInto(GU_IsectCurveSet &curveset, const UT_Vector4 &tol);
    // Removes links to nodes.
    void	 unLink() { myNodes[0] = myNodes[1] = 0; }

    // Empties the curve.
    // Deletes all points in the curve.
    void	 empty();

    // Only works if compact has been called on the curveset.
    int		 isClosed() const 
		 { return (myNodes[0] && (myNodes[0] == myNodes[1])); }

    UT_Vector4	&operator()(int i);
    UT_Vector4   operator()(int i) const;
    
    // Not a very intelligent == operator, something is required for 
    // NT build though.
    int		operator==(const GU_IsectCurve &curve)
		{ return myPoints[0] == curve.myPoints[0]; }

    GU_IsectNode *getNode(int idx) const { return myNodes[idx]; }
    void	 setNode(int idx, GU_IsectNode *node) {myNodes[idx] = node;}
    int 	 getEndPoint(int idx, UT_Vector4 &pt) const;
    int 	 setEndPoint(int idx, const UT_Vector4 &pt);

    // Finds which direction this curve should be.
    void	 findDirection(const GEO_TPSurf &surfxy,
			       const GEO_TPSurf &surfzw,
			       int signxy = 1, int signzw = 1);
    
    // surfside is 0 for XY, 1 for ZW surface.  Result is 1 if
    // surface should be generated in forward direction, else
    // -1.
    int		 getDirection(int surfside) const
		 { return myDirection[surfside]; }
    void	 setDirection(int surfside, int dir)
		 { myDirection[surfside] = dir; }

    // Creates a space curve from the curve, on the specified surface.
    // The given surface is assumed to be correct.
    GEO_Face	*createSpaceCurve(GU_Detail *gdp, int surface,
				    GEO_TPSurf *surf, 
				    GEO_TPSurf *base1, GEO_TPSurf *base2, 
				    GEO_Face *&edge1, GEO_Face *&edge2, 
				    GEO_Face *&prof1, GEO_Face *&prof2, 
				    float tol = 1e-3F) const;

    // Creates a profile out of this curve, in the direction specified
    // by myDirection[surface].  Generates it into the specified profile
    // set.
    void	 createProfile(int surface, int direction,
				GEO_Profiles &profs,
			        GA_PrimitiveGroup *profgroup = 0,
				float tol = 1e-3F) const;

    // Creates a profile, and generates trim regions from it
    GD_TrimLoop *createTrimmedProfile(int surface, int direction,
				  GEO_Profiles &profs, 
				  GA_PrimitiveGroup *profgroup = 0,
				  float tol = 1e-3F) const;

    // Create a trim loop:
    GD_TrimLoop *createTrimLoop(int surface, int direction, int parameter,
				float tol = 1e-3F) const;

private:
    // 0 is start of point list, 1 is end.
    GU_IsectPoint		*myPoints[2];
    GU_IsectPoint		*myLastPoint;
    // Start and end nodes, indexed 0 and 1 resp.
    GU_IsectNode		*myNodes[2];
    // Direction of curve along two surfaces.
    // 0 is for XY, 1 is for ZW.
    int				 myDirection[2];
    int				 myNumPts;
};

class GU_API GU_IsectCurveSet
{
protected:
    // A sanity test on the curve net work.  Useful for tracking down
    // orphaned curves, etc.  Returns 1 if all is fine, writes an error
    // to stderr and returns 0 otherwise.
    int			verifyNetwork() const;
public:
    GU_IsectCurveSet();
    ~GU_IsectCurveSet();

    // Add in a new curve, searching this set for any existant
    // curves that could be merged in:
    // Both of these STEAL the points/curves from the source set/curve.
    void 		merge(GU_IsectCurve *curve);
    // Add in another curveset, again merging nearby curves:
    void 		merge(GU_IsectCurveSet &curveset);
    // And this one will duplicate the curve, but not the points.
    void 		merge(GU_IsectCurve &curve);
    // This constructs copies of the requested curves, and does not steal
    void		mergeCopy(GU_IsectCurve *curve);
    void		mergeCopy(const GU_IsectCurveSet &curveset);
    
    int  		isEmpty() const 
				{ return (myCurves.entries()) ? 0 : 1; }
    int			numCurves() const { return (int)myCurves.entries(); }

    // Clears all local information without destroying the refarray.
    void		reset();
    void		setSurfaces(GEO_TPSurf *surfxy, GEO_TPSurf *surfzw)
			{ mySurfXY = surfxy; mySurfZW = surfzw; }

    // Reparameterizes all the points in the curve from 0,1 into
    // start, start+length:
    void reparameterize(const UT_Vector4 &start, const UT_Vector4 &length);

    // NB: This CAN be NULL, in which case the curve should be ignored
    // as it was eliminated in a collapse.
    GU_IsectCurve	*&operator()(int i) { return myCurves((unsigned)i); }
    GU_IsectCurve	 *operator()(int i) const { return myCurves((unsigned)i); }

    // Not a very intelligent == operator, something is required for 
    // NT build though.
    int		operator==(const GU_IsectCurveSet &curveSet)
		{ return myCurves == curveSet.myCurves; }
    
    GU_IsectNode	*findNode(const UT_Vector4 &p, 
				    const UT_Vector4 &tol) const;
    GU_IsectNode	*findNearestNode(const UT_Vector4 &p, int surface,
				    float tol, GU_IsectNode *ignore) const;
    GU_IsectNode	*createNode(const UT_Vector4 &p);

    // Reduces number of curves in curveset by generating vertex list
    // and merging all the adjacent curves.  Uses tol to judge identical
    // points, and looptol to decide if a loop is degenerate and should
    // be replaced by a line.
    // xtol & ytol are the tolerances for identicalness on the
    // base curve, likely MUCH larger than tol.
    // If ignroe dir is false, only connect pieces whose directions
    // are copacetic.
    void		 compact(const UT_Vector4 &tol, float looptolxy,
				float looptolzw, int ignoredir=1);

    // Takes compacted curveset & handles brachiations & tachnodes,
    // creating closed curves where possible.
    // Returns 0 if curveset unchanged.
    int			 resolveTachnodes(int surface, float tol);

    // Sets the direction fields of all curves:
    void		 findCurveDirections(int signxy = 1, int signzw = 1);

    // Generates profile curves on the specified surface.
    // 0 will generate profile on XY surface, 1 on ZW surface.
    // trim determines if profiles will be generated along entire isect
    // curve, or only those not already trimmed out on the cutting
    // surface
    // Direction = 1 will generate according to results of
    // findCurveDirections, -1 will do so oppositely.
    void		 generateProfiles(int surface, int trim = 0,
				    int direction = 1,
				    GA_PrimitiveGroup *profgroup = 0,
				    float tol = 1e-3F,
				    bool *buildloop = 0) const;

#if 0
    void		 generateTrimProfs(const UT_BoundingRect &brect,
				    int surface, int direction,
				    float tol = 1e-3F) const;
#endif

    // Stretches all points to the domain all the way over.
    void		 stretchToDomain(int surface, 
			    const UT_BoundingRect &rect, float tol);

    // Cuts all our curves by the specified trim loop
    void		 cutByTrimLoops(int surface, const GD_TrimLoop *cutter,
					int inside = 1);
private:
    // Eliminates topological information of set.
    void		 clearNodeList();

    // Builds topological info of set.
    void		 buildNodeList(const UT_Vector4 &tol);

    // Collapses degenerate loops
    int 		 mergeLoops(float looptolxy, float looptolzw);

    // Joins adjacent edges
    int 		 joinAdjacent(int ignoredir);
    
    // List of all curves that are present.
    UT_ValArray<GU_IsectCurve *>	 myCurves;
    // Head of node list.
    GU_IsectNode			*myNodes;
    // "Owner" surfaces:
    GEO_TPSurf				*mySurfXY, *mySurfZW;
};

#endif
