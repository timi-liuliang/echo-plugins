/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_TileBuffer.h ( IMG Library, C++)
 *
 * COMMENTS:	Defines a base class for the random access tile devices.
 */

#ifndef __IMG_TileBuffer__
#define __IMG_TileBuffer__

#include "IMG_API.h"
class IMG_Raster;

class IMG_API IMG_TileBuffer {
public:
	     IMG_TileBuffer() {}
    virtual ~IMG_TileBuffer() {}

    virtual int		getTileWidth() const = 0;
    virtual int		getTileHeight() const = 0;

    virtual void	setQuitFlag(int val=1) = 0;
    virtual int		getQuitFlag() const = 0;

    virtual void	setMouseXY(int x, int y) = 0;
    virtual void	getMouseXY(int &x, int &y) const = 0;

    // Semaphore mechanism for safe tile read/write
    virtual int		canWriteTile() const = 0;
    virtual int		canReadTile() const = 0;

    virtual void	writeTile(int xl, int xr, int yb, int yt,
				    const void *data) = 0;

    // If coordinates are passed in, they will be filled with:
    //	coords[0] = xl, coords[1] = xr;
    //	coords[2] = yb, coords[3] = yt;
    virtual void	readTile(IMG_Raster &rp, int *coords=0) = 0;
    virtual void	getKeyString(char *str) = 0;

protected:
    virtual void	setTileSize(int w, int h) = 0;
};

#endif

