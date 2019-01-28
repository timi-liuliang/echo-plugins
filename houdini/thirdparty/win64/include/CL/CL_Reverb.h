/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_Reverb.h ( Clip Library, C++)
 *
 * COMMENTS:
 *	Does realtime echoing. 
 *
 */

#ifndef __CL_Reverb__
#define __CL_Reverb__

#include "CL_API.h"
#include "CL_SlidingWindow.h"
#include <SYS/SYS_Types.h>

class CL_API CL_Reverb : public CL_SlidingWindow
{
public:

		 CL_Reverb(int size, int overlap);
    virtual     ~CL_Reverb();

    void	 setReverbFilter(int nechos, fpreal *reverb);
    void	 setMaxReverb(int max);

private:

    virtual void  doWindowOp(int nsamples, fpreal *dest);

    fpreal	*myReverb;
    int		 myNumEchos;

};


#endif
