/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_TileFile (C++)
 *
 * COMMENTS:	This class actually writes out to a file.  No if's ands or
 *		buts.
 *
 */

#ifndef __IMG_TileFile_H__
#define __IMG_TileFile_H__

#include "IMG_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_IntArray.h>
#include "IMG_TileDevice.h"
#include "IMG_Stat.h"

class IMG_File;

class IMG_API IMG_TileFileMapEntry
{
public:
    // When opening multiple files, we get a list of plane definitions
    // Each plane may be written to a "shared" file, or it might be written to
    // aunique file.
    // This class keeps track of the mappings of each input channel to the
    // output file/channel.
    // For each source channel, we store
    //	int	SourceOffset;	// Offset into source data
    //	int	SourceSize;	// Bytes per pixel
    //	int	DestFile;	// Destination file index
    //	int	DestPlane;	// Destination plane index
     IMG_TileFileMapEntry()	{}
    ~IMG_TileFileMapEntry()	{}

    int		getSourceOffset() const		{ return mySourceOffset; }
    int		getSourceSize() const		{ return mySourceSize; }
    int		getDestFileIndex() const	{ return myDestFileIndex; }
    int		getPlaneIndex() const		{ return myDestPlane; }
    IMG_File	*getFile() const		{ return myDestFile; }
    void	setupMap(int off, int size,
			 IMG_File *fp, int findex, int plane)
		{
		    mySourceOffset = off;
		    mySourceSize = size;
		    myDestFile = fp;
		    myDestFileIndex = findex;
		    myDestPlane = plane;
		}

private:
    IMG_File	*myDestFile;
    int		 mySourceOffset;
    int		 mySourceSize;
    int		 myDestFileIndex;
    int		 myDestPlane;
};

class IMG_API IMG_TileFile : public IMG_TileDevice
{
public:
	     IMG_TileFile();
    virtual ~IMG_TileFile();

    virtual const char		*className() const;
    virtual void		 getDescription(UT_WorkBuffer &wbuf) const;
//
//  Virtual method to open a picture.  Returns 0 on failure
    virtual int		open(const IMG_TileOptions &finfo,
				int xres, int yres,
				int twidth, int theight,
				fpreal aspect);
    virtual int		openMulti(IMG_TileOptionList &flist,
				int xres, int yres,
				int twidth, int theight,
				fpreal aspect);
    
//  Method to actually write a tile to the file.  The data to write is passed
//    relative to the dataWindow() on the IMG_TileDevice.
    virtual int			writeTile(const void *data,
					  unsigned x0, unsigned x1,
					  unsigned y0, unsigned y1);
    virtual void		checkpoint();
    virtual int			close(bool keep_alive=false);

protected:
    virtual void		writeCustomTag(const char *tagname, int size,
					       const char *const *values);
private:

    void		prepData();

    UT_ValArray<IMG_File *>		myFiles;
    UT_StringArray			myFilenames;
    UT_ValArray<IMG_TileFileMapEntry *> myPlaneMap;
    int					myBPP;
};

#endif
