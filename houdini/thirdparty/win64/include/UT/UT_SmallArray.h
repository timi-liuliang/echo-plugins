/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_SmallArray.h (UT Library, C++)
 *
 * COMMENTS:    
 */

#pragma once

#ifndef __UT_SMALLARRAY_H_INCLUDED__
#define __UT_SMALLARRAY_H_INCLUDED__

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_Assert.h"
#include <SYS/SYS_Compiler.h>
#include <SYS/SYS_StaticAssert.h>
#include <utility>
#include <stddef.h>

/// An array class with the small buffer optimization, making it ideal for
/// cases when you know it will only contain a few elements at the expense of
/// increasing the object size by MAX_BYTES (subject to alignment).
template <typename T, size_t MAX_BYTES = 64>
class UT_API_TMPL UT_SmallArray : public UT_Array<T>
{
    // As many elements that fit into MAX_BYTES with 1 item minimum
    enum { MAX_ELEMS = MAX_BYTES/sizeof(T) < 1 ? 1 : MAX_BYTES/sizeof(T) };

public:

// gcc falsely warns about our use of offsetof() on non-POD types. We can't
// easily suppress this because it has to be done in the caller at
// instantiation time. Instead, punt to a runtime check instead.
#if defined(__clang__)
     #define UT_SMALL_ARRAY_SIZE_ASSERT()    \
         using ThisT = UT_SmallArray<T,MAX_BYTES>; \
       SYS_STATIC_ASSERT( \
               __builtin_offsetof(ThisT, myBuffer) == sizeof(UT_Array<T>))
#elif !SYS_IS_GCC_GE(4, 8) || defined(_MSC_VER)
    #define UT_SMALL_ARRAY_SIZE_ASSERT()    \
        using ThisT = UT_SmallArray<T,MAX_BYTES>; \
	SYS_STATIC_ASSERT(offsetof(ThisT, myBuffer) == sizeof(UT_Array<T>))
#else
    #define UT_SMALL_ARRAY_SIZE_ASSERT()    \
	UT_ASSERT_P(!UT_Array<T>::isHeapBuffer());
#endif

    /// Default construction
    UT_SmallArray()
	: UT_Array<T>(/*capacity*/0)
    {
	UT_Array<T>::unsafeShareData((T*)myBuffer, 0, MAX_ELEMS);
	UT_SMALL_ARRAY_SIZE_ASSERT();
    }
    
    /// Copy constructor
    /// @{
    explicit UT_SmallArray(const UT_Array<T> &copy)
	: UT_Array<T>(/*capacity*/0)
    {
	UT_Array<T>::unsafeShareData((T*)myBuffer, 0, MAX_ELEMS);
	UT_SMALL_ARRAY_SIZE_ASSERT();
	UT_Array<T>::operator=(copy);
    }
    explicit UT_SmallArray(const UT_SmallArray<T,MAX_BYTES> &copy)
	: UT_Array<T>(/*capacity*/0)
    {
	UT_Array<T>::unsafeShareData((T*)myBuffer, 0, MAX_ELEMS);
	UT_SMALL_ARRAY_SIZE_ASSERT();
	UT_Array<T>::operator=(copy);
    }
    /// @}

    /// Move constructor
    /// @{
    UT_SmallArray(UT_Array<T> &&movable) SYS_NOEXCEPT
    {
	UT_Array<T>::unsafeShareData((T*)myBuffer, 0, MAX_ELEMS);
	UT_SMALL_ARRAY_SIZE_ASSERT();
	UT_Array<T>::operator=(std::move(movable));
    }
    UT_SmallArray(UT_SmallArray<T,MAX_BYTES> &&movable) SYS_NOEXCEPT
    {
	UT_Array<T>::unsafeShareData((T*)myBuffer, 0, MAX_ELEMS);
	UT_SMALL_ARRAY_SIZE_ASSERT();
	UT_Array<T>::operator=(std::move(movable));
    }
    /// @}

    /// Initializer list constructor
    explicit UT_SmallArray(std::initializer_list<T> init)
    {
        UT_Array<T>::unsafeShareData((T*)myBuffer, 0, MAX_ELEMS);
        UT_SMALL_ARRAY_SIZE_ASSERT();
        UT_Array<T>::operator=(init);
    }

#undef UT_SMALL_ARRAY_SIZE_ASSERT

    /// Assignment operator
    /// @{
    UT_SmallArray<T,MAX_BYTES> &
    operator=(const UT_SmallArray<T,MAX_BYTES> &copy)
    {
	UT_Array<T>::operator=(copy);
	return *this;
    }
    UT_SmallArray<T,MAX_BYTES> &
    operator=(const UT_Array<T> &copy)
    {
	UT_Array<T>::operator=(copy);
	return *this;
    }
    /// @}

    /// Move operator
    /// @{
    UT_SmallArray<T,MAX_BYTES> &
    operator=(UT_SmallArray<T,MAX_BYTES> &&movable)
    {
	UT_Array<T>::operator=(std::move(movable));
	return *this;
    }
    UT_SmallArray<T,MAX_BYTES> &
    operator=(UT_Array<T> &&movable)
    {
        UT_Array<T>::operator=(std::move(movable));
        return *this;
    }
    /// @}

    UT_SmallArray<T,MAX_BYTES> &
    operator=(std::initializer_list<T> src)
    {
        UT_Array<T>::operator=(src);
        return *this;
    }
private:
    alignas(T) char myBuffer[MAX_ELEMS*sizeof(T)];
};

// For UT::ArraySet.
namespace UT
{
template <typename T>
struct DefaultClearer;

template <typename T, size_t BYTES>
struct DefaultClearer<UT_SmallArray<T, BYTES>>
{
    static void clear(UT_SmallArray<T, BYTES> &v)
    {
        // Just in case, destruct and reconstruct,
        // instead of using setCapacity(0).
        UT_SmallArray<T, BYTES> *p = &v;
        p->~UT_SmallArray<T, BYTES>();
        new ((void *)p) UT_SmallArray<T, BYTES>();
    }
    static bool isClear(const UT_SmallArray<T, BYTES> &v)
    {
        return v.isEmpty();
    }
    static void clearConstruct(UT_SmallArray<T, BYTES> *p)
    {
        new ((void *)p) UT_SmallArray<T, BYTES>();
    }
    static const bool clearNeedsDestruction = false;
};
} // namespace UT

#endif // __UT_SMALLARRAY_H_INCLUDED__
