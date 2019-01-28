/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_TileRead.h ( IMG Library, C++)
 *
 * COMMENTS:	This class reads tiles using the imdisplay standard pipe
 *		format.  It's up to the sub-class to do anything with the data
 *		read.
 */

#ifndef __IMG_TileRead__
#define __IMG_TileRead__

#include "IMG_API.h"
#include "IMG_Stat.h"
class PXL_Raster;

class IMG_API IMG_TileRead
{
public:
    // flip will flip the image (i.e. y at top)
    // keep_open will keep reading data until the "close" packet is received
	     IMG_TileRead(int flip, bool keep_open);
    virtual ~IMG_TileRead();

    // Read an image over the pipe.  If there are errors reading/writing data
    // (sub-class responsibility), this function will return 0 (and do an early
    // abort).
    int		readImage();

protected:
    // Return the number of elements read.
    // Return <= 0 if failed
    virtual int		readData(void *data, int size, int nelem) = 0;

    virtual int		openWrite() = 0;
    virtual int		writeTile(int tx0, int tx1, int ty0, int ty1,
				  const void *data, int plane = 0) = 0;
    virtual int		closeWrite() = 0;

    //
    // Error messages can be printed out by providing this method (default is
    // to do nothing)
    virtual void	error(const char *message);

    // Convenience function to write the data into an IMG_Raster class
    void		writeTileToRaster(PXL_Raster &raster,
					  int tx0, int tx1, int ty0, int ty1,
					  const void *data, int plane);

    // Find out information about image being read.  This will be defined by
    // the time openWrite() is called (but not before readData() is called the
    // first time.
    const IMG_Stat	&getStat() const	{ return myStat; }

private:
    void		 setPlaneFormat(int format, int asize,
					IMG_DataType &dtype,
					IMG_ColorModel &cmodel);
    void		 growData(int bytes);
    void		*myData;
    int			 myPoolSize;
    int			 mySwab;	// Swap bytes
    int			 myFlip;	// Flip vertically
    IMG_Stat		 myStat;
    bool		 myKeepOpen;
};

#endif
