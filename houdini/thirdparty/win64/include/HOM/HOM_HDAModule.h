/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_HDAModule_h__
#define __HOM_HDAModule_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"

#include <PY/PY_InterpreterAutoLock.h>

class PY_EvaluationContext;

SWIGOUT(%rename(HDAModule) HOM_HDAModule;)

class HOM_API HOM_HDAModule
{
public:
    HOM_HDAModule()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_HDAModule(const HOM_HDAModule &module)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_HDAModule()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    // Helper method for hou.hda.reloadHDAModule().
    virtual void __reload() = 0;

#if defined(SWIG) && defined(SWIGPYTHON)
%extend
{
    // __getattr__ will look up the attribute name in the evaluation
    // context's dictionaries and return the corresponding object if
    // it exists.  This way, the HDAModule object can give a dictionary
    // the behaviour of an object.
    InterpreterObject __getattr__(const char *name)
    {
	HOM_AutoLock hom_lock;
	PY_InterpreterAutoLock py_lock;

	// First check the context's locals dictionary.  If the context
	// pointer is null we treat it as though the dictionary is empty.
	PY_EvaluationContext *context = self->getEvaluationContext();
	PyObject *dict = context
	    ? (PyObject *)context->getGlobalsDict() : NULL;
	PyObject *attribute = (dict && name)
	    ? PyDict_GetItemString(dict, name) : NULL;

	// If the lookup failed, try the globals dictionary.
	if (!attribute && dict)
	{
	    dict = (PyObject *)context->getGlobalsDict();
	    attribute = name ? PyDict_GetItemString(dict, name) : NULL;
	}

	// If we found an object in the dictionary, return it, being careful
	// to increment the reference count on the object.
	if (attribute)
	    return HOMincRef(attribute);

	// We didn't find the object, so raise an exception.
	if (!name)
	    name = "";
	UT_WorkBuffer error_message;
	error_message.sprintf("'module' object has no attribute '%s'", name);
	PyErr_SetString(PyExc_AttributeError, error_message.buffer());
	return NULL;
    }
}

#endif

    // This method is not wrapped by swig.  It is overridden by the HOMF
    // subclass to provide access to the Python evaluation context containing
    // the module's contents.  It is ok for this method to return null.
    SWIGOUT(%ignore getEvaluationContext;)
    virtual PY_EvaluationContext *getEvaluationContext() = 0;
};

#endif
