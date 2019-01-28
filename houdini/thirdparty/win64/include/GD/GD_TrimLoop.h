/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This class handles trimming loops.  Each loop is a series of
 * bezier segments which in totality is closed.  Loops themselves
 * are embedded in a linked list of loops.
 * 
 */

#ifndef __GD_TrimLoop_h__
#define __GD_TrimLoop_h__

#include "GD_API.h"
#include <UT/UT_Defines.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_BoundingRect.h>
#include <UT/UT_Vector2.h>

class GD_TrimPiece;
class GD_TrimHitInfo;
class gdTrimIntersection;
class GD_Detail;

class GD_API GD_TrimLoopFlags
{
public:
	GD_TrimLoopFlags()	{ closed = 0; }
	unsigned	  closed:1,
			  clockwise:1;
};

enum GD_IsoparmDirection
{
    GD_YISOPARM	= 1,
    GD_XISOPARM = 0
};

enum GD_DomainBoundary
{
    GD_UMIN	= 0,
    GD_VMIN	= 1,
    GD_UMAX	= 2,
    GD_VMAX	= 3
};

enum GD_TrimRule
{
    GD_TrimWindingRule		= 0,
    GD_TrimAlternatingRule	= 1
};


class GD_API GD_TrimLoop
{
public:
    // Constructor which creates an empty loop.
    GD_TrimLoop();

    // Generates curve out of a list of pieces.
    GD_TrimLoop(GD_TrimPiece *curve);

    // Destructor.  Will destroy this loop and ALL children/later siblings.
    ~GD_TrimLoop();

    int64 getMemoryUsage(bool inclusive) const;

    // Evaluates curves value at a specified parameter value. Returns 1 if 
    // successful and 0 otherwise.
    int			 evaluate(float u, UT_Vector2 &pos) const;
    int			 evaluate(float u, UT_Vector2 &pos, UT_Vector2 &der) 
				    const;
    int			 evaluateHead(UT_Vector2 &pos) const;
    int			 evaluateTail(UT_Vector2 &pos) const;

    // Find approximate normal. normal is NOT normalized!
    void		 evaluateNormal(UT_Vector3 &normal) const;

    // Find whether clockwise or counter clockwise:
    void		 findDirection();

    // Is the loop closed?  Result of cuts are not closed.
    unsigned		 isClosed() const { return flags.closed; }

    // Close up the loop.
    void		 close(int connect_ends = 0);

    // Find length:
    float		 length() const;

    // Is the loop clockwise?  Ie: is it trim out?
    unsigned		 isClockwise() const { return flags.clockwise; }

    // Appends given loop to our loop list
    void		 appendLoop(GD_TrimLoop *loop);

    // Appends the given loop to our loop piece list, then deletes the loop
    // if the delete flag is set.
    void		 append(GD_TrimLoop *loop,  int consolidate = 0,
						    int deleteloop  = 0,
						    int join = 0);

    // Appends a point to our loop through the simple expedient of 
    // generating a new GD_TrimPiece
    void		 appendPoint(float u, float v);

    // Append at point of intersection.  If no intersection, we return zero
    // and don't append.
    int			 appendAtIntersect(GD_TrimLoop *loop, int deleteloop,
					   float tol=1E-4F);

    // Append the given piece to our loop.
    void		 append(GD_TrimPiece *piece);

    // Return a copy of myself & my siblings, or just myself. The second
    // method erases the child and next information.
    GD_TrimLoop		*copy() const;
    GD_TrimLoop		*copyThis() const;

    // Reverse the loop orientation for myself & my siblings, or just myself.
    void		 reverse();
    void		 reverseThis();

    //	1 if given object is contained.  Assumes no intersections
    // Always zero if this is open.
    // Only refers to this trim loop.
    // Returns -1 if point/object is within tolerance.
    // Jitter version tries up to three points above and below the given
    // point until a consensus is reached.  It is used by the primitive
    // method.
    int			 isInsideJitter(const UT_Vector2 &pt, float tol = 1e-4F,
					float dither = 1e-5F) const;
    int			 isInside(const GD_TrimLoop &prim, float tol = 1E-4F) 
								const;
    int			 isInside(const UT_Vector2 &pt, float tol = 1E-4F) 
								const;

    // Returns one if point contained by this set of trim loops.
    int			 isTrimmedIn(const UT_Vector2 &pt, 
				 GD_TrimRule rule = GD_TrimWindingRule) const;
    int			 isTrimmedIn(float u, float v,
				 GD_TrimRule rule = GD_TrimWindingRule) const;
    // The valid loop list acts as a mask of which loops can be tested.
    // They are ascending, and built from setLoopIds.
    int			 isTrimmedIn(const UT_Vector2 &pt,
				 UT_IntArray &validloops,
				 GD_TrimRule rule = GD_TrimWindingRule) const;
    int			 isTrimmedIn(float u, float v,
				 UT_IntArray &validloops,
				 GD_TrimRule rule = GD_TrimWindingRule) const;
    
    // Uses the slower, jittered method to ensure more accurate testing
    int			 isTrimmedInJitter(const UT_Vector2 &pt,
				 GD_TrimRule rule = GD_TrimWindingRule) const;
    int			 isTrimmedInJitter(float u, float v,
				 GD_TrimRule rule = GD_TrimWindingRule) const;
    int			 isTrimmedIn(GD_TrimLoop *loop,
				 GD_TrimRule rule = GD_TrimWindingRule) const;

    void		 getParameterRange(float &umin, float &umax) const;

    // Returns distance in parameter space, closure aware.
    float		 parametricDist(float u1, float u2) const;
    // Returns length from u1 to u2 in increasing direction,
    // closure aware.
    float		 parametricLength(float u1, float u2) const;

    // Cuts out a piece of the trim loop.  Resulting loop is open.
    GD_TrimLoop		*cut(float u1, float u2) const;

    // Cuts family of loops on isoparm, generating two copies, right & left.
    // Acts in place, so copy before hand if req'd
    void		 cutAtIsoparm(GD_TrimLoop **left, GD_TrimLoop **right,
					GD_IsoparmDirection iso, float val,
					float tol = 1e-4f);

    // Cuts family of loops into inside/outside components, as
    // determined by the cutter group:
    void		 cutAtTrim(GD_TrimLoop **inside, GD_TrimLoop **outside,
				    const GD_TrimLoop *cutter, 
				    GD_TrimLoop *source);

    // Intersects all the pieces, returning a list of hits.
    int			 intersect(const GD_TrimLoop &loop,
				UT_Array<GD_TrimHitInfo> &hitlist,
				float tol = 1E-4F) const;

    // Intersects with all of this pieces, giving hit list
    int			 intersect(GD_TrimPiece &piece,
				UT_Array<GD_TrimHitInfo> &hitlist,
				float tol = 1e-4F) const;

    // Intersects with all of loops hiearchy, giving hit list
    int			 intersectAll(const GD_TrimLoop &loop,
				UT_Array<GD_TrimHitInfo> &hitlist,
				float tol = 1e-4F) const;

    // Intersects all pieces with the domain boundaries. The list is sorted
    // in t:
    int			 intersectDomain(const UT_BoundingRect &brect,
					 UT_Array<GD_TrimHitInfo> &hitlist,
					 float tol = 1E-4F) const;

    // Cuts at all boundary isoparms and returns a new loop (a new set of
    // loops). Acts in place. Returns itself if the loop is closed or no
    // intersection with the domain boundary is found.
    GD_TrimLoop		*domainClip(const UT_BoundingRect &brect,
				float tol = 1E-4F, int preservedirection = 1);

    // Handles incremental tesselation of loop:
    int			 startWalk(float inc, float minstep, float maxstep,
				    float &u, float &v);
    // Returns 1 if more points.
    int			 doWalk(float &u, float &v);
    void		 endWalk();

    // Return all the loop data (including its siblings and kids) into the
    // arrays provided. The return value has the number of loops. The arrays
    // will not have the number of entries. This is for rib conversion mostly.
    int			 getData(UT_IntArray &ncurves,
			     UT_IntArray &order, UT_IntArray &ncvs,
			     UT_FloatArray &knots,
			     UT_FloatArray &min, UT_FloatArray &max,
			     UT_FloatArray &u, UT_FloatArray &v,
			     UT_FloatArray &w) const;

    // set all the loop ids.
    int 		 setLoopIds(int id = 0);
    int			 getId() const { return myId; }
    
    GD_TrimLoop		*getNext() const { return myNext; }
    GD_TrimLoop		*getChild() const { return myChild; }

    GD_Detail		*getDetail() const { return myDetail; }
    void		 setDetail(GD_Detail *gdp) { myDetail = gdp; }

    // Altitude is the "sea level" were trimmed in/out boundaries
    // occur.
    GD_TrimLoop		*preprocessLoops(GD_TrimRule rule, int altitude=0);

    // Finds maximum trimmed in height.
    int			 getMaxHeight() const;

    // Finds maximum trimmed in height.
    int			 getBaseDir() const;

    // Makes all trim loops be on the same level.
    GD_TrimLoop		*flatten();

    void		 print(int i = 1, int recurse = 1) const;

    // Returns whether the head is sufficiently close (within "tol") to the
    // tail.
    int			 isHeadNearTail(float tol=1E-4F) const;

    // Test whether the loop's head/tail is near the bound of the rectangle.
    // Returns 1 if it's near the bound (and inserts/appends the hit info
    // into the list.
    int			 hitHead(const UT_BoundingRect &brect, 
				 UT_Array<GD_TrimHitInfo> &hitlist, 
				 float tol = 1E-4F) const;
    int			 hitTail(const UT_BoundingRect &brect, 
				 UT_Array<GD_TrimHitInfo> &hitlist, 
				 float tol = 1E-4F) const;

    // Bridge two points, posa and posb, that lie on the domain boundary, with
    // an open loop.  "brect" is the bounding rectangle for the domain of the 
    // surface to be cut, while "bbox" is the bounding rectangle that contains 
    // both the curve and the domain.  We would build the bridge outside bbox
    // to avoid building a self intersecting curve.  "tol" is the desired 
    // distance between bbox and the bridge.
    // If always counter is true, we always build in counter clockwise
    // direction
    static GD_TrimLoop	*domainBridge(const UT_BoundingRect &brect,
			 const UT_Vector2 &posa, float ua, GD_DomainBoundary a,
			 const UT_Vector2 &posb, float ub, GD_DomainBoundary b,
			 float tol, const UT_BoundingRect &bbox,
			 int alwayscounter = 0);

    // Get approximately "numdivs" number of points.  if the flag "usebreak"
    // is set, then, use the "evaluateBreakSeg" to get breakpoints along with
    // other points.  Otherwise, just get numdiv+1 points.
    int			getPoints(int numdivs, UT_Vector3Array &domain,
				  int usebreak);

    /// Walks over all trim pieces of this loop. Pass NULL to it in the first call,
    /// and then the current piece in all subsequent calls. Once the traversal
    /// reaches the end, a NULL pointer is returned.
    GD_TrimPiece*	getPiece(GD_TrimPiece* prev_piece) const;

protected:
    // For all loops in the network, ensure all internal pieces match
    // up exactly.
    void		snapPieces();

    // This will transform the loop list into one which is winding
    // compliant.  Ie: No loops intersect (except at tangents), and
    // the loop hiararchy has trim in loops at the top level followed
    // by trim out loops as children thereof.
    // NB: May delete this, returns new root.
    GD_TrimLoop 	*applyWindingRule(int isfragmented = 0,
					    int altitude = 0,
					    int *maxdepth = 0,
					    int *basedir = 0);

    // Intersect two provided loops generating a new list of loops
    // comprising the resulting pieces.  Takes into account whether the
    // sources are in the same direction.
    // Returns NULL if no intersections.
    GD_TrimLoop		*fragment(const GD_TrimLoop *loop) const;

    // Intersects all pieces with the isoparm provided:
    int			 intersect(float val, GD_IsoparmDirection isoparm,
				UT_Array<GD_TrimHitInfo> &hitlist,
				float tol = 1E-4F) const;

    // Return the number of hits after post processing the hit list generated
    // by intersecting with an isoparm.
    int			 processIsoparmHits(UT_Array<GD_TrimHitInfo> &hits,
					    int hit, float tol = 1E-4F) const;

    // Intersect with one isoparm and do internal stuff...
    void		 doIsoparm(int isoparm, float val, float tol,
				   GD_DomainBoundary code,
				   UT_Array<GD_TrimHitInfo> &hitlist,
				   int &hit, int &i) const;

    // Adds loop as child if contained in this, otherwise as a sibling.
    // NB: Loop's children are ignored and loops next pointer is destroyed.
    // Thus, both must be NULL before calling.
    // Returns new root, either this or loop.
    GD_TrimLoop		*addPossibleChild(GD_TrimLoop *loop);

    // Removes all degenerate loops.  May delete this.
    GD_TrimLoop		*removeDegenerateLoops(float tol=1E-4F);

    // Removes all redundant loops in this loop tree.  May delete this.
    GD_TrimLoop		*removeRedundantLoops();

    // Calculates depths of all loops
    void		 findDepths(int depth);

    // Finds most trimmed in depth.
    int			 findMaxDepth() const;

    // Finds trim direction of base loop:
    int			 getBaseTrimDir() const;

    // Adjusts all depths by delta depth.
    void		 adjustDepths(int deltadepth);

public:
    UT_BoundingRect	 myBBox;

    // We own these:
    GD_TrimPiece	*myTrimPieces;
    GD_TrimPiece	*myLastTrimPiece;	// Tail pointer
private:
    GD_TrimLoop		*myNext, *myChild;
    GD_Detail		*myDetail;
    GD_TrimLoopFlags	 flags;

    float		 myUStart, myULength;

    // Note we do NOT own this.
    gdTrimIntersection	*myNextIntersection;
    // If we are a result of a cut, which curves contributed.  Parent
    // is source of cut, other parent is what was cut by.  Latter must
    // be set by the cutter.
    // this != myParent->myChild!
    GD_TrimLoop const	*myParent;
    GD_TrimLoop	const 	*myOtherParent;
    int			 inOtherParent;		// Contained in other parent?
    int			 myId;		// Two loops with equal ids are
					// considered to not intersect.
    int			 myDepth;	// How deep this loop is.

    // Current state information for walk:
    GD_TrimPiece	*myWalkPiece;
    float		 myWalkU;
    float		 myWalkUInc, myWalkMinStep2, myWalkMaxStep2;
    float		 myWalkLastX, myWalkLastY;

    // State information for append to handle initial points:
    UT_Vector2		 myInitPoint;
    int			 myHasInitPoint;

    // Return a copy of myself only.
    GD_TrimLoop		*doCopy() const;
};

#endif
