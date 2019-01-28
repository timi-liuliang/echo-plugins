/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_PerformanceMonitor_h__
#define __HOM_PerformanceMonitor_h__

#include "HOM_PaneTab.h"
#include "HOM_EnumValue.h"

SWIGOUT(%rename(PerformanceMonitor) HOM_PerformanceMonitor;)

class HOM_API HOM_PerformanceMonitor : virtual public HOM_PaneTab
{
public:
    HOM_PerformanceMonitor()
    { HOM_CONSTRUCT_OBJECT(this) }
    // Because the lowermost base classes initialize the virtual bases
    // before any non-virtual bases, the correct thing to do here is
    // explicitly call the constructor for HOM_PaneTab.
    HOM_PerformanceMonitor(const HOM_PerformanceMonitor &pane)
    : HOM_PaneTab(pane)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_PerformanceMonitor()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual bool isLiveUpdatesEnabled() = 0;
    virtual void enableLiveUpdates(bool on) = 0;

    virtual bool isRecording() = 0;
    virtual void startRecording() = 0;
    virtual void stopRecording() = 0;

    virtual bool isSamplingCookStats() = 0;
    virtual bool isSamplingErrors() = 0;
    virtual bool isSamplingFrameStats() = 0;
    virtual bool isSamplingMemoryStats() = 0;
    virtual bool isSamplingObjectDrawStats() = 0;
    virtual bool isSamplingObjectGPUDrawStats() = 0;
    virtual bool isSamplingScriptStats() = 0;
    virtual bool isSamplingSolveStats() = 0;
    virtual bool isSamplingThreadStats() = 0;
    virtual bool isSamplingViewportStats() = 0;
    virtual void sampleCookStats(bool on) = 0;
    virtual void sampleErrors(bool on) = 0;
    virtual void sampleFrameStats(bool on) = 0;
    virtual void sampleMemoryStats(bool on) = 0;
    virtual void sampleObjectDrawStats(bool on) = 0;
    virtual void sampleObjectGPUDrawStats(bool on) = 0;
    virtual void sampleScriptStats(bool on) = 0;
    virtual void sampleSolveStats(bool on) = 0;
    virtual void sampleThreadStats(bool on) = 0;
    virtual void sampleViewportStats(bool on) = 0;

    virtual HOM_EnumValue &objectView() = 0;
    virtual void setObjectView(HOM_EnumValue &structure) = 0;

    virtual HOM_EnumValue &timeFormat() = 0;
    virtual void setTimeFormat(HOM_EnumValue &format) = 0;
};

#endif
