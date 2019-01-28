/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_MemoryOrder.h ( UT Library, C++)
 *
 * COMMENTS:	Enumerated type for memory order of atomic operations.
 */

#ifndef __SYS_MemoryOrder__
#define __SYS_MemoryOrder__

#include "SYS_Compiler.h"

#if defined(FORCE_NON_SIMD)
    #define SYSloadFence()
    #define SYSstoreFence()
    #define SYSmemoryFence()
#else
    #if defined(LINUX) && SYS_IS_GCC_GE(3, 4) && defined(__SSE2__)
        #include <emmintrin.h>
        #include <xmmintrin.h>
    #elif defined(LINUX) && SYS_IS_GCC_GE(3, 4) && defined(__SSE__)
        #include <xmmintrin.h>
        #define SYSloadFence()
        #define SYSmemoryFence()
    #elif defined(WIN32)
        #include <emmintrin.h>
        #include <xmmintrin.h>
    #elif defined(MBSD_INTEL)
        #include <emmintrin.h>
        #include <xmmintrin.h>
    #else
        #define SYSloadFence()
        #define SYSstoreFence()
        #define SYSmemoryFence()
    #endif
#endif


enum SYS_MemoryOrder
{
    /// Any reordering the compiler or hardware chooses to do is okay.
    SYS_MEMORY_ORDER_RELAXED,

    /// Prevents any reads by the same thread that follow this in
    /// program order from occurring before this read, i.e. a
    /// compiler and CPU load fence is placed after the read.
    SYS_MEMORY_ORDER_LOAD,

    /// Prevents any writes by the same thread that precede this in
    /// program order from occurring after this write, i.e. a
    /// compiler and CPU store fence is placed before the write.
    SYS_MEMORY_ORDER_STORE,

    /// The current operation will be executed before any loads or
    /// stores that follow it in program order and after any loads
    /// or stores that precede it.  Moreover, sequential consistency
    /// is assured meaning that all observers will see this operation
    /// in the same order relative to any other MEMORY_ORDER_SEQ_CST
    /// operations.
    SYS_MEMORY_ORDER_SEQ_CST
};

#ifndef SYSloadFence
/// Memory load fence:
///
/// This forces any memory reads from before this fence to finish before
/// any following memory reads, but does not enforce any ordering with
/// respect to memory writes.
inline void SYSloadFence()
{
    _mm_lfence();
}
#endif

#ifndef SYSstoreFence
/// Memory store fence:
///
/// This forces any memory writes from before this fence to finish before
/// any following memory writes, but does not enforce any ordering with
/// respect to memory reads.
inline void SYSstoreFence()
{
    _mm_sfence();
}
#endif

#ifndef SYSmemoryFence
/// Full memory fence:
///
/// This forces any memory reads or writes from before this fence to finish before
/// any following memory reads or writes.
/// NOTE: This is *not* equivalent to a load fence and a store fence, because
///       e.g., that would not ensure that any writes have finished before any
///       following reads or vice versa.
inline void SYSmemoryFence()
{
    _mm_mfence();
}
#endif

#endif
