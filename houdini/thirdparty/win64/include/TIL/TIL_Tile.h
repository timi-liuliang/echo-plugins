/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_Tile.h	(Tile Image Library, C++)
 *
 * COMMENTS:
 *	Defines the structure of an image tile.
 */
#ifndef TIL_TILE_H
#define TIL_TILE_H

#include "TIL_API.h"
#include <SYS/SYS_Types.h>

#include <iosfwd>
#include <sys/types.h>
#include <UT/UT_Endian.h>
#include <UT/UT_LinkList.h>
#include <UT/UT_String.h>
#include <UT/UT_Array.h>

#include "TIL_HoldingQueue.h"
#include "TIL_Defines.h"
#include "TIL_TileList.h"

class UT_TokenString;
class TIL_TileCache;
class TIL_Plane;
class til_AlignedBlock;

class TIL_API TIL_Tile : public TIL_HoldingNode
{
public:
    // Constructor and Destructor are private; only TIL_CacheManager can create
    // and destroy tiles.
    
    // resets all the information for re-allocation.
    void		reset();

    // utility functions ****
    void		clear(float value = 0.0F, int scanstep=1,
			      int scanoff =0);
    // odd means double the odd scanlines to the even ones.
    void		lineDouble(bool odd);
    
    int			matches(const UT_TokenString *image_id,
				int tilex, int tiley) const;
    // like strcmp.
    int			compare(const UT_TokenString *image_id,
				int tilex, int tiley) const;
   

    // Accesser functions *****
    
    // plane name and vector index (0-3).
    void		setVectorName(const char *name, int vector =0);
    const char *	getVectorName() const		{return myVectorName;}
    void		setCompName(const char *name);
    const char *	getCompName() const		{ return myCompName; }
    int			getVectorIndex() const		{return myVectorIndex;}

    // format of the tile (int8,int16,int32,float32)
    void		setFormat(TIL_DataFormat f);
    TIL_DataFormat	getFormat() const		{ return myFormat; }

    // storage type of this type (locked, proxy, nocache, cached). 
    void		setStorage(TIL_Storage storage) { myStorage = storage;}
    TIL_Storage		getStorage() const		{ return myStorage; }

    // the "frame" number; first image is always 0 regardless of time offset.
    void		setImageNum(int n)		{ myImageNum = n; }
    int			getImageNum() const		{ return myImageNum; }

    // black/white support for integer formats.
    void		setBlackWhitePoints(int black,int white);
    int			usesBlackWhitePoints() const;
    void		getBlackWhitePoints(unsigned int &black,
					    unsigned int &white) const
			{ black = myBlackLevel;  white = myWhiteLevel; }

    // tile offset in the main image (in pixels).
    void		setOffset(int x,int y) { myXOffset=x; myYOffset=y; }
    void		getOffset(int &x, int &y) const
				{ x=myXOffset; y=myYOffset; }

    // the size of the tile (from 1 to theTileX/YSize)
    void		setSize(int x, int y);
    void		getSize(int &x, int &y) const
				{ x = myXSize; y = myYSize; }

    int			getNumPixels() const;

    // the COP this tile belongs to (void * due to lib linking). 
    void		setParent(void *parent)		{ myParent = parent; }
    void *		getParent() const		{ return myParent; }


    // the full path of the COP this tile belongs to.
    void		setNodePath(const char *path);
    const char *	getNodePath() const { return myNodePath;}

    // the image id; guarenteed to be unique if the tiles are different. 
    void		  setImageIdentifier(UT_TokenString *token)
				{ myImageIdentifier = token; }
    const UT_TokenString *getImageIdentifier() const
				{ return myImageIdentifier; }

    // has this tile been cooked yet?
    void		setCooked(bool done = true)	{ myCookedFlag =done; }
    bool		isCooked() const		{ return myCookedFlag;}

    bool		isCheckpoint() const  { return myCheckpointCount != 0; }
    int			getCheckpointCount() const { return myCheckpointCount; }
    void		incrementCheckpointCount();
    void		decrementCheckpointCount();

    bool		determineIfConstant();
    bool		isConstantTile() const	{ return myConstantTileFlag; }
    float		getConstantColor() const;
    
    // When flagging a tile as constant, ensure that you've already
    // initialized the data as this method will read it.
    void		setConstantTile(bool constant);
    
    // used to access this tile in a multithreaded environment
    // access can be TILE_LOCK_READ_ONLY or TILE_LOCK_WRITE
    int			lock(int access, int block = 1);
    // used when cooking is done. score is the priority score for the tile.
    void		lockWriteToRead(int score);
    void		unlock(int access);

    // if on, this tile is being held for proxy writing.
    void		setProxyHold(bool on)	 { myProxyHoldFlag = on; }
    
    // the amount of memory the tile's image takes up (not including overhead).
    int			getMemSize() const		{ return myMemSize; }

    // the amount of memory the tile takes up (including overhead)
    int64		getMemoryUsage(bool inclusive) const;
    
    // Image data accessor functions; returns NULL if the data type doesn't
    // match the function called. the void * functions always return non-null.
    void		setImageData(til_AlignedBlock * data)
					{ myImageData = data; }
    
    void		getImageData(const unsigned char  *&data) const;
    void		getImageData(const unsigned short *&data) const;
    void		getImageData(const unsigned int   *&data) const;
    void		getImageData(const float	  *&data) const;
    void		getImageData(const fpreal16	  *&data) const;
    const void *	getImageData() const	{ return (void *) *myImageData; }
    
    void		getImageData(unsigned char  *&data);
    void		getImageData(unsigned short *&data);
    void		getImageData(unsigned int   *&data);
    void		getImageData(float	    *&data);
    void		getImageData(fpreal16	    *&data);
    void *		getImageData()		{ return (void *) *myImageData; }

    til_AlignedBlock	*getImageBlock() { return myImageData; }
    
    // caching functions *****
    
    // the priority of this tile.
    void		setPriority(unsigned int p)	{ myPriority = p; }
    int			getPriority() const		{ return myPriority; }

    // returns 1 if this tile is locked in any way
    int			isCacheLocked() const;
    bool		isReadLocked() const;
    bool		isWriteLocked() const;

    bool		isBorrowedImage() const { return myBorrowedImageFlag; }
    void		setBorrowedImage(bool b) { myBorrowedImageFlag = b;
						   myMemSize = 0; }

    // Static tile functions. 
    
    // selects a new tile size. Must be called before the Cache Manager is
    // initialized (or re-initialized). Use with extreme care.
    static void		setNewTileSize(int xsize, int ysize);

    // query tiles sizes and memory requirements.
    static int		getTileSizeX();
    static int		getTileSizeY();
    static int		getTile8Mem();
    static int		getTile16Mem();
    static int		getTile32Mem();

    static void		setTileCachePtr(TIL_TileCache *tc);

    // DEBUG
    void		print(std::ostream &os) const;
    static void		setCopPrintCallback(void(*callback)(void *,std::ostream &));

private:
    // Only the Tile Cache can create and destroy tiles.
    TIL_Tile();
   ~TIL_Tile();

    // TIL_HoldingNode (12)
    // identification (16)
    const char		*myVectorName;
    const char		*myCompName;
    int			myVectorIndex;
    int			myImageNum;

    // description (40)
    TIL_DataFormat	myFormat;
    unsigned int	myBlackLevel;
    unsigned int	myWhiteLevel;
    int			myXSize;
    int			myYSize;
    int			myXOffset;
    int			myYOffset;
    int			myMemSize;
    TIL_Storage		myStorage;
    char	       *myNodePath;

    // caching (16)
    unsigned int	myPriority;
    void *		myParent;
    UT_TokenString *	myImageIdentifier;
    int			myCheckpointCount;  // num planes checkpointing tile

    // flags (4)
    unsigned int	myCompNameAlloc		: 1,
			myConstantTileFlag	: 1,
			myBorrowedImageFlag	: 1,
			myCookedFlag		: 1,
			myProxyHoldFlag		: 1,
			myVectorNameAlloc	: 1;

    // constant value, if const (4)
    union {
	unsigned char	myConstantVal8;
	unsigned short	myConstantVal16;
	unsigned int	myConstantVal32;
	float		myConstantValFP;
    };
    fpreal16	myConstantValFP16;

    // locking (24)
    TIL_FastLock		myMainLock;
    int			myReadLocks;
    int			myWriteLock;
    
    // actual image data: not owned by this object (4)
    til_AlignedBlock	*myImageData;

    friend class TIL_TileCache;
};


#endif // TIL_TILE_H
