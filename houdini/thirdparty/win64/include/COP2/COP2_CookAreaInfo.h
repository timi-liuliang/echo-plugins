/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        COP2_CookAreaInfo.h
 *
 * COMMENTS:
 *	This class describes an area of a plane to be cooked.
 */

#ifndef __COP2_CookAreaInfo_h__
#define __COP2_CookAreaInfo_h__

#include "COP2_API.h"
#include <UT/UT_SmallObject.h>
#include <UT/UT_Assert.h>
#include <SYS/SYS_Math.h>
#include <UT/UT_ValArray.h>
#include <iosfwd>

class COP2_Node;
class COP2_Context;
class TIL_Plane;

class COP2_API COP2_CookAreaInfo
    : public UT_SmallObject<COP2_CookAreaInfo,
			    UT_SMALLOBJECT_CLEANPAGES_OFF, 250,
			    UT_SMALLOBJECT_THREADSAFE_ON>
{
public:
    // In this constructor, the plane is marked as not needed.
    COP2_CookAreaInfo(COP2_Node &node,
		      const TIL_Plane &plane,
		      int array_index,
		      int bounds_xstart, int bounds_ystart,
		      int bounds_xend, int bounds_yend,
		      int frame_xres, int frame_yres,
		      float time);

    // In this constructor, the plane is marked as not needed.  Also, the
    // plane does not have to be in the node's sequence.
    COP2_CookAreaInfo(COP2_Node &node,
		      const TIL_Plane &ref_plane,
		      int array_index,
		      float time);

    // In this constructor, the is marked as needed.
    COP2_CookAreaInfo(COP2_Node &node,
		      const TIL_Plane &plane,
		      int array_index,
		      int bounds_xstart, int bounds_ystart,
		      int bounds_xend, int bounds_yend,
		      int frame_xres, int frame_yres,
		      float time,
		      int needed_xstart, int needed_ystart,
		      int needed_xend, int needed_yend);

    // Create an identicaly copy of this object.
    COP2_CookAreaInfo *clone(int array_index) const;

    // True is returned if the area needed to be grown.  Note that pixels_left,
    // pixels_down, etc. may be negative in enlargeNeededArea().
    bool	 enlargeNeededArea(int xstart, int ystart, int xend, int yend);
    bool	 enlargeNeededArea(COP2_CookAreaInfo &area,
				   int pixels_left = 0,
				   int pixels_down = 0,
				   int pixels_right = 0,
				   int pixels_up = 0);
    bool	 enlargeNeededAreaToBounds();

    // This method expands the needed area outward by the specified number
    // of pixels.  The needed area must already be defined before calling
    // this method.  This method does not shrink the needed area, so
    // pixels_left, pixels_down, etc. must not be negative.
    bool	 expandNeededArea(int pixels_left, int pixels_down,
				  int pixels_right, int pixels_up);

    // This convenience method gets the context data for the node, using
    // the information stored in this object.
    COP2_Context	*getNodeContextData();


    // Returns true if the node, plane, array index, time and frame res match.
    // The cook area is NOT matched.
    bool		 matchPlaneInfo(const COP2_CookAreaInfo &match,
					bool node_too = true) const;

    // Accessor functions:
    const COP2_Node	&getNode() const { return myNode; }
	  COP2_Node	&getNode()	 { return myNode; }

    const TIL_Plane	&getPlane() const { return *myPlane; }

    int			 getArrayIndex() const { return myArrayIndex; }

    // The image bounds for the entire plane, in frame space:
    int			 getBoundsXStart() const { return myBoundsXStart; }
    int			 getBoundsYStart() const { return myBoundsYStart; }
    int			 getBoundsXEnd() const { return myBoundsXEnd; }
    int			 getBoundsYEnd() const { return myBoundsYEnd; }

    // The scaled resolution of the frame:
    int			 getFrameXRes() const { return myFrameXRes; }
    int			 getFrameYRes() const { return myFrameYRes; }

    // The time at which the plane is cooking:
    void		 setTime(fpreal t)  { myTime = t; }
    fpreal		 getTime() const { return myTime; }

    // Is this plane needed by another one?
    bool		 isNeeded() const { return myIsNeeded; }

    // The image bounds for the needed area of the plane, in frame space:
    int			 getNeededXStart() const { return myNeededXStart; }
    int			 getNeededYStart() const { return myNeededYStart; }
    int			 getNeededXEnd() const { return myNeededXEnd; }
    int			 getNeededYEnd() const { return myNeededYEnd; }

    void		 print(std::ostream &os) const;

private:
    void		 alignNeededAreaToTileBoundaries();

    // Data:

    COP2_Node		&myNode;
    const TIL_Plane	*myPlane;

    int			 myArrayIndex;

    // The image bounds for the entire plane, in frame space:
    int			 myBoundsXStart;
    int			 myBoundsYStart;
    int			 myBoundsXEnd;
    int			 myBoundsYEnd;

    // The scaled resolution of the frame:
    int			 myFrameXRes;
    int			 myFrameYRes;

    // The time at which the plane is cooking:
    fpreal		 myTime;

    // Is this plane needed by another node?
    bool		 myIsNeeded;

    // The image bounds for the needed area of the plane, in frame space:
    int			 myNeededXStart;
    int			 myNeededYStart;
    int			 myNeededXEnd;
    int			 myNeededYEnd;
};

inline std::ostream &
operator<<(std::ostream &os, const COP2_CookAreaInfo &area)
{
    area.print(os);
    return os;
}


// This class acts like a pointer array of COP2_CookAreaInfo's, but it ensures
// that duplicate COP2_CookAreaInfo's are unioned together.

class COP2_API COP2_CookAreaList 
{
public:
    explicit COP2_CookAreaList(unsigned int sz = 0) : myList(sz) { } 
	    ~COP2_CookAreaList() { }

    COP2_CookAreaList(const COP2_CookAreaList &tocopy)
	: myList(tocopy.myList) { }

    // This append method MAY change your pointer if it's already found.
    unsigned int append(COP2_CookAreaInfo *&area);
    unsigned int entries() const { return myList.entries(); }
    void	 remove(int index) { myList.removeIndex(index); }
    
    void	 resize(int entries) { myList.setCapacity(entries); }
    void	 entries(int entries) { myList.entries(entries); }


    COP2_CookAreaInfo *operator()(unsigned int i) const { return myList(i); }

private:
    UT_ValArray<COP2_CookAreaInfo *> myList;
};

#endif
