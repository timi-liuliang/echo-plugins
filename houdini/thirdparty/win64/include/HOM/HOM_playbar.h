/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_playbar_h__
#define __HOM_playbar_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_ElemPtr.h"
#include "HOM_EnumModules.h"
#include "HOM_EnumValue.h"
#include "HOM_Errors.h"

#include <map>
#include <vector>


class HOM_Pane;
class HOM_Vector2;

SWIGOUT(%rename(playbar) HOM_playbar;)
class HOM_API HOM_playbar
{
public:
    virtual ~HOM_playbar()
    {}

    virtual std::string __repr__() = 0;

    virtual bool isPlaying() = 0;
    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void reverse() = 0;

    virtual void jumpToNextKeyframe() = 0;
    virtual void jumpToPreviousKeyframe() = 0;

    virtual HOM_EnumValue &playMode() = 0;
    virtual void setPlayMode(HOM_EnumValue &mode) = 0;

    virtual double frameIncrement() = 0;
    virtual void setFrameIncrement(double increment) = 0;

    SWIGOUT(%newobject playbackRange;)
    virtual HOM_Vector2 *playbackRange() = 0;
    virtual void setPlaybackRange(double start, double end) = 0;

    virtual bool isRangeRestricted() = 0;
    virtual void setRestrictRange(bool on) = 0;

    virtual bool usesIntegerFrames() = 0;
    virtual void setUseIntegerFrames(bool on) = 0;

    virtual bool isRealTime() = 0;
    virtual void setRealTime(bool on) = 0;

    virtual double realTimeFactor() = 0;
    virtual void setRealTimeFactor(double factor) = 0;

    virtual bool isRealTimeSkipping() = 0;
    virtual void setRealTimeSkipping(bool on) = 0;

    virtual bool isAudioShown() = 0;
    virtual void showAudio(bool on) = 0;
    virtual bool areKeysShown() = 0;
    virtual void showKeys(bool on) = 0;
    virtual bool isSimCacheShown() = 0;
    virtual void showSimCache(bool on) = 0;
    virtual bool isRangeSliderShown() = 0;
    virtual void showRangeSlider(bool on) = 0;
    virtual bool areTicksShown() = 0;
    virtual void showTicks(bool on) = 0;

    virtual void moveToBottom() = 0;
    virtual void moveToPane(HOM_Pane &pane) = 0;

    SWIGOUT(%ignore addOpaqueEventCallback;)
    virtual void addOpaqueEventCallback(void *callback) = 0;
    SWIGOUT(%ignore removeOpaqueEventCallback;)
    virtual void removeOpaqueEventCallback(void *callback) = 0;
    SWIGOUT(%ignore opaqueEventCallbacks;)
    virtual std::vector<void *> opaqueEventCallbacks() = 0;
    SWIGOUT(%ignore clearOpaqueEventCallbacks;)
    virtual void clearOpaqueEventCallbacks() = 0;

#ifdef SWIG
%extend
{
    void addEventCallback(InterpreterObject callback)
    { self->addOpaqueEventCallback(callback); }

    void removeEventCallback(InterpreterObject callback)
    { self->removeOpaqueEventCallback(callback); }

    void clearEventCallbacks()
    { self->clearOpaqueEventCallbacks(); }

    // Note that we return a vector of borrowed references because swig
    // will increment the reference counts.
    std::vector<InterpreterObject> eventCallbacks()
    {
	std::vector<void *> opaque_callbacks(self->opaqueEventCallbacks());

	std::vector<InterpreterObject> callbacks;
	for (int i=0; i< opaque_callbacks.size(); ++i)
	    callbacks.push_back((InterpreterObject)opaque_callbacks[i]);
	return callbacks;
    }
}
#endif

    virtual std::map<
		HOM_ElemPtr<HOM_Parm>,
		std::vector< HOM_ElemPtr<HOM_BaseKeyframe> >
	    > selectedKeyframes() = 0;

    SWIGOUT(%newobject selectionRange;)
    virtual HOM_Vector2 *selectionRange() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Vector2> > selectionRanges() = 0;

    SWIGOUT(%newobject timelineRange;)
    virtual HOM_Vector2 *timelineRange() = 0;

    SWIGOUT(%newobject timeRange;)
    virtual HOM_Vector2 *timeRange() = 0;
    virtual void setTimeRange(double start, double end) = 0;

    SWIGOUT(%newobject frameRange;)
    virtual HOM_Vector2 *frameRange() = 0;
    virtual void setFrameRange(double start, double end) = 0;
};

#endif
