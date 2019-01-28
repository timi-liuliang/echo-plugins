/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	Instances of this class hold a reference to a Python object.  When this
 *	object is destroyed, the reference count on the Python object will be
 *	decremented.
 *
 *	Unlike PY_AutoObject.h, it is ok to include this header file from other
 *	header files.
 */

#ifndef __PY_OpaqueObject_h__
#define __PY_OpaqueObject_h__

#include "PY_API.h"
#include <string.h>

class PY_API PY_OpaqueObject
{
public:
    // Construct instances of this class with an opaque PY_PyObject pointer
    // (which may be null).  The constructor increments the reference count
    // (so the caller should not) and the destructor decrements the count.
    PY_OpaqueObject(void *opaque_py_object = NULL);
    ~PY_OpaqueObject();

    PY_OpaqueObject(const PY_OpaqueObject &opaque_object);
    PY_OpaqueObject &operator=(const PY_OpaqueObject &opaque_object);

    // Return a borrowed reference to our opaque PyObject *.  If the caller
    // will hold a reference to this object after the PY_OpaqueObject has been
    // deleted, it is up to the caller to increment the reference count.
    void *opaqueObject() const
    { return myOpaqueObject; }

    // The comparison operators and equalsOpaqueObject compare using the __eq__
    // operator.
    bool operator==(const PY_OpaqueObject &opaque_object) const
    { return equalsOpaqueObject(opaque_object.myOpaqueObject); }
    bool operator!=(const PY_OpaqueObject &opaque_object) const
    { return !operator==(opaque_object); }
    bool equalsOpaqueObject(void *opaque_py_object) const;

private:
    void *myOpaqueObject;
};

#endif
