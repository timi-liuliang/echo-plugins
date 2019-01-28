/*
* PROPRIETARY INFORMATION.  This software is proprietary to
* Side Effects Software Inc., and is not to be reproduced,
* transmitted, or disclosed in any way without written permission.
*
* NAME:        TIL_AdaptiveImage.h (TIL Library, C++)
*
* COMMENTS:    A tree-based sparse image class
*/

#pragma once

#ifndef __TIL_AdaptiveImage__
#define __TIL_AdaptiveImage__

#include "TIL_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_Assert.h>
#include <UT/UT_UniquePtr.h>

class TIL_PixelFilter;
class TIL_Raster;

class TIL_API TIL_AdaptiveImage
{
public:
    TIL_AdaptiveImage(int width, int height,
        int nplanes, const int *num_plane_components,
        int adaptivity_plane = -1,
        int levels_below_pixel = 0,
        int min_samples_per_pixel = 1,
        int max_samples_per_pixel = 1024,
        float relative_noise_threshold = 0.005f);

    bool sample(uint64 seed, int &chosen_x, int &chosen_y, int &pixel_sample);

    void insert(int x, int y, int pixel_sample, const float *data);

    struct Level
    {
        /// Per-component sums for each pixel:
        /// nComponents x width x height
        UT_Array<UT_UniquePtr<float[]> > myComponentSums;

        /// Sums for each pixel over all components of
        /// squared clamped sample values:
        /// width x height
        UT_Array<UT_UniquePtr<float[]> > mySumsOfSquares;

        /// Number of samples "committed" for each pixel, i.e. the number of
        /// times that sample() has chosen a pixel in the corresponding area.
        /// width x height
        UT_UniquePtr<exint[]> mySampledCounts;

        /// Number of samples actually inserted for each pixel:
        /// width x height
        UT_UniquePtr<exint[]> myCounts;

        /// Unnormalized weights for adaptive sampling, before interpolating
        /// between levels:
        /// width x height
        UT_UniquePtr<float[]> myAdjustedVariances;

        /// Unnormalized weights for adaptive sampling, after interpolating
        /// between levels:
        /// width x height
        UT_UniquePtr<float[]> myWeights;

        /// Width of this level in pixels
        int myWidth;

        /// Height of this level in pixels
        int myHeight;
    };

    int getPixelLevelNum() const
    {
        return (myRootPowerOf2+1) - 1 - myLevelsBelowPixel;
    }
    const Level &getLevel(int leveli) const
    {
        return myLevels[leveli];
    }

    bool isPriorityCircleOn() const
    {
        return myPriorityCircleOn;
    }
    void getPriorityCircle(float &x, float &y, float &radius) const
    {
        x = myPriorityCircleX;
        y = myPriorityCircleY;
        radius = myPriorityCircleRadius;
    }
    void setPriorityCircle(float x, float y, float radius)
    {
        myPriorityCircleX = x;
        myPriorityCircleY = y;
        if (radius < 0.5f)
            radius = 0.5f;
        myPriorityCircleRadius = radius;
        myPriorityCircleOn = true;
    }
    void clearPriorityCircle()
    {
        myPriorityCircleOn = false;
    }

    static exint pixelArea(
        int x, int y, int level_width, int level_height, int leveli,
        int pixel_width, int pixel_height, int pixel_leveli);

    /// Makes the image as if no samples had been inserted or sampled yet.
    void clearSamples();

    exint getInsertedCount() const
    {
        return myInsertedCount;
    }

    /// Filters the given plane into the specified raster.
    void filterPlane(
        TIL_Raster &raster,
        int planei) const;

    const TIL_PixelFilter *getPixelFilter(int planei) const
    {
        UT_ASSERT(planei >= 0 && planei < myNumPlanes);
        if (myPixelFilters && planei >= 0 && planei < myNumPlanes)
            return myPixelFilters[planei];
        return nullptr;
    }
    void initPixelFilterArray()
    {
        UT_ASSERT(myNumPlanes > 0);
        if (myNumPlanes <= 0)
            return;
        myPixelFilters.reset(new const TIL_PixelFilter*[myNumPlanes]);
        for (int i = 0; i < myNumPlanes; ++i)
            myPixelFilters[i] = nullptr;
    }
    void setPixelFilter(int planei, const TIL_PixelFilter *filter);

private:
    void updateVarianceEntry(
        const int cur_x, const int cur_y,
        const exint cur_i, const int leveli);
private:
    UT_UniquePtr<Level[]> myLevels;
    exint mySampledCount;
    exint myInsertedCount;
    int myAdaptivityPlane;
    int myRootPowerOf2;
    int myWidth;
    int myHeight;

    /// This is the number of levels that are below the level of the
    /// final image pixels.  These levels are so that extra detail can be
    /// present for better pixel filtering later, instead of being
    /// limited to one combined sample per pixel.
    int myLevelsBelowPixel;

#if 0
    /// If every final image pixel (myLevelsBelowPixel above the lowest level),
    /// has at least this many samples, and any pixel with more relative noise
    /// than myRelativeNoiseThreshold has hit myMaxSamplesPerPixel, we can stop.
    /// This is to ensure that at least enough samples are sent to detect
    /// possible noise, e.g. in very indirectly lit regions, where most samples
    /// are all black, but an occasional few are bright.
    int myMinSamplesPerPixel;
#endif

    /// If every final image pixel (myLevelsBelowPixel above the lowest level),
    /// has at least myMinSamplesPerPixel samples, and any pixel with more
    /// relative noise than myRelativeNoiseThreshold has hit this many samples,
    /// we can stop.
    /// This is to avoid a few pixels that would effectively never converge
    /// from preventing an otherwise good render from finishing.
    int myMaxSamplesPerPixel;

#if 0
    /// If every final image pixel (myLevelsBelowPixel above the lowest level),
    /// has at least myMinSamplesPerPixel samples, and any pixel with more
    /// relative noise than this has hit myMaxSamplesPerPixel, we can stop.
    /// This is effectively the target final relative noise.
    float myRelativeNoiseThreshold;
#endif

    /// Variables indicating where any priority circle goes.
    /// @{
    bool myPriorityCircleOn;
    float myPriorityCircleX;
    float myPriorityCircleY;
    float myPriorityCircleRadius;
    /// @}

    int myNumPlanes;
    UT_UniquePtr<int[]> myPlaneStartComponents;
    UT_UniquePtr<int[]> myPlaneNumComponents;
    UT_UniquePtr<const TIL_PixelFilter*[]> myPixelFilters;
};

#endif
