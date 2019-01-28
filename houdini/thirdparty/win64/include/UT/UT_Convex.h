/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Convex.h ( UT Library, C++)
 *
 * COMMENTS:	This code does general polygon convexing.  It's down here
 *		in UT since there are places other than the GEO lib which
 *		may need to use convexing code (i.e. the RE lib, or UI).
 *		This code convexes 2D polygons.
 */

#ifndef __UT_Convex__
#define __UT_Convex__

#include "UT_API.h"
#include "UT_Vector3.h"
#include <SYS/SYS_Types.h>
#include <stdlib.h>

class UT_ConvexPoint;
class UT_Interrupt;

class UT_API UT_Convex {
public:
	     UT_Convex()
	     {
		 myData = 0;
		 myConvexInfo = 0;
		 myConvexSize = 0;
		 myConvexEntries = 0;
		 myPointList = 0;
		 myPointSize = 0;
	     }
    virtual ~UT_Convex()
	     {
		 if (myConvexInfo) free(myConvexInfo);
		 if (myPointList) free(myPointList);
	     }

    bool		isConcave();
    void		convex(exint maxsides, bool allowinterrupt = true,
                               bool avoiddegeneracy = false);

    virtual void	initialize();
    /// It's the sub-class responsibility to fill these out...
    virtual int		getPointCount() const = 0;
    virtual void	getPoint(int num, float &x, float &y) const = 0;

    /// beginPolygon will be told when the last polygon is being added
    virtual void	beginPolygon(int npoints, bool lastone) = 0;
    virtual void	polyVertex(int num) = 0;
    virtual void	endPolygon() = 0;

    /// If the last polygon to be added won't be added after all, e.g. if it is
    /// degenerate and avoiddegeneracy is true, the earlyTermination() method
    /// will be called.  This is so that the caller can clean up properly, since
    /// beginPolygon will *not* have been called with lastone==true.
    /// By default, this method does nothing.
    virtual void	earlyTermination();

    /// Returns true if splitting the specified quad along the line from
    /// point 0 to point 2 is possible
    static inline bool canSplitQuad02(const UT_Vector3 &p0,
                                      const UT_Vector3 &p1,
                                      const UT_Vector3 &p2,
                                      const UT_Vector3 &p3)
    {
        const UT_Vector3 v0 = p1 - p0;
        const UT_Vector3 v1 = p2 - p1;
        const UT_Vector3 v2 = p3 - p2;
        const UT_Vector3 v3 = p0 - p3;
        // Okay if normals at 1 and 3 point in the same direction
        return ((cross(v0, v1)).dot(cross(v2, v3)) > 0);
    }

    /// Returns true if the specified quad is convex.
    /// NOTE: This may return false negatives for highly non-planar quads,
    ///       since the definition of convex breaks down.
    static inline bool isQuadConvex(const UT_Vector3 &p0,
                                    const UT_Vector3 &p1,
                                    const UT_Vector3 &p2,
                                    const UT_Vector3 &p3)
    {
        const UT_Vector3 v0 = p1 - p0;
        const UT_Vector3 v1 = p2 - p1;
        const UT_Vector3 v2 = p3 - p2;
        const UT_Vector3 v3 = p0 - p3;
        // Convex if normals at 1 and 3 point in the same direction
        // and normals at 0 and 2 point in the same direction
        return (cross(v0, v1).dot(cross(v2, v3)) > 0)
            && (cross(v1, v2).dot(cross(v3, v0)) > 0);
    }

    virtual int64 getMemoryUsage(bool inclusive) const;

private:
    UT_ConvexPoint	*polySetup(bool avoiddegeneracy);
    int			leftOrRight(int v1, int v2, int v3, bool &double_back);
    int			leftOrRight(int v1, int v2, int v3)
			{
			    bool double_back;
			    return leftOrRight(v1, v2, v3, double_back);
			}
    uint		leftMost(uint a, uint b, int ccwise);
    bool		testConcave(uint first, uint last);
    bool		arePointsSame(uint i, uint j);
    bool		isDegenerate(uint first, uint last);
    bool		addPolygon(
				UT_Interrupt *boss, uint first,
				uint last, bool lastone,
                                bool avoiddegeneracy);
    void                addTriangle(uint a, uint b, uint c,
                                    bool lastone);
    void		doSplit(uint first, uint newfirst,
				uint newlast, uint last);
    bool		cornerTest(int side, uint la, uint lb,
				   uint first, uint last);
    bool		insideOut(uint k, uint la,
				  uint lb, uint first, uint last);
    uint		chooseCoincidentIntruder(
				uint l, uint k1, uint k2,
				uint first, uint last, uint ccwise);
    uint		findClosestInRange(uint lb, uint la,
				 uint ls, uint first, uint last);
    uint		testIntersect(uint l, uint lb, uint la,
				      uint first, uint last, int &ccwise);

protected:
    /// NOTE: You *must* set myData to NULL before
    ///       calling isConcave() or convex() !
    UT_ConvexPoint	*myData;

private:
    UT_ConvexPoint	*myConvexInfo;
    uint		 myConvexSize;
    uint		 myConvexEntries;
    int			*myPointList;
    uint		 myPointSize;

    /// NOTE: The only purpose of this bool is because isConvex() must call
    ///       polySetup(true), so if there were degenerate points and
    ///       convex(__, __, false) is then called, it needs to know to call
    ///       polySetup(false) to get those omitted points.
    bool                 myAvoidDegeneracyPrev;
};

#endif

