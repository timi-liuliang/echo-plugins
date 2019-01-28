/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH_Filters.h (CH Library, C++)
 *
 * COMMENTS:
 */

#ifndef __CH_FILTERS_H_INCLUDED__
#define __CH_FILTERS_H_INCLUDED__

#include "CH_API.h"
#include <SYS/SYS_Types.h>

class CH_Channel;
class CH_Segment;

// How to Bake CHOP Exports
// When baking a new channel is created and it overrides the channel animation on the parm.
// It won't override CHOP Clips.
enum CH_BakeChop
{
    CH_BAKECHOP_NONE,			// Don't bake CHOP exports on channels.
    CH_BAKECHOP_KEEP_EXPORT_FLAG,	// Bake but leave the export flag on. You'll have to manually toggle the chop export flag.
    CH_BAKECHOP_DISABLE_EXPORT_FLAG,	// Bake and disable the CHOP export flags. This won't work properly if you work on a subset of the exports as it will disable all.
    CH_BAKECHOP_CREATE_DELETE_CHOP,	// Bake and create a Delete CHOP node after the CHOP export node. This enables you to bake a single channel.
    CH_BAKECHOP_EXPRESSIONS,	        // Bake expression even if there is no CHOP export.
};

// Builder for ranges of selected segments
// Abstracts the use of JIVE_Iterators/Script glob in order to reuse the
// code for scripting
class CH_API CH_FilterRangeBuilder
{
public:
    virtual ~CH_FilterRangeBuilder();

    // Reset Channel and Segments
    virtual void resetChannel() = 0;
    virtual void resetSegment() = 0;

    //Iterate over Channels and Segments
    virtual CH_Channel *nextChannel() = 0;
    virtual CH_Segment *nextSegment(bool &is_selected) = 0;

    // External Undo
    virtual void undoStart() = 0;
    virtual void undoChannel( CH_Channel *chp) = 0;
    virtual void undoEnd() = 0;

    // Callback to copy a dummy Chop channel back as a real channel
    virtual void beginCopyChopChannels(CH_BakeChop mode) = 0;
    virtual void copyChopChannel(CH_Channel *chp, fpreal global_start, fpreal global_end) = 0;
    virtual void endCopyChopChannels() = 0;
};

struct chRefitChannels;

class CH_API CH_Filters
{
public:
    CH_Filters();

    // Performs a Refitting operation to simplify the channels.
    // This can be used to bake channel expressions.
    // This can be used on channels with cycle extrapolation to bake the cycles.
    void applyRefitFilter(
	CH_FilterRangeBuilder *builder, // range builder to extract the channels and the time ranges 

	bool refit_on,			// perform the cubic refitting using CH_Channel::refit()
	fpreal  refit_tol,		// refitting tolerance in abs value.
	bool refit_preserve_extrema,	// refitting preserves extremas
	bool refit_bezier,		// set the new keyframes to bezier instead of cubic.

	bool resample_on,		// do resampling before doing the refitting.
	fpreal resample_rate,		// resample rate in frames
	fpreal resample_tol,		// resample tolerance to avoid adding samples in frames.

	bool range_on,			// force a global frame range, on top of the extracted ranges.
	fpreal range_start,		//  start frame
	fpreal range_end, 		//  end frame.

	CH_BakeChop bake_chop=CH_BAKECHOP_NONE // See CH_BakeChop for description of the modes.
	);

    // Performs only a Refitting operation to simplify the channels.
    void applyRefitFilter(
	CH_FilterRangeBuilder *builder, // range builder to extract the channels and the time ranges 

	fpreal  refit_tol,		// refitting tolerance in abs value.
	bool refit_preserve_extrema,	// refitting preserves extremas
	bool refit_bezier,		// set the new keyframes to bezier instead of cubic.

	bool range_on,			// force a global frame range, on top of the extracted ranges.
	fpreal range_start,		//  start frame
	fpreal range_end 		//  end frame.
	);

    // Performs a Resample operation to bake channel expressions.
    void applyResampleFilter(
	CH_FilterRangeBuilder *builder, // range builder to extract the channels and the time ranges 

	fpreal resample_rate,		// resample rate in frames
	fpreal resample_tol,		// resample tolerance to avoid adding samples in frames.

	bool range_on,			// force a global frame range, on top of the extracted ranges.
	fpreal range_start,		//  start frame
	fpreal range_end 		//  end frame.
	);

    // Performs a Extend operation to turn channel extrapolation into real keyframes.
    void applyExtendFilter(
	CH_FilterRangeBuilder *builder, // range builder to extract the channels and the time ranges 
	fpreal range_start,		//  start frame
	fpreal range_end 		//  end frame.
	);

    // Make a copy of the dummy CHOP channels on the real param
    void applyCopyChopFilter(
	CH_FilterRangeBuilder *builder, // range builder to extract the channels and the time ranges 
	bool range_on,			// force a global frame range, on top of the extracted ranges.
	fpreal range_start,		//  start frame
	fpreal range_end, 		//  end frame.
	CH_BakeChop bake_chop=CH_BAKECHOP_NONE // See CH_BakeChop for description of the modes.
	);

private:
    
    bool myFirst; // Flag to do proper undo/redo when applyRefitFilter calls the other methods
    chRefitChannels *myRanges;
};

#endif
