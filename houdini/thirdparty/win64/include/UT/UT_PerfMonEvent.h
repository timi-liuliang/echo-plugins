/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PerfMonEvent.h (UT Library, C++)
 *
 * COMMENTS:
 *
 *		Performance monitor generic event.
 */

#ifndef __UT_PerfMonEvent__
#define __UT_PerfMonEvent__

#include "UT_API.h"

#include <SYS/SYS_Time.h>
#include "UT_Assert.h"
#include "UT_PerfMonTypes.h"
#include "UT_ValArray.h"
#include "UT_String.h"
#include "UT_StringHolder.h"
#include "UT_PerfMonUtils.h"

class UT_PerfMonEvent;
typedef UT_ValArray<UT_PerfMonEvent *> UT_PerfMonEventList;

class UT_API UT_PerfMonEvent
{
public:
    UT_PerfMonEvent(
	int id, 
	const UT_StringHolder &name, 
	const UT_StringHolder &object,
	bool auto_nest_events, 
	UT_PerfMonCategory category,
	UT_PerfMonEventType type,
	UT_PerfMonObjectType object_type, 
	const UT_StringHolder &object_icon=NULL, 
	const UT_StringHolder &extra_info=NULL,
	int frame_num = UT_PERFORMANCE_INVALID_FRAME);
    virtual ~UT_PerfMonEvent();

    /// Return the unique id assigned by the performance monitor.
    int				 id() const;

    /// Return the frame number in which the event occured.
    int				 frameNum() const;

    /// Return the event name.
    const char			*name() const;

    /// Return the object that the event applies to.
    const char			*object() const;

    /// Set the object that the event applies to.
    void			 setObject(const UT_StringHolder &object);

    /// Return the icon used for the object.
    /// Return NULL if the object has no associated icon.
    const char			*objectIcon() const;

    /// Return extra information about the event.
    /// Return NULL if there is no extra information.
    const char			*extraInfo() const;

    /// Set extra information that will be displayed with the event
    /// in profile statistics and in the event log.
    void			 setExtraInfo(
				    const UT_StringHolder &extra_info);

    /// Return the event type.
    UT_PerfMonEventType		 type() const;

    /// Return the event object type.
    UT_PerfMonObjectType	 objectType() const;

    /// Return true if this event has automatic child event nesting enabled.
    bool			 isAutoNestEnabled() const;

    /// Return the category.
    UT_PerfMonCategory		 category() const;

    /// Return true if the event is a timed event.
    virtual bool		 isTimedEvent() const
				    {
					UT_ASSERT(
					    myType != UT_PERFMON_TIMED_EVENT
					);
					return false; 
				    }

    /// Start the event.  This puts the event in a running state.
    /// Real work is performed in subclassStart_().
    void			 start();

    /// Stop the event.  This removes the event from a running state.
    /// Real work is performed in subclassStop_(). 
    void			 stop();

    /// Return true if the event is in the running state.
    /// Return false otherwise.
    bool			 isRunning() const;

    /// Return the event value.
    /// If no value has been set, then return 0.
    fpreal			 value() const;

    /// Return the event value minus the value of any of the descendent events.
    /// If no self value has been calculated, then return 0.
    fpreal			 selfValue() const;

    /// Set the event value.
    void			 setValue(fpreal val);

    /// Override the event's value.
    /// Handy when you know the event's value and not its starting
    /// or stopping values.
    void			 overrideValue(fpreal value);

    /// Calculate the event's self value.
    /// Do nothing if the event's main value has not been set.
    /// This can be an expensive call since the values of the event's
    /// descendents must be examined.
    void			 calculateSelfValue();

    /// Add a child event.
    void			 appendChild(UT_PerfMonEvent *event);

    /// Remove the given child event.  Do nothing if the given event
    /// is not actually a child of this event.
    void			 removeChild(UT_PerfMonEvent *event);

    /// Remove all child events.
    void			 removeChildren();

    /// Return the number of child events.
    int				 numChildren() const;

    /// Return the child at the given index.
    const UT_PerfMonEvent	*getChild(int index) const;
    UT_PerfMonEvent		*getChild(int index);

    /// Add a parent event.
    void			 appendParent(UT_PerfMonEvent *event);

    /// Remove the given parent event.  Do nothing if the given event
    /// is not actually a parent of this event.
    void			 removeParent(UT_PerfMonEvent *event);

    /// Remove all parent events.
    void			 removeParents();

    /// Return the number of parent events.
    int				 numParents() const;

    /// Return the parent at the given index.
    const UT_PerfMonEvent	*getParent(int index) const;
    UT_PerfMonEvent		*getParent(int index);

    /// Format the event into a pretty string that can be outputted in a log.
    virtual void		 getFormattedString(
				    UT_PerfMonLogTimeFormat time_format,
				    UT_String &str_event) const;

protected:

    /// Return true if the value has been overridden.
    bool			 isValueOverridden_() const;

    /// Perform work when the event is started.
    virtual void		 subclassStart_();

    /// Perform work when the event is stopped.
    virtual void		 subclassStop_();

    /// Perform work when the event's value has been overridden.
    virtual void		 subclassOverrideValue_();

    /// Perform work to calculate the event's self value.
    /// Return the calculated self value.
    virtual fpreal		 subclassCalculateSelfValue_();

    /// Collect the data from the given child event and merge it into this
    /// event's children data.
    virtual void		 subclassCollectDataFromChild_(
				    const UT_PerfMonEvent *child);

    /// Pass back a formatted string of this event's object path and extra info.
    void			 getFormattedObjectPathAndExtraInfo_(
				    bool enclose_obj_path_in_parens,
				    UT_String &object_path,
				    UT_String &extra_info) const;

private:

    /// Collect the data from the given child event and merge it into this
    /// event's children data.
    void			 collectDataFromChild_(
				    const UT_PerfMonEvent *child);

    int				 myId;
    UT_StringHolder		 myName;
    UT_StringHolder		 myObject;
    UT_StringHolder		 myObjectIcon;
    UT_StringHolder		 myExtraInfo;
    bool			 myIsAutoNestEnabled;
    int				 myFrameNum;
    fpreal			 myValue;
    fpreal			 mySelfValue;
    bool			 myIsValueSet;
    bool			 myIsSelfValueSet;
    bool			 myIsValueOverridden;
    UT_PerfMonCategory		 myCategory;
    UT_PerfMonEventList		 myChildren;
    UT_PerfMonEventList		 myParents;
    UT_PerfMonEventType		 myType;
    UT_PerfMonObjectType	 myObjectType;

    bool			 myIsRunning;
};

#endif

