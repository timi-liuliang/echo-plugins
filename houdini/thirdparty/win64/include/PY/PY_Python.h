/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	This file contains functions to run python code.
 */

#ifndef __PY_Python_h__
#define __PY_Python_h__

#include "PY_API.h"
#include "PY_Result.h"
#include <UT/UT_String.h>

class PY_EvaluationContext;

/// Expressions cannot contain multiple statements (defs, classes, print
/// statements, etc.) and they cannot contain multiple expressions.  The
/// *InNewContext version creates a new PY_EvaluationContext, run the Python
/// code inside it, and destroys the context afterward.
PY_API PY_Result PYrunPythonExpression(
    const char *python_code, PY_Result::Type desired_result_type,
    PY_EvaluationContext *context = NULL);
PY_API PY_Result PYrunPythonExpressionInNewContext(
    const char *python_code, PY_Result::Type desired_result_type);

/// Evaluate a Python expression that should not generate any exceptions.  If an
/// exception is raised the traceback will be displayed to the user, prefixed by
/// the specified heading.
PY_API PY_Result PYrunPythonExpressionAndExpectNoErrors(
    const char *python_code,
    PY_Result::Type desired_result_type,
    const char *heading = NULL,
    PY_EvaluationContext *context = NULL);

/// Evaluate a Python expression that should not generate any exceptions and
/// should evaluate to an exact type.  If an exception is raised a traceback
/// is displayed with the specified heading.  If the wrong type is returned,
/// the specified error message is displayed with the heading.  Returns true
/// on success and stores the result in "result".
PY_API bool PYrunPythonExpressionOfExactType(
    const char *python_code,
    PY_Result::Type desired_result_type,
    PY_Result &result, 
    const char *error_heading,
    const char *error_for_wrong_type,
    PY_EvaluationContext *context = NULL);

/// The result type from running statements in the current thread can be none
/// (indicating normal completion), error, or exit.  The *InNewContext version
/// creates a new PY_EvaluationContext, run the Python code inside it, and
/// destroys the context afterward.
PY_API PY_Result PYrunPythonStatements(
    const char *python_code,
    PY_EvaluationContext *context = NULL);
PY_API PY_Result PYrunPythonStatementsInNewContext(
    const char *python_code);

/// This version of PYrunPythonStatements will set Python's sys.argv to the
/// arguments you supply.  argv[0] will be the name of the Python file.
PY_API PY_Result PYrunPythonStatements(
    const char *python_code,
    int argc, char *argv[],
    PY_EvaluationContext *context = NULL,
    bool update_path=true);

/// Run Python code that should not generate any exceptions.  If an exception
/// is raised the traceback will be displayed to the user, prefixed by the
/// specified heading.
PY_API bool PYrunPythonStatementsAndExpectNoErrors(
    const char *python_code,
    const char *heading = NULL,
    PY_EvaluationContext *context = NULL);
PY_API bool PYrunPythonStatementsInNewContextAndExpectNoErrors(
    const char *python_code,
    const char *heading = NULL);

/// The argv version of PYrunPythonStatementsFromFile will set Python's sys.argv
/// to the arguments you supply.  argv[0] will be the name of the Python file.
PY_API PY_Result PYrunPythonStatementsFromFile(
    const char *filename,
    PY_EvaluationContext *context = NULL);
PY_API PY_Result PYrunPythonStatementsFromFile(
    int argc, char *argv[],
    PY_EvaluationContext *context = NULL,
    bool update_path=true);

/// A non-empty string is returned if there are parse errors, a new thread
/// could not be started, or the file could not be loaded.
PY_API UT_String PYrunPythonStatementsInNewThread(
    const char *python_code);
PY_API UT_String PYrunPythonStatementsFromFileInNewThread(
    const char *file_name);
PY_API UT_String PYrunPythonStatementsFromFileInNewThread(
    int argc, char *argv[], bool update_path=true);

/// Return the python result containing a stack trace and exception information.
/// If no exception occurred, the result type is none.
PY_API PY_Result PYextractPythonException();

/// Given a python object and a desired type, try to convert it to that type.
/// python_object should be a PyObject*, but it's a void* to avoid having to
/// include Python.h from here.
///
/// There are two versions: a performance-sensitive version that takes the
/// return PY_Result object by reference, and a convenience version one that
/// returns it by value.
PY_API PY_Result PYextractResultFromPythonObject(
    void *opaque_python_object, PY_Result::Type desired_result_type);
PY_API void PYextractResultFromPythonObject(
    void *opaque_python_object, PY_Result::Type desired_result_type,
    PY_Result &result);

/// Given an (opaque) callable Python object, call it with no arguments and
/// return its Python object result.  If an exception occurred, it remains set
/// in the Python interpreter and this function returns null.  This function
/// behaves much like calling PyObject_CallObject with a null argument
/// parameter, but the difference is that if calling the code generates a crash,
/// the signal is caught, an exception is set, and this function returns null.
PY_API void *PYcallObjectAndCatchCrashes(void *callable);

/// We don't want to have to prefix things with "hou." in expressions.
/// So, we create a separate evaluation context for expressions where
/// "from hou import *" has been run.
PY_API PY_EvaluationContext &PYgetPythonExpressionEvaluationContext();

/// Given a string containing the contents of a module and the module's name,
/// compile the string and import that as a module.  If no exception occurred,
/// the result type is none.
PY_API PY_Result PYimportModuleFromString(
    const char *module_name, const char *module_contents);

/// Return a PyCodeObject * corresponding to the frame before the current eval
/// frame.
PY_API void *PYgetCodeObjectForPrevFrame();

/// Return a PyCodeObject * corresponding to the specified PyObject
PY_API void *PYgetCodeObjectForFunction(void *opaque_python_object);

/// Set a callback that will be invoked by PYrunPython*AndExpectNoErrors
/// whenever an error occurs.  If this callback is not set, the errors will
/// be printed to the console.  This callback provides an opportunity to
/// pop up a dialog to display the errors.
PY_API void PYsetPythonTracebackDisplayCallback(
    void (*callback)(const char *heading, const char *error_message));

/// Display a Python traceback with a given heading.  If the traceback display
/// callback was set, it will be used to display the traceback.
PY_API void PYdisplayPythonTraceback(
    const char *heading, const char *traceback_message);

/// This function controls whether running Python code automatically sets the
/// HOM_Module implementation (by importing the hou Python module).  Call
/// PYsetAutoInitializeFromPython to false if you don't hou to be imported
/// automatically; otherwise, it will.  You must call this static method
/// before running any Python code.  Once any Python code is run, this
/// setting has no effect.
PY_API void PYsetAutoInitializeFromPython(bool auto_initialize);

/// This function is used by PY_InterpreterAutoLock to determine if
/// it should automatically import the hou module
PY_API bool PYautoInitializeFromPython();


/// Returns the absolute path to Houdini's Python modules. 
PY_API const char *PYgetHoudiniModulePath();

/// Bind and return a PY_Object 'function' from a specific module. The caller is responsible to
/// increment the Python object ref count.
PY_API PY_Result PYbindFunctionFromModule(const char* module_name, const char *func_name);

/// Log result on std error, doesn't print anything if result is not an ERR type. 
/// The error message is based on the format returned by PYgetErrorFormat()
/// params:
/// result: PY_Result object
/// title: title to use for building the error message
///
PY_API void PYlogError(PY_Result const & result, const char* title);

/// Return a formatted error message filled with a title and the detailed error 
/// message set in the PY_Result object. Returns an empty string if the PY_Result 
/// object is not an ERR type.
/// params:
/// result: PY_Result object
/// title: title to use for building the error message
/// format: error message format. Must be of the form "%s %s". See PYgetErrorFormat 
/// for an example.
///
PY_API UT_StringHolder PYformatError(
    PY_Result const &	result, 
    const char*		title, 
    const char*		format );

/// Returns the default error format string "%s\n%s\n" usable with PYformatError 
/// and PYprocessError.
PY_API const char* PYgetErrorFormat();

/// Process a python error object with a user function. PYprocessError creates an 
/// error message and passes it to process_func for processing. The process_func 
/// function would typically log the error on the console, in a file or throw an 
/// exception. 
///
/// params:
/// result: PY_Result object set to ERR. Function does nothing if not set to ERR.
/// title: title of the error message
/// format: error message format. Must be of the form "%s %s". See 
/// PYgetErrorFormat for an example.
/// process_func: user function to process the error, takes a pre-formatted error 
/// string of type const char*.

template<typename FUNC>
void PYprocessError(
    PY_Result const &	result, 
    const char*		title,
    const char*		format,
    FUNC const&		process_func )
{
    if (result.myResultType != PY_Result::ERR)
    {
	// just return if the result is not an error
	return;
    }

    auto && errmsg = PYformatError(result, title, format);
    process_func(errmsg.c_str());
}
#endif
