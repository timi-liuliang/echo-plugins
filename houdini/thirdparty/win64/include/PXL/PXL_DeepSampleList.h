/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_DeepSampleList.h (PXL Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __PXL_DeepSampleList__
#define __PXL_DeepSampleList__

#include "PXL_API.h"

#include "PXL_Forward.h"
#include "PXL_DeepChannelList.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_IntrusivePtr.h>

class UT_JSONParser;
class UT_JSONWriter;

/// Class to represent a list of deep pixel samples.
///
/// The class stores multiple samples for multiple image channels.  The image
/// channels Each pixel list has:
/// - <tt>int sampleCount</tt>		@n The number of samples in the list
/// Each sample has:
/// - <tt>float z</tt>:			@n Z-Depth of the sample
/// - <tt>uint8 flags</tt>:		@n Flags associated with the sample
/// - <tt>uint64 coverage</tt>:		@n The sample coverage mask
/// - <tt>float *channel_data</tt>:	@n Image channel values
///
/// The coverage mask will either be 0 (indicating full coverage) or a bit-mask
/// representing the coverage.  If there are more than 64 samples, the sample
/// id's rotate (i.e. mask |= 1 << (sample_id % 64)).
///
class PXL_API PXL_DeepSampleList
    : public UT_IntrusiveRefCounter<PXL_DeepSampleList>
{
public:
    PXL_DeepSampleList(const PXL_DeepChannelListPtr &channels,
                       int nsamples,
                       bool precomposited);
    PXL_DeepSampleList(const PXL_DeepSampleList &src);
    ~PXL_DeepSampleList();

    /// Merge two lists sorted into a single list.  If the definition of the
    /// pixel lists (i.e. the channels) don't match, then the method will
    /// return @c a.  The method properly handles @c nullptr values.
    static PXL_DeepSampleListPtr	combine(const PXL_DeepSampleListPtr &a,
						const PXL_DeepSampleListPtr &b);

    /// Split any volume segments into multiple segements based on the z values
    /// of the other sample list.
    PXL_DeepSampleListPtr splitSegments(const PXL_DeepSampleList &l) const;

    /// Clear buffers etc.  This keeps a reference to the channels and the
    /// precompositing state.
    void clear();

    enum SampleFlag
    {
	VOLUME_SAMPLE	= 0x01,
	MATTE_SURFACE	= 0x02,
    };

    /// @{
    /// Return number of samples in the deep pixel list
    int sampleCount() const	{ return mySampleCount; }
    int size() const		{ return mySampleCount; }
    /// @}

    /// Return the maximum number of samples
    int sampleCapacity() const	{ return mySampleCapacity; }

    /// Return whether the samples in the list are pre-composited or
    /// un-composited
    bool preComposited() const	{ return myPreComposited; }

    /// Initialize data structures, possibly enlarging buffers for the sample
    /// count.
    void setSampleCount(int nsamples);

    /// Set the pre-composited state
    void setPreComposited(bool c)	{ myPreComposited = c; }

    /// Accessors
    const PXL_DeepChannelListPtr &channels() const { return myChannels; }

#define GET_SCALAR(TYPE, VMETHOD) \
    { \
	TYPE	value; \
	return *VMETHOD(&value, sample, sample+1); \
    }
#define GET_VECTOR(TYPE, VMETHOD) \
    { \
	if (end == -1) end = sampleCount(); \
	return VMETHOD(storage, start, end); \
    }

    /// Return z-back for given sample.
    float	zfront(int sample) const
		    { GET_SCALAR(float, fillZfront); }

    /// Return a buffer with all z-front values for all samples.
    ///
    /// If the sample list doesn't store the z-front values in a flat array,
    /// the values will be copied to the @c storage array and that value will
    /// be returned.  If the sample list is able to return a pointer to
    /// internal data, the @c storage array will be left unchanged.
    const float	*zfront(float *storage, int start=0, int end=-1) const
		    { GET_VECTOR(float, fillZfront); }

    /// Return z-back for a given sample.
    float	zback(int sample) const
		    { GET_SCALAR(float, fillZback); }
    /// Return a buffer with all z-back values for all samples.
    /// See @c zfront() for an explanation of the @c storage array.
    const float	*zback(float *storage, int start=0, int end=-1) const
		    { GET_VECTOR(float, fillZback); }

    /// Return the z midpoint for a given sample
    float	zmid(int sample) const
		    { return 0.5f*(zfront(sample) + zback(sample)); }

    /// Return flags for a given sample (see SampleFlag enum)
    uint8	flags(int sample) const
		    { GET_SCALAR(uint8, fillSampleFlags); }
    /// Check to see whether a sample is a volume
    bool	isVolume(int sample) const
		    { return (flags(sample) & VOLUME_SAMPLE)
			&& (zfront(sample) < zback(sample)); }
    /// Check to see whether a sample is a matte sample
    bool	isMatte(int sample) const
		    { return flags(sample) & MATTE_SURFACE; }
    /// Fill buffer with flags for the samples (see SampleFlag enum).
    /// See @c zfront() for an explanation of the @c storage array.
    const uint8	*flags(uint8 *storage, int start=0, int end=-1) const
		    { GET_VECTOR(uint8, fillSampleFlags); }

    /// Return the sub-sample pixel location for a given sample
    uint64	coverage(int sample) const
		    { GET_SCALAR(uint64, fillCoverage) }
    /// Return an array of the sub-pixel locations for all samples.
    /// See @c zfront() for an explanation of the @c storage array.
    const uint64 *coverage(uint64 *storage,
					int start=0, int end=-1) const
		    { GET_VECTOR(uint64, fillCoverage) }

    /// Return the channel data for a given sample.  The @c storage array must
    /// contain at least @c channelSize(channel) floats.
    const float	*channelData(int channel, int sample, float *storage) const
		    { return fillChannel(channel, storage, sample, sample+1); }
    /// Return all the data for a given channel.  The @c storage array must be
    /// at least @c sampleCount()*channelSize(channel) floats.
    const float	*channelData(int channel, float *storage,
					int start=0, int end=-1) const
		    {
			if (end == -1)
			    end = sampleCount();
			return fillChannel(channel, storage, start, end);
		    }

#undef GET_SCALAR
#undef GET_VECTOR

    /// Encode the sample list into a JSON stream.
    bool        save(UT_JSONWriter &j,
			const PXL_DeepChannelListPtr &channels) const;

    /// Save, handling null pointers properly
    static bool save(UT_JSONWriter &j, const PXL_DeepSampleListPtr &list,
			const PXL_DeepChannelListPtr &channels);

    /// See PXL_DeepSampleList::save().  This method will create a sample list
    /// from an encoded sample.
    static PXL_DeepSampleListPtr load(
        UT_JSONParser &j,
        const PXL_DeepChannelListPtr &c);

    /// Dump sample list for debugging
    void	dump(const PXL_DeepChannelListPtr &channels,
			const char *msg="",
			int start = 0,
			int end = -1) const;
    /// Dump sample list (without channel data) for debugging
    void	dump(const char *msg="",
			int start = 0,
			int end = -1) const;

    /// Return a pointer to an array of the sample z-front values.  If the data
    /// isn't laid out properly for the user, the data can be copied to the @c
    /// storage buffer. @code
    ///   for (int i = start; i != end; ++i) storage[i-start] = getZ(i);
    ///   return storage;
    /// @endcode
    const float *fillZfront(float *storage, int start, int end) const
    { return myZfront + start; }
    /// Return a pointer to an array of the z-back flags.
    /// See @c fillZfront() for explanation of @c storage.
    const float *fillZback(float *storage, int start, int end) const
    { return myZback + start; }
    /// Return a pointer to an array of the sample flags.
    /// See @c fillZfront() for explanation of @c storage.
    const uint8 *fillSampleFlags(uint8 *storage, int start, int end) const
    { return myFlags + start; }
    /// Return a pointer to an array of the sub-pixel locations.
    /// See @c fillZfront() for explanation of @c storage.
    const uint64 *fillCoverage(uint64 *storage, int start, int end) const
    { return myCoverage + start; }
    /// Return a pointer to an array of the channel data.
    /// See @c fillZfront() for explanation of @c storage.
    const float *fillChannel(int ch, float *storage, int start, int end) const
    { return myChannelData[ch] + start*myChannels->tupleSize(ch); }

    /// @{
    /// Get write-able array of z values.
    float		*zfrontBuffer()	{ return myZfront; }
    float		*zbackBuffer()	{ return myZback; }
    /// @}
    /// Get write-able array of flags
    uint8		*flagBuffer()	{ return myFlags; }
    /// Get write-able array of coverage
    uint64		*coverageBuffer()	{ return myCoverage; }
    /// Get write-able array of data for a given channel
    float		*channelBuffer(int i)	{ return myChannelData[i]; }

    /// @{
    /// Get read-only array of z values.
    const float		*zfrontBuffer() const	{ return myZfront; }
    const float		*zbackBuffer() const	{ return myZback; }
    /// @}
    /// Get read-only array of flags
    const uint8		*flagBuffer() const	{ return myFlags; }
    /// Get read-only array of coverages
    const uint64		*coverageBuffer() const	{ return myCoverage; }
    /// Get read-only array of data for a given channel
    const float		*channelBuffer(int i) const { return myChannelData[i]; }

    /// The returns the number of records in the list before full opacity is
    /// reached.
    int		findFullOpacity(int of_channel, float opacity_thresh=1.0) const;

    /// Trim the list to the number of records specified.
    void	trimLength(int nrecords) { mySampleCount = nrecords; }

    /// @{
    /// Copy over data from a source sample list
    void	copyZfront(const PXL_DeepSampleList &s);
    void	copyZback(const PXL_DeepSampleList &s);
    void	copyZ(const PXL_DeepSampleList &s)
		{
		    copyZfront(s);
		    copyZback(s);
		}
    void	copyFlags(const PXL_DeepSampleList &s);
    void	copySampleId(const PXL_DeepSampleList &s);
    void	copyChannel(int ch, const PXL_DeepSampleList &s);
    void	copyAllChannels(const PXL_DeepSampleList &s);
    /// @}

    /// Class to help with weighted sums of pixels
    class Sum
    {
    public:
	Sum(PXL_DeepSampleList &list, int idx, float scale)
	    : myList(list)
	    , myIndex(idx)
	    , myScale(scale)
	    , mySum(0)
	    , myMax(0)
	    , myFirst(true)
	{
	}
	~Sum()
	{
	    UT_ASSERT_P(!myFirst);
	    finishSum();
	}
	void	finishSum()
	{
	    if (myFirst)
	    {
		myList.zeroData(myIndex);
		myFirst = false;
	    }
	}
	void	add(const PXL_DeepSampleList &s, int sidx, float weight=1)
	{
	    mySum += weight;
	    if (myFirst)
	    {
		myFirst = false;
		myMax = weight;
		myList.copySample(myIndex, s, sidx, weight*myScale);
	    }
	    else
	    {
		myList.addSample(myIndex, s, sidx, weight*myScale);
		if (weight > myMax)
		{
		    myMax = weight;
		    myList.setConditional(myIndex, s, sidx);
		}
	    }
	}
	/// Add a volume sample.  The @c zinterp value is a 0 to 1 value
	/// representing <tt>(zback - zsample) / (zback - zfront)</tt>
	void	addVolume(const PXL_DeepSampleList &s, int sidx,
			float zinterp, bool beerlambert=false)
	{
	    if (sidx > 0 && zinterp < 1)
		add(s, sidx, SYSmin(1-zinterp, 1.0f));
	    if (zinterp > 0)
		add(s, sidx, SYSmin(zinterp, 1.0f));
	}
	int	index() const		{ return myIndex; }
	float	totalWeight() const	{ return mySum * myScale; }
	float	maxWeight() const	{ return myMax; }
    private:
	PXL_DeepSampleList	&myList;	// Sample list
	int			 myIndex;	// Sample index
	float			 myScale;	// Scale by 1/myCount
	float			 mySum;		// Total weight (not always 1)
	float			 myMax;		// Current maximum weight
	bool			 myFirst;
    };


    /// Copy the source sample into the destination index.  If a @c scale is
    /// specified, the channel data will be scaled by that amount.
    void	copySample(int didx, const PXL_DeepSampleList &s, int sidx,
			    float scale=1);

    /// Split the source sample into the destination index at the given z value
    void	extractSegment(int didx, const PXL_DeepSampleList &s, int sidx,
			    float zf, float zb, float scale=1);

    /// Set conditinal assignment based on weight.  This only adjusts ordinal
    /// channels.
    void	setConditional(int didx, const PXL_DeepSampleList &s, int sidx);

    /// Add the source sample to this sample (with an optional scale).  This
    /// only adjusts non-ordinal channels.
    void	addSample(int didx, const PXL_DeepSampleList &s, int sidx,
			    float scale=1);

    /// Initialize all records to zero
    void	zeroData();

    /// Clear a single sample
    void	zeroData(int sample);

    /// Perform compositing on the sample array using the given opacity channel
    void	composite(int of_channel, bool force = false);

    /// Perform uncompositing of sampels using the opacity channel
    void	uncomposite(int of_channel, bool force = false);

private:
    PXL_DeepChannelListPtr	  myChannels;
    int				  mySampleCount;
    int				  mySampleCapacity;
    float			 *myZfront;
    float			 *myZback;
    uint8			 *myFlags;
    uint64			 *myCoverage;
    float			**myChannelData;
    bool			  myPreComposited;
};

#endif
