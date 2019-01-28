/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	Instances of this class hold a Python callback (any callable Python
 *	object).  You can invoke this callback with the call() method, passing
 *	it optional arguments.  When this object is destroyed, the reference
 *	count on the Python callback will be decremented.
 */

#ifndef __PY_Callback_h__
#define __PY_Callback_h__

#include "PY_API.h"
#include "PY_CPythonAPI.h"
#include "PY_InterpreterAutoLock.h"
#include "PY_OpaqueObject.h"
#include "PY_Result.h"

class PY_API PY_Callback : public PY_OpaqueObject
{
public:
    // Construct instances of this class with an opaque PY_PyObject
    // containing a callable Python object.
    PY_Callback(void *python_callable_object)
    : PY_OpaqueObject(python_callable_object)
    {}

    PY_Callback(const PY_Callback &callback)
    : PY_OpaqueObject(callback)
    {}

    PY_Callback &operator=(const PY_Callback &callback)
    { return (PY_Callback &)PY_OpaqueObject::operator=(callback); }

    // Given two strings containing Python expressions (one that evaluates to
    // a tuple of arguments and another that evaluates to a dictionary of
    // keyword arguments), call the callback.  Either or both of the argument
    // tuple and keyword argument dictionary may be null.  The result is
    // returned in the PY_Result object.
    void call(PY_Result &result, const char *args_expression=NULL,
	const char *kwargs_expression=NULL) const;

    // This convenience version of call() returns the PY_Result by value,
    // instead of passing it in by reference.  This versions can be used by
    // code that isn't performance-critical.
    PY_Result call(const char *args_expression=NULL,
	const char *kwargs_expression=NULL) const
    {
	PY_Result result;
	call(result, args_expression, kwargs_expression);
	return result;
    }

    // For callbacks returning values
    PY_Result call( 
	const char *args_expression,
	const char *kwargs_expression,
	PY_Result::Type desired_result_type ) const;

    // Similar to the above methods but take a lambda function to add elements to
    // the evaluated expressions before firing the callback. Also takes a module name
    // to evaluate the expressions, the module is optional and can be set to nullptr if
    // not required.
    // 
    template<typename FUNC>
    PY_Result call(
	const char *args_expression,
	const char *kwargs_expression,
	const char* module_name,
	PY_Result::Type desired_result_type,
	PY_PyObject*& out_args,
	PY_PyObject*& out_kwargs,
	FUNC const& fill_evaluated_expressions ) const
    {
        PY_InterpreterAutoLock auto_lock;

	PY_Result result = evaluate(
	    args_expression, kwargs_expression, module_name, out_args, out_kwargs);
	if (result.myResultType == PY_Result::ERR)
	{
	    return result;
	}

	fill_evaluated_expressions(out_args, out_kwargs);

	return call(out_args, out_kwargs, desired_result_type);
    }

    private:
    // Performs a call with pre-evaluated args and kwargs
    PY_Result call( 
	PY_PyObject * args, 
	PY_PyObject * kwargs,
	PY_Result::Type desired_result_type=PY_Result::NONE ) const;

    // Evaluates positional and keyworded arguments. The returned arguments can 
    // be expanded with other values before performing a call.
    PY_Result evaluate(
	const char *args_expression,
	const char *kwargs_expression,
	const char *module_name,
	PY_PyObject*& out_args,
	PY_PyObject*& out_kwargs ) const;

    PY_PyObject* evaluateExpression(const char *expression, PY_Result &result) const;
    // This version use an input module to evaluate the expression
    PY_PyObject* evaluateExpression(
	const char *expression, const char* module_name, PY_Result &result) const;
};

#endif
