/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_DeepCompressor.h (PXL Library, C++)
 *
 * COMMENTS:
 */

#ifndef __PXL_DeepCompressor__
#define __PXL_DeepCompressor__

#include "PXL_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_Vector2.h>
#include <UT/UT_SharedPtr.h>
#include "PXL_Forward.h"
#include "PXL_DeepSampleList.h"
#include "PXL_DeepChannelList.h"

class UT_Options;

extern "C" {
    /// DSO's should have this function declared.  The function should call
    /// PXL_DeepCompressor::registerCompressor.
    SYS_VISIBILITY_EXPORT void	newPXLDeepCompressor();
};

/// Combines a set of deep sample lists to create a single new deep sample list.
///
/// Deep compressors take multiple sample lists and combine them, possibly
/// compressing them into a single deep sample list.  The compressor may sit
/// between the renderer and a deep image writer.  Or it may be used to create
/// MIP Maps for deep images.
///
/// When used by a renderer, compressors have the ability to add or modify
/// channels.
///
/// Factory compressors include:
///
/// Name = "default
/// Options:
///	- zbias
///
class PXL_API PXL_DeepCompressor
{
public:
    class PXL_API Factory
    {
    public:
	virtual ~Factory();

	/// Method to construct a deep compressor given options.
	/// The caller to the compressor will be passing pixel sample lists 
	virtual PXL_DeepCompressorPtr	create(
						const PXL_DeepChannelListPtr &c,
						const UT_Options &options
					 ) const = 0;

	/// Load all compressor factories
	static void	initCompressors();

	/// Register a new compressor.  This should be called during
	/// initCompressors() only.
	static void	registerCompressor(const char *name,
				const UT_SharedPtr<Factory> &factory);

	/// Create a pixel compressor.  This will return NULL for an invalid
	/// compressor.
	/// Factory compressors include:
	/// - "null" @n
	///	No compression.  Records are just merged with no combining.
	/// - "simple" @n
	///    Option: "float zbias" Controls zbias for compression @n
	///    This compressor will peform simple compression, merging similar
	///    records within a z-tolerance.
	static PXL_DeepCompressorPtr	create(const char *name,
						const PXL_DeepChannelListPtr &c,
						const UT_Options &options);

	/// Return the list of possible compressors
	static void		getCompressors(UT_StringArray &types);
    };

    PXL_DeepCompressor();
    virtual ~PXL_DeepCompressor();

    /// Return the channels as a result of the compress() operation
    virtual PXL_DeepChannelListPtr	channels() const = 0;

    /// Compress the samples provided into a single PXL_DeepSampleListPtr.  The
    /// @c samples array will contain <tt> pixel.myPixelResolution.x() *
    /// pixel.myPixelResolution.y()</tt> entries.
    PXL_DeepSampleListPtr	merge(const PXL_DeepSampleListPtr *slist,
					int nsamples) const
    {
	return doMerge(slist, nsamples);
    }
    PXL_DeepSampleListPtr	merge(
				    const UT_Array<PXL_DeepSampleListPtr> &slist
				) const
    {
	return doMerge(slist.array(), slist.entries());
    }
    /// Compress a single sample list, returning a new sample list.
    PXL_DeepSampleListPtr	compress(const PXL_DeepSampleListPtr &slist) const
    {
	return doCompress(slist);
    }

    void	dump(const char *msg="") const;
					
protected:
    virtual const char			*className() const = 0;

    /// Merge multiple deep sample lists into a single deep sample list.  For
    /// example, this is done when creating MIP maps.
    virtual PXL_DeepSampleListPtr	doMerge(
					    const PXL_DeepSampleListPtr *slist,
					    int nsamples
					) const = 0;
    /// Compress a single sample list, returning a new sample list.
    virtual PXL_DeepSampleListPtr	doCompress(
					    const PXL_DeepSampleListPtr &samples
					) const = 0;

    /// Dump any optional information to stdout
    virtual void			doDump() const;
};

#endif
