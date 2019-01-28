/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Playback.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *	This class is used to store framebar info for
 *	classes above the SI library. 
 *	
 */

#ifndef __UT_Playback__
#define __UT_Playback__

#include "UT_API.h"
#include <iosfwd>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_StaticInit.h>
#include "UT_ValArray.h"

class ut_PlayCB;

// cbdata, UT_PlaybackMode, oldframe, newframe
typedef void (*UT_PlayCallback)(void *, int, fpreal, fpreal);

class UT_API UT_Playback 
{
public:

     UT_Playback();
    ~UT_Playback();

    enum UT_PlaybackMode {
	REVERSE,
	STOPPED,
	FORWARD,
	SCRUB,
	RESET
    };

    void		setPlaybackMode(UT_PlaybackMode mode);

    UT_PlaybackMode	getPlaybackMode() const
			{ return myPlayback; }

    int			getStopCount() const
			{ return myStopCount; }

    void		setRealtimeMode(bool mode)
			{ myRealtimeMode = mode; }

    bool		getRealtimeMode() const
			{ return myRealtimeMode; }

    void		setPlaybackRange(fpreal start,fpreal end)
			{ myStartRange = start; myEndRange = end; }

    void		getPlaybackRange(fpreal &start, fpreal &end) const
			{ start = myStartRange; end = myEndRange; }

    bool		isPlaybackReset();
    bool		isPlaying() const { return (myPlayback == FORWARD ||
						    myPlayback == REVERSE); }

    void		setCurrentFrame(fpreal frame);
    fpreal		getCurrentFrame() { return myCurrentFrame; }

    void		resetAbsoluteFrame(); 
    unsigned long	getAbsoluteFrame() { return myAbsoluteFrame; }

    // tells SI_Playback to stop.
    void		stopPlayback();
    void		setStopCallback(void (*cb)(void *), void *data);

    void		setInterceptMode(int mode) { myInterceptMode = mode; }
    int			getInterceptMode() const   { return myInterceptMode; }

    // Callbacks are passed the UT_PlaybackMode and 'data'.
    void		addPlayCallback(UT_PlayCallback cb, void *cbdata);
    void		removePlayCallback(UT_PlayCallback cb, void *cbdata);
   
    static UT_Playback *getPlayback();
    
private:

    UT_PlaybackMode				 myPlayback;
    fpreal					 myStartRange;
    fpreal					 myEndRange;
    fpreal					 myCurrentFrame;
    unsigned long				 myAbsoluteFrame;
    bool					 myRealtimeMode;
    bool					 myResetFlag;
    int						 myInterceptMode;
    UT_ValArray<ut_PlayCB *>			 myPlayCallbacks;

    int						 myStopCount;
};

SYSdeclareStaticObject(UT_API, UTplaybackObject);

#endif

