/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Dialog_h__
#define __HOM_Dialog_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"

SWIGOUT(%rename(Dialog) HOM_Dialog;)

class HOM_API HOM_Dialog
{
public:
    HOM_Dialog()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Dialog(const HOM_Dialog &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Dialog()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual void destroy() = 0;

    virtual void setValue(const char *name, const char *value) = 0;
    virtual void setValue(const char *name, int value) = 0;
    virtual void setValue(const char *name, double value) = 0;

    SWIGOUT(%ignore boostAnyValue;)
    virtual hboost::any boostAnyValue(const char *name) = 0;

    virtual void enableValue(const char *name, bool onoff) = 0;

    SWIGOUT(%ignore addOpaqueCallback;)
    virtual void addOpaqueCallback(const char *name, void *callback) = 0;

    SWIGOUT(%ignore removeOpaqueCallback;)
    virtual void removeOpaqueCallback(const char *name, void *callback) = 0;

    SWIGOUT(%ignore opaqueCallbacks;)
    virtual std::vector<void *> opaqueCallbacks(const char *name) = 0;

    virtual void waitForValueToChangeTo(const char *name, const char *new_value) = 0;

    virtual void waitForValueToChangeTo(const char *name, int new_value) = 0;

    virtual std::vector<std::string> menuItems(const char *name) = 0;
    virtual void setMenuItems(const char *name,
                              const std::vector<std::string> &items) = 0;

#ifdef SWIG
%extend
{
    InterpreterObject value(const char *name)
    { return HOMboostAnyToInterpreterObject(self->boostAnyValue(name)); }

    virtual void addCallback(const char *name, InterpreterObject callback)
    { self->addOpaqueCallback(name, callback); }

    virtual void removeCallback(const char *name, InterpreterObject callback)
    { self->removeOpaqueCallback(name, callback); }


    // Note that we return a vector of borrowed references because swig
    // will increment the reference counts.
    std::vector<InterpreterObject> callbacks(const char *name)
    {
	std::vector<void *> opaque_callbacks(self->opaqueCallbacks(name));

	std::vector<InterpreterObject> callbacks;
	for (int i=0; i< opaque_callbacks.size(); ++i)
	    callbacks.push_back((InterpreterObject)opaque_callbacks[i]);
	return callbacks;
    }
}
#endif
};

#endif
