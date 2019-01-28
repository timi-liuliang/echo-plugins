/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	Class used to disable undos for a Python code block
 */

#ifndef __HOM_UndosDisabler_h__
#define __HOM_UndosDisabler_h__

#include "HOM_API.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"


SWIGOUT(%rename(UndosDisabler) HOM_UndosDisabler;)


class HOM_API HOM_UndosDisabler
{
public:

    HOM_UndosDisabler()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_UndosDisabler(const HOM_UndosDisabler &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_UndosDisabler()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    virtual std::string __repr__() = 0;

    virtual HOM_UndosDisabler *__enter__() = 0;

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
