/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *      This file provides a set of functions, types, and macros that wrap
 *      Python's C API.  This way, we can choose at runtime which version
 *      of the Python library we load, and we do not add a compile/link-time
 *      dependency on a particular version of Python.  Use this wrapper API
 *      as follows:
 *
 *      - Instead of including <python/Python.h>, instead include
 *        PY_CPythonAPI.h.  Still include this file first, just like you would
 *        with Python.h.
 *        (It's even in the Python docs that you need to include Python.h
 *        before anything else.  Otherwise you get strange gcc warnings.)
 *
 *      - Add a "PY_" prefix to all Python functions, types, and macros.  For
 *        example,
 *            PyObject *dict = PyDict_New();
 *        becomes
 *            PY_PyObject *dict = PY_PyDict_New();
 *
 *      - Access to Python constants, such as PyExc_SystemExit, become function
 *        calls.  For example,
 *            PyErr_SetObject(PyExc_IndexError, error_message);
 *        becomes
 *            PY_PyErr_SetObject(PY_PyExc_IndexError(), error_message);
 *
 *      - When directly accessing members of Python structures, change the
 *        data member accesses into method calls.  For example, when accessing
 *        the ob_type member of a [PY_]PyObject,
 *            op->ob_type
 *        becomes
 *            ob->ob_type()
 *
 *      - Constructing instances of Python objects like PySequenceMethods,
 *        or creating subclasses of PyObject, is different.
 */

#ifndef __PY_CPythonAPI_h__
#define __PY_CPythonAPI_h__

#include "PY_API.h"
#include <UT/UT_WorkBuffer.h>
#include <SYS/SYS_Types.h>
#include <hboost/preprocessor/seq/for_each.hpp>
#include <hboost/preprocessor/seq/for_each_i.hpp>
#include <hboost/preprocessor/seq/seq.hpp>
#include <hboost/preprocessor/punctuation/comma_if.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/stringize.hpp>
#include <string.h>
#include <stdarg.h>

#ifdef WIN32
// Disable MSVC warnings about not enough actual parameters for macros.
// Otherwise, wrapped functions that take no parameters will generate
// warnings.
#pragma warning(disable:4003)
#endif

using PY_AtExitFunc = void (*)();

// These are the functions we'll automatically wrap, of the form
// (function_name)(return_type)(parm_types).  parm_types is itself a sequence
// of (T1)(T2)(T3).  While the wrapped functions don't have a "PY_" prefix,
// any non-native data types do.
#define PY_WRAPPED_FUNCTIONS \
    ((Py_AtExit)(int)((PY_AtExitFunc))) \
    ((Py_InitializeEx)(void)((int))) \
    ((Py_IsInitialized)(int)()) \
    ((Py_SetPythonHome)(void)((char*))) \
    ((Py_SetProgramName)(void)((char*))) \
    ((PyGILState_Ensure)(PY_PyGILState_STATE)()) \
    ((PyGILState_Release)(void)((PY_PyGILState_STATE))) \
    ((PyGILState_GetThisThreadState)(PY_PyThreadState *)()) \
    ((PyRun_SimpleStringFlags)(int)((const char *)(PY_PyCompilerFlags *))) \
    ((Py_CompileStringFlags)(PY_PyObject *) \
	((const char *)(const char *)(int)(PY_PyCompilerFlags *))) \
    ((PyEval_InitThreads)(void)()) \
    ((PyEval_SaveThread)(PY_PyThreadState *)()) \
    ((PyEval_RestoreThread)(void)((PY_PyThreadState *))) \
    ((PyEval_GetFrame)(PY_PyFrameObject *)()) \
    ((PyEval_EvalCode)(PY_PyObject *) \
	((PY_PyCodeObject *)(PY_PyObject *)(PY_PyObject *))) \
    ((PyThread_get_thread_ident)(long)()) \
    ((PyThreadState_SetAsyncExc)(int)((long)(PY_PyObject *))) \
    ((PyMem_Malloc)(void *)((size_t))) \
    ((Py_Main)(int)((int)(char **))) \
    ((Py_IncRef)(void)((PY_PyObject *))) \
    ((Py_DecRef)(void)((PY_PyObject *))) \
    ((PyObject_GetAttr)(PY_PyObject *)((PY_PyObject *)(PY_PyObject *))) \
    ((PyObject_GetAttrString)(PY_PyObject *)((PY_PyObject *)(const char *))) \
    ((PyObject_SetAttr)(int)((PY_PyObject *)(PY_PyObject *)(PY_PyObject *))) \
    ((PyObject_Repr)(PY_PyObject *)((PY_PyObject*))) \
    ((PyObject_Str)(PY_PyObject *)((PY_PyObject *))) \
    ((PyObject_IsTrue)(int)((PY_PyObject *))) \
    ((PyObject_CallObject)(PY_PyObject *)((PY_PyObject *)(PY_PyObject *))) \
    ((PyObject_Call)(PY_PyObject *) \
	((PY_PyObject *)(PY_PyObject *)(PY_PyObject *))) \
    ((PyObject_GetIter)(PY_PyObject *)((PY_PyObject *))) \
    ((PyObject_RichCompareBool)(int)((PY_PyObject *)(PY_PyObject *)(int))) \
    ((PyObject_Print)(int)((PY_PyObject *)(FILE *)(int))) \
    ((Py_VaBuildValue)(PY_PyObject *)((const char *)(va_list))) \
    ((PyArg_VaParse)(int)((PY_PyObject *)(const char *)(va_list))) \
    ((PyType_IsSubtype)(int)((PY_PyTypeObject *)(PY_PyTypeObject *))) \
    ((PyType_Ready)(int)((PY_PyTypeObject *))) \
    ((PyErr_Occurred)(PY_PyObject *)()) \
    ((PyErr_ExceptionMatches)(int)((PY_PyObject *))) \
    ((PyErr_Fetch)(void)((PY_PyObject **)(PY_PyObject **)(PY_PyObject **))) \
    ((PyErr_Clear)(void)()) \
    ((PyErr_SetString)(void)((PY_PyObject *)(const char *))) \
    ((PyErr_SetNone)(void)((PY_PyObject *))) \
    ((PyErr_SetObject)(void)((PY_PyObject *)(PY_PyObject *))) \
    ((PyErr_CheckSignals)(int)()) \
    ((PyCObject_GetDesc)(void *)((PY_PyObject *))) \
    ((PyCObject_AsVoidPtr)(void *)((PY_PyObject *))) \
    ((PyLong_FromLongLong)(PY_PyObject *)((int64))) \
    ((PyLong_FromLong)(PY_PyObject *)((long))) \
    ((PyLong_AsLong)(long)((PY_PyObject *))) \
    ((PyLong_AsVoidPtr)(void*)((PY_PyObject *))) \
    ((PyFloat_AsDouble)(double)((PY_PyObject *))) \
    ((PyFloat_FromDouble)(PY_PyObject *)((double))) \
    ((PySequence_Check)(int)((PY_PyObject *))) \
    ((PySequence_Size)(PY_Py_ssize_t)((PY_PyObject *))) \
    ((PySequence_GetItem)(PY_PyObject *)((PY_PyObject *)(PY_Py_ssize_t))) \
    ((PyList_New)(PY_PyObject *)((PY_Py_ssize_t))) \
    ((PyList_Size)(PY_Py_ssize_t)((PY_PyObject *))) \
    ((PyList_GetItem)(PY_PyObject *)((PY_PyObject *)(PY_Py_ssize_t))) \
    ((PyList_SetItem)(int)((PY_PyObject *)(PY_Py_ssize_t)(PY_PyObject *))) \
    ((PyList_Append)(int)((PY_PyObject *)(PY_PyObject *))) \
    ((PyTuple_New)(PY_PyObject *)((PY_Py_ssize_t))) \
    ((PyTuple_SetItem)(int)((PY_PyObject *)(PY_Py_ssize_t)(PY_PyObject *))) \
    ((PyTuple_Size)(PY_Py_ssize_t)((PY_PyObject *))) \
    ((PyDict_Copy)(PY_PyObject *)((PY_PyObject *))) \
    ((PyDict_New)(PY_PyObject *)()) \
    ((PyDict_Clear)(void)((PY_PyObject *))) \
    ((PyDict_Next)(int) \
	((PY_PyObject *)(PY_Py_ssize_t *)(PY_PyObject **)(PY_PyObject **))) \
    ((PyDict_GetItemString)(PY_PyObject *)((PY_PyObject *)(const char *))) \
    ((PyDict_SetItemString)(int) \
	((PY_PyObject *)(const char *)(PY_PyObject *))) \
    ((PyDict_SetItem)(int)((PY_PyObject *)(PY_PyObject *)(PY_PyObject *))) \
    ((PyDict_DelItemString)(PY_PyObject *)((PY_PyObject *)(const char *))) \
    ((PyIter_Next)(PY_PyObject *)((PY_PyObject *))) \
    ((PyFunction_GetCode)(PY_PyObject *)((PY_PyObject *))) \
    ((PyCallable_Check)(int)((PY_PyObject *))) \
    ((PyImport_ImportModule)(PY_PyObject *)((const char *))) \
    ((PyImport_ExecCodeModule)(PY_PyObject *)((char *)(PY_PyObject *))) \
    ((PyImport_AddModule)(PY_PyObject *)((const char *))) \
    ((PyImport_GetMagicNumber)(long)()) \
    ((PyModule_GetDict)(PY_PyObject *)((PY_PyObject *))) \
    ((PyModule_AddObject)(int)((PY_PyObject *)(const char *)(PY_PyObject *))) \
    ((PySys_SetArgv)(void)((int)(char **))) \
    ((PySys_SetArgvEx)(void)((int)(char **)(int))) \
    ((PySys_GetObject)(PY_PyObject *)((char *))) \
    ((PyOS_InitInterrupts)(void)()) \
    ((PyMarshal_ReadObjectFromString)(PY_PyObject *)((char *)(PY_Py_ssize_t))) \
    ((PyMarshal_WriteObjectToString)(PY_PyObject *)((PY_PyObject *)(int))) \
    ((PyRun_InteractiveLoopFlags)(int) \
	((FILE *)(const char *)(PY_PyCompilerFlags *)))

// These are the constants we'll automatically wrap, of the form
// (constant_name)(type).  The names of the constants must correspond directly
// to symbols in the Python shared object.
typedef char *(*PY_PyOS_ReadlineFunctionPointerType)(FILE *, FILE *, char *);
#define PY_WRAPPED_CONSTANTS \
    ((Py_TabcheckFlag)(int)) \
    ((_Py_NoneStruct)(PY_PyObject)) \
    ((_PyOS_ReadlineTState)(PY_PyThreadState *)) \
    ((_PyThreadState_Current)(PY_PyThreadState *)) \
    ((PyOS_ReadlineFunctionPointer)(PY_PyOS_ReadlineFunctionPointerType)) \
    ((PyExc_RuntimeError)(PY_PyObject *)) \
    ((PyExc_IndexError)(PY_PyObject *)) \
    ((PyExc_AttributeError)(PY_PyObject *)) \
    ((PyExc_TypeError)(PY_PyObject *)) \
    ((PyExc_KeyboardInterrupt)(PY_PyObject *)) \
    ((PyExc_SystemExit)(PY_PyObject *)) \
    ((PyExc_BaseException)(PY_PyObject *)) \
    ((PyLong_Type)(PY_PyTypeObject)) \
    ((PyBool_Type)(PY_PyTypeObject)) \
    ((PyFloat_Type)(PY_PyTypeObject)) \
    ((PyUnicode_Type)(PY_PyTypeObject)) \
    ((PyTuple_Type)(PY_PyTypeObject)) \
    ((PyList_Type)(PY_PyTypeObject)) \
    ((PyDict_Type)(PY_PyTypeObject)) \
    ((PyFunction_Type)(PY_PyTypeObject))

//----------------------------------------------------------------------------

// These initial macros simplify usage of Boost's preprocessor SEQ_FOR_EACH
// macros.  SEQ_FOR_EACH works as follows: you pass it a callback macro,
// some extra data, and a sequence of the form (a)(b)(c).  It then calls
// the callback macro with a repetition construct (named r, and only used
// internally by boost), the extra data you pased in, and each element of
// the sequence.
//
// Almost all the time, you don't need extra data, so you pass in _ and
// ingore the second parameter in the callback macro.
//
// Often, the sequence contains other sequences, and that's where these
// macros come in handy.  PY_FOR_EACH_SEQ_OF_2, for example, takes a callback
// and a sequence of pair sequences of the form ((A1)(A2))((B1)(B2))((C1)(C2)).
// It then calls the callback with (A1, A2), then with (B1, B2), and so on.
// PY_FOR_EACH_SEQ_OF_3 works with sequences of triplets.

// This helper macro works by passing in the caller's macro as the extra data.
// The caller's macro will then be the second argument of the
// PY_CALLBACK_SEQ_OF_2 macro.
#define PY_FOR_EACH_SEQ_OF_2(macro, seq) \
    HBOOST_PP_SEQ_FOR_EACH(PY_CALLBACK_SEQ_OF_2, macro, seq)

// Pull off the first and second elements of the sequence, and pass them into
// the caller's macro.
#define PY_CALLBACK_SEQ_OF_2(r, macro, seq) \
    macro(PY_SEQ_ELEM_0(seq), PY_SEQ_ELEM_1(seq))


// This helper macro is similar to the last one, except it works with
// subsequences of 3 elements.
#define PY_FOR_EACH_SEQ_OF_3(macro, seq) \
    HBOOST_PP_SEQ_FOR_EACH(PY_CALLBACK_SEQ_OF_3, macro, seq)
#define PY_CALLBACK_SEQ_OF_3(r, macro, seq) \
    macro(PY_SEQ_ELEM_0(seq), PY_SEQ_ELEM_1(seq), PY_SEQ_ELEM_2(seq))


// These helper macros extract the first, second, and third elements from
// an (a)(b)(c) style sequence.
#define PY_SEQ_ELEM_0(seq) \
    HBOOST_PP_SEQ_HEAD(seq)
#define PY_SEQ_ELEM_1(seq) \
    HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_TAIL(seq))
#define PY_SEQ_ELEM_2(seq) \
    HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_TAIL(HBOOST_PP_SEQ_TAIL(seq)))

//----------------------------------------------------------------------------

// This macro takes a sequence of of parameter types (T1)(T2)(T3) and creates a
// parameter list of the form T1 arg1, T2 arg2, T3 arg3.  SEQ_FOR_EACH_I
// passes in the index as well as the element into PY_MAKE_PARM.
#define PY_MAKE_PARM_LIST(parm_types) \
    HBOOST_PP_SEQ_FOR_EACH_I(PY_MAKE_PARM, _, parm_types)

// unused_data is the second parameter passed to FOR_EACH_I.  i is the index
// of the element in the parm types sequence, and elem is that element.
// If i is not 0, we add a comma to make sure the parameters are comma-
// separated.
#define PY_MAKE_PARM(r, unused_data, i, elem) \
    HBOOST_PP_COMMA_IF(i) elem HBOOST_PP_CAT(arg, i)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// This macro takes a sequence of of parameter types (T1)(T2)(T3) and creates
// an argument list of the form arg1, arg2, arg3.  The length of the parameter
// type sequence is used, but the actual parameter types are not.
#define PY_MAKE_ARG_LIST(parm_types) \
    HBOOST_PP_SEQ_FOR_EACH_I(PY_MAKE_ARG, _, parm_types)

#define PY_MAKE_ARG(r, unused_data, i, unused_elem) \
    HBOOST_PP_COMMA_IF(i) HBOOST_PP_CAT(arg, i)

//============================================================================

// These enums and opaque types are used by the wrapped Python functions:
class PY_API PY_PyTypeObject;
class PY_API PY_PyCodeObject;
class PY_API PY_PyCompilerFlags;
class PY_API PY_PyThreadState;
struct PY_API PY_PyMethodDef;
typedef int PY_PyGILState_STATE;

#if defined(WIN32)
    // Windows doesn't define ssize_t (a signed version of size_t).  pyconfig.h
    // (included by Python.h) does define it, though.  So, rather than defining
    // ssize_t and risking a compiler error if Python.h is ever included in our
    // code or HDK code, we instead just define PY_Py_ssize_t directly.
    #if SIZEOF_VOID_P == 8
	typedef int64 PY_Py_ssize_t;
    #else
	typedef int32 PY_Py_ssize_t;
    #endif
#else
    typedef ssize_t PY_Py_ssize_t;
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// This helper function is used below to emulate the result of looking up a
// struct element via an offset, without actually knowing the type definition
// of the object.
template <typename RETURN_TYPE>
static inline RETURN_TYPE &
pyAccessStructMember(void *c_struct, int offset)
{ return *(RETURN_TYPE *)((char *)c_struct + offset); }

// We can't actually declare Python structures, since their size can change
// depending on defines that were set when Python was compiled.  Also, their
// size and organization could change in future Python versions.  Instead,
// we create wrapper classes with methods to look up the corresponding
// data member.  For example, if you would normally write op->ob_type using
// the standard Python API, you instead write op->ob_type() using this API.
//
// Start by building up the Python class hierarchy.  Since Python is written
// in C, the class hierarchy is built manually, using macros for common
// members between subclasses.  These members aren't likely to change between
// Python versions, but if they do we can still preserve the API (i.e. using
// methods to access members), although we'll have to manually give some class
// definitions instead of letting macros take care of everything.

// PY_PyObject_HEAD_EXTRA is at the beginning of most Python object structures.
// Unfortunately, its size varies depending on compiler defines, so we compute
// its size when we load the Python shared object.  Its constructor is private
// and unimplemented so that you can't accidentally instance it or any of its
// subclasses.
class PY_API PY_PyObject_HEAD_EXTRA
{
public:
    static int theStructSize;
private:
    PY_PyObject_HEAD_EXTRA(); // unimplemented
};

// This macro creates a new subclass, adding one member.
#define PY_ADD_SUBCLASS_1(subclass, base_class, member1_type, member1_name) \
class PY_API subclass : public base_class \
{ \
public: \
    member1_type member1_name() \
    { return pyAccessStructMember<member1_type>( \
	this, base_class::theStructSize); } \
\
    static void computeStructSize() \
    { theStructSize = base_class::theStructSize + sizeof(member1_type); } \
\
    static int theStructSize; \
};

// This macro creates a new subclass, adding two members.
#define PY_ADD_SUBCLASS_2(subclass, base_class, \
    member1_type, member1_name, member2_type, member2_name) \
class PY_API subclass : public base_class \
{ \
public: \
    member1_type member1_name() \
    { return pyAccessStructMember<member1_type>( \
	this, base_class::theStructSize); } \
\
    member2_type member2_name() \
    { return pyAccessStructMember<member2_type>( \
	this, base_class::theStructSize + sizeof(member1_type)); } \
\
    static void computeStructSize() \
    { theStructSize = base_class::theStructSize + \
	sizeof(member1_type) + sizeof(member2_type); } \
\
    static int theStructSize; \
};

// The elements in the PyObject_HEAD_EXTRA are dependent on Python compilation
// settings.  See the .C file for more information.  The other structures
// we create are:
//
// PyObject_HEAD (common to all PyObject's) is defined as:  (It's changed
// slightly in Python 3.0, but the size is the same.  See PEP 3123.)
//     PyObject_HEAD_EXTRA
//     Py_ssize_t ob_refcnt;
//     PyTypeObject *ob_type;
//
// PyObject_VAR_HEAD (common to all PyVarObject's) is defined as:
//     PyObject_HEAD
//     Py_ssize_t ob_size;
//
// PyTypeObject starts with:
//     PyObject_VAR_HEAD
//     const char *tp_name;
//     ...more...
//
// PyFrameObject starts with:
//     PyObject_VAR_HEAD
//     PyFrameObject *f_back;
//     PyCodeObject *f_code;
//     ...more...
//
// PyListObject starts with
//     PyObject_VAR_HEAD
//     PyObject **ob_item;
//     ...more...
//
// PyTupleObject starts with
//     PyObject_VAR_HEAD
//     PyObject *ob_item[1];
//     ...more...

PY_ADD_SUBCLASS_2(PY_PyObject, PY_PyObject_HEAD_EXTRA,
    PY_Py_ssize_t, ob_refcnt,
    PY_PyTypeObject *, ob_type)

PY_ADD_SUBCLASS_1(PY_PyVarObject, PY_PyObject,
    PY_Py_ssize_t, ob_size)

PY_ADD_SUBCLASS_1(PY_PyTypeObject, PY_PyVarObject,
    const char *, tp_name)

PY_ADD_SUBCLASS_2(PY_PyFrameObject, PY_PyVarObject,
    PY_PyFrameObject *, f_back,
    PY_PyCodeObject *, f_code)

PY_ADD_SUBCLASS_1(PY_PyListObject, PY_PyVarObject,
    PY_PyObject **, ob_item)

// We need to manually specify PY_PyTupleObject.  Its struct member is declared
// as "PyObject *ob_item[1]", so ob_item is a "PyObject **" whose value is
// computed as an offset from the "this" pointer, _without_ doing any
// dereferencing.
class PY_API PY_PyTupleObject : public PY_PyVarObject
{
public:
    PY_PyObject **ob_item()
    { return (PY_PyObject **)((char *)this + PY_PyVarObject::theStructSize); }
};

//----------------------------------------------------------------------------

// These few concrete types make it much easier to extend Python to add new
// classes, methods, etc.  If Python changes their definition in future
// versions, we'll have to rework how we do things.  Currently, though, these
// few classes are small.

typedef PY_PyObject *(*PY_PyCFunction)(PY_PyObject *, PY_PyObject *);

struct PY_API PY_PyMethodDef
{
    const char *ml_name;
    PY_PyCFunction ml_meth;
    int ml_flags;
    const char *ml_doc;
};

//----------------------------------------------------------------------------

// This macro takes a (function_name)(return_type)(parm_types) sequence and
// calls PY_DECLARE_FUNCTION.
#define PY_DECLARE_FUNCTION_FROM_SEQ(seq) \
    PY_CALLBACK_SEQ_OF_3(_, PY_DECLARE_FUNCTION, seq)

// This macro forward declares a function, prefixing "PY_" to the function
// name.
#define PY_DECLARE_FUNCTION(function_name, return_type, parm_types) \
    PY_API return_type HBOOST_PP_CAT(PY_, function_name)( \
	PY_MAKE_PARM_LIST(parm_types));

// This macro forward declares the function that wraps a constant, prefixing
// "PY_" to the constant's name.
#define PY_DECLARE_WRAPPED_CONSTANT(constant_name, type) \
    PY_API type &HBOOST_PP_CAT(PY_, constant_name)();

// For each element in the big list of wrapped functions, create forward
// declarations.  Create similar forward declarations for the big list of
// constants.
PY_FOR_EACH_SEQ_OF_3(PY_DECLARE_FUNCTION, PY_WRAPPED_FUNCTIONS)
PY_FOR_EACH_SEQ_OF_2(PY_DECLARE_WRAPPED_CONSTANT, PY_WRAPPED_CONSTANTS)

//----------------------------------------------------------------------------

// These functions need to be manually-wrapped:
PY_API PY_PyObject *PY_Py_InitModule(const char *name, PY_PyMethodDef *methods);
PY_API int PY_PyArg_ParseTuple(PY_PyObject *args, const char *format, ...);
PY_API PY_PyObject *PY_Py_BuildValue(const char *format, ...);
PY_API PY_PyObject *PY_PyObject_CallMethod(
    PY_PyObject *o, const char *name, const char *format, ...);

#define PY_FUNC_SEQ_PyInt_AsLong (PyInt_AsLong)(long)((PY_PyObject *))
#define PY_FUNC_SEQ_PyInt_FromLong (PyInt_FromLong)(PY_PyObject *)((long))
#define PY_FUNC_SEQ_PyString_AsString \
    (PyString_AsString)(char *)((PY_PyObject *))
#define PY_FUNC_SEQ_PyString_AsStringAndSize \
    (PyString_AsStringAndSize)(int)((PY_PyObject *)(char **)(PY_Py_ssize_t *))
#define PY_FUNC_SEQ_PyString_FromString \
    (PyString_FromString)(PY_PyObject *)((const char *))
#define PY_FUNC_SEQ_PyString_FromStringAndSize \
    (PyString_FromStringAndSize)(PY_PyObject *)((const char *)(PY_Py_ssize_t))
#define PY_FUNC_SEQ_PyString_AsUTF8String \
    (PyString_AsUTF8String)(PY_PyObject *)((PY_PyObject *))

PY_DECLARE_FUNCTION_FROM_SEQ(PY_FUNC_SEQ_PyInt_AsLong)
PY_DECLARE_FUNCTION_FROM_SEQ(PY_FUNC_SEQ_PyInt_FromLong)
PY_DECLARE_FUNCTION_FROM_SEQ(PY_FUNC_SEQ_PyString_AsString)
PY_DECLARE_FUNCTION_FROM_SEQ(PY_FUNC_SEQ_PyString_AsStringAndSize)
PY_DECLARE_FUNCTION_FROM_SEQ(PY_FUNC_SEQ_PyString_FromString)
PY_DECLARE_FUNCTION_FROM_SEQ(PY_FUNC_SEQ_PyString_FromStringAndSize)
PY_DECLARE_FUNCTION_FROM_SEQ(PY_FUNC_SEQ_PyString_AsUTF8String)


// These wrapper functions replace constants.  If the constants change between
// Python versions, we can use the value of pyPythonVersion to decide what to
// return.  First do a search to make sure they're not called from static
// constructors, though (like PY_METH_VARARGS), since pyPythonVersion may not
// be intialized yet.
static inline int PY_Py_single_input() { return 256; }
static inline int PY_Py_file_input() { return 257; }
static inline int PY_Py_eval_input() { return 258; }
static inline int PY_Py_MARSHAL_VERSION() { return 2; }
static inline int PY_METH_VARARGS() { return 0x0001; }
static inline int PY_Py_EQ() { return 2; }
static inline int PY_PyGILState_LOCKED() { return 0; }
static inline int PY_PyGILState_UNLOCKED() { return 1; }


// _Py_TrueStruct and _Py_ZeroStruct need to be manually wrapped.  Before
// Python 3.0, _Py_TrueStruct and _Py_ZeroStruct are PyIntObjects.  Starting in
// 3.0, they're PyLongObjects, since ints were removed.  For our purposes, we
// don't care -- we just treat them as PyObjects.  However, the automatically-
// generated wrapper functions will give compiler errors because it can't
// convert PyIntObject/PyLongObjects to PyObjects.  So, we wrap them manually.
PY_DECLARE_WRAPPED_CONSTANT(_Py_TrueStruct, PY_PyObject)
PY_DECLARE_WRAPPED_CONSTANT(_Py_ZeroStruct, PY_PyObject)

// Even though int and string have been replaced by long and unicode in Python
// 3.0, we still support the old API, including the old types.  They're simply
// mapped to the replacement types internally.  So, PyInt_Check and
// PyString_Check will actually be using longs and unicodes in Python 3.0.
PY_DECLARE_WRAPPED_CONSTANT(PyInt_Type, PY_PyTypeObject)
PY_DECLARE_WRAPPED_CONSTANT(PyString_Type, PY_PyTypeObject)
#define PY_PyInt_Check(op) PY_PyObject_TypeCheck(op, &PY_PyInt_Type())
#define PY_PyString_Check(op) PY_PyObject_TypeCheck(op, &PY_PyString_Type())


// Various macros follow, to account for API elements that do not have symbols
// in the Python shared object.  Note that they might need to be modified if
// the they change between Python versions.
#define PY_Py_INCREF PY_Py_IncRef
#define PY_Py_DECREF PY_Py_DecRef
#define PY_Py_XDECREF(op) if ((op) == NULL) ; else PY_Py_DECREF(op)

#define PY_Py_False() (&PY__Py_ZeroStruct())
#define PY_Py_True() (&PY__Py_TrueStruct())
#define PY_Py_None() (&PY__Py_NoneStruct())

#define PY_Py_RETURN_NONE return PY_Py_INCREF(PY_Py_None()), PY_Py_None()
#define PY_PyMODINIT_FUNC extern "C" SYS_VISIBILITY_EXPORT void

#define PY_PyRun_SimpleString(s) PY_PyRun_SimpleStringFlags(s, NULL)
#define PY_Py_CompileString(str, p, s) PY_Py_CompileStringFlags(str, p, s, NULL)
#define PY_PyRun_InteractiveLoop(f, p) PY_PyRun_InteractiveLoopFlags(f, p, NULL)

#define PY_PyList_SET_ITEM(op, i, v) \
    (((PY_PyListObject *)(op))->ob_item()[i] = (v))
#define PY_PyTuple_SET_ITEM(op, i, v) \
    (((PY_PyTupleObject *)(op))->ob_item()[i] = (v))

#define PY_PyList_GET_ITEM(op, i, v) \
    (((PY_PyListObject *)(op))->ob_item()[i])
#define PY_PyTuple_GET_ITEM(op, i) \
    (((PY_PyTupleObject *)(op))->ob_item()[i])

// TODO: These checking defines may need to change for Python 3.0.
#define PY_PyObject_TypeCheck(ob, tp) \
    ((ob)->ob_type() == (tp) || PY_PyType_IsSubtype((ob)->ob_type(), (tp)))
#define PY_PyCObject_Check(op) (op->ob_type() == &PY_PyCObject_Type())
#define PY_PyFloat_Check(op) PY_PyObject_TypeCheck(op, &PY_PyFloat_Type())
#define PY_PyLong_Check(op) PY_PyObject_TypeCheck(op, &PY_PyLong_Type())
#define PY_PyBool_Check(op) PY_PyObject_TypeCheck(op, &PY_PyBool_Type())
#define PY_PyUnicode_Check(op) PY_PyObject_TypeCheck(op, &PY_PyUnicode_Type())
#define PY_PyTuple_Check(op) PY_PyObject_TypeCheck(op, &PY_PyTuple_Type())
#define PY_PyList_Check(op) PY_PyObject_TypeCheck(op, &PY_PyList_Type())
#define PY_PyDict_Check(op) PY_PyObject_TypeCheck(op, &PY_PyDict_Type())
#define PY_PyFunction_Check(op) ((op)->ob_type() == &PY_PyFunction_Type())

// TODO: Before Python 3.0, this macro also checks returns true if
//       PyInstance_Check returns true.  Starting with 3.0, it no longer
//       does the PyInstance_Check.
// To do the proper thing, We need to make this a manually-implemented function.
#define PY_PyExceptionInstance_Check(op) \
    PY_PyObject_TypeCheck(op, (PY_PyTypeObject *)&PY_PyExc_BaseException())

// Unfortunately, there's no uniform way to directly access struct members
// for both compile-time and non-compile-time dependencies on Python.  The
// compile-time acccess doesn't use a method call, but the non-compile-time
// one does.  So, we instead use macros.
#define PY_PYFRAME_BACK(op) ((op)->f_back())
#define PY_PYFRAME_CODE(op) ((op)->f_code())

// This function loads the appropriate Python library and looks up symbols
// in it.  It should only be called from PY_InterpreterAutoLock, when it
// initializes Python.
PY_API void PYloadPythonLibrary();

// This function returns the Python major and minor version (e.g. a return
// value of 25 means version 2.5).  It is up to you to ensure that you call
// this function after the Python library has been loaded (creating a
// PY_InterpreterAutoLock will load the Python library and initialize the
// interpreter).
PY_API int PYgetPythonVersion();

// This function returns the path to HFS's Python's site-packages directory.
PY_API void PYgetHFSPythonSitePackagesPath(UT_WorkBuffer &site_packages_path);

// Return true if the Python library in $HFS is loaded.
PY_API bool PYisHFSPythonLibraryLoaded();

#endif
