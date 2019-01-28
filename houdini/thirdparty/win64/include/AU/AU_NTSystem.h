/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	AU_NTSystem.h ( Audio Library, C++)
 *
 * COMMENTS:
 */

#ifndef __AU_NTSystem__
#define __AU_NTSystem__

#include "AU_API.h"
#include <UT/UT_SysClone.h>
#include <UT/UT_TBBSpinLock.h>
#include <UT/UT_ValArray.h>

#include <condition_variable>
#include <mutex>

#ifdef WIN32
#include <windows.h>
#include <mmsystem.h> // Need this because of -DWIN32_LEAN_AND_MEAN
#if !defined(IA64) && !defined(AMD64)
#define DWORD_PTR	DWORD
#endif
#endif

class AU_API AU_NTSystem
{
public:
			 AU_NTSystem();
			~AU_NTSystem();

protected:
    void		 stopDevice(int now);
    int			 isOpen();
    void		 setClosed();
    int			 bytesPerSample();
    void		 setFormatInfo( int channels, int rate, int bps );

    static int		 addOpenDevice(AU_NTSystem *device);
    static void		 removeOpenDevice(AU_NTSystem *device);
    static AU_NTSystem * getOpenDevice(int portfd);

    using		 CritSect = UT_TBBSpinLock;
    using		 CritSectScope = CritSect::Scope;

    // Set to 1 for input devices, 0 for output devices.
    int			 myInput;

    // Set to 1 if all wave packets for a given device are cleared
    // out of the device queue.
    int                  myDone;
    
    // Number of bytes currently in the device waiting to be played,
    // or the number of bytes in the device queue waiting to be filled.
    int                  myCurrentQueueSize;

    // Buffer for incoming or outgoing samples.
    char		*myQueue;

    // Size of the above buffer in bytes.
    int                  myQueueSize;

    // The index in the above buffer where the next samples should be
    // written to or read from (expressed as a byte position).
    int                  myQueuePos;

    // Set to 1 if audio is supposed to be 4 channel. NT doesn't
    // support 4 channel audio natively, so we fake it.
    int			 my4Channel;
    
    // Handles multi-threaded access to our queue of wave packets.
    CritSect		 myCritSect;
    
    HANDLE		 myHandle;
    WAVEFORMATEX	 myFormat;

    // This must be public so we can get the address.
    static void CALLBACK callbackFunc( HWAVEOUT hwo, UINT uMsg, DWORD dwInst,
				       DWORD_PTR dwParam1, DWORD dwParam2 );
private:

    static DWORD WINAPI	 discardLoop( void *parm );

    using DiscardMutex   = std::mutex;
    using DiscardScope   = std::unique_lock<DiscardMutex>;
    using DiscardCondVar = std::condition_variable;
    
    static int				 theDiscardLoopStarted;
    static UT_ValArray< WAVEHDR * >	 theWaveHeaders;
    static DiscardMutex			 theDiscardCritSect;
    static DiscardCondVar		 theDiscardCondition;
};

#endif // __AU_NTSystem__
