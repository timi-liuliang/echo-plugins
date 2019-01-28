/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_DeepSampleFilter.h (PXL Library, C++)
 *
 * COMMENTS:
 */

#ifndef __PXL_DeepSampleFilter__
#define __PXL_DeepSampleFilter__

#include "PXL_API.h"
#include "PXL_Forward.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_IntrusivePtr.h>

/// This sample filter creates a new PXL_DeepsampleList from a source deep
/// sample list.
///
/// Multiple filters can be chained together and applied after the 
class PXL_API PXL_DeepSampleFilter
    : public UT_IntrusiveRefCounter<PXL_DeepSampleFilter>
{
public:
    PXL_DeepSampleFilter();
    virtual ~PXL_DeepSampleFilter();

    /// Return the tuple size of the filtered result.  This may be called
    /// before the channel list is defined.  It should return 0 if the channel
    /// will be filtered out of the resulting samples.
    ///
    /// By default, the tuple size will remain unchanged.
    virtual int		filteredTupleSize(const char *name, int src_size) const;

    /// Update the channel list.  This should call the protected setChannels().
    /// It returns the channel list output by this filter.
    const PXL_DeepChannelListPtr	&updateChannels(
					    const PXL_DeepChannelListPtr &ch)
    {
	doUpdateChannels(ch);
	return myChannels;
    }

    /// Modify the incoming sample list to create a new sample list
    virtual PXL_DeepSampleListPtr  filter(const PXL_DeepSampleListPtr &l) const = 0;

    /// Return the channel list after the filter has run
    const PXL_DeepChannelListPtr	&channels() const { return myChannels; }

    // Create a null filter.  This passes through the channels and sample lists
    static PXL_DeepSampleFilterPtr	nullFilter();

    // Create filter to convert opacity to monochrome
    static PXL_DeepSampleFilterPtr
		monochrome(const UT_StringHolder &opac = UT_StringRef("Of"));

    // Create a filter to pre-composite samples
    static PXL_DeepSampleFilterPtr
		precomposite(const UT_StringHolder &opac = UT_StringRef("Of"));

    // Create a filter to uncomposite samples
    static PXL_DeepSampleFilterPtr
		uncomposite(const UT_StringHolder &opac = UT_StringRef("Of"));

protected:
    virtual void	doUpdateChannels(const PXL_DeepChannelListPtr &ch) = 0;

    void		setChannels(const PXL_DeepChannelListPtr &c)
			    { myChannels = c; }

protected:
    PXL_DeepChannelListPtr	myChannels;
};

#endif
