/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	Class used to create an undo block
 */

#ifndef __HOM_UndosGroup_h__
#define __HOM_UndosGroup_h__

#include "HOM_API.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"


SWIGOUT(%rename(UndosGroup) HOM_UndosGroup;)


class HOM_API HOM_UndosGroup
{
public:

    HOM_UndosGroup()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_UndosGroup(const HOM_UndosGroup &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_UndosGroup()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    virtual std::string __repr__() = 0;

    virtual std::string label() = 0;

    virtual HOM_UndosGroup *__enter__() = 0;

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
