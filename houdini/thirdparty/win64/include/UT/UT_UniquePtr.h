/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_UniquePtr.h (UT Library, C++)
 *
 * COMMENTS:	
 *
 */

#ifndef __UT_UNIQUEPTR_H_INCLUDED__
#define __UT_UNIQUEPTR_H_INCLUDED__

#include <stddef.h>
#include <memory>
#include <type_traits>
#include <utility>

/// @file

#if defined(__clang__)

/// @brief A smart pointer for unique ownership of dynamically allocated objects
///
/// UT_UniquePtr mimics a built-in pointer except that it guarantees deletion
/// of the object pointed to, either upon destruction or via an explicit
/// reset(). UT_UniquePtr is a simple solution for simple needs; use
/// UT_SharedPtr/UT_IntrusivePtr if your needs are more complex.
///
#define UT_UniquePtr std::unique_ptr

#else // !defined(__clang__)

/// @brief A smart pointer for unique ownership of dynamically allocated objects
///
/// UT_UniquePtr mimics a built-in pointer except that it guarantees deletion
/// of the object pointed to, either upon destruction or via an explicit
/// reset(). UT_UniquePtr is a simple solution for simple needs; use
/// UT_SharedPtr/UT_IntrusivePtr if your needs are more complex.
///
/// @{
template<
    class T,
    class Deleter = std::default_delete<T>
>
class UT_UniquePtr;

template<
    class T,
    class Deleter // = std::default_delete<T>
>
class UT_UniquePtr : public std::unique_ptr<T, Deleter>
{
public:
    using std::unique_ptr<T, Deleter>::unique_ptr;
};

/// Array specialization of UT_UniquePtr
template<
    class T,
    class Deleter
>
class UT_UniquePtr<T[], Deleter> : public std::unique_ptr<T[], Deleter>
{
public:
    using std::unique_ptr<T[], Deleter>::unique_ptr;
};
/// @}

#endif // !defined(__clang__)

/// Constructs an object of type T and wraps it in an UT_UniquePtr. The args
/// are passed to the constructor of T.
template<
    class T,
    class... REST
>
inline typename std::enable_if<
    !std::is_array<T>::value,
    UT_UniquePtr<T>
>::type
UTmakeUnique(REST&&... args)
{
    return UT_UniquePtr<T>(new T(std::forward<REST>(args)...));
}

/// Constructs an 1D array of type T with len elements and wraps it in an
/// UT_UniquePtr.
template<class T>
inline typename std::enable_if<
    std::is_array<T>::value && std::extent<T>::value == 0,
    UT_UniquePtr<T>
>::type
UTmakeUnique(size_t len)
{
    typedef typename std::remove_extent<T>::type ElemT;
    return UT_UniquePtr<T>(new ElemT[len]());
}

/// Construction of arrays of known bound is disallowed. Just do it directly!
template<
    class T,
    class... REST
>
typename std::enable_if<std::extent<T>::value != 0, void>::type
UTmakeUnique(REST&&...) = delete;

// For UT::ArraySet.
namespace UT
{
template <typename T>
struct DefaultClearer;

template <typename T>
struct DefaultClearer<UT_UniquePtr<T>>
{
    static void clear(UT_UniquePtr<T> &v) { v.reset(nullptr); }
    static bool isClear(const UT_UniquePtr<T> &v) { return v.get() == nullptr; }
    static void clearConstruct(UT_UniquePtr<T> *p)
    {
        new ((void *)p) UT_UniquePtr<T>(nullptr);
    }
    static const bool clearNeedsDestruction = false;
};
} // namespace UT

#endif // __UT_UNIQUEPTR_H_INCLUDED__
