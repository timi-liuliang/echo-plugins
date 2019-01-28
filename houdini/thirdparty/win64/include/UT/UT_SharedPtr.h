/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SharedPtr.h (UT Library, C++)
 *
 * COMMENTS:	
 *
 */

#ifndef __UT_SHAREDPTR_H_INCLUDED__
#define __UT_SHAREDPTR_H_INCLUDED__

#include <memory>
#include <stddef.h>

/// @brief Wrapper around std::shared_ptr
///
/// An shared pointer takes ownership of the object referenced.  The reference
/// counting is done outside the object interface, so the size of a
/// UT_SharedPtr is larger than a vanilla pointer.
///
/// As this is a wrapper around the boost shared pointer template, the type (T)
/// must have a <b>Copy Constructor</b> and a valid <b>Assignment Operator</b>
/// @see UT_IntrusivePtr
template <typename T>
using UT_SharedPtr = std::shared_ptr<T>;

#define UTmakeShared std::make_shared

#define UTstatic_pointer_cast	    std::static_pointer_cast
#define UTdynamic_pointer_cast	    std::dynamic_pointer_cast
#define UTconst_pointer_cast	    std::const_pointer_cast
#define UTreinterpret_pointer_cast  std::reinterpret_pointer_cast

/// Return a UT_SharedPtr<T> that points to an array of T[len] buffer.
/// Only needed until we have proper support in C++20.
template <typename T>
inline UT_SharedPtr<T>
UTmakeSharedArrayPtr(size_t len)
{
    return UT_SharedPtr<T>(new T[len], [](T* p) { delete [] p; });
}

// For UT::ArraySet.
namespace UT
{
template <typename T>
struct DefaultClearer;

template <typename T>
struct DefaultClearer<UT_SharedPtr<T>>
{
    static void clear(UT_SharedPtr<T> &v) { v = UT_SharedPtr<T>(); }
    static bool isClear(const UT_SharedPtr<T> &v) { return v.get() == nullptr; }
    static void clearConstruct(UT_SharedPtr<T> *p)
    {
        new ((void *)p) UT_SharedPtr<T>(nullptr);
    }
    static const bool clearNeedsDestruction = false;
};
} // namespace UT

#endif // __UT_SHAREDPTR_H_INCLUDED__
