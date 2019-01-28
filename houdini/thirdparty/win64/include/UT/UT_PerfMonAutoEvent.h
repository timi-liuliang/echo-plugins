/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PerfMonAutoEvent.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *		Convenience classes for automatically starting and stopping
 *		timed and memory events in the Performance Monitor.
 *
 */

#ifndef __UT_PerfMonAutoEvent__
#define __UT_PerfMonAutoEvent__

#include "UT_NonCopyable.h"
#include "UT_Performance.h"

class SIM_Solver;

/// Base class.
/// Use one of the derived classes (below) instead. 
class UT_PerfMonAutoEvent : UT_NonCopyable
{
public:
    UT_PerfMonAutoEvent()
    {
	myTimedEventId = UT_PERFMON_INVALID_ID;
	myMemoryEventId = UT_PERFMON_INVALID_ID;
	myShouldStopEvents = true;
    }

    ~UT_PerfMonAutoEvent()
    {
	if (!myShouldStopEvents)
	    return;

	if (myMemoryEventId != UT_PERFMON_INVALID_ID)
	    UTgetPerformance()->stopEvent(myMemoryEventId);
	if (myTimedEventId != UT_PERFMON_INVALID_ID)
	    UTgetPerformance()->stopEvent(myTimedEventId);
    }

    void setExtraInfo(const char *extra_info)
    {
	if (myTimedEventId != UT_PERFMON_INVALID_ID)
	    UTgetPerformance()->setEventExtraInfo(myTimedEventId, extra_info);

	if (myMemoryEventId != UT_PERFMON_INVALID_ID)
	    UTgetPerformance()->setEventExtraInfo(myMemoryEventId, extra_info);
    }

    // These exist because the unit tests need them.
    int getTimedEventId() const { return myTimedEventId; }
    int getMemoryEventId() const { return myMemoryEventId; }

protected:
    void setTimedEventId_(int event_id) { myTimedEventId = event_id; }
    void setMemoryEventId_(int event_id) { myMemoryEventId = event_id; }
    void setShouldBaseClassStopEvents_(bool stop) { myShouldStopEvents = stop; }

private:
    int myTimedEventId;
    int myMemoryEventId;
    bool myShouldStopEvents;
};

/// Convenience class for node cook events.
class UT_PerfMonAutoCookEvent : public UT_PerfMonAutoEvent
{
public:
    UT_PerfMonAutoCookEvent(int node_id, const char *cook_event_name=NULL)
    {
	UT_Performance *perf = UTgetPerformance();

	/// The constructor's signature must match the signature
	/// of UT_Performance::start*CookEvent().
	if (perf->isRecordingCookStats())
	{
	    setTimedEventId_(
		perf->startTimedCookEvent(node_id, cook_event_name));
	}

	if (perf->isRecordingMemoryStats())
	{
	    setMemoryEventId_(
		perf->startMemoryCookEvent(node_id, cook_event_name));
	}
    }
    ~UT_PerfMonAutoCookEvent() {};
};

/// Convenience class for DOP solve events.
class UT_PerfMonAutoSolveEvent : public UT_PerfMonAutoEvent
{
public:
    /// The constructor's signature must match the signature
    /// of UT_Performance::start*SolveEvent().
    UT_PerfMonAutoSolveEvent(
	const SIM_Solver *solver, const char *solver_event_name=NULL)
    {
	UT_Performance *perf = UTgetPerformance();

	if (perf->isRecordingSolveStats())
	{
	    setTimedEventId_(
		perf->startTimedSolveEvent(solver, solver_event_name));
	}
		
	if (perf->isRecordingMemoryStats())
	{
	    setMemoryEventId_(
		perf->startMemorySolveEvent(solver, solver_event_name));
	}
    }
    ~UT_PerfMonAutoSolveEvent() {};
};

/// Convenience class for object draw events.
class UT_PerfMonAutoDrawEvent : public UT_PerfMonAutoEvent
{
public:
    /// The constructor's signature must match the signature
    /// of UT_Performance::start*DrawEvent().
    UT_PerfMonAutoDrawEvent(
	const char *object, const char *draw_event_name, bool is_gpu_draw=false)
    {
	UT_Performance *perf = UTgetPerformance();

	if ((!is_gpu_draw && perf->isRecordingObjectDrawStats())
	    || (is_gpu_draw && perf->isRecordingObjectGPUDrawStats()))
	{
	    setTimedEventId_(
		perf->startTimedDrawEvent(object, draw_event_name, 
		    is_gpu_draw));
	}
		
	if (perf->isRecordingMemoryStats())
	{
	    setMemoryEventId_(
		perf->startMemoryDrawEvent(object, draw_event_name,
		    is_gpu_draw));
	}
    }
    ~UT_PerfMonAutoDrawEvent() {};
};

/// Convenience class for viewport draw events.
class UT_PerfMonAutoViewportDrawEvent : public UT_PerfMonAutoEvent
{
public:
    /// The constructor's signature must match the signature
    /// of UT_Performance::start*DrawEvent().
    UT_PerfMonAutoViewportDrawEvent(
	const char *viewport_name, const char *draw_event_name,
	UT_PerfMonViewportType viewport_type, bool is_gpu_draw=false)
    {
	UT_Performance *perf = UTgetPerformance();

	if (perf->isRecordingViewportStats())
	{
	    setTimedEventId_(
		perf->startTimedViewportDrawEvent(
		    viewport_name, draw_event_name, viewport_type, 
		    is_gpu_draw));
	}
		
	if (perf->isRecordingMemoryStats())
	{
	    setMemoryEventId_(
		perf->startMemoryViewportDrawEvent(
		    viewport_name, draw_event_name, viewport_type, 
		    is_gpu_draw));
	}
    }
    ~UT_PerfMonAutoViewportDrawEvent() {};
};

/// Convenience class for script events.
class UT_PerfMonAutoScriptEvent : public UT_PerfMonAutoEvent
{
public:
    /// The constructor's signature must match the signature
    /// of UT_Performance::start*ScriptEvent().
    UT_PerfMonAutoScriptEvent(
	const char *code_description, UT_PerfMonScriptType script_type,
	bool auto_nest_events=true)
    {
	UT_Performance *perf = UTgetPerformance();

	if (perf->isRecordingScriptStats())
	{
	    setTimedEventId_(
		perf->startTimedScriptEvent(
		    code_description, script_type, auto_nest_events));
	}
		
	if (perf->isRecordingMemoryStats())
	{
	    setMemoryEventId_(
		perf->startMemoryScriptEvent(
		    code_description, script_type, auto_nest_events));
	}
    }
    ~UT_PerfMonAutoScriptEvent() {};
};

/// Convenience class for render events.
class UT_PerfMonAutoRenderEvent : public UT_PerfMonAutoEvent
{
public:
    /// The constructor's signature must match the signature
    /// of UT_Performance::start*RenderEvent().
    UT_PerfMonAutoRenderEvent(
	const char *renderer, const char *operation,
	bool is_renderer_a_node, int frame_num, const char *object=NULL)
    {
	UT_Performance *perf = UTgetPerformance();

	if (perf->isRecordingRenderStats())
	{
	    setTimedEventId_(
		perf->startTimedRenderEvent(
		    renderer, operation, is_renderer_a_node, 
		    frame_num, object));
	}
		
	if (perf->isRecordingMemoryStats())
	{
	    setMemoryEventId_(
		perf->startMemoryRenderEvent(
		    renderer, operation, is_renderer_a_node, 
		    frame_num, object));
	}
    }
    ~UT_PerfMonAutoRenderEvent() {};
};

/// Convenience class for node creation events.
class UT_PerfMonAutoNodeCreationEvent : public UT_PerfMonAutoEvent
{
public:
    /// The constructor's signature must match the signature
    /// of UT_Performance::start*NodeCreationEvent().
    UT_PerfMonAutoNodeCreationEvent()
	: myNodeId(-1)
	, myRecording(false)
    {
	// Make sure that the base class does not attempt
	// to stop the underlying events.
	setShouldBaseClassStopEvents_(false);

	UT_Performance *perf = UTgetPerformance();
	
	if (perf->isRecordingCookStats())
	{
	    myRecording = true;
	    setTimedEventId_(
		perf->startTimedNodeCreationEvent());
	}
		
	if (perf->isRecordingMemoryStats())
	{
	    myRecording = true;
	    setMemoryEventId_(
		perf->startMemoryNodeCreationEvent());
	}
    }
    ~UT_PerfMonAutoNodeCreationEvent() 
    {
	if (getTimedEventId() != UT_PERFMON_INVALID_ID)
	{
	    UTgetPerformance()->stopNodeCreationEvent(
		getTimedEventId(), myNodeId);
	}
	if (getMemoryEventId() != UT_PERFMON_INVALID_ID)
	{
	    UTgetPerformance()->stopNodeCreationEvent(
		getMemoryEventId(), myNodeId);
	}
    };

    /// Set the node id that should be attached to the underlying
    /// node creation events.  This method should be called
    /// immediately after the node is created.
    void setNodeId(int node_id) { myNodeId = node_id; }

    bool isRecording() const { return myRecording; }

private:
    int myNodeId;
    bool myRecording;
};

#endif

