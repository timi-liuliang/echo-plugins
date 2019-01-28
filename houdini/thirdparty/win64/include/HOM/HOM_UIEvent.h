/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef HOM_UIEvent_h
#define HOM_UIEvent_h

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_ElemPtr.h"

class HOM_UIEventDevice;
class HOM_Vector3;

SWIGOUT(%rename(UIEvent) HOM_UIEvent;)
SWIGOUT(%feature("notabstract") HOM_UIEvent;)

class HOM_API HOM_UIEvent
{
public:
#ifdef SWIG
%extend
{
    HOM_UIEvent(int64 val)
    { 
	return HOM().newUIEvent(val); 
    }
}
#else
    HOM_UIEvent()
    { 
	HOM_CONSTRUCT_OBJECT(this) 
    }
#endif

    virtual ~HOM_UIEvent()
    { 
	HOM_DESTRUCT_OBJECT(this) 
    }

    virtual std::string __repr__() = 0;

    virtual HOM_UIEventDevice* device() = 0;
    virtual bool hasQueuedEvents() = 0;
    virtual std::vector<HOM_ElemPtr<HOM_UIEventDevice> > queuedEvents() = 0;
    virtual hboost::any value() = 0;

    // returns uiEventReason
    virtual HOM_EnumValue& reason() = 0;

    // returns uiEventValueType
    virtual HOM_EnumValue& valueType() = 0;
};

#endif
