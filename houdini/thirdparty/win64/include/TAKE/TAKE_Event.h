/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TAKE_Event.h ( TAK Library, C++)
 *
 * COMMENTS:	Take events
 */

#ifndef __TAKE_Event__
#define __TAKE_Event__

#include "TAKE_API.h"
#include <UT/UT_Notifier.h>

class TAKE_Take;

class TAKE_API TAKE_Event {
public:
    enum TAKE_EventType {
	TAKE_SWITCH,		// Current take was switched
	TAKE_ADDED,		// New take was created
	TAKE_DELETED,		// Take was deleted
	TAKE_RENAME,		// Take was re-named
	TAKE_RELABEL,		// Take was re-labelled
	TAKE_MOVED,		// Take was moved in the hierarchy
	TAKE_INCLUDE,		// Parameter included/excluded
	TAKE_DEFNAME,		// Default take name changed
	TAKE_MODECHANGE,	// Auto include mode was changed
    };

    TAKE_Event(TAKE_EventType type, const char *name, const TAKE_Take *take=0)
		: myEventType(type), myTakeName(name), myTake(take) { }

    const char		*getTakeName() const	{ return myTakeName; }
    const TAKE_Take	*getTake() const	{ return myTake; }
    TAKE_EventType	 getType() const	{ return myEventType; }

    static const char	*getEventName(TAKE_EventType type);

private:
    TAKE_EventType	 myEventType;
    const TAKE_Take	*myTake;
    const char		*myTakeName;
};


#endif
