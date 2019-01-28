/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Performance.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *		The performance monitor.
 */

#ifndef __UT_Performance__
#define __UT_Performance__

#include "UT_API.h"

#include "UT_Condition.h"
#include "UT_Lock.h"
#include "UT_Map.h"
#include "UT_PerfMonProfile.h"
#include "UT_PerfMonTimedEvent.h"
#include "UT_PerfMonTypes.h"
#include "UT_Color.h"
#include "UT_Ramp.h"
#include "UT_ThreadSpecificValue.h"
#include <SYS/SYS_AtomicInt.h>

#include <tbb/spin_rw_mutex.h>

#define UT_PERFMON_INVALID_ID	-1

enum UT_PerfMonProfileEventType
{
    UT_PERFMON_PROFILE_STARTED,
    UT_PERFMON_PROFILE_STOPPED,
    UT_PERFMON_PROFILE_REMOVED,
    UT_PERFMON_PROFILE_RENAMED,
    UT_PERFMON_PROFILE_LOADED,
    UT_PERFMON_PROFILE_STATS_UPDATED,
    UT_PERFMON_PROFILE_RECORD_OPTIONS_CHANGED,
    UT_PERFMON_PROFILE_STATS_CLEARED,
    UT_PERFMON_ACTIVE_PROFILE_CHANGED,
    UT_PERFMON_NUM_PROFILE_EVENT_TYPES
};

typedef void (*UT_PerfMonProfileEventMethod)
    (void *callee, UT_PerfMonProfileEventType type, int profile_id);

typedef void (*UT_PerfMonEventCB)
    (void *callee, const UT_PerfMonEvent *event);

class OP_Node;
class SIM_Solver;
class UT_NetSocket;
class UT_PerfMonEvent;
class UT_PerfMonProfile;
class UT_PerfMonStats;
class ut_PerfMonProfileInterest;
class ut_PerfMonEventInterest;
class UT_WorkBuffer;

class UT_API UT_Performance
{
public:
     UT_Performance();
    virtual ~UT_Performance();

    /// ********************************************************
    /// EVENT MANAGEMENT
    /// ********************************************************

    /// Start a generic timed event or memory event.  For custom monitoring.
    /// Check if there are more specific startTimed*() and startMemory*()
    /// methods below that you can call instead before using these methods.
    ///
    /// Return the event id, or UT_PERFMON_INVALID_ID if the event
    /// was not recorded (i.e. no profile has been started).
    /// If `auto_nest_events` is true, then any other events
    /// that are started and stopped before this event is stopped
    /// will be treated as dependencies on this event.
    int			     startTimedEvent(
				const char *event, 
				const char *object,
				bool auto_nest_events=true);
    int			     startMemoryEvent(
				const char *object,
				bool auto_nest_events=true);

    /// Start a timed event or memory event for OP cooks.
    /// Return the event id, or UT_PERFMON_INVALID_ID if the event
    /// was not recorded (i.e. no profile has been started).
    int			     startTimedCookEvent(
				int node_id, 
				const char *cook_event_name=NULL);
    int			     startMemoryCookEvent(
				int node_id,
				const char *cook_event_name=NULL);

    /// Add a new timed cook event.
    /// Instead of starting and stopping the event,
    /// immediately set the event's run time to `time`. The event is
    /// added to any interested profiles and observers.
    void		     addTimedCookEvent(
				int node_id,
				fpreal time,
				const char *cook_event_name=NULL);

    /// Start a timed event or memory event for DOP solves.
    /// Return the event id, or UT_PERFMON_INVALID_ID if the event
    /// was not recorded (i.e. no profile has been started).
    int			     startTimedSolveEvent(
				const SIM_Solver *solver, 
				const char *solver_event_name=NULL);
    int			     startMemorySolveEvent(
				const SIM_Solver *solver, 
				const char *solver_event_name=NULL);

    /// Start a timed event or memory event for a generic draw event.
    /// Set `is_gpu_draw` to true if the draw event occurred on the GPU.
    /// Return the event id, or UT_PERFMON_INVALID_ID if the event
    /// was not recorded (i.e. no profile has been started).
    int			     startTimedDrawEvent(
				const char *object,
				const char *draw_event_name,
				bool is_gpu_draw=false);
    int			     startMemoryDrawEvent(
				const char *object,
				const char *draw_event_name,
				bool is_gpu_draw=false);

    /// Start a timed event or memory event for viewport draws.
    /// Set `is_gpu_draw` to true if the draw event occurred on the GPU.
    /// Return the event id, or UT_PERFMON_INVALID_ID if the event
    /// was not recorded (i.e. no profile has been started).
    int			     startTimedViewportDrawEvent(
				const char *viewport_name, 
				const char *draw_event_name,
				UT_PerfMonViewportType viewport_type,
				bool is_gpu_draw=false);
    int			     startMemoryViewportDrawEvent(
				const char *viewport_name, 
				const char *draw_event_name,
				UT_PerfMonViewportType viewport_type,
				bool is_gpu_draw=false);

    /// Add a new timed draw event.
    /// Instead of starting and stopping the event,
    /// immediately set the event's run time to `time`.  The event is
    /// added to any interested profiles and observers.
    void		     addTimedDrawEvent(
				const char *object,
				const char *draw_event_name,
				fpreal time,
				bool is_gpu_draw=false);

    /// Add a new timed viewport draw event.
    /// Instead of starting and stopping the event,
    /// immediately set the event's run time to `time`.  The event is
    /// added to any interested profiles and observers.
    void		     addTimedViewportDrawEvent(
				const char *object,
				const char *draw_event_name,
				UT_PerfMonViewportType viewport_type,
				fpreal time,
				bool is_gpu_draw=false);

    /// Start a timed script event or memory script event.
    /// Return the event id, or UT_PERFMON_INVALID_ID if the event
    /// was not recorded (i.e. no profile has been started).
    int			     startTimedScriptEvent(
				const char *code_description,
				UT_PerfMonScriptType script_type,
				bool auto_nest_events=true);
    int			     startMemoryScriptEvent(
				const char *code_description,
				UT_PerfMonScriptType script_type,
				bool auto_nest_events=true);

    /// Start a timed render event or a memory render event.
    /// Return the event id, or UT_PERFMON_INVALID_ID if the event
    /// was not recorded (i.e. no profile has been started).
    int			     startTimedRenderEvent(
				const char *renderer, 
				const char *operation, 
				bool is_renderer_a_node,
				int frame_num,
				const char *object=NULL);
    int			     startMemoryRenderEvent(
				const char *renderer, 
				const char *operation, 
				bool is_renderer_a_node,
				int frame_num,
				const char *object=NULL);

    /// Add a new timed or memory render event.
    /// Instead of starting and stopping the event,
    /// immediately set the event's run time or memory.  
    /// The event is added to any interested profiles and observers.
    void		     addTimedRenderEvent(
				const char *object,
				bool is_renderer_a_node,
				int frame_num,
				fpreal time);
    void		     addMemoryRenderEvent(
				const char *object,
				bool is_renderer_a_node,
				int frame_num,
				fpreal value);

    /// Start a timed or memory frame event.
    /// Return the event id, or UT_PERFMON_INVALID_ID if the event
    /// was not recorded.
    int			     startTimedFrameEvent(fpreal frame);
    int			     startMemoryFrameEvent(fpreal frame);

    /// Stop a running event.
    /// Return the event's (self) value if `return_value` is set to true.
    /// If the event defined by `event_id` does not exist, or is not running,
    /// or if `return_value` is set to false, then return 0.
    /// Note that calculating the event's value can be expensive.
    fpreal		     stopEvent(int event_id, bool return_value=false);

    /// These methods start and stop events that are specifically designed 
    /// to measure the creation of nodes.
    int			     startTimedNodeCreationEvent();
    int			     startMemoryNodeCreationEvent();
    void		     stopNodeCreationEvent(int event_id, int node_id);

    /// Cancel a running event.  The event is stopped but is not added
    /// to any profiles nor is it broadcast to any observers.
    /// Do nothing if the event does not exist or if it is not running.
    void		     cancelEvent(int event_id);

    /// Log a generic message event.
    void		     logMessage(const char *message);

    /// Return the specified event. (read-only)
    /// Return NULL if the requested event does not exist.
    const UT_PerfMonEvent	 *getEvent(int event_id) const;

    /// Return the specified timed event. (read-only)
    /// Return NULL if the requested event does not exist 
    /// or is not a timed event.
    const UT_PerfMonTimedEvent   *getTimedEvent(int event_id) const;

    /// Start timing the given task scope.
    /// Return the id of the event that the task is attached to.
    /// Return UT_PERMON_INVALID_ID if no such event exists.
    /// This method should only be called in UT_TaskScope's constructor.
    virtual int		     startTaskScope(const UT_TaskScope* task_scope);

    /// Stop timing the given task scope.
    /// This method should only be called in UT_TaskScope's destructor.
    virtual void	     stopTaskScope(
				const UT_TaskScope* task_scope,
				int event_id);

    /// Set extra information that will appear with the event in the
    /// stats and event log.
    void		     setEventExtraInfo(
				int event_id, const char *extra_info) const;

    /// Return the port that the performance monitor is listening on
    /// for remote events.  Return 0 if the performance monitor
    /// is currently not listening for events.
    int			     getEventListenPort() const;

    /// ********************************************************
    /// PROFILE MANAGEMENT
    /// ********************************************************

    /// Start a new profile and capture timed and memory events.
    /// When the profile is stopped, the events are processed
    /// to produce a statistical report.
    /// Return the unique id for the new profile.
    int			     startProfile(
				const char *title,
				const UT_PerfMonRecordOptions *options=NULL);

    /// Stop the specified profile.  Tabulate timings.
    /// Do nothing if the profile does not exist or has already stopped.
    void		     stopProfile(int profile_id);

    /// Cancel the specified profile.  Do nothing if the profile
    /// does not exist or is not profiling.
    void		     cancelProfile(int profile_id);

    /// Pause the specified profile.  Do nothing if the profile
    /// does not exist, or if it is not profiling,
    /// or if it is already paused.
    void		     pauseProfile(int profile_id);

    /// Resume the specified profile.  Do nothing if the profile
    /// does not exist, or if it is not profiling,
    /// or if it is not paused.
    void		     resumeProfile(int profile_id);

    /// Load a profile from disk.  Return the id of the loaded profile.
    /// Return UT_PERFMON_INVALID_ID if the load failed.
    int			     loadProfile(
				const char *file_path, 
				UT_StringArray &errors);

    /// Delete the specified profile and unregister it from
    /// the performance monitor.  Do nothing if the profile does not exist.
    void		     removeProfile(int profile_id);

    /// Rename the specified profile.
    void		     renameProfile(
				int profile_id, 
				const char *profile_name);

    /// Change the recording options for the specified profile.
    void		     changeProfileRecordOptions(
				int profile_id,
				const UT_PerfMonRecordOptions &options);

    /// Clear the stats for the given profile.
    void		     clearProfileStats(int profile_id);

    /// Return the specified profile. (read-only)
    /// Return NULL if the requested profile does not exist.
    const UT_PerfMonProfile *getProfile(int profile_id) const;

    /// Return the id of the active profile.
    /// Return UT_PERFMON_INVALID_ID if there is no active profile.
    int			     getActiveProfileId() const
				{ return myActiveProfileId; }

    /// Set the active profile to the given id
    void		     setActiveProfileId(int profile_id);

    /// Return the number of completed profiles that the performance monitor
    /// is storing.  A completed profile is a profile that is no longer 
    /// recording events and has generated stats.
    int			     numCompletedProfiles() const;

    /// Return the completed profile at the specified index.
    /// A completed profile is a profile that is no longer recording events
    /// and has generated stats.
    const UT_PerfMonProfile *getCompletedProfile(int index) const;

    /// Add an interest for profile events.
    /// Do nothing if the interest has been previously added.
    void		     addProfileInterest(
				void *callee, 
				UT_PerfMonProfileEventMethod callback);

    /// Remove an interest for profile events.
    /// Do nothing if the specified interest does not exist.
    void		     removeProfileInterest(
				void *callee,
				UT_PerfMonProfileEventMethod callback);

    /// Return true if the performance monitor already
    /// has the given profile interest.  Return false otherwise.
    bool		     hasProfileInterest(
				void *callee,
				UT_PerfMonProfileEventMethod callback);

    /// Add an interest for running events.
    /// You can pass in a set of record options to define the types of events
    /// that you are interested in.
    void		     addEventInterest(
				void *callee,
				UT_PerfMonEventCB callback,
				const UT_PerfMonRecordOptions &options);

    /// Update the recording options for the specified event interest.
    void		     changeEventInterestRecordOptions(
				void *callee,
				UT_PerfMonEventCB callback,
				const UT_PerfMonRecordOptions &options);

    /// Remove an interest for events.
    void		     removeEventInterest(
				void *callee,
				UT_PerfMonEventCB callback);

    /// Return true if the performance monitor already
    /// has the given profile interest.  Return false otherwise.
    bool		     hasEventInterest(
				void *callee,
				UT_PerfMonEventCB callback);

    /// Create a new profile that stores difference in stats between profiles
    /// Return the unique id for the new profile.
    int			     createDiffProfile(int id1, int id2);
 
    /// ********************************************************
    /// RECORDING METHODS
    /// ********************************************************

    /// Return true if the performance monitor is recording Houdini events.
    /// Return false otherwise.
    bool		     isRecording() const { 
				return myIsRecordingCount.load() > 0
				    || getDisplayNodeInfo(); 
			     }

    /// Return true if Houdini is recording cook stats.
    bool		     isRecordingCookStats() const {
				return myRecordCookStatsCount.load() > 0
				    || getDisplayNodeInfo();
			     }

    /// Return true if Houdini is recording simulation solve stats.
    bool		     isRecordingSolveStats() const {
				return myRecordSolveStatsCount.load() > 0;
			     }

    /// Return true if Houdini is recording object draw stats.
    bool		     isRecordingObjectDrawStats() const {
				return 
				    myRecordObjectDrawStatsCount.load() > 0;
			     }

    /// Return true if Houdini is recording object draw stats.
    bool		     isRecordingObjectGPUDrawStats() const {
				return 
				    myRecordObjectGPUDrawStatsCount.load() > 0;
			     }
    
    /// Return true if Houdini is recording viewport draw stats.
    bool		     isRecordingViewportStats() const {
				return myRecordViewportStatsCount.load() > 0;
			     }

    /// Return true if Houdini is recording script stats.
    bool		     isRecordingScriptStats() const {
				return myRecordScriptStatsCount.load() > 0;
			     }

    /// Return true if Houdini is recording render stats.
    bool		     isRecordingRenderStats() const {
				return myRecordRenderStatsCount.load() > 0;
			     }
				    
    /// Return true if Houdini is recording thread stats.
    bool		     isRecordingThreadStats() const {
				return myRecordThreadStatsCount.load() > 0;
			     }
				    
    /// Return true if Houdini is recording frame stats.
    bool		     isRecordingFrameStats() const {
				return myRecordFrameStatsCount.load() > 0;
			     }
				    
    /// Return true if Houdini is recording memory stats.
    bool		     isRecordingMemoryStats() const {
				return myRecordMemoryStatsCount.load() > 0;
			     }
				    
    /// Return true if Houdini is recording errors.
    bool		     isRecordingErrors() const {
				return myRecordErrorsCount.load() > 0;
			     }

    /// Returns true if the particular node passed is allowed to
    /// participate in recording of the statistics, false otherwise.
    /// This is currently used for disallowing recording of the contents
    /// of compiled nodes.
    virtual bool	     isRecordingAllowedFor(OP_Node* node) const;

    /// ********************************************************
    /// FRAME STATISTIC METHODS
    /// ********************************************************
    
    /// Return the number of timed frame events 
    /// recorded by the performance monitor.
    int			     getNumRecordedTimedFrameEvents() const
				{ return myTimedFrameEventCounter; }
    
    /// Return the number of memory frame events 
    /// recorded by the performance monitor.
    int			     getNumRecordedMemoryFrameEvents() const
				{ return myMemoryFrameEventCounter; }

    /// Return the total time spent in the frame events
    /// recorded by the performance monitor.
    fpreal		     getTotalRecordedFrameEventTime() const
				{ return myTotalFrameTime; }

    /// Return the total memory accumulation in the frame events
    /// recorded by the performance monitor.
    fpreal		     getTotalRecordedFrameEventMemory() const
				{ return myTotalFrameMemory; }

    /// Reset the performance monitor's accumulated frame statistics.
    void		     resetCumulativeFrameStats();

    /// ********************************************************
    /// MISCELLANEOUS
    /// ********************************************************

    /// Return the icon name to use for the given stats object.
    /// Return NULL if there is no known icon to use for the object.
    /// Subclasses can override this method to set up their own mappings
    /// of stats objects to icons.
    virtual const char	    *getStatsIcon(const UT_PerfMonStats *stats) const;

    /// ********************************************************
    /// DISPLAY OPTIONS
    /// ********************************************************
    
    /// Toggle whether or not the performance info is displayed in
    /// the Node info.
    void		     setDisplayNodeInfo(bool display);
    bool		     getDisplayNodeInfo() const
				{ return myDisplayNodeInfo; }

    /// Thresholds for displaying various colours in the network editor and 
    /// in the Spreadsheet.
    void		     getPerformanceColor(fpreal pos, UT_Color &c);

    /// Return the ramp color scheme used by the performance monitor.
    const UT_Ramp	    &getRampColorScheme() const;

    /// Set the performance monitor's ramp color scheme.
    void		     setRampColorScheme(const UT_Ramp &src_ramp);

    /// Load the ramp color scheme from the given string representation.
    void		     loadRampColorScheme(const char *str_color_scheme);

    /// Save a representation of the ramp color scheme to the given string.
    void		     saveRampColorScheme(UT_String &str_color_scheme);
  
    /// Return the default ramp color scheme.
    const UT_Ramp	    &getDefaultColorRampScheme();

    /// ********************************************************
    /// MAINTENANCE 
    /// ********************************************************

    /// Reset the state of the performance monitor.
    /// Reset counters and delete events and profiles.
    /// This method should never be called.  It is handy for testing purposes.
    void		     reset();
    
protected:

    /// Return the current frame.
    virtual int		     getCurrentFrameSubclass_();

    /// Return a pointer to the node specified by the given id.
    virtual OP_Node	    *getNodeSubclass_(int node_id) const;

    /// Return a pointer to the node that is bound to the given SIM solver.
    virtual OP_Node	    *getNodeFromSolverSubclass_(
				const SIM_Solver *solver) const;

    /// Pass back an object path that can be tied to a UT_PerfMonStats object.
    /// The path is generated from the given node and event name.
    virtual void	     getObjectPathFromNodeSubclass_(
				OP_Node *node, const char *event_name,
				UT_WorkBuffer &object_path) const;

    /// Pass back an object path that can be tied to a UT_PerfMonStats object.
    /// The path is generated from the given solver, node and event name.
    virtual void	     getObjectPathFromSolverAndNodeSubclass_(
				const SIM_Solver *solver, OP_Node *node,
				const char *event_name, UT_WorkBuffer &object_path)
				const;

    /// Pass back extra info that can be tied to a UT_PerfMonStats object.
    /// The extra info is generated from the given node.
    virtual void	     getExtraInfoFromNodeSubclass_(
				OP_Node *node, UT_WorkBuffer &extra_info) const;

private:

    int			     newEventId_();
    int			     newProfileId_();


    /// Return the specified timed event.
    /// Return NULL if the requested event does not exist
    /// or is not a timed event.
    UT_PerfMonTimedEvent    *getTimedEvent_(int event_id) const;

    /// Return the specified event.
    /// Return NULL if the requested event does not exist.
    UT_PerfMonEvent	    *getEvent_(int event_id) const;

    /// Start a event.  
    /// Subclasses must call this method when starting their own events.
    int			     startEvent_(
				const char *event,
				const char *object,
				bool auto_nest_events,
				UT_PerfMonCategory category,
				UT_PerfMonEventType type,
				UT_PerfMonObjectType object_type,
				const char *object_icon,
				int *frame_num=NULL,
				const char *extra_info=NULL);

    /// Start a cook event.
    int			     startCookEvent_(
				UT_PerfMonEventType type,
				int node_id, const char *cook_event_name);

    /// Start a solve event.
    int			     startSolveEvent_(
				UT_PerfMonEventType type,
				const SIM_Solver *solver,
				const char *solver_event_name);

    /// Start a draw event.
    int			     startDrawEvent_(
				UT_PerfMonEventType type,
				const char *object,
				const char *draw_event_name,
				bool is_gpu_draw);

    /// Start a viewport draw event.
    int			     startViewportDrawEvent_(
				UT_PerfMonEventType type,
				const char *viewport_name, 
				const char *draw_event_name,
				UT_PerfMonViewportType viewport_type,
				bool is_gpu_draw);

    /// Start a generic draw event.
    int			     startGenericDrawEvent_(
				UT_PerfMonEventType type,
				const char *object,
				const char *draw_event_name,
				UT_PerfMonObjectType object_type,
				const char *object_icon,
				bool is_gpu_draw);

    /// Start a script event.
    int			     startScriptEvent_(
				UT_PerfMonEventType type, 
				const char *code_description, 
				UT_PerfMonScriptType script_type, 
				bool auto_nest_events);

    /// Start a render event.
    int			     startRenderEvent_(
				UT_PerfMonEventType type, 
				const char *renderer, 
				const char *operation, 
				bool is_renderer_a_node, 
				int frame_num, 
				const char *object);

    /// Return the specified profile.
    /// Return NULL if the requested profile does not exist.
    UT_PerfMonProfile	    *getProfile_(int profile_id) const;

    /// Activate profile.
    void		     activateProfile_(UT_PerfMonProfile *profile);

    /// De-activate profile.
    void		     deactivateProfile_(UT_PerfMonProfile *profile);

    /// Increment recording counters based on the specified options.
    void		     incrementRecordingCounters_(
				const UT_PerfMonRecordOptions &options);

    /// Decrement recording counters based on the specified options.
    void		     decrementRecordingCounters_(
				const UT_PerfMonRecordOptions &options);

    /// Remove and delete the specified profile. Returns the profile id
    int			     removeProfile_(UT_PerfMonProfile *profile);

    /// Notify that given profile id has been destroyed
    void		     notifyRemovedProfile_(int profile_id);

    /// Remove the event.  Delete it if it is a top-level event.
    void		     removeEvent_(
				int event_id, 
				UT_PerfMonEvent *event,
				bool is_cancelled=false);

    // Find the running parent event (if any) of the given event
    // and create a link between them.
    void		     findAndLinkToRunningParentEvent_(
				UT_PerfMonEvent *event);
    
    UT_PerfMonEvent	    *findRunningParentEvent_(
				const UT_TaskScope *task,
				bool for_memory_event,
				bool require_auto_nest);

    /// Return the list of running events for the given task.
    /// The most recently started event will appear at the end of the list.
    /// This method will create a new list if no such list already exists.
    UT_PerfMonEventList	    *getOrCreateRunningEventList_(
				    const UT_TaskScope *task,
				    bool for_memory_event);

    /// Return the list of running events for the given task.
    /// Return NULL if no such list exists.
    /// It is up to the caller to acquire the appropriate thread events lock.
    UT_PerfMonEventList	    *getRunningEventList_(
				    const UT_TaskScope *task,
				    bool for_memory_event) const;

    /// Remove the specified event from the running events list
    /// associated with the given task.
    /// It is up to the caller to acquire the appropriate thread events lock.
    void		     removeFromRunningEventList_(
				const UT_TaskScope *task, 
				UT_PerfMonEvent *event);

    /// Add the specified event to the running events list
    /// associated with the given task.
    void		     addToRunningEventList_(
				const UT_TaskScope *task,
				UT_PerfMonEvent *event);

    /// Reset the thread tasks to running event lists data structure.
    void		     resetRunningEventLists_();

    /// Contribute the event's data to any interested profiles.
    void		     contributeEventToProfiles_(UT_PerfMonEvent *event);

    /// Return the index of the profile interest that matches the given
    /// callee and callback.  Return -1 if no such interest exists.
    int			     findProfileInterest_(void *callee,
				UT_PerfMonProfileEventMethod callback) const;

    /// Notify interests that the given profile event has taken place.
    void		     notifyProfileInterests_(
				UT_PerfMonProfileEventType event_type,
				int profile_id) const;

    /// Return the index of the event interest that matches the given
    /// callee and callback.  Return -1 if no such interest exists.
    int			     findEventInterest_(void *callee,
				UT_PerfMonEventCB callback) const;

    /// Notify interests that the given event has been stopped.
    void		     notifyEventInterests_(
				const UT_PerfMonEvent *event) const;

    using RWLock = tbb::spin_rw_mutex;

    /// Return the lock to use for the given task's running events list.
    RWLock		    &getRunningEventsLock_(
				const UT_TaskScope *task,
				bool for_memory_event);

    /// Return true if the given options are interested in recording
    /// the passed-in event.
    bool		     areOptionsInterestedInEvent_(
				const UT_PerfMonRecordOptions &options,
				const UT_PerfMonEvent *event) const;

    /// Helper method for stopEvent().
    /// Update cumulative frame statistics with the given frame value.
    void		     addToCumulativeFrameStats_(
				fpreal frame_value, bool is_memory_value);

    /// Start and initialize a thread to listen for incoming events
    /// from remote processes.  Do nothing if the listen thread has already
    /// been started.
    void		     initAndStartEventListenThread_();

    /// Stop the thread that is listening for incoming events from remote
    /// processes.  Do nothing if there is no listen thread.
    void		     stopEventListenThread_();

    /// Block until the event listen thread has signaled that it is initialized.
    /// Return immediately If there is no event listen thread.
    void		     ensureEventListenThreadIsInitialized_();

    /// Initialize the default color ramp scheme.
    void		     initDefaultColorRampScheme_();

    /// Keep track of the active profile.
    int			     myActiveProfileId;

    /// Option that determines whether performance information is displayed 
    /// with Node info.
    bool		     myDisplayNodeInfo;

    UT_Ramp		     myColorRamp;
    UT_Ramp		     myDefaultColorRamp;
 
    // Members dealing with events.
    SYS_AtomicInt32	     myEventIdCounter;
    UT_Map<int, UT_PerfMonEvent *> myEvents;

    // Members dealing with profiles.
    SYS_AtomicInt32	     myProfileIdCounter;
    UT_Map<int, UT_PerfMonProfile *> myProfiles;
    UT_PerfMonProfileList    myRecordingProfiles;
    UT_PerfMonProfileList    myCompletedProfiles;

    // These locks protect the performance monitor's events and profiles.
    // If you plan to use both locks in a method, be sure to
    // lock the profiles lock first, then the events lock.
    mutable RWLock	     myEventsLock;
    mutable RWLock	     myProfilesLock;
    mutable RWLock	     myProfileInterestsLock;
    mutable RWLock	     myEventInterestsLock;

    /// A map of task scopes to running timed event lists for each thread.
    typedef UT_Map<const UT_TaskScope *, UT_PerfMonEventList *> TaskToEventListMap;
    UT_ThreadSpecificValue<TaskToEventListMap *> myTasksToRunningTimedEvents;
    UT_ThreadSpecificValue<RWLock>	  myTasksToRunningTimedEventsLock;

    /// A map of task scopes to running memory event lists for each thread.
    UT_ThreadSpecificValue<TaskToEventListMap *> myTasksToRunningMemoryEvents;
    UT_ThreadSpecificValue<RWLock>	  myTasksToRunningMemoryEventsLock;

    /// A list of interests on profile events.
    UT_ValArray<ut_PerfMonProfileInterest *> myProfileInterests;

    /// A list of interests on events.
    UT_ValArray<ut_PerfMonEventInterest *>   myEventInterests;

    /// Socket for listening on incoming events from other processes
    /// (i.e. Mantra).
    UT_NetSocket	    *myEventSocket;
    UT_Thread		    *myEventListenThread;
    bool		     myListenThreadIsInitialized;
    UT_Lock		     myListenThreadIsInitializedLock;
    UT_Condition	     myListenThreadIsInitializedCond;

    // Recording counters.
    SYS_AtomicInt32	     myIsRecordingCount;
    SYS_AtomicInt32	     myRecordCookStatsCount;
    SYS_AtomicInt32	     myRecordSolveStatsCount;
    SYS_AtomicInt32	     myRecordObjectDrawStatsCount;
    SYS_AtomicInt32	     myRecordObjectGPUDrawStatsCount;
    SYS_AtomicInt32	     myRecordViewportStatsCount;
    SYS_AtomicInt32	     myRecordScriptStatsCount;
    SYS_AtomicInt32	     myRecordRenderStatsCount;
    SYS_AtomicInt32	     myRecordThreadStatsCount;
    SYS_AtomicInt32	     myRecordFrameStatsCount;
    SYS_AtomicInt32	     myRecordMemoryStatsCount;
    SYS_AtomicInt32	     myRecordErrorsCount;

    // Frame stat counters.
    fpreal		     myTotalFrameTime;
    fpreal		     myTotalFrameMemory;
    int			     myTimedFrameEventCounter;
    int			     myMemoryFrameEventCounter;

    static void *	     listenForRemoteEvents_(void *data);
};

UT_API UT_Performance	*UTgetPerformance(bool create=true);
UT_API void		 UTsetPerformance(UT_Performance *perfmon);

#endif

