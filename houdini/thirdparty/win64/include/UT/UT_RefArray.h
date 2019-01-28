/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This is a class template that implements a resizable array of 
 *      arbitrary objects. The template parameter represents the type
 *	of object, and is called utRef. You can instantiate this class
 *	with any object or pointer, such as:
 *		UT_Array<int>		iObj;
 *		UT_Array<GeoPrimMesh>	mObj;
 *		UT_Array<GeoPoint*>	pObj; ... etc
 *	The "Ref" in the class name stands for "passed by reference", which 
 *	is the argument passing strategy used by this class. Use this class
 *	if you want to construct arrays of arbitrary objects. Pointers and
 *	primitive types are stored and passed more efficiently in another
 *	class, UT_ValArray, which passes arguments by value to avoid the
 *	referencing overhead.
 */

#ifndef __UT_RefArray_h__
#define __UT_RefArray_h__

#include <SYS/SYS_Types.h>
#include "UT_ValArray.h"

// TODO: Do not add new methods to this class.  The methods implemented
// here are only provided for compatibility until we sweep the baseline to
// use the UT_Array interface exclusively.
template <typename T>
class SYS_DEPRECATED(12.5) UT_RefArray : public UT_Array<T>
{
public:
    // gcc doesn't warning about deprecated attributes on class names, so add
    // it to the constructors as well
    SYS_DEPRECATED(12.5) explicit UT_RefArray(exint sz = 0) 
	: UT_Array<T>::UT_Array(sz) {}
    SYS_DEPRECATED(12.5) UT_RefArray(exint sz, exint count)
	: UT_Array<T>::UT_Array(sz, count) {}

    exint	remove(const T &t)
		{ return UT_Array<T>::findAndRemove(t); }

    exint	remove(exint index)
		{ return UT_Array<T>::removeIndex(index); }
};

#endif
