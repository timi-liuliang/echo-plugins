/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_DeepRaster.h ( TIL Library, C++)
 *
 * COMMENTS:	A data structure containing multiple planes of TIL_Raster data.
 * 		This class will ensure that all rasters have data allocated.
 */

#ifndef __TIL_DeepRaster__
#define __TIL_DeepRaster__

#include "TIL_API.h"

#include "TIL_AdaptiveImage.h"
#include "TIL_Raster.h"
#include "TIL_Sequence.h"
#include <UT/UT_Rect.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_UniquePtr.h>
#include <SYS/SYS_Types.h>

class TIL_API TIL_DeepRaster
{
public:
	     TIL_DeepRaster();
	     TIL_DeepRaster(const TIL_DeepRaster &);
    virtual ~TIL_DeepRaster();

    virtual int64	getMemoryUsage(bool inclusive) const;

    /// Copies info and data over from src, and replaces the existing one.
    /// 'scope' allows a selective copy of planes from src (ie, "C A Pz")
    /// and 'invert_scope' will interpret the list as planes not to copy if true
    void		copy(const TIL_DeepRaster &src,
			     const char *scope = NULL,
			     bool invert_scope = false);

    /// Appends the info and data from src to the existing data.
    void		append( const TIL_DeepRaster &src);

    /// Allocate a TIL_Raster for each plane defined by the sequence.  Each
    /// raster will be allocated with interleaved packing.  If the plane
    /// can't be allocated using a TIL_Raster (i.e. vector size of 7, array
    /// size of 2), then the raster is un-initialized.
    ///
    /// When a vector size of 2 is detected as a plane type, we have the
    /// option of bumping the vector size to 3.  This is so that the
    /// TIL_Raster can be displayed by the OGL code.  This is turned on by
    /// default.  Just be aware of this case when dealing with the rasters.
    void	create(const TIL_Sequence &sequence,
			bool change_vector2_to_vector3=true,
			bool allocate_raster_storage=true);

    int			 getRasterCount() const
				    { return mySequence.getNumPlanes(); }
    TIL_Sequence	&getSequence()		{ return mySequence; }
    const TIL_Sequence	&getSequence() const	{ return mySequence; }
    TIL_Raster		&getRaster(int i)	{ return myRasters[i]; }
    const TIL_Raster	&getRaster(int i) const	{ return myRasters[i]; }

    /// Write a tile of interleaved data into the plane specified.
    bool		 writeTile(int plane_index,
				int x0, int x1, int y0, int y1,
				const void *data);

    /// Write the given sample data into the image.
    bool writeSamples(
        int nsamples,
        int floats_per_sample,
        const UT_Vector3T<int> *samples,
        const float *data);

    /// Clear the raster outside the bounding box specified.  If the plane
    /// index is -1, all planes will be cleared to the color.  This does not
    /// affect the "written" bounding box.  To clear the whole raster,
    /// simply pass in:  y0=0, y1=-1 (or y0=yres, y1=0)
    bool		 clearOutside(int plane_index,
				int x0, int x1, int y0, int y1,
				fpreal value=0);

    /// Clear all pixels to the given value.
    void                 clearPixels(
                            fpreal value = 0,
                            bool stash_samples = false);

    /// Clear all samples in myAdaptiveImage, without changing
    /// the rasters.
    void                 clearSamples(
                            bool stash_samples = false);

    // Set/Get attributes of the deep raster
    const UT_StringHolder &getName() { return myName; }
    void		 setName(const UT_StringHolder &name);
    void		 setCropWindow(const UT_DimRect &rect)
			    { myCropWindow = rect; }
    bool		 hasCropWindow() const
			    { return !myCropWindow.isEmpty(); }
    const UT_DimRect &	 getCropWindow() const
			    { return myCropWindow; }
    UT_DimRect &	 getCropWindow()
			    { return myCropWindow; }
    bool		 hasDataWritten() const
			    { return !myWriteBox.isEmpty(); }
    UT_DimRect &	 getWrittenWindow()
			    { return myWriteBox; }
    const UT_DimRect &	 getWrittenWindow() const
			    { return myWriteBox; }

    // Set attributes of individual planes
    void		 setEdgeStreak(int plane,
				int ex0, int ex1, int ey0, int ey1);
    bool		 getEdgeStreak(int plane,
				int &ex0, int &ex1, int &ey0, int &ey1);
    void		 clearEdgeStreak(int plane);
    
    void		 setBlackPoint(int plane, fpreal bpoint);
    void		 setWhitePoint(int plane, fpreal wpoint);

    bool		 swapOut()	{ return writeToDisk(); }
    bool		 swapIn()
			 {
			     return mySwapped ? readFromDisk() : true;
			 }
    void		 clearSwapFile()
			 {
			     if (mySwapped)
			     {
				 swapIn();
				 removeDiskFile();
			     }
			 }

    const TIL_AdaptiveImage *getAdaptiveImage() const
    {
        return myAdaptiveImage.get();
    }

    int numPlaneSourceComponents(int planei) const;
    int numPlaneDestComponents(int planei) const;

    int planeSourceStartComponent(int planei) const;

private:

    void		 copyArrays( TIL_Raster *tmp_Rasters, int *tmp_BPP, 
				bool *tmp_2Bit, bool *tmp_EdgeStreakFlag, 
				int *tmp_EdgeStreak, const TIL_DeepRaster &src);
    void		 clear();
    void		 initialize();

    bool		 writeToDisk();
    bool		 readFromDisk();
    void		 removeDiskFile();

    TIL_Sequence	 mySequence;
    UT_UniquePtr<TIL_Raster[]> myRasters;
    UT_UniquePtr<TIL_AdaptiveImage> myAdaptiveImage;
    UT_UniquePtr<int[]>  myBPP;
    UT_UniquePtr<bool[]> my2Bit;
    UT_UniquePtr<bool[]> myEdgeStreakFlag;
    UT_UniquePtr<int[]>  myEdgeStreak;
    char		*mySwapFile;

    // Global attributes
    UT_StringHolder      myName;        // Name of raster
    UT_DimRect           myCropWindow;  // Crop window (data window)
    UT_DimRect           myWriteBox;    // Box of written pixels
    bool                 mySwapped;
};

#endif
