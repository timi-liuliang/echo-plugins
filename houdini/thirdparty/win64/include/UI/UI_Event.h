#ifndef __UI_Event__
#define __UI_Event__

#include "UI_API.h"
#include <string.h>
#ifdef DEBUG_TRIGGER
#include "UI_Global.h"		// Need debug macros for inline function...
#endif
#include "UI_DeviceEvent.h"
#include "UI_Value.h"

template<typename T> class UT_Array; 

class UI_API UI_Event {
public:
    UI_Event();
    ~UI_Event();

    UI_Event(const UI_Event &e);

    UI_Event &operator=(const UI_Event &e);

    UI_Event(
	UI_EventType	 t,
	UI_Object	*to,
	UI_Object	*from = (UI_Object *)0,
	UI_DeviceEvent	*e    = (UI_DeviceEvent *)0
    );

    UI_Event(
	UI_Value	*v,
	UI_Object	*to,
	UI_Object	*from = (UI_Object *)0,
	UI_Reason	 r    = UI_VALUE_CHANGED
    );

    UI_Event(
	UI_Value	*v,
	UI_Object	*to,
	UI_EventMethod	 callback,
	UI_Object	*from = (UI_Object *)0,
	UI_Reason	 r    = UI_VALUE_CHANGED
    );

    UI_Event(
	UI_EventType	 t,
	UI_Object	*to,
	UI_EventMethod	 callback,
	UI_Object	*from = (UI_Object *)0
    );

    UI_Object	 	*source;
    UI_Object	 	*target;
    UI_EventMethod	 method;
    UI_Value		*value;
    UI_EventType	 type;
    
    UI_DeviceEvent	 state;
    
    UI_Reason		 reason;

    // history of collapsed events, 0 if no events were collapsed.
    // Because of how we merge, this is a STACK.
    // Ie, event->state is the most recent event and
    // (*event->myStateHistory)[0] is the oldest, with
    // *event->myStateHistory->last() the event just before event->state.
    // This lets the usual process of merging events to just
    // append to the list rather than prepend.
    UT_Array<UI_DeviceEvent>	*myStateHistory;

    void		 trigger()
			 {
#ifdef DEBUG_TRIGGER
			     UI_DEBUG(DBG_EVENTS,
			     UI_debug("Trigger %s, source: %s, target: %s\n",
				    name(),
				    source ? source->className() : "NULL",
				    target ? target->className() : "NULL"));
#endif
			     if( target ) 
				 (target->*method)(this);
			 }

    const char		*name() const;
    const char		*className() const { return "UI_Event"; }
    void		 display() const;
    static const char	*type_name(UI_EventType type);
    static const char	*reason_name(UI_Reason reason);

    // Contents of prevevent are somehow merged into this.
    // Used to consolidate mouse moves into the UI_DeviceEvent history.
    void		 mergeEvent(const UI_Event *prevevent);
};


#endif
