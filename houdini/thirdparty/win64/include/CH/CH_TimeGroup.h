/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH library (C++)
 *
 * COMMENTS:	Keyframe Group container
 *
 */

#ifndef __CH_TimeGroup_h__
#define __CH_TimeGroup_h__

#include "CH_API.h"
#include "CH_Types.h"
#include <UT/UT_String.h>
#include <UT/UT_Array.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class CH_API CH_TimeGroup {
public:
     CH_TimeGroup(const char *name, fpreal time);
    ~CH_TimeGroup();

    const UT_String	&getName() const	{ return myName; }
    fpreal		 getTime() const	{ return myTime; }
    void		 setTime(fpreal t) 	{ myTime = t; }
    int			 getIndex() const;

    void		 setSelect(bool state)	{ mySelectState = state; }
    bool		 isSelected() const	{ return mySelectState;  }

    void		 clearAndDestroy();

			 // returns a sorted channel list
    const CH_ChannelList&getChannels() { return myChannels; }
    CH_Segment		*getEntry( CH_Channel *chp, int i );
    int			 numEntries( CH_Channel *chp ) const;
    int			 numEntries() const;

    bool		 findEntry(const CH_Segment *segp) const;
			 // this will check for duplicates
    void		 addEntry(CH_Segment *segp, bool affect_segments=true);
    void		 removeEntry(CH_Segment *segp,
					bool affect_segments = true);

    bool		 testShift( bool snap_to_frame,
				    fpreal accepted_dt, fpreal &attempted_dt );
    void		 shift(fpreal t);

    void		 save(std::ostream &os, int binary) const;
    bool		 load(UT_IStream &is);
    void		 printEntries(std::ostream &os, bool use_time,
					bool show_keys=true) const;

    void		 setAcceptedDt( fpreal a ) { myAcceptedDt = a; }
    fpreal		 getAcceptedDt() const { return myAcceptedDt; }

#ifdef INTEL_COMPILER	
private:
	void operator=(const CH_TimeGroup& ) { }
	CH_TimeGroup(const CH_TimeGroup& ) { }
#endif
	
private:
	
    friend class CH_Manager;
    void		 setName(const char *name)	{ myName.harden(name); }

    int			 findChannel(const CH_Channel *) const;
    int			 addChannel(CH_Channel *);

    UT_String				myName;
    fpreal				myTime;
    fpreal				myAcceptedDt;
    CH_ChannelList			myChannels;
    UT_Array< CH_SegmentList >	mySegments;
    unsigned				mySelectState:1;
};

#endif
