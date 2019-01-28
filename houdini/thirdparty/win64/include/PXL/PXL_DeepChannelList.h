/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_DeepChannelList.h (PXL Library, C++)
 *
 * COMMENTS:
 */

#ifndef __PXL_DeepChannelList__
#define __PXL_DeepChannelList__

#include "PXL_API.h"
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_StringArray.h>
#include "PXL_Forward.h"

class PXL_API PXL_DeepChannelList
    : public UT_IntrusiveRefCounter<PXL_DeepChannelList>
{
public:
    class ChannelInfo
    {
    public:
	ChannelInfo(const UT_StringHolder &name, int size, int offset,
		bool ord=false)
	    : myName(name)
	    , myTupleSize(size)
	    , myOffset(offset)
	    , myOrdinal(ord)
	{
	}
	bool	operator==(const ChannelInfo  &i) const
	{
	    return myName == i.myName
		&& myTupleSize == i.myTupleSize
		&& myOffset == i.myOffset
		&& myOrdinal == i.myOrdinal;
	}
	bool	operator!=(const ChannelInfo &i) const { return !(*this == i); }
	const UT_StringHolder	&name() const { return myName; }
	int			 tupleSize() const { return myTupleSize; }
	int			 offset() const { return myOffset; }
	bool			 isOrdinal() const { return myOrdinal; }
    private:
	UT_StringHolder	myName;
	int		myTupleSize;
	int		myOffset;
	bool		myOrdinal;
    };
     PXL_DeepChannelList()
	 : myChannels()
	 , myMaxSize(0)
	 , myTotalSize(0)
     {}
    ~PXL_DeepChannelList() {}

    /// Add a channel to the channel list
    void	addChannel(const UT_StringHolder &name, int size,
			bool ord=false)
		{
		    myChannels.append(ChannelInfo(name, size, myTotalSize,ord));
		    myMaxSize = SYSmax(myMaxSize, size);
		    myTotalSize += size;
		}

    /// @{
    /// Comparison
    bool	isEqual(const PXL_DeepChannelList &src) const;
    bool	operator==(const PXL_DeepChannelList &s) { return isEqual(s); }
    bool	operator!=(const PXL_DeepChannelList &s) { return !isEqual(s); }
    /// @}

    /// Number of channels in the channel list
    int		channelCount() const	{ return myChannels.entries(); }

    /// Return the name of the given channel
    const UT_StringHolder	&name(int i) const
					{ return myChannels(i).name(); }
    /// Return tuple size of the given channel
    int				 tupleSize(int i) const
					{ return myChannels(i).tupleSize(); }
    /// Return the "offset" of a given channel.  This assumes a linear array of
    /// interleaved channel data.
    int				 offset(int i) const
					{ return myChannels(i).offset(); }
    /// Return whether the channel data can be interpolated
    bool			isOrdinal(int i) const
					{ return myChannels(i).isOrdinal(); }
    /// Return the maximum tuple size of all channels
    int				 maxSize() const	{ return myMaxSize; }
    /// Return the sum of the sizes of all channels
    int				 totalSize() const	{ return myTotalSize; }

    /// Return -1 if channel isn't found
    int			findChannel(const char *name) const;

    /// Dump the channel list to stdout
    void		dump(const char *msg="") const;
private:
    UT_Array<ChannelInfo>	myChannels;
    int				myMaxSize;
    int				myTotalSize;
};

#endif
