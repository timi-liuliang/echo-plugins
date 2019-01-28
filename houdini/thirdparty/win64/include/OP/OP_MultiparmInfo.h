/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_MultiparmInfo.h ( OP Library, C++)
 *
 * COMMENTS:
 *	This class holds information usedto initialize multiparm instances
 *	in OP_Parameters::initMultiparmInstance.
 */

#ifndef __OP_MultiparmInfo_h__
#define __OP_MultiparmInfo_h__

#include "OP_API.h"
#include "OP_Parameters.h"
#include <UT/UT_StringArray.h>
#include <iosfwd>

class OP_API OP_MultiparmInfo
{
public:
			 OP_MultiparmInfo(OP_Parameters &owner);
			~OP_MultiparmInfo();

    // Returns the number of multiparm links stored here.
    int			 entries() const;
    // Returns the specified referenced parm.
    const UT_StringHolder	&getReferencedChannel(int index) const;
    // Returns the specified instance parm.
    const UT_StringHolder	&getInstanceChannel(int index) const;

    // Associates an instance channel with a referenced channel. If the
    // instance channel is already referenced, the new value replaces the
    // old. A null or empty reference clears that reference. A detailed
    // error message is returned in the error parameter.
    bool		 setReference(const char *instancechannel,
				const char *referencedchannel,
				UT_String &error);

    // Performs pattern matching to find the instance channel that best
    // matches the passed in channel name. The referenced channel is returned
    // with appropriate replacements already done.
    bool		 getReferencedChannel(const char *instancechannel,
				UT_String &referencedchannel) const;

    // Saves the opmultiparm commands required to set up the current
    // set of references. The "opmultiparm" and node name parts of the
    // command are not saved, and must be output before calling this
    // funtion.
    void		 saveCommands(std::ostream &os) const;

    // Saves the multiparm info as into a hip file.
    void		 save(std::ostream &os, bool binary) const;

    // Loads multiparm info as from a hip file.
    bool		 load(UT_IStream &is);

    // Save undo block
    void		 saveForUndo();

    // Tries to match a specific channel name (instancechannel) with a
    // generic channel name containing "#"s (genericinstancechannel).
    // The numeric replacements required to match the channel names
    // are stored in the hashreplacements array.
    static bool		 matchChannels(const char *instancechannel,
				const char *genericinstancechannel,
				UT_IntArray &hashreplacements);

    int64		 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myInstanceChannels.getMemoryUsage(false);
        mem += myReferencedChannels.getMemoryUsage(false);
        return mem;
    }

    class Undo;
    friend class Undo;

private:
    OP_Parameters	&myOwner;
    UT_StringArray	 myInstanceChannels;
    UT_StringArray	 myReferencedChannels;
};

#endif

