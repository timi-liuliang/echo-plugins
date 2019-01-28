/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __UT_Collider_h__
#define __UT_Collider_h__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include "UT_Vector3.h"

/// This is a class for various primitive collisions 
//
// TODO: it really doesn't belong in UT. It could possibly go in BV, since
// it's kind of a "follow-up" stage after using a bounding-volume
// hierarchy.
class UT_API UT_Collider
{
public:
       
    /// Find times when the given four moving points are coplanar. Only
    /// returns times between 0 and timestep. Returns the number of times
    /// found (maximum of three), or -1 if the points are coplanar for the
    /// entire timestep.
    static int findTimes4PointsCoplanar(const UT_Vector3 *pos[4],
					const UT_Vector3 *vels[4],
					fpreal times[3],
					fpreal timestep);

    /// Find times when the three given moving points are collinear. Only
    /// returns times between 0 and timestep. Returns the number of times
    /// found (maximum of two), or -1 if the points are collinear for the
    /// entire timestep.
    static int findTimes3PointsCollinear(const UT_Vector3 *pos[3],
                                         const UT_Vector3 *vels[3],
                                         fpreal times[2],
                                         fpreal timestep);

    /// Finds the first time when the given two moving points are within dist. 
    /// Only returns times between 0 and timestep. Returns the number of times
    /// found (either 0 or 1).
    ///
    /// Requires two points to be collinear for entire timestep, which
    /// makes it simpler and more robust than collideMovingPoints.
    static int findTimes2CollinearPointsCoincident(const UT_Vector3 &pos0,
                                         const UT_Vector3 &pos1,
                                         const UT_Vector3 &vel0,
                                         const UT_Vector3 &vel1,
                                         fpreal &time,
                                         fpreal timestep,
                                         fpreal dist);
    
    /// Finds the first time when the given two moving points are within dist. 
    /// Only returns times between 0 and timestep. Returns the number of times
    /// found (either 0 or 1).
    /// Warning: I'm not convinced this works for dist != 0.
    /// ... and in fact, it's probably fragile for dist != 0 too.
    static int	collideMovingPoints(const UT_Vector3 &pos1,
				    const UT_Vector3 &pos2,
				    const UT_Vector3 &vel1,
				    const UT_Vector3 &vel2,
				    fpreal &time,
				    fpreal timestep,
				    fpreal dist);
    
    /// Finds the first time when the given point and line are within dist.
    /// Only returns times between 0 and timestep.  Returns the number of times
    /// found (max of 1), or -1 if the point and the line are exactly 
    /// dist away for the entire timestep.  pos0 is the point.
    static int	collideMovingPointLine(const UT_Vector3 &pos0,
				    const UT_Vector3 &pos1,
				    const UT_Vector3 &pos2,
				    const UT_Vector3 &vel0,
				    const UT_Vector3 &vel1,
				    const UT_Vector3 &vel2,
				    fpreal &time,
				    fpreal timestep,
				    fpreal dist);

    /// Finds the first time when the given lines are within dist of eachother.
    /// Only returns times between 0 and timestep.  Returns the number of times
    /// found (max of 1), or -1 if the lines are exactly dist away for the
    /// entire timestep.  <pos0, pos1> forms one line, <pos2, pos3> forms the
    /// other
    static int	collideMovingLineLine(const UT_Vector3 &pos0,
				    const UT_Vector3 &pos1,
				    const UT_Vector3 &pos2,
				    const UT_Vector3 &pos3,
				    const UT_Vector3 &vel0,
				    const UT_Vector3 &vel1,
				    const UT_Vector3 &vel2,
				    const UT_Vector3 &vel3,
				    fpreal &time,
				    fpreal timestep,
				    fpreal dist);

};

#endif


