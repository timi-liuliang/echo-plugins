/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_PickRecord (C++)
 *
 * COMMENTS:
 *	Holds information about a single picked entity.
 * 
 */

#ifndef __GR_PickRecord_h__
#define __GR_PickRecord_h__

#include "GR_API.h"
#include <UT/UT_Assert.h>
#include <SYS/SYS_TypeDecorate.h>
#include <iosfwd>
#include <string.h>

// Entities we can pick:
#define	GR_PICK_GEOPOINT	0x00000001 // GEO_Point
#define	GR_PICK_GUIDEPOINT	0x00000002 // "guide" points (sphere, tube...)
#define	GR_PICK_GEOEDGE		0x00000004 // edge between two GEO_Points 
#define	GR_PICK_GUIDEEDGE	0x00000008 // "guide" edges (sphere, tube...)
#define	GR_PICK_PRIMITIVE	0x00000010 // Primitives
#define	GR_PICK_POINTNORMAL	0x00000020 // GEO_Point normal
#define	GR_PICK_PRIMNORMAL	0x00000040 // Primitive normals
#define GR_PICK_BREAKPOINT	0x00000080 // primitive break points
#define GR_PICK_MIDPOINT	0x00000100 // midpoint of a GEO_Edge
#define GR_PICK_VERTEX		0x00000200 // GEO_Vertex
#define GR_PICK_STATEPOINT	0x00000400 // point drawn by the current state
#define GR_PICK_GEOALL		0x000007ff // all of the geometry pick types

#define	GR_PICK_HANDLE		0x00000800
#define	GR_PICK_OBJECT		0x00001000
#define GR_PICK_XRAY_OBJECT	0x00002000

// WARNING: If you change these, update any matching defines in the $SHS/glsl
//	    directory or snapping to the construction plane will break.
#define	GR_PICK_FLOOREDGE	0x00010000
#define	GR_PICK_FLOORPOINT	0x00020000
#define	GR_PICK_UVTILEEDGE	0x00040000
#define	GR_PICK_UVTILEPOINT	0x00080000
#define GR_PICK_FLOORALL	0x000f0000 // all of the floor pick types

#define GR_PICK_ALL		0x000f1fff // Everything above

class GR_API GR_PickRecord
{
public:
		 GR_PickRecord()
		 { memset(myData, 0, sizeof(myData)); }
		 GR_PickRecord(const int *data)
		 { memcpy(myData, data, sizeof(myData)); }
		 GR_PickRecord(const int *base_data, const int *component_data)
		 {
		     memcpy(myData, base_data, sizeof(myData)/2);
		     memcpy(&myData[3], component_data, sizeof(myData)/2);
		 }
		 GR_PickRecord(const GR_PickRecord &base,
			       const int *component_data)
		 {
		     memcpy(myData, base.myData, sizeof(myData)/2);
		     if (base.isGeometry())
		     {
			 myData[3] = component_data[0] + 1;
			 myData[4] = component_data[1] + 1;
			 myData[5] = component_data[2] + 1;
		     }
		     else
			 memcpy(&myData[3], component_data, sizeof(myData)/2);
		 }

    void	 clear()
		 { memset(myData, 0, sizeof(myData)); }
    bool	 isSet() const
		 { return (myData[0] != 0); }
    bool	 operator==(const GR_PickRecord &cmp) const
		 { return (memcmp(myData, cmp.myData, sizeof(myData)) == 0); }
    bool	 operator!=(const GR_PickRecord &cmp) const
		 { return !(*this == cmp); }

    int		 getPickType() const
		 { return myData[0]; }
    bool	 isGeometry() const
		 { return (myData[0] & GR_PICK_GEOALL) != 0; }
    bool	 isHandle() const
		 { return (myData[0] & GR_PICK_HANDLE) != 0; }
    bool	 isObject() const
		 { return (myData[0]&(GR_PICK_OBJECT|GR_PICK_XRAY_OBJECT))!=0; }
    bool	 isFloor() const
		 { return (myData[0] & GR_PICK_FLOORALL) != 0; }

    int		 getObjectId() const
		 { UT_ASSERT(isObject()); return myData[3]; }

    int		 getHandleIndex() const
		 { UT_ASSERT(isHandle()); return myData[1]; }
    int		 getHandlePickType() const
		 { UT_ASSERT(!isSet() || isHandle()); return myData[3]; }
    int		 getHandleValue1() const
		 { UT_ASSERT(!isSet() || isHandle()); return myData[4]; }
    int		 getHandleValue2() const
		 { UT_ASSERT(!isSet() || isHandle()); return myData[5]; }

    int		 getFloorId() const
		 { UT_ASSERT(isFloor()); return myData[1]; }
    int		 getFloorValue1() const
		 { UT_ASSERT(!isSet() || isFloor()); return myData[3]; }
    int		 getFloorValue2() const
		 { UT_ASSERT(!isSet() || isFloor()); return myData[4]; }
    int		 getFloorValue3() const
		 { UT_ASSERT(!isSet() || isFloor()); return myData[5]; }

    int		 getLookId() const
		 { UT_ASSERT(isGeometry() || isObject()); return myData[1]; }
    int		 getDetailIndex() const
		 { UT_ASSERT(isGeometry() || isObject()); return myData[2]; }

    int		 getComponentId1() const
		 { UT_ASSERT(isGeometry()); return myData[3]-1; }
    int		 getComponentId2() const
		 { UT_ASSERT(isGeometry()); return myData[4]-1; }
    int		 getComponentId3() const
		 { UT_ASSERT(isGeometry()); return myData[5]-1; }

    void	 changeComponent(int pick, int id1, int id2 = -1, int id3 = -1)
		 {
		     UT_ASSERT(isGeometry());
		     myData[0] = pick;
		     myData[3] = id1 + 1;
		     myData[4] = id2 + 1;
		     myData[5] = id3 + 1;
		 }

    void	 dump(std::ostream &os) const;

    static int	 comparatorFast(const GR_PickRecord *p1,
			const GR_PickRecord *p2)
		 {
		     // We are only interested in comparing the first three
		     // values in the pick record. We don't really care
		     // about sorting the specific component ids. This
		     // saves a lot of time on a big sort.
		     return memcmp(p1->myData, p2->myData, sizeof(int)*3);
		 }
    static int	 comparatorForDups(const GR_PickRecord *p1,
			const GR_PickRecord *p2)
		 {
		     // Compare everything, otherwise duplicates may not end
		     // up next to each other.
		     return memcmp(p1->myData, p2->myData, sizeof(int)*6);
		 }

    static bool	 areEquivalentEdges(const GR_PickRecord &p1,
				    const GR_PickRecord &p2)
		 {
		     if (p1.getPickType() == GR_PICK_GEOEDGE &&
			 p2.getPickType() == GR_PICK_GEOEDGE)
		     {
			 // Identical edges are equialent.
			 if (p1.myData[1] == p2.myData[1] &&
			     p1.myData[2] == p2.myData[2] &&
			     p1.myData[4] == p2.myData[4] &&
			     p1.myData[5] == p2.myData[5])
			     return true;
			 // Non-identical edges are equivalent if they differ
			 // only in swapping their last two component ids.
			 if (p1.myData[1] == p2.myData[1] &&
			     p1.myData[2] == p2.myData[2] &&
			     p1.myData[4] == p2.myData[5] &&
			     p1.myData[5] == p2.myData[4])
			     return true;
		     }

		     return false;
		 }

private:
    int		 myData[6];
};
SYS_DECLARE_IS_POD(GR_PickRecord);

static inline std::ostream &
operator<<(std::ostream &os, const GR_PickRecord &pr) 
{ pr.dump(os); return os; }

#endif
