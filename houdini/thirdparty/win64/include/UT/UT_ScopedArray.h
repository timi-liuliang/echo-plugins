/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ScopedArray.h (UT Library, C++)
 *
 * COMMENTS:	A shared ptr for encapsulating dynamically allocated arrays
 *
 */

#ifndef __UT_SCOPEDARRAY_H_INCLUDED__
#define __UT_SCOPEDARRAY_H_INCLUDED__

#include <SYS/SYS_Deprecated.h>
#include <hboost/scoped_array.hpp>
#include <stddef.h>

/// @brief A smart pointer for unique ownership of dynamically allocated arrays
///
/// UT_ScopedArray stores a pointer to a dynamically allocated array, without
/// reference counting. The array pointed to is guaranteed to be deleted,
/// either upon destruction, or via an explicit reset. UT_ScopedArray is a
/// simple solution for simple needs; use UT_SharedArray if your needs are more
/// complex.
///
/// As this is a wrapper around the hboost::scoped_array template, the
/// type (T) must have a <b>Copy Constructor</b> and a valid <b>Assignment
/// Operator</b>.
template<class T>
class UT_ScopedArray : public hboost::scoped_array<T>
{
public:
    SYS_DEPRECATED_REPLACE(16.0, UT_UniquePtr<T[]>) 
    UT_ScopedArray(T *p = 0) : hboost::scoped_array<T>(p)	{ }
};

#endif // __UT_SCOPEDARRAY_H_INCLUDED__
