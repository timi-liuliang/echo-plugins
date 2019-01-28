/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	This class represents a compiled piece of python code that
 *	can be evaluated.
 */

#ifndef __PY_CompiledCode_h__
#define __PY_CompiledCode_h__

#include "PY_API.h"
#include "PY_Result.h"
#include "PY_InterpreterAutoLock.h"
#include "PY_EvaluationContext.h"
#include <UT/UT_String.h>
#include <iosfwd>
class PY_EvaluationCache;
class UT_IStream;

#define ALLOW_DIFFERENT_CONSTRUCTOR_AND_EVAL_THREADS

class PY_API PY_CompiledCode
{
public:
    enum CodeType { STATEMENTS, EXPRESSION };

    // Python code can be compiled as either an expression or a (list of)
    // statements.  Statements do not return anything -- even if the last
    // statement is an expression, evaluating code as statements will never
    // return a value.  See the compile() python builtin for more information;
    // expressions correspond to 'eval' and statements correspond to 'exec'.
    //
    // The python code is compiled when this class is instanced.  Check
    // for syntax errors with hasSyntaxErrors() and syntaxErrors().  You
    // can always access the code regardless of whether or not there were
    // syntax errors.
    //
    // as_file is the file name used when reporting syntax errors.
    //
    // If allow_function_bodies is true, code_type is EXPRESSION, and the
    // Python code contains multiple lines, it will be interpreted as the
    // body of a Python expression.
    //
    // NOTE: It's important to call the evaluate*() methods from the
    //       same thread from which this object is constructed.
    PY_CompiledCode(
	const char *python_code,
	CodeType code_type,
	const char *as_file = NULL,
	bool allow_function_bodies = false);

    // This constructor reads in compiled python code from a stream.
    PY_CompiledCode(
	UT_IStream &is,
	CodeType code_type);

    ~PY_CompiledCode();

    // Normally this is called from PY_CompiledCode's constructor.
    // If you create this class in one thread but then evaluate it from a
    // different thread, then you need to call this from the other thread prior
    // to evaluation in order to ensure that the correct evaluation cache stack
    // is used.
    void setEvaluationCacheStackForCurrentThread();

    // This can't get the size of any compiled code object from
    // Python, but can still count this, its strings, and its
    // evaluation cache.
    int64 getMemoryUsage(bool inclusive) const;

    // The syntax errors will be empty if there are none.
    bool hasSyntaxErrors() const;
    const UT_String &syntaxErrors() const;

    // Get the code that was compiled.
    const UT_String &code() const;

    // Use marchalling to convert the compiled code object to a string.
    // We use a UT_WorkBuffer because the returned string may have null
    // characters in it.
    void compiledCodeAsString(UT_WorkBuffer &result) const;

    // This method saves the compiled form of our code.
    bool saveCompiledCode(std::ostream &os);

    // Evaluate the compiled python code.  The desired result type really
    // only has an effect if the compiled code is an expression.
    //
    // Evaluating code that had syntax errors will set the result type to
    // PY_ResultType::ERR, with the string containing the syntax errors.
    void evaluate(
	PY_Result::Type desired_result_type, PY_Result &result) const;

    // evaluateInContext() is the same as evaluate() except it uses the
    // local and global dictionaries from the context passed to it.  This
    // way, any code run won't set variables in the __main__ module's global
    // dictionary.  It is inlined for speed.
    void evaluateInContext(
	PY_Result::Type desired_result_type,
	PY_EvaluationContext &context,
	PY_Result &result) const
    {
	PY_InterpreterAutoLock auto_lock;
	evaluateUsingDicts(desired_result_type,
	    context.myOpaqueGlobalsDict, context.myOpaqueGlobalsDict, result);
    }

    // Similar to the above, but an additional local variable dictionary can
    // be passed in. This is used along with the global dictionary from the
    // context.
    void evaluateInContext(
	PY_Result::Type desired_result_type,
	PY_EvaluationContext &context,
	void* opaque_locals_dict,
	PY_Result &result) const
    {
	PY_InterpreterAutoLock auto_lock;
	evaluateUsingDicts(desired_result_type,
	    context.myOpaqueGlobalsDict, opaque_locals_dict, result);
    }

    // These convenience versions of evaluate() and evaluateInContext() return
    // the PY_Result by value, instead of passing it in by reference.
    // These versions can be used by code that isn't performance-critical.
    PY_Result evaluate(PY_Result::Type desired_result_type) const
    {
	PY_Result result;
	evaluate(desired_result_type, result);
	return result;
    }

    PY_Result evaluateInContext(
	PY_Result::Type desired_result_type,
	PY_EvaluationContext &context) const
    {
	PY_Result result;
	evaluateInContext(desired_result_type, context, result);
	return result;
    }

    inline bool isPureCompiled()
    {
        return !myCode.isstring() && !mySyntaxErrors.isstring() && myOpaqueCodeObject;
    }

private:
    PY_CompiledCode(const PY_CompiledCode &); // unimplemented

    void initializeThreadInfoAndEvalCache();

    PY_Result evaluateUsingDicts(
	PY_Result::Type desired_result_type,
	void *opaque_globals_dict,
	void *opaque_locals_dict) const;
    void evaluateUsingDicts(
	PY_Result::Type desired_result_type,
	void *opaque_globals_dict,
	void *opaque_locals_dict,
	PY_Result &result) const;

    int startInterruptableEvaluation() const;
    void endInterruptableEvaluation(int id) const;

    void *myOpaqueCodeObject;
    CodeType myCodeType;
    UT_String myCode;
    UT_String mySyntaxErrors;

#ifndef ALLOW_DIFFERENT_CONSTRUCTOR_AND_EVAL_THREADS
    UT_ValArray<PY_EvaluationCache *> *myEvaluationCacheStack;
#endif
    mutable PY_EvaluationCache *myEvaluationCache;
};

#endif
