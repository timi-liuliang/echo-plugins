/*
* PROPRIETARY INFORMATION.  This software is proprietary to
* Side Effects Software Inc., and is not to be reproduced,
* transmitted, or disclosed in any way without written permission.
*
* NAME:        TIL_PixelFilter.h (TIL Library, C++)
*
* COMMENTS:    Pixel filter classes, for processing TIL_AdaptiveImage samples.
*/

#pragma once

#ifndef __TIL_PixelFilter__
#define __TIL_PixelFilter__

#include "TIL_API.h"
#include "TIL_Raster.h"
#include <PXL/PXL_Common.h>
#include <UT/UT_Assert.h>
#include <SYS/SYS_Inline.h>

class TIL_AdaptiveImage;

/// This is the super-duper-class for all pixel filters in TIL.
class TIL_API TIL_PixelFilter
{
public:
    virtual ~TIL_PixelFilter() {}

    virtual void filterPlaneVirtual(
        const TIL_AdaptiveImage &src,
        TIL_Raster &dest,
        int planei,
        int plane_src_components) const = 0;

    /// When vetsSamples() returns true, this should be called
    /// so that the pixel filter can decide how to combine samples,
    /// instead of just summing.
    /// NOTE: nnew_samples is 1 when adding a new sample,
    ///       but may be more than 1 when combining pixels
    ///       into higher levels of the TIL_AdaptiveImage.
    virtual void combineSample(
        int ncomponents,
        exint nexisting_samples,
        float *existing,
        exint nnew_samples,
        const float *new_sample) const
    {
        UT_ASSERT_MSG(0, "Either this should be overridden by the subclass, or it shouldn't be called.  Check vetsSamples().");
        // This is just an example of the default behaviour.
        for (int i = 0; i < ncomponents; ++i)
            existing[i] += new_sample[i];
    }

    /// Returns true iff this filter requires TIL_AdaptiveImage
    /// to initialize TIL_AdaptiveImage::Level::mySumsOfSquares for
    /// the plane that this filter is applied to.
    SYS_FORCE_INLINE
    bool needsSquares() const
    {
        return myNeedsSquares;
    }

    /// Returns true iff this filter requires TIL_AdaptiveImage
    /// to initialize TIL_AdaptiveImage::Level::myWeights from
    /// the adaptivity plane.
    SYS_FORCE_INLINE
    bool needsWeights() const
    {
        return myNeedsWeights;
    }

    /// Returns true iff TIL_AdaptiveImage needs to call
    /// combineSample when inserting a sample to a plane
    /// with this filter.
    SYS_FORCE_INLINE
    bool vetsSamples() const
    {
        return myVetsSamples;
    }

    /// Returns false iff TIL_AdaptiveImage needs to create/delete it.
    /// true means that it's owned globally, so will be deleted when the
    /// process exits.
    virtual bool ownedBySingleton() const
    {
        return false;
    }

protected:
    TIL_PixelFilter()
        : myNeedsSquares(false)
        , myVetsSamples(false)
        , myNeedsWeights(false)
    {}
    TIL_PixelFilter(
        bool needs_squares,
        bool vets_samples,
        bool needs_weights)
        : myNeedsSquares(needs_squares)
        , myVetsSamples(vets_samples)
        , myNeedsWeights(needs_weights)
    {}

private:
    const bool myNeedsSquares:1;
    const bool myNeedsWeights:1;
    const bool myVetsSamples:1;
};

/// Pixel filters should inherit from this using the "curiously-recurring
/// template pattern", so that this class can call the templated filterPlane
/// function in the subclass, e.g.:
/// class TIL_MyPixelFilter : public TIL_PixelFilterT<TIL_MyPixelFilter>
/// {
///     template<int NSRCCOMPONENTS,typename T,int NDSTCOMPONENTS,bool INTERLEAVED>
///     void filterPlane(
///         const TIL_AdaptiveImage &src,
///         PXL_RasterWrapper<T,NDSTCOMPONENTS,INTERLEAVED> dest,
///         int planei,
///         int plane_start_src_component) const;
/// };
/// The primary advantage of this middle class is to unpack how the raster
/// is stored, so that it can be accessed more efficiently in the pixel filter.
/// Of course, this means that filterPlane will get compiled 45 times, for the 5
/// data types and 9 packing formats, (including the two 2-to-3 formats).
template<typename SUBCLASS>
class TIL_PixelFilterT : public TIL_PixelFilter
{
public:
    typedef SUBCLASS SubclassType;

    virtual ~TIL_PixelFilterT() {}

    virtual void filterPlaneVirtual(
        const TIL_AdaptiveImage &src,
        TIL_Raster &dest,
        int planei,
        int plane_src_components) const override
    {
        // Switch on the packing type to have compile-time numbers of components
        // and whether the dest raster is interleaved or not.
        PXL_Packing packing = dest.getPacking();
        switch (packing)
        {
            case PACK_SINGLE:
                filterPlanePart<1,1,true>(src, dest, planei); return;
            case PACK_DUAL:
                filterPlanePart<2,2,true>(src, dest, planei); return;
            case PACK_DUAL_NI:
                filterPlanePart<2,2,false>(src, dest, planei); return;
            case PACK_RGB:
                if (plane_src_components == 3)
                    filterPlanePart<3,3,true>(src, dest, planei);
                else
                {
                    UT_ASSERT(plane_src_components == 2);
                    filterPlanePart<2,3,true>(src, dest, planei);
                }
                return;
            case PACK_RGBA:
                filterPlanePart<4,4,true>(src, dest, planei); return;
            case PACK_RGB_NI:
                if (plane_src_components == 3)
                    filterPlanePart<3,3,false>(src, dest, planei);
                else
                {
                    UT_ASSERT(plane_src_components == 2);
                    filterPlanePart<2,3,false>(src, dest, planei);
                }
                return;
            case PACK_RGBA_NI:
                filterPlanePart<4,4,false>(src, dest, planei); return;
            case PACK_UNKNOWN:
                return;
        }
    }

protected:
    TIL_PixelFilterT(
        bool needs_squares=false,
        bool vets_samples=false,
        bool needs_weights=false)
        : TIL_PixelFilter(needs_squares, vets_samples, needs_weights)
    {}

private:
    template<int NSRCCOMPONENTS,int NDSTCOMPONENTS,bool INTERLEAVED>
    void filterPlanePart(
        const TIL_AdaptiveImage &src,
        TIL_Raster &dest,
        int planei) const
    {
        // Switch on the dest data type to have it at compile-time.
        PXL_DataFormat format = dest.getFormat();
        const SUBCLASS *sub = (const SUBCLASS *)this;
        switch (format)
        {
            case PXL_INT8:
                sub->template filterPlane<NSRCCOMPONENTS>(src,
                    PXL_RasterWrapper<uint8,NDSTCOMPONENTS,INTERLEAVED,false>(&dest),
                    planei);
                return;
            case PXL_INT16:
                sub->template filterPlane<NSRCCOMPONENTS>(src,
                    PXL_RasterWrapper<uint16,NDSTCOMPONENTS,INTERLEAVED,false>(&dest),
                    planei);
                return;
            case PXL_INT32:
                sub->template filterPlane<NSRCCOMPONENTS>(src,
                    PXL_RasterWrapper<uint32,NDSTCOMPONENTS,INTERLEAVED,false>(&dest),
                    planei);
                return;
            case PXL_FLOAT16:
                sub->template filterPlane<NSRCCOMPONENTS>(src,
                    PXL_RasterWrapper<fpreal16,NDSTCOMPONENTS,INTERLEAVED,false>(&dest),
                    planei);
                return;
            case PXL_FLOAT32:
                sub->template filterPlane<NSRCCOMPONENTS>(src,
                    PXL_RasterWrapper<fpreal32,NDSTCOMPONENTS,INTERLEAVED,false>(&dest),
                    planei);
                return;
            case PXL_MAX_DATA_FORMAT:
                return;
        }
    }
};

class TIL_API TIL_PixelFilterPixelLevel : public TIL_PixelFilterT<TIL_PixelFilterPixelLevel>
{
public:
    template<int NSRCCOMPONENTS,typename T,int NDSTCOMPONENTS,bool INTERLEAVED>
    void filterPlane(
        const TIL_AdaptiveImage &src,
        PXL_RasterWrapper<T,NDSTCOMPONENTS,INTERLEAVED,false> dest,
        int planei) const;

    virtual bool ownedBySingleton() const
    {
        return true;
    }

    static TIL_PixelFilterPixelLevel theInstance;
};

class TIL_API TIL_PixelFilterSampleCount : public TIL_PixelFilterT<TIL_PixelFilterSampleCount>
{
public:
    template<int NSRCCOMPONENTS,typename T,int NDSTCOMPONENTS,bool INTERLEAVED>
    void filterPlane(
        const TIL_AdaptiveImage &src,
        PXL_RasterWrapper<T,NDSTCOMPONENTS,INTERLEAVED,false> dest,
        int planei) const;

    virtual bool ownedBySingleton() const
    {
        return true;
    }

    static TIL_PixelFilterSampleCount theInstance;
};

class TIL_API TIL_PixelFilterPower2Block : public TIL_PixelFilterT<TIL_PixelFilterPower2Block>
{
public:
    template<int NSRCCOMPONENTS,typename T,int NDSTCOMPONENTS,bool INTERLEAVED>
    void filterPlane(
        const TIL_AdaptiveImage &src,
        PXL_RasterWrapper<T,NDSTCOMPONENTS,INTERLEAVED,false> dest,
        int planei) const;

    virtual bool ownedBySingleton() const
    {
        return true;
    }

    static TIL_PixelFilterPower2Block theInstance;
};

template<bool IS_MAX>
class TIL_API TIL_PixelFilterMinMax : public TIL_PixelFilterT<TIL_PixelFilterMinMax<IS_MAX> >
{
    typedef TIL_PixelFilterT<TIL_PixelFilterMinMax<IS_MAX> > Base;
public:
    TIL_PixelFilterMinMax()
        : Base(false,true) // vets_samples
    {}

    template<int NSRCCOMPONENTS,typename T,int NDSTCOMPONENTS,bool INTERLEAVED>
    void filterPlane(
        const TIL_AdaptiveImage &src,
        PXL_RasterWrapper<T,NDSTCOMPONENTS,INTERLEAVED,false> dest,
        int planei) const;

    virtual void combineSample(
        int ncomponents,
        exint nexisting_samples,
        float *existing,
        exint nnew_samples,
        const float *new_sample) const
    {
        if (nnew_samples == 0)
            return;

        if (nexisting_samples == 0)
        {
            for (int i = 0; i < ncomponents; ++i)
                existing[i] = new_sample[i];
            return;
        }

        if (IS_MAX)
        {
            for (int i = 0; i < ncomponents; ++i)
            {
                float v = new_sample[i];
                if (v > existing[i])
                    existing[i] = v;
            }
        }
        else
        {
            // min
            for (int i = 0; i < ncomponents; ++i)
            {
                float v = new_sample[i];
                if (v < existing[i])
                    existing[i] = v;
            }
        }
    }

    virtual bool ownedBySingleton() const
    {
        return true;
    }

    static TIL_PixelFilterMinMax<IS_MAX> theInstance;
};

template<bool LEVEL_INTERP>
class TIL_API TIL_PixelFilterDebug : public TIL_PixelFilterT<TIL_PixelFilterDebug<LEVEL_INTERP> >
{
    typedef TIL_PixelFilterT<TIL_PixelFilterDebug<LEVEL_INTERP> > Base;
public:
    /// NOTE: This does not need its own plane's squares, just the adaptivity plane's weights.
    TIL_PixelFilterDebug(int level)
        : Base(false,false,true) // needs_squares,vets_samples,needs_weights
        , myLevel(level)
    {}

    template<int NSRCCOMPONENTS,typename T,int NDSTCOMPONENTS,bool INTERLEAVED>
    void filterPlane(
        const TIL_AdaptiveImage &src,
        PXL_RasterWrapper<T,NDSTCOMPONENTS,INTERLEAVED,false> dest,
        int planei) const;

private:
    int myLevel;
};

#if 1
class TIL_API TIL_PixelFilterBlurThreshold : public TIL_PixelFilterT<TIL_PixelFilterBlurThreshold>
{
    typedef TIL_PixelFilterT<TIL_PixelFilterBlurThreshold> Base;
public:
    TIL_PixelFilterBlurThreshold(float threshold)
        : Base(true) // needs_squares
        , myThresholdSquared(threshold*threshold)
    {}

    template<int NSRCCOMPONENTS,typename T,int NDSTCOMPONENTS,bool INTERLEAVED>
    void filterPlane(
        const TIL_AdaptiveImage &src,
        PXL_RasterWrapper<T,NDSTCOMPONENTS,INTERLEAVED,false> dest,
        int planei) const;

private:
    float myThresholdSquared;
};
#endif

#endif
