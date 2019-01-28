/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_Region.h	(Tile Image Library, C++)
 *
 * COMMENTS:
 *	Handles arbitrary sized regions of planes, for algorithms that
 *	require more information than just a tile.
 */
#ifndef TIL_REGION_H
#define TIL_REGION_H

#include "TIL_API.h"
#include "TIL_Defines.h"

#include <UT/UT_SmallObject.h>

class TIL_Plane;
class TIL_Tile;
class TIL_TileList;

enum TIL_RegionExtend
{
    TIL_BLACK		= 0,
    TIL_HOLD_LEFT	= 0x1,
    TIL_HOLD_RIGHT	= 0x2,
    TIL_HOLD_TOP	= 0x4,
    TIL_HOLD_BOTTOM	= 0x8,
    TIL_HOLD		= 0xF
};

class TIL_API TIL_Region
    : public UT_SmallObject<TIL_Region,
			    UT_SMALLOBJECT_CLEANPAGES_OFF, 32,
			    UT_SMALLOBJECT_THREADSAFE_ON, 1024>
{
public:
    // Do NOT derive classes from this method, or add virtual functions.
    
    // region allocation routines.
    static TIL_Region	*allocRegion();
    static void		 freeRegion(TIL_Region *&freeme);

   
    void	init(const TIL_Plane *parent, int input_index,
		     int array_index, float time, int xres, int yres,
		     int x1, int y1, int x2, int y2,
		     int bx1, int by1, int bx2, int by2,
		     TIL_RegionExtend hold = TIL_BLACK,
		     int share_count = 1, int nid = 0,
		     bool output = false, int xpad = 0);

    void	reset();

    // sets the image data to zero. Does not set the constant flags.
    void	clear();
    
    int		getID() const { return myID; }
    int		getNodeID() const { return myNodeID; }
    
    // call to init and destroy (regions are ref-counted). If you supply your
    // own memory regions, pass the pointers (you are responsible for freeing
    // them, unlike the automatically alloced memory which is freed for you).
    int		open(void *regionlist[PLANE_MAX_VECTOR_SIZE] = 0);
    int		close();

    // info accessors

    // identification
    const TIL_Plane	*getParent() const	    { return myParent; }
    int			 getInputRef() const	    { return myInputRef; }
    int			 getArrayIndex() const      { return myArrayIndex; }
    float		 getTime() const	    { return myCookTime; }
    void		 getRegionRes(int &x, int &y) const
			 { x=myXres; y=myYres; }
    // 
    void	getRegionBounds(int &x1, int &y1, int &x2, int &y2) const
		{ x1 = myX1+myXShift; y1 = myY1+myYShift;
		  x2 = myX2+myXShift; y2 = myY2+myYShift; }

    // for output regions, allows you to mark the region as filled.
    void	markFilled(bool filled = true) { myRemainingTiles = filled ? 0 : 1; }
    
    // tells you if the regions is completely filled.
    int		isFilled() const { return myRemainingTiles == 0; }
    int		remainingTiles() const { return myRemainingTiles; }
    bool	isInitialized() const { return myRegionInit; }

    bool	workOnRegion();
    
    // tells you which tile needs to be cooked next, if 0 is returned,
    // all the remaining tiles in the process of being cooked.
    int		getNextNeededTile(int &xoff,int &yoff,
				  int &xshift, int &yshift);

    // call when you're done filling the region with the tile recommended by
    // getNextNeededTile.
    void	finishedTile(int xoff, int yoff);

    // checks to see if the region is still constant.
    void	checkConstant(const TIL_Tile *tile, int component);
    bool	isConstantChecked() const { return myConstantChecked; }
    bool	isConstant(bool force_check = false);
    bool	isConstantValue() const;
    void	getConstantColor(float color[4]) const;
    // does not set the color for the region, only the constant flag & color.
    void	setConstantColor(float color[4]);
    
    // can't cook a tile that getNextNeededTile passed (likely due to a
    // write lock on the tile held by another thread). 
    void	rejectTile(int xoff, int yoff);

    // returns the data associated with index or component name 'compname'
    void *	getImageData(int index);
    const void *getImageData(int index) const;
    
    void *	getImageData(const char *compname);
    const void *getImageData(const char *compname) const;

    void	setImageData(void *data, int index, bool reference = true);

    // Returns the size, in bytes, of the image data.  The result is the size
    // for one component, not for all components.
    exint	getImageDataSizePerComponent() const;

    // Total memory size of this region (approx). 
    exint	getMemSize();

    bool	isSharedRegion() const { return myRegionShared; }
    bool	isSharedRegionDone() const { return myRegionUnshared; }
    int		getCurrentRefCount() const { return myRefCount; }
    int		getCurrentShareCount() const { return myShareCount; }
    void	unshareRegion();

    // Region hash routines - return this region's hash code.
    unsigned	    getHash() const { return myHashCode; }
    
    // Return a hash code would correspond to the hash code of a region
    // created with these parms.
    static unsigned getHash(const TIL_Plane *plane, int aindex, int inref,
			    float t, int x1, int y1, int x2, int y2,
			    int xres, int yres, int nid, bool output);

    // Use these methods to get a tile list which corresponds to this region.
    // The region should be equal to or less than the current tile size.
    // Please note that the extra information in the tiles, such as the offset
    // and other information, may not be accurate.
    // Call cleanupTiles to free the tilelist. Do NOT call
    // COP2_Node::releaseTile or freeTile on this tilelist!
    TIL_TileList *getTilesFromRegion();
    void	  cleanupTiles(TIL_TileList *&tiles);

    // sets the time to cook this region (mostly for output regions where you
    // want to distribute the time spent filling it across all the tiles)
    void	  setWorkTime(float cooktime) { myWorkTime = cooktime; }
    float	  getWorkTime() const { return myWorkTime; }
    
private:
		 TIL_Region();
		~TIL_Region();

    void	fillOutsideData();
    float	fillOutsideBlack(unsigned char *region);
    void	fillOutsideHold(unsigned char *region);
    void	determineIfConstant();
    
private:
    // Parent plane (and allocation routine next ptr).
    union {
	const TIL_Plane *myParent;
	TIL_Region	*myNext;
    };

    // more image id.
    short	     myArrayIndex;
    short	     myInputRef;
    int		     myID;
    int		     myNodeID;
    float	     myCookTime;

    // parent image res
    int		     myXres, myYres;
    
    // region coordinates
    int		     myX1, myY1;
    int		     myX2, myY2;

    // coordinates clipped to canvas bounds
    int		     myRX1, myRY1;
    int		     myRX2, myRY2;

    // coordinates clipped to (0,0)->(xres-1,yres-1)
    int		     myBX1, myBY1;
    int		     myBX2, myBY2;

    int		     myXShift, myYShift;

    // region image data
    void	    *myRegion[PLANE_MAX_VECTOR_SIZE];
    void	    *myRawRegion[PLANE_MAX_VECTOR_SIZE];
    char	     myRegionAlloced[PLANE_MAX_VECTOR_SIZE];

    // cooperative cooking
    UT_Lock	     myLock;
    int		     myRefCount;
    int		     myShareCount;

    int		     myTileOffX;
    int		     myTileOffY;
    int		     myTileX2;
    int		     myTileY2;
    int		     myTileXCount;
    int		     myTileYCount;
    int		     myRemainingTiles;
    int		     myAvailableTiles;
    int		     myTileStride;
    unsigned char    *myTileStates;
    float	     myWorkTime;

    float	     myConstantColor[4];

    unsigned	     myHashCode;
    unsigned int     myRegionOutside :1,
		     myHold :4,
		     myConstantChecked : 1,
		     myConstantFlag :1,
		     myConstantColorSetR : 1,
		     myConstantColorSetG : 1,
		     myConstantColorSetB : 1,
		     myConstantColorSetA : 1,
		     myRegionInit :1,
		     myRegionWorkFlag : 1,
		     myRegionShared : 1,
		     myRegionUnshared : 1;
};

#endif
