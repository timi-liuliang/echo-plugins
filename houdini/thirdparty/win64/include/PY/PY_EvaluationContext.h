/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	This class represents an evaluation context in which pieces of compiled
 *	python code can run.  Each evaluation context contains a globals
 *	dict, so it effectively acts as a namespace.
 */

#ifndef __PY_EvaluationContext_h__
#define __PY_EvaluationContext_h__

#include "PY_API.h"
#include <UT/UT_String.h>

class PY_API PY_EvaluationContext
{
public:
    explicit PY_EvaluationContext(const char* default_module="hou");
    ~PY_EvaluationContext();

    void clear();

    void *getGlobalsDict()
    { return myOpaqueGlobalsDict; }

private:
    PY_EvaluationContext(const PY_EvaluationContext &); // unimplemented

    void initializeContents();

    friend class PY_CompiledCode;

    void *myOpaqueGlobalsDict;
    UT_String myDefaultModule;
};

#endif
