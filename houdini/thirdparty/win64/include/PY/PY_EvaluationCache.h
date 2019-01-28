/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	An instance of this class is stored with each instance of
 *	PY_CompiledCode.  Each time the code is evaluated, the cache is
 *	made active, and subsequent evaluations can use information that
 *	was cached during previous evaluations.
 *
 *	The cache has nothing to do with python, and is used to cache
 *	the results of expensive computations done in Houdini's code.
 *
 *	This class only exists at the PY level so PY_CompiledCode can store
 *	an instance of it and so it can deleted it.  HOMF_Module will set
 *	a callback to create new instances of HOMF_EvaluationCache objects.
 *	These objects store things like OP_ExprFindOp caches that speed up
 *	channel references, etc.
 */

#ifndef __PY_EvaluationCache_h__
#define __PY_EvaluationCache_h__

#include "PY_API.h"
#include <UT/UT_ValArray.h>

class PY_API PY_EvaluationCache
{
public:
    virtual ~PY_EvaluationCache()
    {}

    // Count all memory owned by the PY_EvaluationCache.  Remember to
    // include sizeof(*this) if inclusive, else don't.
    virtual int64 getMemoryUsage(bool inclusive) const
    { return inclusive ? sizeof(*this) : 0; }

    // Use this static function to set a callback that creates instances of a
    // subclass of PY_EvaluationCache.  This callback is invoked from
    // PY_CompiledCode when evaluating Python code.  This static function is
    // called when the hou Python module is imported and the HOMF_Module
    // instance is created.
    static void setConstructionCallback(
	PY_EvaluationCache *(*callback)());

    static bool isConstructionCallbackSet();

    // This static function uses the creation callback to create a new instance
    // of the evaluation cache subclass.  Don't call this function unless
    // the callback has been set.
    static PY_EvaluationCache *createNew();

    // Each thread has a stack of evaluation caches, and this static function
    // returns that stack.  Note that you must use the evaluation stack from
    // the same thread that you call evaluationCacheStack() from.
    static UT_ValArray<PY_EvaluationCache *> &getThreadSpecificStack();

private:
    static PY_EvaluationCache *(*theConstructionCallback)();
};

#endif
