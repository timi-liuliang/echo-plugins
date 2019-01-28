/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_EditableDopGeometryGuard_h__
#define __HOM_EditableDopGeometryGuard_h__

#include "HOM_API.h"
#include "HOM_Module.h"

class HOM_Geometry;

SWIGOUT(%rename(EditableDopGeometryGuard) HOM_EditableDopGeometryGuard;)

class HOM_API HOM_EditableDopGeometryGuard
{
public:
    HOM_EditableDopGeometryGuard()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_EditableDopGeometryGuard(const HOM_EditableDopGeometryGuard &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_EditableDopGeometryGuard()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject __enter__;)
    virtual HOM_Geometry *__enter__() = 0;

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
