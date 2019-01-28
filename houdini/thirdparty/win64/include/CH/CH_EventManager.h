/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH_EventManager.h ( Channel Library, C++)
 *
 * COMMENTS:
 *		CH_EventManager
 *		    . this is the class that manages channel event handlers
 *		    . there is one global channel manager
 *		CH_EventHandler
 *		    . objects of this type handle events and do things such as:
 *		    	. keeping track of added channels
 *		    	. sending out events on destruction
 *		CH_AutoEventHandler
 *		    . this class is provided for convenience
 *		    . if constructed with a CH_EventHandler, it will register
 *		    it with the event manager; when the AutoEventHandler object
 *		    goes out of scope, it will unregerister and destroy the
 *		    event handler.
 */

#ifndef __CH_EventManager__
#define __CH_EventManager__

#include "CH_API.h"
#include <UT/UT_Map.h>

class CH_Channel;

/*
 * the difference between CH_CHANNELS_MAJOR_CHANGE and CH_CHANNEL_CHANGED is that
 * the former indicates a major change to the segments and will lead to an
 * OP_PARM_ANIMATED event being sent out in OP_Channels, whereas the latter is
 * not so major.
 * 
 * order matters with these events
 */
typedef enum {
    CH_UNCHANGED,		// used to dirty caches without sending extra
				//   events (see PRM_Parm::indirectChange())
    CH_CHANNELS_DELETED,	// one or more fewer channels (data: NULL)
    CH_CHANNELS_ADDED,		// new channel (singular?)
    CH_CHANNELS_MAJOR_CHANGE,	// segment pointers changed/added/removed, etc.
    CH_CHANNEL_CHANGED,		// key values changed, etc.
    CH_CHANNEL_UICHANGED,	// eg. expression mode
    CH_PARM_ENABLE_CHANGED,	// parm enable state changed
    CH_PARM_VISIBLE_CHANGED,	// parm visible state changed
    CH_CHANNELS_MOVED,		// time value(s) changed
    CH_PERMISSION_CHANGED,	// permission to this channel changed

    // These CH_GROUP* events are used by the CH_Manager only
    CH_GROUP_CREATED,	// must be first CH_GROUP item!
    CH_GROUP_REMOVED,
    CH_GROUP_RENAMED,
    CH_GROUP_MOVED,
    CH_GROUP_SELCHANGED,
    CH_GROUP_CURCHANGED,
    CH_GROUP_CHANGED,	// must be last CH_GROUP item!

    CH_SCOPE_CHANGED,
    CH_SCOPE_PIN_CHANGED,
    CH_SCOPE_SELECTION_CHANGED,
    CH_SCOPE_DISPLAY_CHANGED,
    CH_SCOPE_GRAPH_SELECTION_CHANGED,
    
    CH_CHANGE_TYPE_COUNT    // sentinel
} CH_CHANGE_TYPE;

CH_API extern const char * CHeventNames[CH_CHANGE_TYPE_COUNT];

class CH_API CH_EventHandler
{
public:
    CH_EventHandler() {}
    virtual ~CH_EventHandler() {}
    virtual void event( CH_CHANGE_TYPE, void * );

    static CH_EventHandler *newHandler()
    {
	return new CH_EventHandler;
    }
};

typedef CH_EventHandler *(*CH_EventHandlerCreator)();


class CH_Collection;

class CH_API CH_EventHandlerHash: public CH_EventHandler
{
public:
    CH_EventHandlerHash();
    virtual ~CH_EventHandlerHash();
    virtual void event( CH_CHANGE_TYPE, void * );

    static CH_EventHandler *newHandler()
    {
	return new CH_EventHandlerHash;
    }
protected:
    void		 removeReferencingEvents(void *data);
private:
    UT_Map<CH_Channel *, CH_Collection *> myOccurred[CH_CHANGE_TYPE_COUNT];
    bool		 myScopeChanged;
    CH_Collection	*myRootCollection;
};

class CH_API CH_EventManager
{
public:
    CH_EventManager();
    virtual ~CH_EventManager();

    void		beginHandlingEvents( CH_EventHandlerCreator creator );
    void		endHandlingEvents();

    CH_EventHandler 	*getHandler();

    bool enabled() const;
    void setEnabled(bool e);

private:
    CH_EventHandler			*myDefaultHandler;
    CH_EventHandler			*myCurrentHandler;
    int					 myLevels;
    bool                                 myEnabled;
};

CH_API CH_EventManager *CHgetEventManager();

// The following functions are provided for convenience:
CH_API void		CHevent( CH_CHANGE_TYPE, void * );

class CH_API CH_AutoEventHandler
{
public:
    CH_AutoEventHandler( CH_EventHandlerCreator creator )
    {
	CHgetEventManager()->beginHandlingEvents( creator );
    }

    ~CH_AutoEventHandler()
    {
	CHgetEventManager()->endHandlingEvents();
    }
};

#endif
