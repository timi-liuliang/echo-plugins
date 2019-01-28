/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OH_EventHandler.h ( Op Handler Library, C++)
 *
 * COMMENTS:
 *		This is the base class for the various types of event
 *		handlers. Typically gadgets will derive from this
 *		(or some subclass of this class) and redefine how
 *		its virtual methods work.
 *		
 *		For full documentation, please refer to DOCS/index.html
 */

#ifndef __OH_EventHandler__
#define __OH_EventHandler__

#include "OH_API.h"
#include <UI/UI_Object.h>
#include <OP/OP_Value.h>
class OP_Node;
class UI_Value;

#include "OH_TriggerObject.h"

class OH_API OH_EventHandler
{
public:
		 	 OH_EventHandler(OH_TriggerObject &);
    virtual	 	~OH_EventHandler();
    virtual const char 	*ohClassName() const { return "OH_EventHandler"; }

    // You call ohRequestDeferredUpdate in response to an event that
    // should cause an Update. When the time is right, OH will call
    // ohHandleDeferredUpdate for you. You should override 
    // ohHandleDeferredUpdate to do the "Update" computation. If you
    // think that the "Update" should cause you gadget to Redraw, then
    // call ohRequestDeferredRedraw. That in turn, will call
    // ohHandleDeferredRedraw.
    //
    // Note: OH_OpEventHandler overrides ohHandleDeferredUpdate already
    //       and calls ohHandleDeferredNodeUpdate that can be overridden.

    virtual void	 ohRequestDeferredUpdate();
    virtual void	 ohHandleDeferredUpdate();
    virtual void	 ohRequestDeferredRedraw();
    virtual void	 ohHandleDeferredRedraw();

    // These two should only called by OH_TriggerObject. You should typically
    // only override ohHandleTimeChange, and then only if you've called 
    // ohAddTimeInterest.
    virtual void	 ohHandleTrigger();
    virtual void	 ohHandleTimeChange(UI_Event *);

    // The following Time functions are provided purely for convenience.
    // Their functioning is completely independent of the rest of this
    // class.
    void		 ohAddTimeInterest()
			    { myOHtriggerObject.addTimeInterest(); }
    void		 ohRemoveTimeInterest()
			    { myOHtriggerObject.removeTimeInterest(); }

    // ohGetTimeValue returns NULL if you haven't set a time interest
    UI_Value*		 ohGetTimeValue()
			    { return myOHtriggerObject.getTimeValue(); }

    // ohGetTime returns 0 if you haven't set a time interest
    fpreal		 ohGetTime() const
			    { return myOHtriggerObject.getTime(); }

protected:
    virtual void	 ohRequestTrigger();

    bool		 ohUpdatePending() const { return myOHupdatePending; }

private:
    OH_TriggerObject	&myOHtriggerObject;

    bool		 myOHtriggerPending;
    bool		 myOHupdatePending;
    bool		 myOHredrawPending;
};

#endif
