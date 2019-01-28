/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OH_TriggerObject.h ( Op Handler Library, C++)
 *
 * COMMENTS:
 *		This is a little helper class. I need to be able to have
 *		UI interests, yet I can't subclass OH_OpHandler off a
 *		UI_Object because that prevents multiple inheritance of
 *		OH_OpHandler and some UI_Object based class, making
 *		OH_OpHandler useless. So, I just contain this little
 *		UI_Object for expressing interests in UI events.
 */

#ifndef __OH_TriggerObject__
#define __OH_TriggerObject__

#include "OH_API.h"
#include <UI/UI_Object.h>
#include <UI/UI_Value.h>

class OH_EventHandler;

class OH_API OH_TriggerObject : public UI_Object
{
public:
		 	 OH_TriggerObject(OH_EventHandler &);
    virtual	 	~OH_TriggerObject();
    virtual const char 	*className() const;

    void		 addTimeInterest();
    void		 removeTimeInterest();
    void		 sendTriggerEvent();

    // getTimeValue returns NULL if you haven't set a time interest
    UI_Value		*getTimeValue() { return myTimeValue; }

    // getTime returns 0 if you haven't set a time interest
    fpreal		 getTime() const { return myTimeValue ?  
						(fpreal)*myTimeValue : 0; }

private:
    void	 	 handleTriggerEvent(UI_Event *);
    void		 handleTimeChange(UI_Event *);

    UI_Value		*myTimeValue;
    OH_EventHandler	&myEventHandler;
};

#endif
