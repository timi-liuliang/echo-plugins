/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_PerfMonEvent_h__
#define __HOM_PerfMonEvent_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include <string>

SWIGOUT(%rename(PerfMonEvent) HOM_PerfMonEvent;)

class HOM_API HOM_PerfMonEvent
{
public:
    HOM_PerfMonEvent()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_PerfMonEvent(const HOM_PerfMonEvent &event)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_PerfMonEvent()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    // These methods are deprecated.
    virtual int id() = 0;
    virtual std::string name() = 0;

    virtual std::string object() = 0;
    virtual bool isAutoNestEnabled() = 0;

    virtual std::vector<double> stop() = 0;

    // This method is deprecated.  Use isRunning() instead.
    virtual bool isTiming() = 0;
    virtual bool isRunning() = 0;

    virtual double startTime() = 0;

    // Enter and exit methods so that the class
    // can be used in `with` statements.
    virtual HOM_PerfMonEvent *__enter__() = 0;
    SWIGOUT(%ignore opaqueExit;)
    virtual void opaqueExit() = 0;
#ifdef SWIG
%extend
{
    // We can ignore the type, value, and traceback Python objects.
    SWIGOUT(virtual void __exit__(
	    InterpreterObject type, InterpreterObject value,
	    InterpreterObject traceback)
    { self->opaqueExit(); };)
}
#endif

};

#endif
