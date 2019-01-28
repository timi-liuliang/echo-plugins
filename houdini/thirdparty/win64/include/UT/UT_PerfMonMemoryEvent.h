/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PerfMonMemoryEvent.h (UT Library, C++)
 *
 * COMMENTS:
 *
 *		Performance monitor memory event.
 */

#ifndef __UT_PerfMonMemoryEvent__
#define __UT_PerfMonMemoryEvent__

#include "UT_API.h"

#include <SYS/SYS_Time.h>
#include "UT_PerfMonEvent.h"
#include "UT_PerfMonTypes.h"
#include "UT_ValArray.h"
#include "UT_IntArray.h"
#include "UT_String.h"
#include "UT_Thread.h"
#include "UT_ThreadSpecificValue.h"
#include "UT_PerfMonUtils.h"

class UT_API UT_PerfMonMemoryEvent : public UT_PerfMonEvent
{
public:
    UT_PerfMonMemoryEvent(
	int id, const char *name, const char *object, bool auto_nest_events, 
	UT_PerfMonCategory category, UT_PerfMonObjectType object_type, 
	const char *object_icon=NULL, const char *extra_info=NULL,
	int frame_num = UT_PERFORMANCE_INVALID_FRAME);
    virtual ~UT_PerfMonMemoryEvent();

    /// Format the event into a pretty string that can be outputted in a log.
    virtual void		 getFormattedString(
				    UT_PerfMonLogTimeFormat time_format,
				    UT_String &str_event) const;

protected:

    /// Perform work when the event is started.
    virtual void		 subclassStart_();

    /// Perform work when the event is stopped.
    virtual void		 subclassStop_();

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
				    UT_String &str_event) const;

    /// Pass back a formatted string of this (non-frame) event.
    void			 getFormattedEventString_(
				    UT_String &str_event) const;

    size_t			 myStartMemoryUsage;
    size_t			 myStopMemoryUsage;
    size_t			 myChildrenMemoryUsage;
};

#endif

