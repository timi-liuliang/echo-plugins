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
 *	of object, and is called utPtr. You can instantiate this class
 *	with any object or pointer, such as:
 *		UT_PtrArray<GeoPoint*>		pObj;
 */

#ifndef __UT_PtrArray_h__
#define __UT_PtrArray_h__

#include "UT_ValArray.h"

#include <SYS/SYS_Types.h>

// TODO: Do not add new methods to this class.  The methods implemented
// here are only provided for compatibility until we sweep the baseline to
// use the UT_ValArray interface exclusively.
template <typename T>
class SYS_DEPRECATED(13.0) UT_PtrArray : public UT_ValArray<T> {
public:
    UT_PtrArray(const UT_PtrArray<T> &a)
	: UT_ValArray<T>(a) {}
    explicit UT_PtrArray(exint capacity = 0)
	: UT_ValArray<T>(capacity) {}
};

#endif
