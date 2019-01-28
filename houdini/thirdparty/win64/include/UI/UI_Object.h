/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UI_Object.h
 *
 * COMMENTS:
 *	
 *  A UI_Object is the base class for objects that are organized in a
 *  hierarchy and are capable of receiving UI_Events.
 *
 */
#ifndef __UI_Object__
#define __UI_Object__

#include "UI_API.h"
#include "UI_EventType.h"

#include <UT/UT_Assert.h>
#include <hboost/container/stable_vector.hpp>
#include <list>

class RE_Render;
class UT_String;
class UI_Event;
class UI_Manager;
class UI_Queue;
class UI_DeviceEvent;
class UI_Object;
class UI_Value;
class UI_HotkeyEcho;

//
// Turn the following define to 1 to perform proxy pointer referencing of
// objects.  Turn it to 0 to disable the proxy pointers.  Proxy pointers
// prevent UI_Values sending events to deleted objects (which shouldn't happen
// in real life)
//
#define UI_ENABLE_PROXY_POINTERS	1

typedef void (UI_Object::*UI_EventMethod)(UI_Event *);

// stable_vector is a hybrid of list and vector; giving random access and
// stable iterators upon insert/erase (except for an element pointed to), 
// albeit with the loss of contiguity.
typedef hboost::container::stable_vector<UI_Object *> UI_ObjectList;
typedef hboost::container::stable_vector<UI_Value *> UI_ValueList;

class UI_API UI_Object
{
public:
    UI_Object();

    virtual ~UI_Object();

    // !!! WARNING: Do NOT derive from UT_NonCopyable due to Visual C++ bug !!!
    // !!! Otherwise Visual C++ will start causing subclass callback member
    // !!! method pointers (of single inheriance chains) to become larger than
    // !!! UI_EventMethod.
    UI_Object(const UI_Object &) = delete;
    UI_Object &operator=(const UI_Object &) = delete;
    // !!! WARNING: Do NOT derive from UT_NonCopyable due to Visual C++ bug !!!

    virtual void	 handleEvent(UI_Event *event);
    virtual int		 interestingEvent(UI_EventType t, UI_DeviceEvent *event) const;
    virtual void	 deleteReferences(UI_Object *to_whom);

    UI_Object		*getParent() const		{ return myParent; }
    void		 setParent(UI_Object *p)	{ myParent = p; }
    bool		 isAncestor(const UI_Object *who) const;

    void		 addDependent(UI_Object *who);
    bool		 removeDependent(UI_Object *who);
    bool		 replaceDependent(UI_Object *who, UI_Object *with);
    bool		 isDependent(UI_Object *who) const;
    bool		 hasDependents() const { return !myDependents.empty(); }
    const UI_ObjectList &dependents() const { return myDependents; }

    void		 sendEvent(const UI_Event &e) const;
    void		 distributeEvent(UI_Event *event, int upwards);
    void 		 relayEvent(UI_Event *event, UI_Object *target);
    void 		 generateEvent(UI_EventType t,  UI_Object *target);
    void		 purgeEvents(UI_EventType t,  UI_Object *target,
        		             UI_EventMethod method=0);

    // This method is similar to "distributeEvent()" but it bypasses the event
    // queue, triggering the events on the dependents immediately.
    // **This should only be used in special circumstances.**
    void   		 triggerImmediateEvent(UI_Event *event, int upwards);

    virtual const char	*className() const;

#if UI_ENABLE_PROXY_POINTERS == 1
    int			 getProxyId();
#endif

    void		 bumpQueueCount(int dir) { myQueueCount += dir; }


    // only works in debug.
    void		 setName(const char *symbolName);
    const char		*getName() const;
    void		 buildFullPath(UT_String &string) const;

    // Debug events which target this UI_Object coming in from given UI_Value.
    // You must also enable the define UI_QUEUE_DEBUG_TARGET_ORIGIN in
    // UI_Queue.C in order to use this method. The type is optional; if
    // specified, only events of that type are monitored.
    // Either object or value (or both) can be specified.
    void		 debugQueueEventsFor(
			    const UI_Value *value,
			    const char *name, 
			    UI_EventType t = UI_EVENT_NO_EVENT) const;


    static UI_Manager	*getManager()	 { return theUIManager; }
    static UI_Queue	*getInputQueue() { return theUIQueue; }

    static int		 keycmp(const char *, int key);
    static int		 keycmp(const char *, int key,
				const UI_HotkeyEcho &);

    static void		 keyEcho(const char *, const UI_HotkeyEcho &);
    static void		 actionKeyEcho(const char *,
				       const char *custom_suffix = 0);
    static void		 toggleKeyEcho(const char *, bool new_value);

    const UI_ValueList	&valueInterests() const { return myValueInterests; }


    // only UI_Value should call these methods.
    void		 interestedInValue(UI_Value *);
    void		 removeValueInterest(UI_Value *);

protected:
    UI_ObjectList 	&dependents() { return myDependents; }

    // This is the function called by the UI_Manager at the start of a call
    // to terminate, but only if this object has been added to the UI_Manager
    // list of pre-terminate callbacks.
    virtual void	 preTerminateCallback()
			 { }

private:
    UI_Object		*myParent;
    UI_ObjectList	 myDependents;
    UI_ValueList	 myValueInterests;
    int			 myQueueCount;
#if UI_ENABLE_PROXY_POINTERS == 1
    int			 myProxyId;
#endif
#if defined(UI_DEBUG_OBJECT_NAME)
    char		*myName;
#endif
    bool		 myDestroying : 1;

    friend class	 UI_Manager;
    void		 setManager(UI_Manager *m, UI_Queue *q);

    static UI_Manager	*theUIManager;
    static UI_Queue	*theUIQueue;
};

#endif
