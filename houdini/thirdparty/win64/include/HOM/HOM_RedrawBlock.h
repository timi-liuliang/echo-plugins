/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	Class used to turn any Python code block into an interruptable
 *	operation.
 */

#ifndef __HOM_RedrawBlock_h__
#define __HOM_RedrawBlock_h__

#include "HOM_API.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"

SWIGOUT(%feature("notabstract") HOM_RedrawBlock;)
SWIGOUT(%rename(RedrawBlock) HOM_RedrawBlock;)

class HOM_API HOM_RedrawBlock
{
public:

#ifdef SWIG
%extend
{
    %kwargs HOM_RedrawBlock;
    HOM_RedrawBlock()
    {
	return HOM().newRedrawBlock();
    }
}
#else
    HOM_RedrawBlock()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_RedrawBlock(const HOM_RedrawBlock &operation)
    { HOM_CONSTRUCT_OBJECT(this) }
#endif
    virtual ~HOM_RedrawBlock()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    virtual std::string __repr__() = 0;


    virtual HOM_RedrawBlock *__enter__() = 0;

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
