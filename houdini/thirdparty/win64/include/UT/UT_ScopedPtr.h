/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ScopedPtr.h (UT Library, C++)
 *
 * COMMENTS:	A wrapper around hboost::scoped_ptr.
 *
 * WARNINGS:
 *	- Do NOT use this with ARRAY pointers
 *	- Do NOT store these in containers
 */

#ifndef __UT_SCOPEDPTR_H_INCLUDED__
#define __UT_SCOPEDPTR_H_INCLUDED__

#include <SYS/SYS_Deprecated.h>
#include <hboost/scoped_ptr.hpp>

/// @brief A smart pointer for unique ownership of dynamically allocated objects
///
/// UT_ScopedPtr mimics a built-in pointer except that it guarantees deletion
/// of the object pointed to, either upon destruction or via an explicit
/// reset(). UT_ScopedPtr is a simple solution for simple needs; use
/// UT_SharedPtr/UT_IntrusivePtr if your needs are more complex.
///
template<class T>
class UT_ScopedPtr : public hboost::scoped_ptr<T>
{
public:
    SYS_DEPRECATED_REPLACE(16.0, UT_UniquePtr) 
    UT_ScopedPtr(T *p = 0) : hboost::scoped_ptr<T>(p)	{ }
};

#endif // __UT_SCOPEDPTR_H_INCLUDED__
