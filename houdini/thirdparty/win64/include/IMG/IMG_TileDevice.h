/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_TileDevice.h ( IMG Library, C++)
 *
 * COMMENTS:	Class to define tile based I/O for mantra.
 *		When writing an image which has fields, the data sent to
 *		writeTile will not be interlaced.  It's the responsibility of
 *		the device to put in the appropriate data to the other fields.
 */

#pragma once

#ifndef __IMG_TileDevice__
#define __IMG_TileDevice__

#include "IMG_API.h"
#include <UT/UT_Rect.h>
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include "IMG_FileTypes.h"
#include "IMG_Tags.h"

class UT_WorkBuffer;
class IMG_Raster;
class IMG_Format;
class IMG_TileInterface;
class IMG_TileOptions;
class IMG_TileOptionList;
class UT_Options;

class IMG_API IMG_TileDevice {
public:
	     IMG_TileDevice();
    virtual ~IMG_TileDevice();

    enum IMG_TILE_ORDER
    {
	IMG_TILE_BOTTOM_FIRST = 0,
	IMG_TILE_RANDOM_ORDER = 1,
	IMG_TILE_TOP_FIRST = 2,
	IMG_TILE_HILBERT = 3
    };

    typedef IMG_TileDevice *(*IMG_TileDeviceCreator)(const char *filename,
			const UT_Options *options);

    /// Create a new device based on the filename.  This checks:
    ///	 - "null:" - Null device
    ///	 - "imdisplay:" - MPlay device
    ///  - etc.
    /// If no devices match exactly, the file device will be opened.
    static IMG_TileDevice	*newDevice(const char *filename,
					    const UT_Options *options = NULL);

    /// Returns true if filename represents a special device, not a regular
    /// file, i.e. everything that newDevice has a special case for.
    static bool isSpecialFileName(const char *filename);

    /// Returns true if filename represents a null device,
    /// i.e. it will ignore everything sent to it.
    /// This is useful for avoiding work that will be ignored.
    static bool isNullDeviceName(const char *filename);

    /// Register a custom image device.  The label should be specified @b
    /// without the colon (':').  For example: @code
    ///	   registerDevice(createSocket, "socket");
    /// @endcode
    /// The creator function will be called from newDevice() with the
    /// filename/options passed.   newDevice() will invoke the creator if the
    /// filename @b begins with the label.  For example, the following would
    /// invoke the socket device registered above: @code
    ///	    newDevice("socket:{'port':43}", NULL);
    /// @endcode
    /// @note This will replace any existing device
    static void		registerDevice(IMG_TileDeviceCreator creator,
					    const char *label);
    /// Unregister a custom device
    static void		unregisterDevice(const char *label);

    /// @{
    /// Static methods to allocate new devices of built-in device types
    static IMG_TileDevice	*newFileDevice();
    static IMG_TileDevice	*newNullDevice();
    static IMG_TileDevice	*newMPlayDevice(int from_middle=1);
    static IMG_TileDevice	*imdisplayFileDevice(const char *filename);
    static IMG_TileDevice	*newSocketDevice(const char *host);
    /// @}

    /// @{
    /// Replace the creators for the file or mplay devices.
    /// @note The signatures for the constructors are different than the
    /// callbacks uses in @c registerDevice().
    static void			setFileDevCreator(IMG_TileDevice *(*creator)());
    static void			setMPlayDevCreator(IMG_TileDevice *(*creator)(int ));
    /// @}

    /// @{
    /// Query information about the tile device
    virtual const char		*className() const = 0;
    virtual void		 getDescription(UT_WorkBuffer &wbuf) const;
    /// @}

    // The visible region is inclusive (i.e. there are (r-l+1) pixels per scan)
    // Setting the visible region should be done before the image is opened.
    SYS_DEPRECATED_REPLACE(16.0, "setDeviceDataWindow")
    void			setVisibleRegion(int l, int r, int b, int t);

    // The region is always bigger or equal to the visible region.  It
    // represents the region which needs to be "rendered".  The renderable
    // region is broken up into tiles of the tile size specified.
    SYS_DEPRECATED_REPLACE(16.0, "setDeviceRenderWindow")
    void			setRenderSize(int rwidth, int rheight);

    /// Set the data window
    void	setDeviceDataWindow(const UT_DimRect &w)
			{ myDataWindow = w; }

    /// Set the window of pixels that need to be rendered
    /// The render window is always bigger or equal to the data window.  It
    /// might be larger due to pixel filters (i.e. samples outside the data
    /// window might need to be computed for filtering).  The difference
    /// between the data window and the render window size is used to determine
    /// filter width.
    void	setDeviceRenderWindow(const UT_DimRect &w)
			{ myRenderWindow = w; }

    /// Open the device
    virtual int			open(const IMG_TileOptions &info,
				     int xres, int yres,
				     int tile_device, int tile_height,
				     fpreal aspect) = 0;

    /// The default implementation will just open the first entry of the flist
    /// by calling open().
    virtual int			openMulti(
					IMG_TileOptionList &flist,
					int xres, int yres,
					int tile_width, int tile_height,
					fpreal aspect);

    virtual void		setWhitePoint(fpreal white);

    /// Allow the device to perform any "checkpointing" needed.  This is
    /// equivalent to "flushing" the device.
    virtual void		checkpoint();

    // where the render originated from, to provide a half-decent name to ip.
    void			setRenderSourceName( const char *name )
				    { myRenderSourceName.harden(name); }
    const char			*getRenderSourceName() const
				    { return myRenderSourceName; }
    
    /// After the open() is called, then it's possible to query the data type
    /// that the device expects.
    int			getFileCount() const	{ return myFileCount; }
    IMG_DataType	getDataType(int idx=0) const;
    bool		isRandomAccess() const	{ return myRandomAccess; }

    /// Query tile width parameters
    int			getTileWidth() const	{ return myTileWidth; }
    int			getTileHeight() const	{ return myTileHeight; }
    int                 getTileCountX() const   { return myTileCountX; }
    int                 getTileCountY() const   { return myTileCountY; }

    // Default is to return IMG_RGBA
    IMG_ColorModel		getColorModel(int idx=0) const;

    IMG_DataType		*getDataTypes()		{ return myDataType; }
    IMG_ColorModel		*getColorModels()	{ return myColorModel; }

    // Writes a custom tag to the device. The tag must have a name, and some
    // number of string values (zero is acceptable).
    virtual void		writeCustomTag(const char *tagname, int size,
						const char *const *values);

    // Short cut method to write a well-known tag.
    void			writeKnownTag(IMG_TagType tag_id, int size,
						const char *const *values)
    {
	writeCustomTag(IMGtagName(tag_id), size, values);
    }


    /// The data written is in the format specified by the IMG_ColorModel and
    /// IMG_DataType parameters in the open.  The size of the tile is smaller
    /// or equal to the sizes specified in the open.  If the device was
    /// opened using openMulti(), the data is provided as a sequence of
    /// planes - first all pixels for plane 0, then all pixels for plane 1,
    /// etc.
    ///
    /// The rectangle spectified by @c x0, @c x1, @c y0 and @c y1 is an
    /// inclusive rectangle.  That is, the number of pixels in @c data is:
    /// @c (x1-x0+1)*(y1-y0+1).
    ///
    /// The first pixel in the data buffer is the bottom left corner of the
    /// tile (i.e. @c (x0,y0)).
    ///
    /// The rectangle specified is also relative to the data window.  So, to
    /// offset to get the absolute pixel location you need to take the data
    /// window (getDataWindow()) into account.  You can also use the
    /// convenience methods @c getDataX() and getDataY().  For example: @code
    ///	   unsigned pixel_x0 = x0 + getDataX();
    ///	   unsigned pixel_x1 = x1 + getDataX();
    ///	   unsigned pixel_y0 = y0 + getDataY();
    ///	   unsigned pixel_y1 = y1 + getDataY();
    /// @endcode
    virtual int writeTile(const void *data,
                    unsigned x0, unsigned x1,
                    unsigned y0, unsigned y1) = 0;

    /// This is just for sending sample data from Mantra to Houdini
    /// via TIL_TileMPlay.
    /// data contains nsamples tuples of floats_per_sample floats
    /// samples contains nsamples (x,y,pixel_sample) tuples
    virtual bool writeSamples(
        int nsamples,
        int floats_per_sample,
        const UT_Vector3T<int> *samples,
        const float *data)
    {
        UT_ASSERT_MSG(0, "Should be implemented by subclass in relevant cases!");
        return false;
    }

    virtual void		flush();

    virtual int			close(bool keep_alive=false) = 0;

    // If the flush method is unsafe for multi-processing (or very costly), the
    // device should return 1 here.
    virtual int			unsafeFlush();

    /// The renderer will ask us for the next tile location to be rendered.
    /// We give it the location (in renderable region coordinates).  The
    /// function returns true if this is a valid tile, otherwise false.
    /// The tile mode maps to the enum of bottom/top first or random order.
    /// (tx,ty) will be the unique tile index tuple, between (0,0) inclusive
    /// and (myTileCountX,myTileCountY) exclusive.
    ///
    /// NOTE: xl, xr, yb, and yt, are *all* inclusive!
    ///       Also, xl <= yr and yb <= yt if the return value is true,
    ///       i.e. at least one pixel.
    virtual bool         getNextTile(int &xl, int &xr, int &yb, int &yt,
                                    int &tx, int &ty);

    // Returns true if a tile has been clicked; otherwise, false.  If a tile
    // has been clicked then tx and ty will be set to that tile's coordinates;
    // otherwise, the values of tx and ty are undefined.
    // The radius is in pixels.
    virtual bool	getClickedTile(int &tx, int &ty, float &radius) const;

    virtual void	resetTileRequestor();
    virtual int		getTileMode() const;

    virtual void	terminateOnConnectionLost(bool ) {}

    // Whether this object wants the Houdini HScript port to be written
    // to it via writeCustomTag("houdiniportnum", ...)
    virtual bool	wantsHoudiniPort() const { return false; }

    /// If this tile device is interactive (i.e. a flipbook), this method
    /// should return true.  When mantra runs in batch-mode, opening an
    /// interactive device will fail.
    virtual bool	isInteractive() const	{ return false; }

    /// @{
    /// Accessor
    int		xres() const	{ return myDisplayWindow.width(); }
    int		yres() const	{ return myDisplayWindow.height(); }
    /// @}

    // Passes style information to this tile device so it can be sent out
    // later on.
    virtual void	setStyleSheets(UT_StringArray&) {}

    /// Subclasses can override this method if they respond to mouse clicks.
    /// x and y should be set to the mouse position (in pixels) if the mouse
    /// has been clicked.  The method returns true if the mouse has been
    /// clicked; otherwise, false.  If the method returns false then the
    /// values of x and y are undefined.
    virtual bool	getMouseClickPosition(int &x, int &y) const
			    { return false; }

    /// Subclasses can override this method if they respond to mouse clicks
    /// and specify a radius for the click.
    virtual float       getMouseClickRadius() const
    { return -1.0f; }

protected:
    // The sub-class must call this function to initialize the base class
    // correctly.
    void	setResolution(int xres, int yres,
			      int tilewidth, int tileheight,
			      int tile_order = IMG_TILE_BOTTOM_FIRST);

    void	setModel(const IMG_TileOptions &finfo);
    void	setModel(IMG_TileOptionList &flist);
    void	setModel(const IMG_DataType *dt, const IMG_ColorModel *cm);

    // Some random access devices may want to tell the renderer which tile to
    // render next.  This function will get the coordinates of the tile (i.e.
    // if there are N tiles in X and M tiles in Y, tx is (0,N) and ty is (0,M)
    void	mapImageToTile(int ix, int iy, int &tx, int &ty);
    void	getTileBounds(int tx, int ty,
			      int &xl, int &xr, int &yb, int &yt);

    bool	hasDataWindow(int full_xres, int full_yres) const;
    int		getDataX() const	    { return myDataWindow.x1(); }
    int		getDataY() const	    { return myDataWindow.y1(); }
    int		getDataWidth() const	    { return myDataWindow.width(); }
    int		getDataHeight() const	    { return myDataWindow.height(); }

    /// Specify the desired tile order.
    void	setTileOrder(IMG_TILE_ORDER tile_order)
		    { myRandomAccess = tile_order; }

    /// In the open method, the sub-class should set the file count
    void	setFileCount(int count)
		    { myFileCount = count; }

    /// Set the tile width
    void	setTileSize(int width, int height);

    SYS_DEPRECATED_REPLACE(16.0, "dataWindow()")
    UT_InclusiveRect getDataWindow() const;

    /// @{
    /// Accessors to data, display render windows
    const UT_DimRect	&dataWindow() const	{ return myDataWindow; }
    const UT_DimRect	&displayWindow() const	{ return myDisplayWindow; }
    const UT_DimRect	&renderWindow() const	{ return myRenderWindow; }
    /// @}

private:
    fpreal		 myAspectRatio;
    int			 myFileCount;
    int			 myTileWidth, myTileHeight;
    UT_DimRect		 myDataWindow;
    UT_DimRect		 myDisplayWindow;
    UT_DimRect		 myRenderWindow;
    IMG_TILE_ORDER	 myRandomAccess; // If device supports random access

    int			 myClickedTileXY[2];
    float                myClickedRadius;
    int			 myFilterX, myFilterY;
    int			 myTileCountX, myTileCountY;
    int			 myRandomCheck;
    IMG_DataType	*myDataType;
    IMG_ColorModel	*myColorModel;
    IMG_TileInterface	*myTiles;
    UT_String		 myRenderSourceName;
};

#endif
