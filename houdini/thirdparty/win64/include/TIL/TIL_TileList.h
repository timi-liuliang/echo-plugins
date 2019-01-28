/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_TileList.h	(Tile Image Library, C++)
 *
 * COMMENTS:
 *	A simple list of tiles used for passed tile vectors around during a
 *	cook.
 */
#ifndef TIL_TILELIST_H
#define TIL_TILELIST_H

#include "TIL_API.h"

class TIL_Plane;
class TIL_Region;
class TIL_Tile;

class TIL_API TIL_TileList
{
public:
    static TIL_TileList *allocList();
    static void		 freeList(TIL_TileList *&list);

    void		init(const TIL_Plane *, int array_index,
			     int x1, int y1, int image_index);
    void		reset();
    
    // Tile lists do not delete their tiles or their plane.

    void		 setSize(int x,int y);
    
    TIL_TileList &	 operator=(const TIL_TileList &);

    int			 contains(int x,int y) const;

    // returns a region representing the same data as this tilelist. Does NOT
    // make a copy of the data -- it's referenced. Use TIL_Region::freeRegion()
    // to free this Region when done.
    TIL_Region		*getRegionFromTiles(bool uncooked_only = false);

    bool		 assignConstant(TIL_Region *region,
					bool black = false);
    void		 clearToBlack(bool markconstant = true);

    bool		 isEmpty() const
	{ return myItems[0] == 0 && myItems[1] == 0 &&
	         myItems[2] == 0 && myItems[3] == 0; }


    void		setDeferredKey(void *key) { myDeferKey = key; }
    void	       *getDeferredKey()	  { return myDeferKey; }
public:
    // list items & iterators.
    TIL_Tile		*myItems[PLANE_MAX_VECTOR_SIZE];

    // Cook information (parent plane, region covered by the tile)
    union {
	const TIL_Plane	*myPlane;
	TIL_TileList	*myNext;
    };
    short		 myArrayIndex;
    char		 myUsePoints;
    char		 myScanEven;
    char		 myScanStep;
    TIL_Interlace	 myInterlace;
    
    float		 myScanScale;
    float		 myScanOffset;

    // # of the image in the sequence.
    int			 myImageIndex;

    // all of the following information can be found in the plane & tiles of
    // the list, but it's duplicated here since it's commonly accessed.
    exint		 mySize;
    int			 myX1, myY1, myX2, myY2;
    unsigned int	 myBlack, myWhite;

    // information for the proxy manager.
    char		 myTileWrite[PLANE_MAX_VECTOR_SIZE];

private:
    // use the allocators (allocList, freeList) to create/destroy tilelists.
	TIL_TileList();
       ~TIL_TileList();

    // GPU deferred read key.
    void		*myDeferKey;
};

#endif
