/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_Stat.h ( IMG Library, C++)
 *
 * COMMENTS:	Information about a raster IO stream.  This is what's passed
 *		around to read/write the raster.  It's virtual so that each
 *		Format can define it's on private information to stash and keep
 *		around.
 */

#ifndef __IMG_Stat__
#define __IMG_Stat__

#include "IMG_API.h"

#include "IMG_FileTypes.h"
#include "IMG_Plane.h"

#include <UT/UT_String.h>
#include <UT/UT_Rect.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_SharedPtr.h>


class IMG_Format;
class IMG_Plane;
class IMG_DeepStat;


/// @brief Contains the details of a specific image file, used by IMG_File.
/// This class contains all the high-level details needed to save an image file,
/// or query a loaded image file's statistics. It contains such data as the
/// resolution, pixel aspect ratios, data formats and image planes. The set()
/// methods are used when creating a new IMG_Stat for writing an image file, 
/// while the get() methods are generally used when reading.
class IMG_API IMG_Stat
{
public:
    typedef UT_SharedPtr<IMG_DeepStat>	img_DeepStatPtr;
    IMG_Stat();
    /// Sets up the resolution of the image. addPlane() or addDefaultPlane()
    /// must be called to fill out the  
    IMG_Stat(unsigned xres, unsigned yres);

    /// sets up a single image plane format, without requiring addPlane().
    /// Don't use this for deep raster images.
    IMG_Stat(unsigned xres, unsigned yres,
	     IMG_DataType dt, IMG_ColorModel cm);
    
    ~IMG_Stat();

    int64 getMemoryUsage(bool inclusive) const;

			 IMG_Stat(const IMG_Stat &s)
			 {
			     copy(s);
			 }
    IMG_Stat		&operator=(const IMG_Stat &s)
			 {
			     copy(s);
			     return *this;
			 }
    void		copy(const IMG_Stat &src);

    /// @{
    /// Read/write image a specific mode. If the format does not support
    /// it, or the image being read is not of that type, IMG_File::open or
    /// IMG_File::create calls will fail.
    /// The default value is IMG_TYPE_2D.
    void 		setImageType(IMG_ImageType it);

    IMG_ImageType	getImageType() const { return myImageType; }
    /// @}

    /// @{
    /// Sets the resolution for the image, if not specified by the constructor.
    void		setResolution(unsigned xres, unsigned yres)
			{
			    myXres = xres;
			    myYres = yres;
			    recomputeOffsets();
			}

    /// Returns the resolution of the image to be written or the one that was
    /// read.
    /// @sa getDataWidth
    /// @sa getDataHeight
    unsigned		getXres() const	{ return myXres; }
    unsigned		getYres() const { return myYres; }
    /// @}

    /// @{
    /// Pixel aspect ratio of the file (X/Y). Square pixels are 1.0 (default).
    void		setAspectRatio(fpreal aspect) { myAspectRatio=aspect; }
    fpreal		getAspectRatio() const	      { return myAspectRatio; }
    /// @}


    /// filename of the opened file. Files attached to streams may not have 
    /// a filename.
    /// @{
    void		 setFilename(const char *name){ myName.harden(name); }
    const UT_String	&getFilename() const	      { return myName; }
    /// @}

    /// @{
    /// @brief Returns the number of image planes in the image
    /// a plane contains the actual structure of the image data. An image
    /// needs at least one. Deep rasters can have multiple planes.
    int			 getNumPlanes() const;
    /// @brief Get an image plane by index
    IMG_Plane		*getPlane(int i = 0) const;
	
    /// @brief Get an image plane by name
    /// Returns the plane with name 'name', or NULL if no matching plane is 
    /// found.
    IMG_Plane		*getPlaneName(const char *name) const;

    /// @brief Returns the index of the plane with 'name'
    /// Returns the index of the plane with name 'name', or -1 if not found.
    int			 getPlaneIndex(const char *name) const;
    /// @}

    /// @{
    /// @brief Add a plane to an image file.
    /// Add a plane to the image file. This is only used when writing files.
    /// Plane names must be unique. To write a file, at least one plane must be
    /// present.
    IMG_Plane		*addPlane(const char *name,
				  IMG_DataType d,
				  IMG_ColorModel cm,
				  const char *c0name = 0,
				  const char *c1name = 0,
				  const char *c2name = 0,
				  const char *c3name = 0);
    /// @private
    IMG_Plane		*addPlane(const IMG_Plane &pi);
    /// @brief Add a default 8bit, RGBA plane to the image.
    IMG_Plane		*addDefaultPlane()
		{ return addPlane("C", IMG_UCHAR, IMG_RGBA, "r","g","b","a"); }
    /// @}

    /// Adds 6 planes named Left, Right, Top, Bottom, Front and Back, all using
    /// the same image format.
    void		 addEnvMapPlanes(IMG_DataType dt, IMG_ColorModel cm);

    /// isEnvMap() will return true, only if the image is an environment map.
    bool		 isEnvMap() const	{ return myEnvMapPlanes; }

    /// Sets the environment flag, if the image planes can be treated
    /// as an environment map.
    /// @return	    True if setting to the environment map was successful,
    ///		    false otherwise.
    bool		 setEnvMap();
    
    /// @private
    void		 insertPlane(IMG_Plane *plane, int beforeindex);

    /// @{
    /// Remove a plane by index from the stat. Used only for writing files.
    void		 removePlane(int pindex);
    /// Remove a plane by index from the stat. Used only for writing files.
    void		 removePlane(const char *name);
    /// Remove all planes from the stat. Used only for writing files.
    void		 removeAllPlanes();
    /// @}

    /// Alters the order of planes in the stat.
    void		 reorderPlane(IMG_Plane *plane, int beforeindex);

    /// @{
    /// Swap two planes
    void		 swapPlaneOrder(int plane_a, int plane_b);
    void		 swapPlaneOrder(const IMG_Plane *p, int plane_b)
			    { swapPlaneOrder(p->getPlaneIndex(), plane_b); }
    /// @}

    /// @{
    /// @brief Manipulate the data window of an image.
    /// The data window is constant across all planes. It may be smaller than
    /// the actual resolution (crop region) or larger. The data window is 
    /// inclusive in that x2 and y2 are part of the region. You can also use
    /// the UT_DimRect versions for easier manipulations of the area.
    /// Image data is only defined for the area contained by the data window.
    /// By default, it is (0,0) - (xres-1, yres-1) (the entire image). 
    void	 	 setDataWindow(const UT_DimRect &win);
    void		 setDataWindow(int x1, int y1, int x2, int y2);
    void		 clearDataWindow();

    /// @brief Get the data window of an image.
    void		 getDataWindow(UT_DimRect &r) const;
    /// @brief Get the data window of an image.
    void		 getDataWindow(int &x1, int &y1,
				       int &x2, int &y2) const;
    /// @brief Return the data window for the image
    const UT_DimRect	&getDataWindow() const { return myDataWindow; }
    /// @brief Returns true if the image has a data window
    bool		 hasDataWindow() const;
    /// @}

    /// The image data outside the data window can be represented one of two 
    /// ways: with a constant color, or by streaking the edges of the window.
    /// @{
    void		 setBackgroundColor(fpreal col[4]);
    void		 getBackgroundColor(fpreal col[4]) const;
    
    void		 setBackgroundStreak(bool streak)
				{ myDataStreakFlag = streak; }
    bool		 getBackgroundStreak() const {return myDataStreakFlag;}
    /// @}

    /// This returns the width of the image. If no data window is
    /// present, then this is Xres. If a data window is present, then
    /// this is the width of the data window.
    int			 getDataWidth() const;
    /// This returns the height of the image. If no data window is
    /// present, then this is Yres. If a data window is present, then
    /// this is the height of the data window.
    int			 getDataHeight() const;

    /// @deprecated
    /// the number of frames in the file. Mostly for movie formats.
    int			getNumFrames() const { return myNumFrames; }
    /// @deprecated
    /// the number of frames in the file. Mostly for movie formats.
    void		setNumFrames(int nf) { myNumFrames = nf; }

    /// @deprecated the FPS for frame playback.
    fpreal		getFPS() const { return myFPS; }
    /// @deprecated the FPS for frame playback.
    void		setFPS(fpreal fps) { myFPS = fps; }

    /// If the image file has a recorded render time, this
    /// returns the render time, else it returns -1.
    fpreal              getRenderTime() const { return myRenderTime; }
    /// Sets the render time from the image file.
    void                setRenderTime(fpreal t) { myRenderTime = t; }

    /// If the image file has a recorded render memory, this
    /// returns the render memory, else it returns 0.
    int64               getRenderMemory() const { return myRenderMemory; }
    /// Sets the render time from the image file.
    void                setRenderMemory(int64 mem) { myRenderMemory = mem; }

    /// @{
    /// @brief Set how deep samples are interpolated.
    void			 setDeepStat(const img_DeepStatPtr &deep)
					 { myDeepStat = deep; }
    const img_DeepStatPtr	&getDeepStat() const
					 { return myDeepStat; }
    /// @}

	
    // prints out the color model and data type.
    static const char	*getColorModel(IMG_ColorModel cm);
    static const char	*getDataType(IMG_DataType dt);

    /// Returns the total number of components across all planes.
    int64		getComponentCount() const;

    /// Returns the number of components for the deep pixel image. This
    /// excludes any pseudo-planes, such as "Depth-Complexity".
    int64 	 	getDeepPixelComponentCount() const;


    /// Returns the number of bytes for a single pixel made up of all planes.
    int64		bytesPerPixel() const;

    /// Returns the total number of bytes for the entire image. Returns -1 for
    /// deep images, since the size is dependent on sample count.
    int64		bytesPerImage() const;

    /// Returns the total number of bytes for a single scanline. Returns -1 for
    /// deep images, since the size is dependent on sample count.
    int64		bytesPerScanline() const;

    /// @{
    /// Returns the number bytes for a single scanline. Returns -1 for
    /// deep images, since the size is dependent on sample count.
    int64		bytesPerPlaneScan(const IMG_Plane &pi) const;
    int64		bytesPerPlaneScan(int i) const;
    /// @}

    /// @{
    /// For 2D images, returns the offset into the data for a plane in a given
    /// scanline. Plane data is arranged consecutively by scanline.
    /// For deep images, returns the offset into a pixel sample for a given
    /// plane's data. Planes are interleaved by a pixel sample.
    int			planeOffset(const IMG_Plane &pi) const;
    int			planeOffset(int i) const;
    /// @}

    /// Sets the color space for all planes. gamma is only needed for the
    /// PXL_CS_CUSTOM_GAMMA space.
    void		setColorSpace(PXL_ColorSpace cs, fpreal gamma = 0.0);

    /// Set the color space on planes with an unknown color space to 'cs'.
    /// gamma is only needed for the PXL_CS_CUSTOM_GAMMA space.
    void		setColorSpaceForUnknownPlanes(PXL_ColorSpace cs,
						      fpreal gamma = 0.0);


protected:
    friend class IMG_Plane;
    void		recomputeOffsets();
private:
    void		init(int xres, int yres);

    IMG_ImageType		myImageType;
    UT_String			myName;
    UT_ValArray<IMG_Plane *>	myPlanes;
    UT_IntArray			myPlaneOffset;
    fpreal			myDataWindowColor[4];
    UT_DimRect			myDataWindow;
    fpreal			myAspectRatio;
    fpreal			myFPS;
    fpreal                      myRenderTime;
    int64                       myRenderMemory;
    img_DeepStatPtr		myDeepStat;
    unsigned myXres,		myYres;
    int				myNumFrames;
    int				myScanlineSize;
    bool			myDataStreakFlag;
    bool			myHasDataWindow;
    bool			myEnvMapPlanes;
};

#endif

