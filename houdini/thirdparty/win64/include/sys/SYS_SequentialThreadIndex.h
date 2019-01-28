/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_UniqueThreadIndex.h ( SYS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SYS_UniqueThreadIndex__
#define __SYS_UniqueThreadIndex__

#include "SYS_API.h"
#include "SYS_AtomicInt.h"
#include "SYS_MemoryOrder.h"

#include <cstddef>
#if defined(LINUX) || defined(MBSD)
#include <pthread.h>
#endif


/// Helper class to ensure each thread gets allocated a sequential index that
/// can be used to uniquely associate a thread.
class SYS_API SYS_SequentialThreadIndex
{
public:
    enum { FIRST_INDEX = 1 };

    /// Returns the sequential thread index for the current thread.
    /// The first index is always the value of FIRST_INDEX, usually the
    /// main thread.
    static inline int		get();

    /// Returns the index that will be assigned to the next thread that calls
    /// get() for the first time.
    static inline int		next();
private:
    static SYS_AtomicInt32 	myNextIndex;

#if defined(WIN32)
    static int			myTlsIndex;
#elif defined(LINUX) || defined(MBSD)
    static pthread_key_t	myTlsKey;

    // Define a DSO constructor function. This is called prior to any
    // initialization of static objects in the same DSO.
    static __attribute__((constructor)) void initDSO();
#endif

};

SYS_API inline int SYSgetSTID()
{
    return SYS_SequentialThreadIndex::get();
}


// === Implementation ===
int
SYS_SequentialThreadIndex::next()
{
    return myNextIndex.load(SYS_MEMORY_ORDER_SEQ_CST) + FIRST_INDEX;
}


#if defined(WIN32)
// We don't want to drag the Win32 headers in for just two functions, so
// we just forward declare them here.
extern "C" {
    __declspec(dllimport) void *__stdcall TlsGetValue(unsigned long);
    __declspec(dllimport) int __stdcall TlsSetValue(unsigned long, void *);
};

int
SYS_SequentialThreadIndex::get()
{
    ptrdiff_t	tid;

    tid = reinterpret_cast<ptrdiff_t>(::TlsGetValue(myTlsIndex));
    if (tid < FIRST_INDEX)
    {
	tid = myNextIndex.exchangeAdd(1) + FIRST_INDEX;
	::TlsSetValue(myTlsIndex, reinterpret_cast<void *>(tid));
    }

    return static_cast<int>(tid);
}



#elif defined(LINUX) || defined(MBSD)

int
SYS_SequentialThreadIndex::get()
{
    // *** Note for performance freaks, who're likely here because there    ***
    // *** are calls to pthread_getspecific in the profiler output:         ***
    // *** On Linux, it turns out that pthread_getspecific and __thread     ***
    // *** have about the same performance, since the implementation is     ***
    // *** roughly the same. (__thread is implemented on top of calls to    ***
    // *** __tls_get_addr, which has about the same implementation          ***
    // *** complexity as pthread_getspecific, or about 2-3 MOV instructions ***
    // *** in the most-often used case; e.g. normal TLS key creation usage, ***
    // *** threads not popping in-and-out of existence uncontrollably).     ***
    // *** On OSX, however the __thread directive is not supported at all   ***
    // *** since the Mach-O format has no native support, like ELF does.    ***
    // *** However, unlike the glibc versions, pthread_getspecific on OSX   ***
    // *** is implemented with a single MOV instruction, albeit through a   ***
    // *** naked function call.                                             ***
    ptrdiff_t	tid;
    tid = reinterpret_cast<ptrdiff_t>(::pthread_getspecific(myTlsKey));
    if (tid < FIRST_INDEX)
    {
	tid = myNextIndex.exchangeAdd(1) + FIRST_INDEX;
	::pthread_setspecific(myTlsKey, reinterpret_cast<void *>(tid));
    }

    return static_cast<int>(tid);
}

#endif


#endif // __SYS_UniqueThreadIndex__
