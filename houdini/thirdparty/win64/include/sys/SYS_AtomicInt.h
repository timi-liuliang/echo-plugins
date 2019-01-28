/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_AtomicInt.h ( UT Library, C++)
 *
 * COMMENTS:	Thread-safe operations on 32-bit integers
 */

#ifndef __SYS_AtomicInt__
#define __SYS_AtomicInt__

#include "SYS_Types.h"
#include "SYS_MemoryOrder.h"
#include "SYS_AtomicImpl.h"

template <typename T>
class SYS_AtomicInt
{
public:
    explicit SYS_AtomicInt(T value = 0) : myValue(value) {}

    /// An empty constructor. This will initialize the value with garbage,
    /// unless used as a static object (in which case it will be guaranteed
    /// by the C++ standard to be initialized to zero). Only use if required
    /// to be set prior to other, dependent statically initialized objects.
    SYS_AtomicInt(SYS_EmptyConstructor) {}

    /// Atomically assigns val to myValue, returning the prior value of
    /// myValue.
    inline T	exchange(T val);

    /// Atomically adds val to myValue, returning the prior value of
    /// myValue.
    inline T	exchangeAdd(T val);

    /// Atomically adds val to myValue, returning the new value of myValue.
    inline T	add(T val);

    /// Atomically set myValue to the maximum of val and myValue.
    inline void		maximum(T val);

    /// Atomically compares (myValue == expected), and if true, replaces
    /// myValue with desired. Returns prior myValue.
    inline T	compare_swap(T expected, T desired);

    /// @pre The supplied order must be one of [RELAXED, STORE, SEQ_CST].
    inline void		store(T val,
			      SYS_MemoryOrder order = SYS_MEMORY_ORDER_SEQ_CST);

    /// @pre The supplied order must be one of [RELAXED, LOAD, SEQ_CST].
    inline T	load(SYS_MemoryOrder order = SYS_MEMORY_ORDER_SEQ_CST)
									const;
    /// Atomically retrieve value. Same as calling load().
    operator		T() const;

    /// Forced RELAXED memory load
    /// GCC has trouble inlining so we pull the definition up here.
    SYS_FORCE_INLINE	T relaxedLoad() const
#ifdef WIN32
    { return load(SYS_MEMORY_ORDER_RELAXED); }
#else
    { return *static_cast<const volatile T *>(&myValue); }
#endif

    SYS_FORCE_INLINE    void relaxedStore(T val)
    { store(val, SYS_MEMORY_ORDER_RELAXED); }

private:
    T	myValue;

private:
    // Prohibited operators
    SYS_AtomicInt(const SYS_AtomicInt &) = default;
    SYS_AtomicInt	&operator=(const SYS_AtomicInt &) = default;
};

typedef SYS_AtomicInt<int32> SYS_AtomicInt32;
typedef SYS_AtomicInt<int64> SYS_AtomicInt64;

#ifdef AMD64
typedef SYS_AtomicInt<int64> SYS_AtomicCounter;
#else
typedef SYS_AtomicInt<int32> SYS_AtomicCounter;
#endif

template <typename T>
SYS_ATOMIC_INLINE T
SYS_AtomicInt<T>::exchange(T val)
{
    return SYS_AtomicImpl::test_and_set(&myValue, val);
}

template <typename T>
SYS_ATOMIC_INLINE T
SYS_AtomicInt<T>::exchangeAdd(T val)
{
    return SYS_AtomicImpl::test_and_add(&myValue, val);
}

template <typename T>
SYS_ATOMIC_INLINE T
SYS_AtomicInt<T>::add(T val)
{
    return SYS_AtomicImpl::test_and_add(&myValue, val) + val;
}

template <typename T>
SYS_ATOMIC_INLINE void
SYS_AtomicInt<T>::maximum(T val)
{
    while (true)
    {
	T	me = myValue;
	if (val > me)
	{
	    if (me == compare_swap(me, val))
		return;
	}
	else
	{
	    return;
	}
    }
}

template <typename T>
SYS_ATOMIC_INLINE T
SYS_AtomicInt<T>::compare_swap(T expected, T desired)
{
    return SYS_AtomicImpl::compare_and_swap(&myValue, expected, desired);
}

/// Next two methods intentionally not marked as volatile as this would cause 
/// &myValue to evaluate to a pointer to a volatile myValue and that has 
/// implications on MSVC2005+ beyond the C++ spec.
///
/// This shouldn't be a problem unless somebody needs a volatile instance of
/// SYS_AtomicInt<T>.
template <typename T>
SYS_ATOMIC_INLINE void
SYS_AtomicInt<T>::store(T val, SYS_MemoryOrder order)
{
    SYS_AtomicImpl::store(&myValue, val, order);
}

template <typename T>
SYS_ATOMIC_INLINE T
SYS_AtomicInt<T>::load(SYS_MemoryOrder order) const
{
    return SYS_AtomicImpl::load(&myValue, order);
}

template <typename T>
SYS_ATOMIC_INLINE
SYS_AtomicInt<T>::operator T() const
{
    return load();
}

#endif

