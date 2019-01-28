/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_DeepPixel.h ( PXL Library, C++)
 *
 * COMMENTS:
 */

#ifndef __PXL_DeepPixel__
#define __PXL_DeepPixel__

#include "PXL_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_Lock.h>
#include "PXL_Forward.h"

/// Class to accumulate multiple deep samples and merge them into a single list.
///
/// Since this class allocates memory, it can be reused for multiple pixels,
/// reusing it's buffers.
///
/// There are two modes of operation for the class:
///  - Accept a tuple of (int sample_id, uint8 flags, const float *data)
///    representing a single deep sample.  In this mode, the deep pixel
///    accumulates mutiple sample tuples.  The @c extract method will merge
///    them into a single PXL_DeepSampleListPtr based on the compressor passed
///    in.
/// - Accept one to four PXL_DeepSampleListPtr objects.  This mode is typically
///   used when creating MIP maps.  The @c extract method will call the
///   compressor to merge the existing PXL_DeepSampleListPtr objects into a
///   single PXL_DeepSampleListPtr.
///
class PXL_API PXL_DeepPixel
{
public:
    /// SampleKey is used to keep track of the samples in the pixel and can be
    /// used to sort them if need be.
    class SampleKey
    {
    public:
	SampleKey(const PXL_DeepPixel *pixel=0, int id=-1, int off=-1)
	    : myPixel(pixel)
	    , myId(id)
	    , myOffset(off)
	{
	}
	bool	operator<(const SampleKey &src) const
	{
	    // For sorting, we sort by id's, then by zfront values.  This lets
	    // us build sorted records for the sample list.
	    if (myId == src.myId)
		return myPixel->samplez(*this) < myPixel->samplez(src);
	    return myId < src.myId;
	}
	int	getId() const	{ return myId; }
	int	offset() const	{ return myOffset; }
    private:
	const PXL_DeepPixel	*myPixel;
	int			 myId;
	int			 myOffset;
    };

    PXL_DeepPixel(const PXL_DeepChannelListPtr &channels,
		    int sample_lists_per_pixel);
    ~PXL_DeepPixel();

    /// Test whether the pixel is accepting samples
    bool	isOpen() const	{ return myX >= 0; }

    /// Test whether the pixel is collecting PXL_DeepSampleListPtr's (rather
    /// than raw float samples).  Note, if no samples have been written, the
    /// pixel could be in both MIP and raw style.
    bool	isMipStyle() const { return myMipCount.relaxedLoad() > 0; }
    /// Test whether the pixel is collecting raw float data (rather than
    /// PXL_DeepSampleListPtr's).  Note, if no samples have been written, the
    /// pixel could be in both MIP and raw style.
    bool	isRawStyle() const { return mySampleLists.size() > 0; }

    /// Return channel list passed into the c-tor
    const PXL_DeepChannelListPtr	&channels() const { return myChannels; }

    // Pixel Interface

    /// Start collecting samples for the given pixel.  The @c precomposited
    /// flag indicates whether the incoming samples are pre-composited or
    /// uncomposited.
    void	startPixel(int x, int y, bool precomposted);

    /// Store a sample for MIP style accumulation.
    void	store(const PXL_DeepSampleListPtr &sample);
    /// Store a raw sample.  The samples do not have to be added in order.
    /// - @c zfront @n
    ///   The nearest z value for the sample.
    /// - @c zback @n
    ///   The far z value for the sample.  For hard surfaces, this is likely
    ///   the same as @c zfront.
    /// - @c channel_data @n
    ///    The floating point data for all the channels.  The length of this
    ///    array should be @c channels()->totalSize() and the data in the array
    ///    should be ordered according to the channel order.
    /// - @c flags @n
    ///    Please see the list of flags in PXL_DeepSampleList.h
    /// - @c sample_id @n
    ///   This represents the sub-pixel sample id.  For example with 3x3 pixel
    ///   sampling, the sample id's would run between 0 and 9.  A value of -1
    ///   indicates that no sample id is available.  The sample id is used in
    ///   creating the @c coverage for the pixel.
    void	store(float zfront, float zback, const float *channel_data,
			int flags, int sample_id);

    /// Close the pixel, returning the result of the compressor's merge
    /// operation.
    PXL_DeepSampleListPtr	closePixel(
				    const PXL_DeepCompressorPtr &compress);

    /// @{
    /// Return coordinate passed into @c startPixel()
    int		x() const	{ return myX; }
    int		y() const	{ return myY; }
    /// @}

    float	samplez(const SampleKey &k) const
		{
		    int	off = k.offset();
		    UT_ASSERT(off >= 0 && off < mySampleLists.size());
		    return myFloats[off*myFloatCount];
		}
    void	dump(const char *msg="") const;

private:
    int		sort();
    void	extract(PXL_DeepSampleListPtr *samples, int count) const;

    float	zfront(int off) const
		{
		    UT_ASSERT_P(myFloats && !myMipCount.relaxedLoad());
		    UT_ASSERT(off >= 0 && off < mySampleLists.size());
		    off = mySampleLists(off).offset();
		    return myFloats[off*myFloatCount];
		}
    float	zback(int off) const
		{
		    UT_ASSERT_P(myFloats && !myMipCount.relaxedLoad());
		    UT_ASSERT(off >= 0 && off < mySampleLists.size());
		    off = mySampleLists(off).offset();
		    return myFloats[off*myFloatCount+1];
		}
    const float	*data(int off) const
		{
		    UT_ASSERT_P(myFloats && !myMipCount.relaxedLoad());
		    UT_ASSERT(off >= 0 && off < mySampleLists.size());
		    off = mySampleLists(off).offset();
		    return myFloats + off*myFloatCount + 2;
		}
    int		getId(int off) const
		{
		    return mySampleLists(off).getId();
		}
private:
    void			clear();
    PXL_DeepSampleListPtr	extract(int start, int end) const;

    PXL_DeepChannelListPtr	 myChannels;
    UT_Lock			 myLock;
    PXL_DeepSampleListPtr	 myMip[4];
    float			*myFloats;
    uint8			*myFlags;
    UT_Array<SampleKey>		 mySampleLists;
    int				 mySampleListsPerPixel;
    int				 myX, myY;	// Current pixel
    SYS_AtomicInt32		 myMipCount;
    int				 myFloatCount;
    bool			 myPrecomposited;
};

#endif
