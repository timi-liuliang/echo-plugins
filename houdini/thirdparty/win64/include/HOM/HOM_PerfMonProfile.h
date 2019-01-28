/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_PerfMonProfile_h__
#define __HOM_PerfMonProfile_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include <string>

SWIGOUT(%rename(PerfMonProfile) HOM_PerfMonProfile;)

class HOM_API HOM_PerfMonProfile
{
public:
    HOM_PerfMonProfile()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_PerfMonProfile(const HOM_PerfMonProfile &profile)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_PerfMonProfile()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual int id() = 0;
    virtual std::string title() = 0;

    virtual bool isActive() = 0;
    virtual bool isPaused() = 0;
    virtual bool isRecordingCookStats() = 0;
    virtual bool isRecordingSolveStats() = 0;
    virtual bool isRecordingDrawStats() = 0;
    virtual bool isRecordingGPUDrawStats() = 0;
    virtual bool isRecordingViewportStats() = 0;
    virtual bool isRecordingScriptStats() = 0;
    virtual bool isRecordingRenderStats() = 0;
    virtual bool isRecordingThreadStats() = 0;
    virtual bool isRecordingFrameStats() = 0;
    virtual bool isRecordingMemoryStats() = 0;
    virtual bool isRecordingErrors() = 0;

    virtual void stop() = 0;
    virtual void cancel() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual void save(const char *file_path) = 0;
    virtual void exportAsCSV(const char *file_path) = 0;

    virtual std::string _stats() = 0;
};

#endif
