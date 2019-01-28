/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_AudioInput.h ( AU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_AudioInput__
#define __UT_AudioInput__

#include "UT_API.h"
#include <SYS/SYS_Types.h>

#define UT_AUDIO_MIC_IN		0
#define UT_AUDIO_LINE_IN	1
#define UT_AUDIO_DIGITAL	2

class UT_API UT_AudioInput
{
public:

		 UT_AudioInput(int channels, int rate);
    virtual	~UT_AudioInput();

    virtual int		open(float queuesize);
    virtual void	close();
    
    virtual int		hasInput();
    virtual short *	getInput(int size);

    virtual int		getGlobalRate();

    virtual int		setSource(int source);
    virtual int		getSource();
    
    virtual int		setMicrophoneMode(int stereo);
    virtual int		getMicrophoneMode();
    virtual int		getChannelMode();

    virtual int		setVolume(int speaker, float vol);
    virtual float	getVolume(int speaker);

    virtual UT_AudioInput * newAudioInputDevice(int channels, int rate);

    static void		  setAudioInputDevice(UT_AudioInput *);
    static UT_AudioInput *getAudioInputDevice();
private:
    static UT_AudioInput * myDevicePtr;

};



#endif
