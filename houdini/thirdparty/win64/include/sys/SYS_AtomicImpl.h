/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_AtomicImpl.h (SYS Library, C++)
 *
 * COMMENTS:	Platform-specific atomic operations implementation.
 */

#ifndef __SYS_ATOMICIMPL_H_INCLUDED__
#define __SYS_ATOMICIMPL_H_INCLUDED__

#include "SYS_Inline.h"
#include "SYS_Types.h"
#include "SYS_MemoryOrder.h"
#include "SYS_StaticAssert.h"

#ifdef WIN32
    #include <intrin.h>
    #define SYS_ATOMIC_INLINE		SYS_FORCE_INLINE
#else
    #define SYS_ATOMIC_INLINE		inline
#endif
#ifdef MBSD     
#include <libkern/OSAtomic.h>
#endif


namespace SYS_AtomicImpl
{

template <typename T> T test_and_set(T *addr, T val) 
{ 
    SYS_STATIC_ASSERT_MSG(sizeof(T) == 0, 
	"Cannot instantiate test_and_set for unsupported type.");
}
template <typename T> T test_and_add(T *addr, T val) 
{
    SYS_STATIC_ASSERT_MSG(sizeof(T) == 0,
	"Cannot instantiate test_and_add for unsupported type.");
}
template <typename T> T compare_and_swap(volatile T *addr, T oldval, T newval)
{
    SYS_STATIC_ASSERT_MSG(sizeof(T) == 0, 
	"Cannot instantiate compare_and_swap for unsupported type.");
}

// Group these together because the load/store implementation is the same
#if defined(LINUX) || defined(MBSD_INTEL)

#if defined(LINUX)

// GCC 4.0 doesn't support __sync_lock_test_and_set,
// but GCC 4.1 and onwards do
#if !SYS_IS_GCC_GE(4, 1)
#error "Unsupported gcc version"
#endif

template <>
SYS_ATOMIC_INLINE int32
test_and_set<int32>(int32 *addr, int32 val)
{
    return __sync_lock_test_and_set(addr, val);
}

template <>
SYS_ATOMIC_INLINE int32
test_and_add<int32>(int32 *addr, int32 val)
{
    return __sync_fetch_and_add(addr, val);
}

// if (*addr = oldval) *addr = newval
template <>
SYS_ATOMIC_INLINE int32
compare_and_swap<int32>(volatile int32 *addr, int32 oldval, int32 newval)
{
    return __sync_val_compare_and_swap(const_cast<int32 *>(addr),oldval,newval);
}

// NOTE: The int64 GCC built-ins are implemented for 32-bit platforms,
//       using CMPXCHG8B and, if necessary, looping.

template <>
SYS_ATOMIC_INLINE int64
test_and_set<int64>(int64 *addr, int64 val)
{
    return __sync_lock_test_and_set(addr, val);
}

template <>
SYS_ATOMIC_INLINE int64
test_and_add<int64>(int64 *addr, int64 val)
{
    return __sync_fetch_and_add(addr, val);
}

template <>
SYS_ATOMIC_INLINE int64
compare_and_swap<int64>(volatile int64 *addr, int64 oldval, int64 newval)
{
    return __sync_val_compare_and_swap(const_cast<int64 *>(addr),oldval,newval);
}

#else // LINUX

//
// Code for MBSD_INTEL
//

template <>
SYS_ATOMIC_INLINE int32
test_and_set<int32>(int32 *addr, int32 val)
{
    int32       oldval;
    __asm__ __volatile__("lock xchgl %0, %1"
	    : "=r"(oldval), "=m"(*(addr))
	    : "0"(val), "m"(*(addr)));
    return oldval;
}

template <>
SYS_ATOMIC_INLINE int32
test_and_add<int32>(int32 *addr, int32 val)
{
    return __sync_fetch_and_add(addr, val);
}

template <>
SYS_ATOMIC_INLINE int32
compare_and_swap<int32>(volatile int32 *addr, int32 oldval, int32 newval)
{
    return __sync_val_compare_and_swap(const_cast<int32 *>(addr),oldval,newval);
}

// NOTE: MBSD_INTEL implies AMD64
template <>
SYS_ATOMIC_INLINE int64
test_and_set<int64>(int64 *addr, int64 val)
{
    return __sync_lock_test_and_set(addr, val);
}

// NOTE: MBSD_INTEL implies AMD64
template <>
SYS_ATOMIC_INLINE int64
test_and_add<int64>(int64 *addr, int64 val)
{
    return __sync_fetch_and_add(addr, val);
}

// NOTE: MBSD_INTEL implies AMD64
template <>
SYS_ATOMIC_INLINE int64
compare_and_swap<int64>(volatile int64 *addr, int64 oldval, int64 newval)
{
    return __sync_val_compare_and_swap(const_cast<int64 *>(addr),oldval,newval);
}

template <>
SYS_ATOMIC_INLINE time_t
test_and_set<time_t>(time_t *addr, time_t val)
{
    return __sync_lock_test_and_set(addr, val);
}

template <>
SYS_ATOMIC_INLINE time_t
test_and_add<time_t>(time_t *addr, time_t val)
{
    return __sync_fetch_and_add(addr, val);
}

template <>
SYS_ATOMIC_INLINE time_t
compare_and_swap<time_t>(volatile time_t *addr, time_t oldval, time_t newval)
{
    return __sync_val_compare_and_swap(
	const_cast<time_t *>(addr),oldval,newval);
}

#endif // defined(LINUX) || defined(MBSD_INTEL)

template <typename T>
SYS_ATOMIC_INLINE void
store(T *addr, T val, SYS_MemoryOrder order)
{
    if (order == SYS_MEMORY_ORDER_STORE)
    {
        SYSstoreFence();
        *static_cast<volatile T *>(addr) = val;
    }
    else if (order == SYS_MEMORY_ORDER_SEQ_CST)
    {
	T	dummy = 1;

	// __sync_lock_release() is a release barrier, ensuring all previous
	// memory stores are globally visible, and all previous memory loads
	// have been satisfied.
	__sync_lock_release(&dummy);			// release barrier

	// __sync_lock_test_and_set is an acquire barrier, preventing any
	// subsequent memory references from moving before this operation.
	// Consequently, this store will be globally visible before any
	// subsequent stores or loads are processed.
	(void)__sync_lock_test_and_set(addr, val);	// acquire barrier
    }
    else
    {
        // NOTE: order MUST be SYS_MEMORY_ORDER_RELAXED, but we can't use
        //       UT_ASSERT_P in SYS.
	// Use volatile to force the compiler to issue a store instruction, but
	// we don't care what the CPU does.
	*static_cast<volatile T *>(addr) = val;
    }
}

template <typename T>
SYS_ATOMIC_INLINE T
load(const T *addr, SYS_MemoryOrder order)
{
    if (order == SYS_MEMORY_ORDER_LOAD)
    {
        T val = *static_cast<const volatile T *>(addr);
        SYSloadFence();
        return val;
    }
    else if (order == SYS_MEMORY_ORDER_SEQ_CST)
    {
	T tmp = 0;
	return __sync_val_compare_and_swap(const_cast<T *>(addr), tmp, tmp);
    }
    else
    {
        // NOTE: order MUST be SYS_MEMORY_ORDER_RELAXED, but we can't use
        //       UT_ASSERT_P in SYS.
	return *static_cast<const volatile T *>(addr);
    }
}

#elif defined(WIN32)

#pragma intrinsic (_InterlockedExchange)
#pragma intrinsic (_InterlockedExchangeAdd)
#pragma intrinsic (_InterlockedCompareExchange)

template <>
SYS_ATOMIC_INLINE int32
test_and_set<int32>(int32 *addr, int32 val)
{
    return (int32)_InterlockedExchange((long *)addr, (long)val);
}

template <>
SYS_ATOMIC_INLINE int32
test_and_add<int32>(int32 *addr, int32 val)
{
    return (int32)_InterlockedExchangeAdd((long *)addr, (long)val);
}

template <>
SYS_ATOMIC_INLINE int32
compare_and_swap<int32>(volatile int32 *addr, int32 oldval, int32 newval)
{
    return _InterlockedCompareExchange((volatile long *)addr, newval, oldval);
}

// NOTE: _InterlockedCompareExchange64 is available on 32-bit platforms
//       from the Pentium onward, using the CMPXCHG8B instruction.
#pragma intrinsic (_InterlockedCompareExchange64)

template <>
SYS_ATOMIC_INLINE int64
compare_and_swap<int64>(volatile int64 *addr, int64 oldval, int64 newval)
{
    return _InterlockedCompareExchange64(addr, newval, oldval);
}

#if defined(AMD64)

#pragma intrinsic (_InterlockedExchange64)
#pragma intrinsic (_InterlockedExchangeAdd64)

template <>
SYS_ATOMIC_INLINE int64
test_and_set<int64>(int64 *addr, int64 val)
{
    return _InterlockedExchange64(addr, val);
}

template <>
SYS_ATOMIC_INLINE int64
test_and_add<int64>(int64 *addr, int64 val)
{
    return _InterlockedExchangeAdd64(addr, val);
}

#else // AMD64

// On 32-bit platforms, we have to implement our own Exchange64 and
// ExchangeAdd64, using CompareExchange64

template <>
SYS_ATOMIC_INLINE int64
test_and_set<int64>(int64 *addr, int64 val)
{
    int64 retval = *addr;
    do
    {
        int64 newretval = _InterlockedCompareExchange64(addr, val, retval);
        if (retval == newretval)
            return retval;
        retval = newretval;
    } while (true);

    // Unreachable
}

template <>
SYS_ATOMIC_INLINE int64
test_and_add<int64>(int64 *addr, int64 val)
{
    int64 retval = *addr;
    do
    {
        int64 newretval = _InterlockedCompareExchange64(addr, retval+val, retval);
        if (retval == newretval)
            return retval;
        retval = newretval;
    } while (true);

    // Unreachable
}

#endif // AMD64

// The following implementations of store() and load() are valid only for
// MS Visual C++ 2005 and higher.
#pragma intrinsic (_ReadBarrier)
#pragma intrinsic (_WriteBarrier)
#pragma intrinsic (_InterlockedCompareExchange)

template <typename T>
SYS_ATOMIC_INLINE void
store(T *addr, T val, SYS_MemoryOrder order)
{
    // In Visual C++ 2005 and up, reads from volatile variables are defined to
    // have read-acquire semantics, and writes to volatile variables are
    // defined to have write-release semantics.  The compiler will not move
    // any reads and writes past them, and on Windows will ensure that the CPU
    // does not do so either.  Thus there is no need for explicit calls to
    // _ReadWriteBarrier().
    //
    // NOTE:
    //   Visual Studio 2005 had a bug (subsequently fixed) on IA64 where the
    //   compiler did not respect the acquire/release semantics for volatile
    //   floats.
    //	 http://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=288218
    if (order == SYS_MEMORY_ORDER_STORE)
    {
	*static_cast<volatile T *>(addr) = val;
    }
    else if (order == SYS_MEMORY_ORDER_SEQ_CST)
    {
	(void)test_and_set(addr, val);
    }
    else
    {
        // NOTE: order MUST be SYS_MEMORY_ORDER_RELAXED, but we can't use
        //       UT_ASSERT_P in SYS.
	// We want to force the compiler to respect this write.  One way would
	// be to treat addr as volatile, but, as already explained, that would
	// enforce release semantics on both the compiler and CPU. It should
	// theoretically be more efficient to simply prevent the compiler from
	// allowing optimization using any following writes.
	*addr = val;
	_WriteBarrier();
    }
}

template <typename T>
SYS_ATOMIC_INLINE T
load(const T *addr, SYS_MemoryOrder order)
{
    // In Visual C++ 2005 and up, reads from volatile variables are defined to
    // have read-acquire semantics, and writes to volatile variables are
    // defined to have write-release semantics.  The compiler will not move
    // any reads and writes past them, and on Windows will ensure that the CPU
    // does not do so either.  Thus there is no need for explicit calls to
    // _ReadWriteBarrier().
    //
    // NOTE:
    //   Visual Studio 2005 had a bug (subsequently fixed) on IA64 where the
    //   compiler did not respect the acquire/release semantics for volatile
    //   floats.
    //	 http://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=288218
    if (order == SYS_MEMORY_ORDER_LOAD)
    {
	return *static_cast<const volatile T *>(addr);
    }
    else if (order == SYS_MEMORY_ORDER_SEQ_CST)
    {
	T tmp = 0;
	return compare_and_swap(const_cast<T *>(addr), tmp, tmp);
    }
    else
    {
        // NOTE: order MUST be SYS_MEMORY_ORDER_RELAXED, but we can't use
        //       UT_ASSERT_P in SYS.
	// We want to force the compiler to respect this read.  One way would
	// be to treat addr as volatile, but, as already explained, that would
	// enforce acquire semantics on both the compiler and CPU. It should
	// theoretically be more efficient to simply prevent the compiler from
	// equating earlier reads with this one.
	_ReadBarrier();
	return *addr;
    }
}

#else

#error "Unsupported platform"
#endif

} // namespace SYS_AtomicImpl

#endif // __SYS_ATOMICIMPL_H_INCLUDED__
