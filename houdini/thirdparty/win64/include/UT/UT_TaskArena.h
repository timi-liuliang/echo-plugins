/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TaskArena.h (UT Library, C++)
 *
 * COMMENTS:	Wrapper for tbb::task_arena
 */

#pragma once

#ifndef __UT_TASKARENA_H_INCLUDED__
#define __UT_TASKARENA_H_INCLUDED__

#include "UT_API.h"
#include "UT_NonCopyable.h"
#include <SYS/SYS_AtomicInt.h>
#include <stddef.h>


/// @file
/// @class UT_TaskArena
///
/// Simple wrapper for running code within a tbb::task_arena. For most usages,
/// you want to do this if you're going to spawn tasks with a lock held.
class UT_API UT_TaskArena
{
public:
    static const int automatic = -1;

    UT_TaskArena(int max_concurrency = automatic,
		 unsigned reserved_for_masters = 1);
    ~UT_TaskArena();

    template <typename F>
    void execute(F &functor)
    {
        internalExecute(&UT_TaskArena::callback<F>, &functor);
    }

    template <typename F>
    void execute(const F &functor)
    {
        internalExecute(&UT_TaskArena::callbackConst<F>, &functor);
    }

    template <typename M, typename F>
    static void
    lockedExecute(M &mutex, F &functor)
    {
	typename M::Scope scope(mutex);
	UT_TaskArena arena;
	arena.execute(functor);
    }

    template <typename M, typename F>
    static void
    lockedExecute(M &mutex, const F &functor)
    {
	typename M::Scope scope(mutex);
	UT_TaskArena arena;
	arena.execute(functor);
    }

    /// Call this before using the arena if execute
    /// may be called from multiple threads.
    void initialize();

    // Debug counter. Only modified on debug builds
    static SYS_AtomicInt32 theArenaCount;

private:

    template <typename F>
    static void callback(void *data)
    {
	(*reinterpret_cast<F *>(data))();
    }
    template <typename F>
    static void callbackConst(const void *data)
    {
	(*reinterpret_cast<const F *>(data))();
    }

    void    internalExecute(void (*callback)(void *), void *data);
    void    internalExecute(void (*callback)(const void *), const void *data);

private:
    static const size_t SIZEOF_ARENA = 40;
    uint8 myArena[SIZEOF_ARENA];
};

#endif // __UT_TASKARENA_H_INCLUDED__
