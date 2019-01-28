/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_RealtimeStamp.h ( Clip Library, C++)
 *
 * COMMENTS:
 *	Does a realtime stamp or convolve
 *
 */


#ifndef __CL_RealtimeStamp__
#define __CL_RealtimeStamp__

#include "CL_API.h"
#include "CL_SlidingWindow.h"
#include <SYS/SYS_Types.h>


class CL_API CL_RealtimeStamp : public CL_SlidingWindow
{
public:
    enum {
	REALTIME_STAMP = 0,
	REALTIME_CONVOLVE = 1
    };
    
		 CL_RealtimeStamp(int size);
    virtual     ~CL_RealtimeStamp();

    void	 setStampData(int size, const fpreal *data, fpreal threshold);
    void	 setFunction(int func);
    void	 setStampState(int state);
    int		 getStampState();
    void	 setStampCallback(void (*callback)(void *,int,CL_RealtimeStamp*),
				  void *data);

    int		 getLastStampedIndex() const { return myLastStampedSample; }
    virtual void reset(fpreal val = 0.0);
    
private:

    virtual void doShiftOp(int nsamples,const fpreal *source, int nshift);
    virtual void doWindowOp(int nsamples, fpreal *dest);

    void	 doStamp(int nsamples, const fpreal *source);
    void	 doConvolve(int nsamples, const fpreal *source);
    
    int		 myStampSize;
    const fpreal *myStampData;
    int		 myFunction;
    int		 myStampOn;
    int		 myLastStampedSample;
    fpreal	 myStampThreshold;
    void	(*myCallback)(void *,int,CL_RealtimeStamp*);
    void	*myData;
};

#endif
