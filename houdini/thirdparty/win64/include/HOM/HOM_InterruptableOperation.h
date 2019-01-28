/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	Class used to turn any Python code block into an interruptable
 *	operation.
 */

#ifndef __HOM_InterruptableOperation_h__
#define __HOM_InterruptableOperation_h__

#include "HOM_API.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"

SWIGOUT(%feature("notabstract") HOM_InterruptableOperation;)
SWIGOUT(%rename(InterruptableOperation) HOM_InterruptableOperation;)

class HOM_API HOM_InterruptableOperation
{
public:

#ifdef SWIG
%extend
{
    %kwargs HOM_InterruptableOperation;
    HOM_InterruptableOperation(const char *operation_name,
	const char *long_operation_name=NULL, bool open_interrupt_dialog=false)
    {
	return HOM().newInterruptableOperation(operation_name,
	    long_operation_name, open_interrupt_dialog);
    }
}
#else
    HOM_InterruptableOperation()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_InterruptableOperation(const HOM_InterruptableOperation &operation)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif
    virtual ~HOM_InterruptableOperation()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    virtual std::string __repr__() = 0;

    SWIGOUT(%kwargs updateLongProgress;)
    virtual void updateLongProgress(double percentage=-1.0,
	    const char *long_op_status=NULL) = 0;

    SWIGOUT(%kwargs updateProgress;)
    virtual void updateProgress(double percentage=-1.0) = 0;

    virtual HOM_InterruptableOperation *__enter__() = 0;

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
