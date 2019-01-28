/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        COP2_ThreadCookParms.h
 *
 * COMMENTS:
 *	This class contains parameters used by threads to cook a
 *	(plane of a) cop.
 */

#ifndef __COP2_ThreadCookParms_h__
#define __COP2_ThreadCookParms_h__

#include "COP2_API.h"
#include <UT/UT_Assert.h>
#include <UT/UT_Lock.h>
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>
#include <TIL/TIL_Tile.h>
#include <OP/OP_Context.h>
class UT_Thread;
class TIL_Plane;
class TIL_TileMPlay;
class TIL_Raster;
class COP2_Node;

// The class contains data common between all planes that are being cooked.
class COP2_API COP2_CommonCookParms
{
public:
    COP2_CommonCookParms()
    {}

    UT_Lock		 myLock;
    int			 myNumThreads;
    float		 myBlackPoint;
    float		 myWhitePoint;
    int			 myIgnorePoints;
    float		 myGamma;
    bool		 myCompleteFlag;
};

// This class contains data shared by all threads cooking the same plane in
// a node.  The number of objects of this class that are active at once is
// the number of planes that are being cooked simultaneously by different
// threads.
//
// Note that the common parm data stays the same regardless of which planes of
// which nodes are being cooked.  myCmomonParms will be set shortly after the
// object is constructed.
class COP2_API COP2_PlaneCookParms
{
public:
    COP2_PlaneCookParms()
    : myCommonParms(0), myNode(0), myPlane(0), myWorkOrder(0), myTileList(0),
      myArrayIndex(0), myCompMap(0)
    {}

    // This method is called when cooking is started on a plane.  If multiple
    // threads are cooking the same plane, this method is called only when the
    // first thread starts cooking the plane.
    void allocateTileWorkOrder()
    {
	UT_ASSERT(!myWorkOrder);
	int num_tiles = (myTileX2 - myTileX1 + 1) * (myTileY2 - myTileY1 + 1);
	myWorkOrder = new int[num_tiles];
	memset(myWorkOrder, 0, num_tiles * sizeof(int));

	myTileCount = 0;
    }

    // This method is called when cooking is finished on a plane.
    void deallocateTileWorkOrder()
    {
	UT_ASSERT(myWorkOrder);
	delete [] myWorkOrder;
	myWorkOrder = 0;
    }

    // This convenience method returns the number of tiles to be cooked in
    // this plane.
    int totalNumTiles() const
    {
	return (myTileX2 - myTileX1 + 1) * (myTileY2 - myTileY1 + 1);
    }

    // Data:
    COP2_CommonCookParms *myCommonParms;

    COP2_Node		*myNode;
    UT_String		 myTilePath;	// path to the cop node -- TODO: rename
    OP_Context		 myContext;
    const TIL_Plane	*myPlane;
    int			 myWriteProxy;
    int			 myArrayIndex;
    const int		*myCompMap;

    // Bounds information:
    int			myX1;		// Raster co-ords...
    int			myX2;
    int			myY1;
    int			myY2;
    int			myTileX1;	// Tile bounds needed to contain
    int			myTileX2;	//   the raster coords.
    int			myTileY1;
    int			myTileY2;
    int			myTileOffX;	// 0,0 Tile offset from raster 0,0
    int			myTileOffY;
    int			myXShift;	// Tile shift relative to raster.
    int			myYShift;

    // Shared data that is updated/modified by the threads that are cooking
    // this node.
    int			 myTileCount;
    int			*myWorkOrder;	// marks if a tile has been processed

    UT_Lock		 myTileListLock;
    UT_ValArray<TIL_Tile *> *myTileList; // tiles cooked for this plane
};

// This class contains information specific to this thread that's not shared
// with other threads cooking the same plane of a node.
class COP2_API COP2_ThreadCookParms
{
public:
    COP2_ThreadCookParms()
    : myPlaneParms(0), myThread(0), myRaster(0), myDevice(0)
    {}

    void setPlaneParms(COP2_PlaneCookParms *plane_parms)
    { myPlaneParms = plane_parms; }

    // Data:
    COP2_PlaneCookParms *myPlaneParms;
    UT_Thread		*myThread;
    int			 myThreadIndex;
    int			 myCompIndex;

    // Raster information:
    TIL_Raster		*myRaster;

    // Flipbook information:
    bool		 myTileOnlyFlag;
    TIL_TileMPlay	*myDevice;
};

#endif
