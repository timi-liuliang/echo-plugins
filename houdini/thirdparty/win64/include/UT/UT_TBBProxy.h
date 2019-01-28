/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TBBProxy.h (UT Library, C++)
 *
 * COMMENTS:	Proxy interface for using TBB features that may not be
 *		available when loaded into a third-party application.
 */

#pragma once

#ifndef __UT_TBBPROXY_H_INCLUDED__
#define __UT_TBBPROXY_H_INCLUDED__

#include "UT_API.h"
#include <stddef.h>

class UT_API UT_TBBProxy
{
public:

    static UT_TBBProxy* get();

    void *  (*createTaskArena)(
		void *arena, size_t sizeof_arena,
		int max_concurrency, unsigned reserved_for_masters);

    void    (*destroyTaskArena)(void *arena);
    void    (*taskArenaExecute)(
		void *arena, void (*callback)(void *data), void *data);

    int	    (*scalableAllocationCommand)(int cmd, void *param);

    void    (*initializeTaskArena)(void *arena);

    class Lib;

private:
    UT_TBBProxy();

    friend class Lib;
};

#endif // __UT_TBBPROXY_H_INCLUDED__
