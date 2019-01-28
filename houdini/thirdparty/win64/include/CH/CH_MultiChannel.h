/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH_MultiChannel.h (UI library, C++)
 *
 * COMMENTS:	Manages an array of arbitary channels to act as a single one.
 *		This is meant to be a lightweight collection of channels that
 *		may belong to an arbitrary list of CH_Collection's. Ownership
 *		of the pointers are NOT assumed.
 *
 */

#ifndef __CH_MultiChannel_h__
#define __CH_MultiChannel_h__

#include "CH_API.h"
#include "CH_Types.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_String.h>
#include <SYS/SYS_Types.h>

class CH_Channel;
class CH_Manager;

class CH_API CH_MultiChannel
{
public:
			    CH_MultiChannel();
    explicit		    CH_MultiChannel(const char *name);
    virtual		    ~CH_MultiChannel();

			    CH_MultiChannel(const CH_MultiChannel &copy);
			    CH_MultiChannel &operator=(
				    const CH_MultiChannel &copy);

    const UT_String &	    getName() const
				{ return myName; }
    void		    setName(const UT_String &name)
				{ myName.harden(name); }
    void		    appendChannelNames(const char *separator);

    void		    clear()
				{ myChannels.entries(); }
    void		    append(CH_Channel *channel);

    const CH_ChannelList &  getChannelList() const
				{ return myChannels; }

    int			    getNumChannels() const
				{ return myChannels.entries(); }

    void		    removeChannel(int i)
				{ myChannels.removeIndex((unsigned)i); }

    void		    addToList(CH_ChannelList &channels) const;

    bool		    isEmpty() const
				{ return myChannels.entries() == 0; }
    bool		    isAllEnabled() const;
    bool		    hasKeys() const;

    fpreal		    getStart() const;
    fpreal		    getEnd() const;
    fpreal		    getLength() const;

    bool		    isAtHardKey(fpreal gtime) const;
    bool		    isAtHardKeyframe(int frame) const;
    fpreal		    findKey(fpreal gtime, int direction) const;
    int			    findKeyframe(int frame, int direction) const;

    void		    scroll(fpreal newStart, int update = 1);

private:
    // do not use!
    int			    operator==(const CH_MultiChannel &) const
				{ return 0; }

protected:
    // only subclasses can modify the array!
    CH_ChannelList &	    getModifyChannelList()
				{ return myChannels; }

private:
    UT_String		    myName;
    CH_ChannelList	    myChannels;
};

#endif // __CH_MultiChannel_h__

