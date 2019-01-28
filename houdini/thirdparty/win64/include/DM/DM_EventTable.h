/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        DM_EventTable.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Table for all viewport UI event hooks.
 */
#ifndef __DM_EventTable_H__
#define __DM_EventTable_H__

#include "DM_API.h"
#include "DM_MouseHook.h"


class DM_API DM_EventTable
{
public:
    static DM_EventTable *getTable();
    
    /// @brief Register a mouse hook.
    /// Register a mouse hook for creating mouse event hooks.
    /// Returns true if the hook was successfully registered.
    bool	registerMouseHook(DM_MouseHook	    *hook);
    
private:
		 DM_EventTable();
    virtual	~DM_EventTable();


    static void clearEventHookTable(void *);
    static void clearMouseEventHooks(void *);

    friend class DM_Viewport;
};

extern "C" {
    /// The plugin entry point for event hook registration 
    SYS_VISIBILITY_EXPORT extern void DMnewEventHook(DM_EventTable *table);
};



#endif
