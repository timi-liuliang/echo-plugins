/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_audio_h__
#define __HOM_audio_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_EnumValue.h"
#include "HOM_EnumModules.h"
#include "HOM_Errors.h"

SWIGOUT(%rename(audio) HOM_audio;)
class HOM_API HOM_audio
{
public:
    virtual ~HOM_audio()
    {}

    virtual std::string __repr__() = 0;

    // switching the audio panel mode
    virtual void turnOffAudio() = 0;
    virtual void useTimeLineMode() = 0;
    virtual void useTimeSliceMode() = 0;
    virtual void useTestMode() = 0;

    // controlls common to all modes
    virtual void setMono(bool on) = 0;
    virtual void setVolumeTied(bool on) = 0;
    virtual void setMeter(bool on) = 0;
    virtual void setLeftVolume(double volume) = 0;
    virtual void setRightVolume(double volume) = 0;

    // switching between playing a CHOP or a file 
    virtual void useChops() = 0;
    virtual void useAudioFile() = 0;

    // setting the chop playback parameters
    virtual void setChopPath(const char *node_path) = 0;
    
    // setting the file playback parameters
    virtual void setAudioFileName(const char *file_name) = 0;
    virtual void setAudioOffset(double time_offset) = 0;
    virtual void setAudioFrame(double frame) = 0;

    // parameters specific to time line mode (scrub)
    virtual void setScrubRepeat(bool on) = 0;
    virtual void setScrubSustain(double sustain) = 0;
    virtual void setScrubRate(double scrub_rate) = 0;

    // parameters specific to test mode 
    virtual void reverse() = 0;
    virtual void stop() = 0;
    virtual void play() = 0;
    virtual void setLooping( bool on ) = 0;
    virtual void setRewind( bool on ) = 0;
};

#endif
