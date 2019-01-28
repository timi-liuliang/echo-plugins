/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	AU_Device.h ( Audio Library, C++)
 *
 * COMMENTS:
 */

#ifndef __AU_Device__
#define __AU_Device__


#include "AU_API.h"
#if defined(WIN32)
    #include "AU_NTSystem.h"
    class AU_API AU_Device : public AU_NTSystem
#elif defined(MBSD) 
    class AU_MBSDUtilAudioInfo;

    class AU_API AU_Device 
#elif defined(LINUX)
    class AU_ALSADeviceInfo;
    class AU_API AU_Device
#else
    #error Unsupported platform
#endif
{
public:
			 AU_Device();
			~AU_Device();

    bool		 startPlay(int queuesize, int channels, int rate);

    bool		 isReadyToPlay();
    void		 stopPlay(bool now);
    void		 clearPort();
    void		 setFillPoint(int size);
    void		 writeSamples(short *data, int size);
    bool		 passedFillPoint();

private:
#if defined(WIN32)
    int			 myPort;
    int			 myFillSize;

#elif defined(MBSD)
    // amount of space needed for the next sample write
    int			myRequestedFreeSpace;	
    // audio info (rate, buffers, etc).
    AU_MBSDUtilAudioInfo *  myInfo;	

#elif defined(LINUX) 
    AU_ALSADeviceInfo	*myInfo;
#else
    #error Unsupported platform
#endif
};

#endif // __AU_Device__
