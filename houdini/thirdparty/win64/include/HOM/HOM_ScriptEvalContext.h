/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *
 */

#ifndef __HOM_ScriptEvalContext_h__
#define __HOM_ScriptEvalContext_h__

#include "HOM_API.h"
#include "HOM_Module.h"

#include <string>


SWIGOUT(%rename(ScriptEvalContext) HOM_ScriptEvalContext;)
SWIGOUT(%feature("notabstract") HOM_ScriptEvalContext;)


class HOM_API HOM_ScriptEvalContext
{
public:
#ifdef SWIG
%extend {
    HOM_ScriptEvalContext(HOM_Node &node)
	{ return HOM().newScriptEvalContext(node); }
    HOM_ScriptEvalContext(HOM_Parm &parm)
	{ return HOM().newScriptEvalContext(parm); }
};
#else
    HOM_ScriptEvalContext()
	{ HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_ScriptEvalContext()
	{ HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject node;)
    virtual HOM_Node *node() = 0;

    SWIGOUT(%newobject parm;)
    virtual HOM_Parm *parm() = 0;

    virtual HOM_ScriptEvalContext *__enter__() = 0;

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
