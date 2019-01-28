/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	This class is used to tell swig that a function can accept any object
 *	without raising a TypeError exception.  The result is a pointer to the
 *	desired type if an object of that type was actually passed in, or null
 *	otherwise.
 *
 *	Use this class as follows:
 *
 *	bool isNode(HOM_PtrOrNull<HOM_Node> node)
 *	{
 *	    return (node.myPointer != NULL);
 *	}
 */

#ifndef __HOM_PtrOrNull_h__
#define __HOM_PtrOrNull_h__

#include <string.h>

template <typename T>
class HOM_PtrOrNull
{
public:
    HOM_PtrOrNull(T *pointer = NULL)
    : myPointer(pointer)
    {}

    T *myPointer;
};

#ifdef SWIG
// This macro adds a typemap to convert swig interpreter objects into
// HOM_PtrOrNull<Type> objects.
%define %add_ptr_or_null_typemap(Type)
%typemap(in) HOM_PtrOrNull<Type> {
    void *result = NULL;
    int succeeded = SWIG_ConvertPtr($input, &result, SWIGTYPE_p_ ## Type, 0);
    $1 = HOM_PtrOrNull<Type>(
        SWIG_IsOK(succeeded) ? static_cast<Type *>(result) : NULL);
}
%enddef
#endif

#endif
