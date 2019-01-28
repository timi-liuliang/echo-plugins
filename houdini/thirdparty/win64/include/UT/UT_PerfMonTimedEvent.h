/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PerfMonTimedEvent.h (UT Library, C++)
 *
 * COMMENTS:
 *
 *		Performance monitor timed event.
 */

#ifndef __UT_PerfMonTimedEvent__
#define __UT_PerfMonTimedEvent__

#include "UT_API.h"

#include <SYS/SYS_Time.h>
#include "UT_PerfMonEvent.h"
#include "UT_PerfMonTypes.h"
#include "UT_ValArray.h"
#include "UT_Map.h"
#include "UT_IntArray.h"
#include "UT_String.h"
#include "UT_Thread.h"
#include "UT_ThreadSpecificValue.h"
#include "UT_PerfMonUtils.h"

class UT_API UT_PerfMonTimedEvent : public UT_PerfMonEvent
{
public:
    UT_PerfMonTimedEvent(
	int id, 
	const UT_StringHolder &name, 
	const UT_StringHolder &object,
	bool auto_nest_events, 
	UT_PerfMonCategory category,
	UT_PerfMonObjectType object_type, 
	const UT_StringHolder &object_icon = UT_StringHolder::theEmptyString, 
	const UT_StringHolder &extra_info = UT_StringHolder::theEmptyString,
	int frame_num = UT_PERFORMANCE_INVALID_FRAME);
    virtual ~UT_PerfMonTimedEvent();

    /// Return true if the event is a timed event.
    virtual bool		 isTimedEvent() const
				    { return true; }

    /// Start timing a threaded task.
    void			 startTaskTimer(const UT_TaskScope *task);

    /// Stop timing a threaded task. Calculate the elapsed time and store it.
    void			 stopTaskTimer(const UT_TaskScope *task);

    /// Override the event's elapsed time.
    /// Handy when you know the event's elapsed time but not its
    /// start time nor stop time.
    void			 overrideTime(fpreal time)
				    { UT_PerfMonEvent::overrideValue(time); }

    /// Return the start time.
    /// Return NULL if the event has not started or if its time
    /// has been overridden.
    const SYS_TimeVal		*startTime() const;
    
    /// Return the stop time.
    /// Return NULL if the event has not stopped or if its time
    /// has been overridden.
    const SYS_TimeVal		*stopTime() const;

    /// Return the time it took to complete the event.
    /// Return 0 if the event has not started and stopped.
    fpreal			 time() const
				    { return UT_PerfMonEvent::value(); }

    /// Return the time it took to complete the event excluding
    /// the time it took to complete any child events.
    /// Return 0 if the self time has not been calculated yet.
    fpreal			 selfTime() const
				    { return UT_PerfMonEvent::selfValue(); }

    /// Return the thread id in which the event occured.
    int				 threadId() const;

    // Pass back a list of ids for the threads that worked on the event.
    void			 getThreadIds(UT_IntArray &thread_ids) const;

    /// Return the total time spent in the given thread.
    fpreal			 totalTimeForThread(int thread_id) const;

    /// Format the event into a pretty string that can be outputted in a log.
    virtual void		 getFormattedString(
				    UT_PerfMonLogTimeFormat time_format,
				    UT_String &str_event) const;

protected:

    /// Perform work when the event is started.
    virtual void		 subclassStart_();

    /// Perform work when the event is stopped.
    virtual void		 subclassStop_();

    /// Perform work when the event's value has been overridden.
    virtual void		 subclassOverrideValue_();

    /// Calculate the event's self value (time).
    /// Return the self value.
    virtual fpreal		 subclassCalculateSelfValue_();

    /// Collect the data from the given child event and merge it into this
    /// event's children data.
    virtual void		 subclassCollectDataFromChild_(
				    const UT_PerfMonEvent *child);

private:

    /// Pass back a formatted string of this (frame) event.
    void			 getFormattedFrameEventString_(
				    UT_PerfMonLogTimeFormat time_format, 
				    UT_String &str_event) const;

    /// Pass back a formatted string of this (non-frame) event.
    void			 getFormattedEventString_(
				    UT_PerfMonLogTimeFormat time_format, 
				    UT_String &str_event) const;

    /// Pass back a formatted string of the given time.
    void			 getFormattedTime_(
				    fpreal time, 
				    UT_PerfMonLogTimeFormat time_format, 
				    UT_String &str_time) const;

    /// Pass back a formatted string of the frequency calculated 
    /// by the given time.
    void			 getFormattedFrequency_(
				    fpreal time, UT_String &str_frequency) 
				    const;

    SYS_TimeVal			*myStartTime;
    SYS_TimeVal			*myStopTime;
    int				 myThreadId;

    /// List of running times for each thread.  The times are represented
    /// as a list of time blocks where each block is the timespan that the
    /// thread was working on the event.
    /// Assumption:
    ///	    Task scopes for individual threads do not overlap.
    ///	    Therefore we can store them in an array rather than a merged queue
    UT_ThreadSpecificValue<UT_TimeBlockList>		&threadTimes();
    const UT_ThreadSpecificValue<UT_TimeBlockList>	&threadTimes() const;
    mutable UT_ThreadSpecificValue<UT_TimeBlockList>	*myThreadTimes;

    typedef UT_Map<const UT_TaskScope *, SYS_TimeVal *> ThreadTaskTable;
    UT_ThreadSpecificValue<ThreadTaskTable>		&activeThreadTasks();
    const UT_ThreadSpecificValue<ThreadTaskTable>	&activeThreadTasks() const;
    mutable UT_ThreadSpecificValue<ThreadTaskTable>	*myActiveThreadTasks;

    // Time blocks for completed child events.
    UT_TimeBlockQueue		 myChildTimeBlocks;
};

#endif

