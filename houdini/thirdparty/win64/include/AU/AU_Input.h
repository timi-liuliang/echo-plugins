/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	AU_Input.h ( AU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __AU_Input__
#define __AU_Input__

#include "AU_API.h"
#include <UT/UT_AudioInput.h>

#define AU_MAX_VOL_CHANNELS	4


#if defined(WIN32)
    #include "AU_NTSystem.h"
    class AU_API AU_Input : public UT_AudioInput, public AU_NTSystem
#elif defined(MBSD)
    class AU_MBSDUtilAudioInfo;

    class AU_API AU_Input : public UT_AudioInput
#elif defined(LINUX)

    class AU_API AU_Input : public UT_AudioInput
#else
    #error Unsupported platform
#endif
{
public:

		 AU_Input(int channels, int rate);
    virtual	~AU_Input();

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

private:
#ifdef WIN32
    // Creates a new wave header with its data block at pos in myQueue.
    void	appendWaveHeader(int pos);
    // Apply volume adjustments, and copy 2 channels to 4 if required
    void	applyVolume(char *data, int bytesPerChannel);
    
    int		myPort;

    // Specifies the input source.
    int		mySource;
    // The size of each wave packet (in bytes).
    int		myBlockSize;
    // The number of bytes copied from the last incomplete block.
    int		myBlockOffset;
    // NT doesn't allow setting of input volume, so we fake it.
    float	myVolumes[4];

#elif defined(MBSD) 
    // adjust the volume
    void	applyVolume(char *data, int data_size );

    AU_MBSDUtilAudioInfo *  myInfo; // audio info (rate, buffers, etc).
    int		myRate;		    // sample rate
    int		myChannels;	    // number of channels
    int		mySource;	    // specifies the input source
    float	myVolumes[ AU_MAX_VOL_CHANNELS ];   // volumes per channel

#elif defined(LINUX)
    void	adjustRate(int read);
    
    int		myRate;
    int		myIsOpen;

#else
    #error Unsupported platform
#endif
};

#endif
