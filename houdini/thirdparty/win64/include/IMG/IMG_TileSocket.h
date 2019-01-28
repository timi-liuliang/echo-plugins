/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_TileSocket (C++)
 *
 * COMMENTS:	This class writes to a shared memory buffer for mantra
 *		It spawns a reader process which reads the tiles.
 */

#ifndef __IMG_TileSocket_H__
#define __IMG_TileSocket_H__

#include "IMG_API.h"
#include <UT/UT_SysClone.h>
#include "IMG_TileDevice.h"
#include "IMG_TileInetBuf.h"

class UT_BitArray;
class UT_Signal;
class IMG_TileInetBuf;
class IMG_TileInterface;

class IMG_API IMG_TileSocket : public IMG_TileDevice {
public:
    // The command is passed the shared memory key, the size, then the x/y res
    IMG_TileSocket( const char *host );
    virtual ~IMG_TileSocket();

    virtual const char	*className() const;
    virtual void	 getDescription(UT_WorkBuffer &wbuf) const;
    virtual int		 open(const IMG_TileOptions &finfo,
			      int xres, int yres, int twidth, int theight,
			      fpreal aspect);

    virtual IMG_DataType getDataType() const;
    virtual int		 writeTile(const void *data, unsigned x0, unsigned x1,
						     unsigned y0, unsigned y1);
    virtual int		 close(bool keep_alive=false);

private:
    void		 clean();

    char		*myHost;
    IMG_TileInetBuf	*mySock;
};

#endif
