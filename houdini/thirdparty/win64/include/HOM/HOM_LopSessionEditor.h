/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	Class used to create a block for editing the session layer on a
 *	LOP Network.
 */

#ifndef __HOM_LopSessionEdit_h__
#define __HOM_LopSessionEdit_h__

#include "HOM_API.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"

SWIGOUT(%rename(LopSessionEditor) HOM_LopSessionEditor;)

class HOM_API HOM_LopSessionEditor
{
public:
    HOM_LopSessionEditor()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_LopSessionEditor(const HOM_LopSessionEditor &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_LopSessionEditor()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    virtual std::string __repr__() = 0;

    virtual HOM_LopSessionEditor *__enter__() = 0;

    SWIGOUT(%ignore opaqueLayer;)
    virtual void *opaqueLayer() = 0;
    SWIGOUT(%ignore opaqueStage;)
    virtual void *opaqueStage() = 0;

    SWIGOUT(%ignore opaqueExit;)
    virtual void opaqueExit() = 0;

#ifdef SWIG
%extend
{
    InterpreterObject layer()
    { return HOMincRef((InterpreterObject)self->opaqueLayer()); }
    InterpreterObject stage()
    { return HOMincRef((InterpreterObject)self->opaqueStage()); }

    // We can ignore the type, value, and traceback Python objects.
    SWIGOUT(virtual void __exit__(
	    InterpreterObject type, InterpreterObject value, 
	    InterpreterObject traceback)
    { self->opaqueExit(); };)
}
#endif

};

#endif
