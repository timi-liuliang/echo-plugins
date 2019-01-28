/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *      Use this class to automatically decrement the reference count on
 *      python objects.
 *
 *      So many functions in the python API return a new reference
 *      that we don't increment the reference when you assign to this
 *      smart pointer.  If you're calling a python function that returns
 *      a borrowed reference, use a PyObject* instead of this class.
 *
 *	You should only need to include this file in .C files, not in .h files.
 *	For a class that is ok to include from .h files see PY_OpaqueObject.
 */

#ifndef __PY_AutoObject_h__
#define __PY_AutoObject_h__

// Note that including this file may include the Python headers, which need
// to be included first to avoid gcc warnings.
#include "PY_CPythonAPI.h"

// This class takes care of automatically decrementing the reference count
// on python objects.
class PY_AutoObject
{
public:
    // This class will work with null py_object pointers because it calls
    // PY_Py_XDECREF().
    PY_AutoObject(PY_PyObject *py_object)
    : myPyObject(py_object)
    {}

    PY_AutoObject() : myPyObject(nullptr)
    {
	;
    }

    ~PY_AutoObject() 
    { PY_Py_XDECREF(myPyObject); }

    PY_AutoObject& operator=(PY_PyObject *py_object)
    {
	if (myPyObject != py_object)
	{
	    PY_Py_XDECREF(myPyObject);
	    myPyObject = py_object;
	}
	return *this;
    }

    operator PY_PyObject*()
    { return myPyObject; }

    PY_PyObject *operator->()
    { return myPyObject; }

    PY_PyObject* ptr()
    { return myPyObject; }

    PY_PyObject const* ptr() const
    { return myPyObject; }

private:
    PY_PyObject *myPyObject;
};

#endif
