/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RAY_PixelFilter.h ( RAY Library, C++)
 *
 * COMMENTS:	A pixel filter takes a sample buffer and filters the samples
 *		into a pixel buffer.
 */

#ifndef __RAY_PixelFilter__
#define __RAY_PixelFilter__

#include "RAY_API.h"
#include "RAY_SpecialChannel.h"
#include <SYS/SYS_Types.h>
#include <SYS/SYS_Visibility.h>

class RAY_Imager;
class RAY_SampleBuffer;
class RAY_PixelFilter;

extern "C" {
    /// When a pixel filter is defined as a dynamic object, allocPixelFilter()
    /// is called to build a new instance of the filter.  The name passed
    /// in will be the name defined by the table entry.  This can be used for:
    ///
    /// a)  Error checking -- you can verify that you're name is what you
    ///     expect.  However, this isn't mandatory.
    /// b)  To have more than one pixel filter defined per dynamic object.
    ///     Using the name, you can create pixel filters of different types.
    SYS_VISIBILITY_EXPORT extern RAY_PixelFilter *
        allocPixelFilter(const char *name);
}

class RAY_API RAY_PixelFilter {
public:
    RAY_PixelFilter() {}
    virtual ~RAY_PixelFilter() {}

    static RAY_PixelFilter *getPixelFilter(const char *name);
    static RAY_PixelFilter *getSubPixelFilter();

    typedef RAY_PixelFilter*(*Constructor)(const char *name);
    static void registerPixelFilter(
        const char *name,
        Constructor constructor);

    virtual void setArgs(int argc, const char *const argv[]) = 0;
    virtual RAY_PixelFilter *clone() const = 0;

    /// Create any imager planes required by this pixel filter
    virtual void addNeededSpecialChannels(RAY_Imager &imager) {}

    /// Indicate that the specified special channel must be added for this filter.
    static void addSpecialChannel(RAY_Imager &imager, RAY_SpecialChannel channel);

    /// Get the channel index of the specified special channel
    static int getSpecialChannelIdx(const RAY_Imager &imager, RAY_SpecialChannel channel);

    /// Returns -1 if no channel with the specified name exists
    static int getChannelIdxByName(const RAY_Imager &imager, const char *name);

    /// Returns -1 if no channel with the specified channel index exists
    static int getChannelVectorSize(const RAY_Imager &imager, int channel);

    /// Get the data for the channel with the specified index
    static const float *getSampleData(const RAY_SampleBuffer &source, int channel);

    /// Prepare for filtering a tile.
    /// sxres = samples per pixel in x direction
    /// syres = samples per pixel in y direction
    virtual void prepFilter(int sxres, int syres) = 0;

    /// Query the filter width.  A filter width of 1 means that the samples
    /// filtered will map exactly to the corresponding pixels in the output
    /// image (i.e. no sampling outside the bounds)
    virtual void getFilterWidth(float &x, float &y) const = 0;

    /// Perform the filtering
    ///
    /// Read data from source.getSampleData(channel), which is sourcewidth
    /// by sourceheight, and has vectorsize floats per value, (1, 3, or 4).
    ///
    /// Write data to destination, which is destwidth by destheight, has
    /// the same vectorsize, and whose unfiltered (ubox) source data would
    /// have started at (destxoffsetinsource, destyoffsetinsource).  The
    /// number of pixel samples in each pixel's unfiltered data was already
    /// passed into prepFilter, and should have been saved, if needed.  For
    /// each destination pixel, the all of the data that pixel depends upon
    /// should be present in source.
    ///
    /// imager is provided in case any special channels are needed for this
    /// channel's filtering or if addition information about this channel
    /// is needed.
    virtual void filter(
        float *destination,
        int vectorsize,
        const RAY_SampleBuffer &source,
        int channel,
        int sourcewidth,
        int sourceheight,
        int destwidth,
        int destheight,
        int destxoffsetinsource,
        int destyoffsetinsource,
        const RAY_Imager &imager) const = 0;
};

#endif
