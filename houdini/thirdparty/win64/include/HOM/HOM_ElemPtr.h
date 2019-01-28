/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	This class is used to tell swig that it should take ownership of
 *	the items inside a std::vector.  (There is no way with %newobject
 *	to say that the items inside a vector are dynamically allocated.)
 *
 *	Use this class as follows:
 *
 *	std::vector<HOM_ElemPtr<HOM_Node> > returnTupleOfNodes()
 *	{
 *	    std::vector<HOM_ElemPtr<HOM_Node> > result;
 *	    for (...)
 *	        result.push_back(new HOMF_Node(...));
 *	    return result;
 *	}
 *
 *	or
 *
 *	std::map<std::string, HOM_ElemPtr<HOM_Node> > returnDictOfNodes()
 *	{
 *	    std::map<std::string, HOM_ElemPtr<HOM_Node> > result;
 *	    for (...)
 *	        result.insert(std::make_pair("...", new HOMF_Node(...)));
 *	    return result;
 *	}
 *
 *	When swig converts the std::vector into a python object, it will
 *	create a python tuple and add the elements inside the vector into
 *	the tuple.  When it converts a HOM_ElemPtr<T> element inside the
 *	vector, it will just wrap the pointer to the T object, and the
 *	wrapped object will take ownership of the T.
 */

#ifndef __HOM_ElemPtr_h__
#define __HOM_ElemPtr_h__

#include "HOM_Errors.h"

template <typename T>
class HOM_ElemPtr
{
public:
    HOM_ElemPtr(T *pointer = NULL)
    : myPointer(pointer)
    {}

    // We need a less than comparator function if we want HOM_ElemPtr<T>
    // objects to be used as keys in std::maps.
    bool operator<(const HOM_ElemPtr<T> &otherElem) const 
	{ return myPointer < otherElem.myPointer; }

    T *myPointer;
};

#ifdef SWIG
// This macro adds a typemap to convert from HOM_ElemPtr<ElemType> into
// swig interpreter objects.  The interpreter object will own the pointer to
// ElemType.
%define %add_elem_ptr_typemap(ElemType...)
%typemap(out) HOM_ElemPtr<ElemType> {
    $result = SWIG_NewPointerObj($1.myPointer, SWIGTYPE_p_ ## ElemType,
        SWIG_POINTER_OWN | $owner);
}

// For whatever reason, adding the typemap does not customize swig::from(),
// so we do that manually.
%{
namespace swig {
    template <>
    struct traits_from<HOM_ElemPtr<ElemType > >
    {
        static PyObject *from(const HOM_ElemPtr<ElemType > &val)
        {
            return HOMconvertValueForInterpreter(val.myPointer,
                SWIG_POINTER_OWN);
        }
    };
}
%}
%enddef
#endif

#endif
