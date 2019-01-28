/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_NoMemHandler.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *	This class handles memory allocation failures.  Currently only
 *  implemented for Windows and Linux.
 *	
 */
#ifndef __UT_NoMemHandler_h__
#define __UT_NoMemHandler_h__

#include "UT_API.h"
#include <stdlib.h>

// A UT_NoMemHandlerCallback is a callback that the application can register
// if it wants to be notified when memory allocation has failed.
typedef	void (*UT_NoMemHandlerCallback)(void);

// The maximum number of callbacks we allow.  We do not need a dynamic
// array for these since they should all be registered at code startup.
#define	    UT_MEMHANDLER_MAX_CALLBACKS		16

class UT_API UT_NoMemHandler
{
public:
    // Global static accessor to UT_NoMemHandler singleton.
    static UT_NoMemHandler      &getNoMemHandler();

    // Call this method to add a memory handler callback.  When memory 
    // allocation fails, these callbacks are called.  Their current purpose is 
    // to try to free some memory so that we can save before the inevitable
    // crash.
    void		       addCallback(UT_NoMemHandlerCallback callback);

    // The main new handlers called by the C-runtime library.
    int			       classNewHandler(size_t size);

    // Disable handler entirely. Used for non-GUI apps like Houdini Engine.
    void                       disableHandler();

private:
    // Private constructor/destructor because no one is allowed to allocate us.
     UT_NoMemHandler();
    ~UT_NoMemHandler();

     
    // The registered callbacks
    UT_NoMemHandlerCallback	myCallbacks[UT_MEMHANDLER_MAX_CALLBACKS];
    int				myNumCallbacks;

    // This flag is used to avoid recursive calls into our handler in case
    // one of the callbacks called from it causes another malloc/new to fail.
    bool			myInsideHandler;

    // Once disabled, this handler will never allow new callbacks to be added.
    bool                        myHandlerDisabled;
};

#endif
