/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ColliderBFA_h__
#define __SIM_ColliderBFA_h__

#include "SIM_API.h"

#include "SIM_Collider.h"
#include "SIM_BVTree.h"
#include "SIM_Names.h"
#include "SIM_Isect.h"
#include "SIM_Utils.h"

#include <GU/GU_DetailHandle.h>

#include <UT/UT_IntArray.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector3Array.h>

class SIM_BVTree;
class SIM_TriangulatedGeometry;
class BV_LeafIterator;
class SIM_Geometry;
class simBVCallback;


// TODO: these structures have a lot of overlap with the new SIM_ISect
// structure. They should probably be eliminated in favour of SIM_ISect at
// some stage.


struct sim4PointEvent
{
	 sim4PointEvent()
	 { }
	 sim4PointEvent(const SIM_Time &time, const UT_Vector3 &normal,
				fpreal distance)
	     : myTime(time),
	     myNormal(normal),
	     myDistance(distance)
	 { }

    SIM_Time	 myTime;
    /// In a Face-Vertex event, the normal is from the face to the vertex.
    ///   Points on the face should move in -normal direction and the
    ///   vertex should move in the +normal direction to prevent collision.
    /// In a Edge-Edge/Tri-Tri event, the normal points from prim0 to
    ///   prim1. Prim0 should move -normal and prim1 should move +normal
    ///   to prevent collision.
    UT_Vector3	 myNormal;
    fpreal	 myDistance;
};

struct simFaceVertexEvent : public sim4PointEvent
{
     simFaceVertexEvent()
     { }
     // Normals for this event point from the face to the vertex
     simFaceVertexEvent(const SIM_Time &time, const UT_Vector3 &normal,
				fpreal distance,
			        bool firstIsFace, int vid,
			        fpreal bary0, fpreal bary1)
	 : sim4PointEvent(time, normal, distance),
	 myFirstIsFace(firstIsFace),
	 myVid(vid)
     {
	 myBary[0] = bary0;
	 myBary[1] = bary1;
     }

     /// Retrieve a barycentric vector for the face's primitive
     UT_Vector3 getBaryV3() const
     {
	 return UT_Vector3(myBary[0], myBary[1], 1-myBary[0]-myBary[1]);
     }

    /// The face that's the "face" in this event is the first of the
    /// two primitives in this event.
    bool     myFirstIsFace;
    /// Vertex id
    int	     myVid;
    /// Barycentric position within the face
    fpreal   myBary[2];
};

struct simEdgeEdgeEvent : public sim4PointEvent
{
     simEdgeEdgeEvent()
     { }
     // Normals for this event point from edge0 to edge1
     simEdgeEdgeEvent(const SIM_Time &time, const UT_Vector3 &normal,
			      fpreal distance,
			      int eid0, int eid1,
			      fpreal bary0, fpreal bary1)
	 : sim4PointEvent(time, normal, distance)
     {
	 myEid[0] = eid0;
	 myEid[1] = eid1;
	 myBary[0] = bary0;
	 myBary[1] = bary1;
     }

    /// id of edge within primitive. Vertices are eid and (eid+1)%nvertices
    int	     myEid[2];
    /// Parameter showing where point is within edge. 0 corresponds to vertex
    /// e, and 1 corresponds to vertex (eid+1)%nvertices.
    fpreal   myBary[2];
};


/// This class implements a cloth-cloth collider based on Bridson's
/// 2002 paper (BriFedAnd02). We use it for several types of deformable
/// collisions: cloth-cloth, cloth-hair and hair-hair. We also use it for
/// thin plate rigid body collisions.
/// This class can only be used for collision detection. The cloth subclass
/// does the Bridson/Fedkiw/Anderson collision response.
///
/// For deformable objects, the input objects should be updated to the
/// end-of-step positions and velocities. This collider will proposes
/// changes to that end-of-step velocity which can then be used to obtain a
/// new end-of-step position. The start-of-step position is calculated by
/// assuming that explicit integration was used - i.e., by computing x - dt*v.
///
/// This class does both self-collision and object/object collision. It
/// supports "point fusing," where two points in the same object (or one
/// point in each of the two objects) are fused together into a single
/// virtual point. (Typically, the user sets that up using a hard DOPs
/// constraint.) For collision purposes, those points are treated as a
/// single point - so, if two edges collide, but they share a virtual
/// point as an endpoint, the collision will be ignored.
///
//
// TODO: handle non-manifold collisions. I'm not even sure what would
// happen if we tried to do them here. Some non-manifold structures are
// obviously impossible to do, but others are quite legitimate, like a
// T-shirt pocket.
class SIM_API SIM_ColliderBFA : public SIM_Collider
{
public:
    class		 simDetail;
    class		 simPrimPairEvent;

    /// Collision detection for deformable bodies. These bodies:
    /// 1) define velocity via a point attribute
    /// 2) need their bounding volume trees updated every step.
    void                 getCollisionsDeformable(
                             SIM_IsectArray &collisions,
                             SIM_Object &object, SIM_Object &affector,
                             fpreal tolerance,
                             const SIM_Time &startTime,
                             const SIM_Time &endTime,
                             bool doRepulsion) const
                         {
                             getCollisions(collisions, object, affector,
                                           tolerance, startTime, endTime,
                                           true, doRepulsion,
                                           NULL, NULL, NULL, NULL);
                         }

    /// Collision detection for rigid bodies. These bodies:
    /// 1) define velocity via start/end transforms
    /// 2) never need to update their bounding volume trees.
    /// 3) need proxy triangulated geometry, since the source geometry may
    ///    not be triangles.
    void                 getCollisionsRigid(
                             SIM_IsectArray &collisions,
                             SIM_Object &object, SIM_Object &affector,
                             fpreal tolerance,
                             const SIM_Time &startTime,
                             const SIM_Time &endTime,
                             const UT_DMatrix4 &startXformA,
                             const UT_DMatrix4 &startXformB,
                             const UT_DMatrix4 &endXformA,
                             const UT_DMatrix4 &endXformB) const
                         {
                             getCollisions(collisions, object, affector,
                                           tolerance, startTime, endTime,
                                           false, false,
                                           &startXformA, &startXformB,
                                           &endXformA, &endXformB);
                         }

protected:
    explicit		 SIM_ColliderBFA(const SIM_DataFactory *factory);
    virtual		~SIM_ColliderBFA();

    virtual SIM_BVTree::simBVTreeType
                         getBVTreeType() const;

    /// This method should create a BV_LeafIterator class. This class will
    /// iterate over the individual geometric entities to be used for OBB
    /// intersection - in most cases, each leaf will contain a single
    /// primitive from the geometry.
    /// The time used to retrieve the geometry is determined by the
    /// startPosOnly flag:
    ///   true: return the position at the start of the timestep - i.e.,
    ///         position at object - vel*timestep
    ///   false: return the positions at both the start and end of the
    ///          timestep.
    /// The leaf iterator should return points in simulation (world) space,
    /// not local object space.
    virtual BV_LeafIterator
			*createLeafIt(const SIM_Object &object,
				const SIM_Time &timestep,
			        bool startPosOnly) const = 0;

    /// Given a leaf iterator and the id of a leaf, convert the leaf id to
    /// a primitive id and (optionally) an edge id.
    virtual void	 convertLeafToPrim(
				const BV_LeafIterator &leafIt,
			        int leafId,
				int &outPrimId, int &outEdgeId) const = 0;

    /// This method returns a radius to expand the leaf's geometry for
    /// collision detection.  For example, if the leaf's geometry is a
    /// line segment, the expanded geometry used for collision detection
    /// will be a cylinder with hemisphere caps.
    virtual fpreal	 getLeafRadius(
				const BV_LeafIterator &leafIt,
			        int leafId) const = 0;

    /// Test if this class generates events located at the *centre* of the
    /// triangles/edges or at the *surface*. This choice usually depends on
    /// the model of collision response.
    virtual bool         getEventsAtSurface() const;

    void		 retrieveGeometry(const SIM_Object &object,
				GU_ConstDetailHandle &outGdh,
				const GU_Detail *&outGdp,
				UT_DMatrix4	*outEndTransform,
				const SIM_TriangulatedGeometry **tgeo) const;

    /// Return true if a face and point share a point.
    /// If the "fuse mapping" point attribute is present on both
    /// GU_Details, it is used to determine if two points are fused;
    /// If absent, sharing is only possible if the face and point belong to
    /// the same GU_Detail - i.e., self-collision.
    bool	         testFaceSharesPoint(const simDetail &sdpF,
					int pidF0, int pidF1, int pidF2,
					const simDetail &sdpV,
					int pidV) const;
    /// Return true if the two edges (defined by their endpoint ids) share
    /// an endpoint.
    /// If the "fuse mapping" point attribute is present on both
    /// GU_Details, it is used to determine if two points are fused;
    /// If absent, sharing is only possible if the face and point belong to
    /// the same GU_Detail - i.e., self-collision.
    bool	         testEdgesSharePoint(const simDetail &sdpA,
					int pidA0, int pidA1,
					const simDetail&sdpB,
					int pidB0, int pidB1) const;
    /// returns true iff tangling triangles should not be processed for
    /// collision detection
    /// The RBD collision detection uses this to allow objects that are
    /// tangled at the start of the timestep to untangle without the
    /// attempt to disentangle triggering a collision.
    virtual bool	 doCheckTangle() const
			 { return false; }

    virtual void	 intersectPrims(simPrimPairEvent &event,
		                simDetail &sdpA, simDetail &sdpB,
			        fpreal tolerance, bool doRepulsion,
			        bool selfCollision) const;
private:

    /// The heart of this class. It does the actual collision detection.
    void		 getCollisions(
				SIM_IsectArray &collisions,
                                SIM_Object &object, SIM_Object &affector,
				fpreal tolerance,
				const SIM_Time &startTime,
				const SIM_Time &endTime,
                                bool isDeformable,
                                bool doRepulsion,
				const UT_DMatrix4 *startXformA,
				const UT_DMatrix4 *startXformB,
				const UT_DMatrix4 *endXformA,
				const UT_DMatrix4 *endXformB) const;

    /// Test for intersection between a face and a point, and add events to
    /// the given event parameter. The face and point are drawn from
    //    Face: firstIsFace ? sdp0 primitive event.myPrimId[0]
    //                      : sdp1 primitive event.myPrimId[1]
    //    Vertex: firstIsFace ? sdp1 primitive event.myPrimId[1] vertex vid
    //			      : sdp0 primitive event.myPrimId[0] vertex vid
    /// If the searchInTimestep flag is set, the algorithm will search for
    /// any events between the start and end time; otherwise, only the
    /// positions at the start time will be considered.
    int			 intersectFaceVertex(simPrimPairEvent &event,
				simDetail &sdp0, simDetail &sdp1,
				bool firstIsFace, int vid,
				bool searchInTimestep,
				fpreal tol) const;

    /// Test for intersection between two edges, and add events to the
    /// given event parameter. The edges are drawn from:
    ///   A: sdp0 primitive event.myPrimId[0], edge eid0.
    ///   B: sdp1 primitive event.myPrimId[1], edge eid1.
    /// If the searchInTimestep flag is set, the algorithm will search for
    /// any events between the start and end time; otherwise, only the
    /// positions at the start time will be considered.
    int			 intersectEdgeEdge(simPrimPairEvent &event,
			        int eid0, simDetail &sdp0,
			        int eid1, simDetail &sdp1,
			        bool searchInTimestep,
			        fpreal tol) const;
    
    ///	Test for intersection of two *static* triangles at the beginning
    /// of the timestep (tangle), and add events to the given event parameters.
    /// Returns true if no dynamic collision 
    /// detection/response (Edge/Edge,Face/Vertex) is to be performed.
    bool		 intersectTriTri(simPrimPairEvent &event,
				const simDetail &sdp0, 
				const simDetail &sdp1,
				bool &searchInTimestep,
				fpreal tol) const;
    
    /// Retrieve a BV tree from the given object, either creating it or
    /// updating its extents using the given timestep.
    /// If the "startPosOnly" flag is not set, the BV tree will include
    /// leaf positions from both the start and end of the timestep;
    /// otherwise, it will include only the start of the timestep.
    SIM_BVTree		*getBVTree(BV_LeafIterator* &leafIt,
			           SIM_Object &object,
			           const SIM_Time &timestep,
			           bool startPosOnly,
                                   bool isDeformable) const;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_ColliderBFA, SIM_Collider);

    friend class simBVCallback;
};

class SIM_API SIM_ColliderBFA::simDetail
{
public:
    /// The difference between rigid/deformable is the presence/absence of
    /// tgeo data.
		     simDetail(int objectId,
			       const GU_Detail &gdp,
			       const SIM_TriangulatedGeometry *tgeo,
			       const UT_DMatrix4 &startXform,
			       const UT_DMatrix4 &endXform,
			       const SIM_Time &startTime,
			       const SIM_Time &endTime);

    int		     getObjectId() const
		     { return myObjectId; }
    const GU_Detail &getGdp() const
		     { return myGdp; }
    bool	     operator==(const simDetail &rhs) const
		     { return &myGdp == &rhs.myGdp; }

    const SIM_Time  &getStartTime() const
		     { return myStartTime; }
    const SIM_Time  &getEndTime() const
		     { return myEndTime; }
    const 
    SIM_Time	     getTimestep() const
		     { return myEndTime - myStartTime; }

    /// Get point id of vertex on a given face. Optimized for triangles.
    int              getTriPointId(int fid, int vid) const
                     {	
                        UT_ASSERT_P(fid >= 0);
                        UT_ASSERT_P(vid >= 0 && vid < 3);
                        return myTriPointIds(fid*3 + vid);
                     } 
    /// Unoptimized version of getTriPointId for arbitrary primitives.
    int              getPrimPointId(int fid, int vid) const;
    bool             hasTriEdgeIds() const
                     {
                         return myTriEdgeIds.entries() > 0;
                     }
    int              getTriEdgeId(int fid, int vid) const
                     {
                        UT_ASSERT_P(fid >= 0);
                        UT_ASSERT_P(vid >= 0 && vid < 3);
                        return myTriEdgeIds(fid*3 + vid);
                     }
    /// Retrieve the "simulation" point id (or "equation number")
    /// associated with the given point. This allows for testing if points
    /// from two different gdps are "fused" into a single point within the
    /// simulation. It is stored as an attribute on the GU_Detail.
    /// Collisions between fused points are ignored.
    /// If the attribute is absent, fusing is disabled.
    int              getFusePointId(int pid) const
                     {
                         return myFusePointIds(pid);
                     }
    bool             hasFusePointIds() const
                     {
                         return myFusePointIds.entries() > 0;
                     }

    UT_Vector3	     getDetailPointPos(int pid) const;

    const UT_Vector3&getPointVel(int pid) const
                     { return myVel(pid); }
    float	     getPointMass(int pid) const;
    const UT_Vector3&getPointStartPos(int pid) const
                     { return myStartPos(pid); }
    const UT_Vector3&getPointEndPos(int pid) const
                     { return myEndPos(pid); }
    UT_Vector3	     getPointPos(int pid, const SIM_Time &time) const;
    UT_Vector3	     getTriBaryPos(int primId,
				   const UT_Vector3 &baryV3,
				   const SIM_Time &time) const;
    UT_Vector3	     getTriBaryVel(int primId,
				   const UT_Vector3 &baryV3) const;
    fpreal	     getTriBaryMass(int primId,
				    const UT_Vector3 &baryV3) const;
    /// Get a position of a point somewhere on the edge of a triangle.
    UT_Vector3	     getTriEdgePos(int primId, int eid, fpreal bary,
				   const SIM_Time &time) const;
    /// Optimized variant of getTriEdgePos for non-triangular primitives.
    UT_Vector3       getPrimEdgePos(int primId, int eid, fpreal bary,
                                    const SIM_Time &time) const;
    UT_Vector3	     getTriStartBaryCenter(int primId) const;

    /// Flag that point pidB of sdpB has been tested against primIdA from
    /// this simDetail. Returns true if it had already been tested before.
    bool             setPointChecked(int primIdA, const simDetail &sdpB,
                                     int pidB);
    /// Flag that edge (pidA0,pidA1) of this detail has been tested against
    /// edge (pidB0,pidB1) of sdpB. Returns true if it had already been
    /// tested before.
    bool             setEdgeChecked(int eidA, const simDetail &sdpB, int eidB);

protected:
    /// If mass attribute is missing (e.g., solid object), infinity is assumed.
    /// TODO: change this?
    GA_ROHandleF    			 myAttMass;

private:
    int		     			 myObjectId;
    const GU_Detail 			&myGdp;
    SIM_EdgeData                         myEdgeData;
    /// This array contains a lookup for the edge ids (in myEdgeData) for a
    /// given primitive.
    ///   eid = myTriEdgeIds[primId*3 + i]
    /// defines the index into myEdgeData for edge i of primitive primId.
    UT_IntArray                          myTriEdgeIds;
    const SIM_TriangulatedGeometry 	*myTgeo;	    
    UT_DMatrix4	     			 myStartXform, myEndXform;
    SIM_Time	     			 myStartTime;
    SIM_Time	     			 myEndTime;

    /// We precompute these because they're used repeatedly by different
    /// primitives+different times within the substep, and they're
    /// expensive to compute (due to transformation required, etc.)
    // @{
    UT_Vector3Array                      myStartPos;
    UT_Vector3Array                      myEndPos;
    UT_Vector3Array                      myVel;
    UT_IntArray                          myTriPointIds;
    UT_IntArray                          myFusePointIds;
    // @}
    
    /// Entry "i" of this array is a list of the point ids that have already
    /// been collided with primitive #i.
    UT_Array<UT_IntArray>             myPointsChecked;

    /// Entry "i" of this array is a list of the edges that have already
    /// been collided with edge #i.
    UT_Array<UT_IntArray>             myEdgesChecked;
};

/// A single potential-collision event involving a pair of primitives.
class SIM_API SIM_ColliderBFA::simPrimPairEvent
{
public:
		     simPrimPairEvent(int primId0, int primId1, 
				      int edgeId0, int edgeId1,
				      fpreal radius0, fpreal radius1,
                                      int &adjacent);

    int		     myPrimId[2];
    int		     myEdgeId[2];
    fpreal	     myRadii[2];
    /// A flag to indicate that the two primitives are adjacent
    /// (i.e., share one or more vertices)
    int             &myAdjacent;
    UT_Array<simEdgeEdgeEvent>	myEdgeEdgeEvents;
    UT_Array<simFaceVertexEvent>	myFaceVertexEvents;
    UT_Array<SIM_Isect> 		myTriTriEvents;
};


#endif
