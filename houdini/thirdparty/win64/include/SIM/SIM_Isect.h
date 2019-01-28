/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Isect_h__
#define __SIM_Isect_h__

#include "SIM_API.h"

#include "SIM_Time.h"
#include <UT/UT_PriorityQueue.h>
#include <GU/GU_Detail.h>

/// This class defines various types of collisions.
class SIM_API SIM_Isect
{
public:
    /// Information about one half of the collision (wrt one of the two
    /// entities involved)
    struct simIsectInfo
    {
	/// ID of object involved in the collision
	int		 myObjectId;

	/// World-space position of deepest point of intersection
	UT_Vector3	 myPosition;

	/// The following is a discriminating union describing an alternate
	/// representation of the deepest point of intersection
	enum simIsectInfoType
	{
	    NONE,
	    PRIM_EDGE_U,
	    PRIM_UV,
	    POINT
	} myType;

	union
	{
	    struct
	    {
		int myPrimitiveId;
		int myEdgeId;
		fpreal myU;
	    } myPEU;
	    struct
	    {
		int myPrimitiveId;
		fpreal myU;
		fpreal myV;
	    } myPUV;
	    struct
	    {
		int myPointId;
	    } myPt;
	};

	                     simIsectInfo() { }

        /// Named constructors
        // @{
        static simIsectInfo  makeNone(int obj_id, const UT_Vector3 &position)
                             {
                                return simIsectInfo(obj_id, position, NONE);
                             }
        static simIsectInfo  makeEdgeU(int obj_id, const UT_Vector3 &position,
                                      int primId, int eid, fpreal u)
                             {
                                simIsectInfo result(obj_id, position,
                                                    PRIM_EDGE_U);
                                result.myPEU.myPrimitiveId = primId;
                                result.myPEU.myEdgeId = eid;
                                result.myPEU.myU = u;
                                return result;
                             }
	static simIsectInfo  makePrimUV(int obj_id, const UT_Vector3 &position,
				       int primid, fpreal u, fpreal v)
			     {
				simIsectInfo result(obj_id, position, PRIM_UV);
				result.myPUV.myPrimitiveId = primid;
				result.myPUV.myU = u;
				result.myPUV.myV = v;
				return result;
			     }
	static simIsectInfo  makePoint(int obj_id, const UT_Vector3 &position,
				      int pointid)
			     {
				simIsectInfo result(obj_id, position, POINT);
				result.myPt.myPointId = pointid;
				return result;
			     }
        // @}
        
    private:
                            simIsectInfo(int obj_id, const UT_Vector3 &position,
                                         simIsectInfoType type)
                                : myObjectId(obj_id), myPosition(position),
                                  myType(type)
                            { }
    };



             SIM_Isect() {}
    // Default copy constructor is fine.
    //       SIM_Isect(const SIM_Isect &);
    
             SIM_Isect(const UT_Vector3 &normal,
                       fpreal64 depth,
                       const SIM_Time &time,
                       const simIsectInfo &objectInfoA,
                       const simIsectInfo &objectInfoB)
                : myDepth(depth),
                  myNormal(normal),
                  myTime(time),
                  myObjectInfoA(objectInfoA),
                  myObjectInfoB(objectInfoB)
             { }
             SIM_Isect(const UT_Vector3 &position, 
                       const UT_Vector3 &normal,
                       const SIM_Time &time,
                       fpreal64 depth,
                       int obj_a, int obj_b) 
                : myDepth(depth),
                  myNormal(normal),
                  myTime(time),
                  myObjectInfoA(simIsectInfo::makeNone(obj_a, position)),
                  myObjectInfoB(simIsectInfo::makeNone(obj_b, position))
             { }

    bool     isFaceVertex() const
    {
        return (myObjectInfoA.myType == simIsectInfo::PRIM_UV &&
                myObjectInfoB.myType == simIsectInfo::POINT) ||
               (myObjectInfoB.myType == simIsectInfo::PRIM_UV &&
                myObjectInfoA.myType == simIsectInfo::POINT);
    }
    bool     isEdgeEdge() const
    {
        return myObjectInfoA.myType == simIsectInfo::PRIM_EDGE_U &&
               myObjectInfoB.myType == simIsectInfo::PRIM_EDGE_U;
    }


    /// Depth of the intersection. The value is expected to be negative.
    // TODO: why? That's bizarre.
    // TODO: rename to a "signed distance" to intersection point.
    fpreal64	 myDepth;

    /// Direction object A needs to move to resolve the intersection
    UT_Vector3	 myNormal;

    /// Time of intersection.
    SIM_Time     myTime;

    /// Information about the point of intersection on object A
    simIsectInfo myObjectInfoA;

    /// Information about the point of intersection on object B
    simIsectInfo myObjectInfoB;
};

/// This class provides standard PriorityQueue Comparison for SIM_Isect Class
class SIM_API SIM_IsectCompare
{
public:
    bool operator()(const SIM_Isect *a, const SIM_Isect *b) const
    { return a->myDepth > b->myDepth; }
};

typedef UT_PriorityQueue<SIM_Isect *, SIM_IsectCompare, false>
					    SIM_IsectPriorityQueue;
typedef UT_Array<SIM_Isect> SIM_IsectArray;

#endif
