/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_IterableList_h__
#define __HOM_IterableList_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include <string>
#include <vector>
#include <stdexcept>

// In the general case, __getitem__ returns a pointer to a newly allocated
// object.  For "native" data types like strings and numbers, though, we
// don't want to tell swig that it returns a pointer to one of those types.
// (If we do, calling tuple() or list() on an IterableList of strings gives
// lists of "<Swig Object of type 'std::string *' at 0x85feca0>" and warnings
// about memory leaks.)  So, we specialize this class to return those types by
// value.
//
// Note that we always tell swig that __getitem__ always returns a new object,
// even if it's returning an int/float/double/string by value.  There's no
// harm in doing that, and it simplifies the macro a little.

// We could throw a std::out_of_range exception from __getitem__ and have
// swig catch that and in turn raise a python IndexError exception.  However,
// unwinding the stack from the exception turns out to be extremely slow
// and the exception is thrown so much because that's how python knows to
// stop iterating, that throwing the C++ exception dramatically slow things
// down.  The time needed to iterate through the vertices of a bunch of
// primitives, for example, is roughly doubled.  So, instead of throwing that
// exception we use %extend to add a method that returns a PyObject*, and
// we set the python IndexError exception ourselves.
#ifdef SWIG
#define GETITEM_SWIG_EXTENSION \
%extend { \
    /* TODO: We don't suppport slice objects as the key. */ \
    %newobject __getitem__; \
    InterpreterObject __getitem__(int key) \
    { \
        int length = self->__len__(); \
        if (key < 0) \
            key += length; \
        if (key < 0 || key >= length) \
        { \
            PyErr_SetString(PyExc_IndexError, "Invalid index"); \
            return NULL; \
        } \
        return HOMconvertValueForInterpreter( \
	    self->getItemWithValidIndex(key), SWIG_POINTER_OWN); \
    } \
}
#else
#define GETITEM_SWIG_EXTENSION
#endif

#define DEFINE_HOM_ITERABLE_LIST(template_parm, template_type, specialization, getitem_type) \
template <template_parm> \
class HOM_IterableList specialization \
{ \
public: \
    HOM_IterableList() \
    { HOM_CONSTRUCT_OBJECT(this) } \
    HOM_IterableList(const HOM_IterableList<template_type> &) \
    { HOM_CONSTRUCT_OBJECT(this) } \
    virtual ~HOM_IterableList() \
    { HOM_DESTRUCT_OBJECT(this) } \
\
    /* Note that we don't need to implement __iter__, since for loops will */ \
    /* work if they find __getitem__. */ \
    GETITEM_SWIG_EXTENSION \
    \
    virtual int __len__() = 0; \
    virtual std::string __repr__() = 0; \
\
    SWIGOUT(%ignore getItemWithValidIndex;) \
    virtual getitem_type getItemWithValidIndex(int key) = 0; \
};

// Swig doesn't like it when you skip macro parameters, so we use empty
// comments instead.
DEFINE_HOM_ITERABLE_LIST(typename T, T, /**/, T*)
DEFINE_HOM_ITERABLE_LIST(/**/, int, <int>, int)
DEFINE_HOM_ITERABLE_LIST(/**/, float, <float>, float)
DEFINE_HOM_ITERABLE_LIST(/**/, double, <double>, double)
DEFINE_HOM_ITERABLE_LIST(/**/, std::string, <std::string>, std::string)

#undef DEFINE_HOM_ITERABLE_LIST
#undef GETITEM_SWIG_EXTENSION

#endif
