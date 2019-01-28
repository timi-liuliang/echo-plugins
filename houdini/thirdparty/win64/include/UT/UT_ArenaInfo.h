/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ArenaInfo.h
 *
 * COMMENTS:
 *	This is a simple interface to mallinfo or some alternative
 *	if this is not available.
 */

#ifndef __UT_ArenaInfo__
#define __UT_ArenaInfo__

#include "UT_API.h"
#include <stddef.h>
#include <iosfwd>

class UT_WorkBuffer;

class UT_API UT_ArenaInfo
{
public:
    // Arena size is a bit old school.  Just use totalInUse.
    static size_t	arenaSize();
    static size_t	totalInUse();

    /// Print memory/arena usage to the stream (with a terminating new line)
    static void		outputUsage(std::ostream &os, bool output_in_bytes=false);

    /// Print memory usage into a work buffer (without terminating new line)
    static void		outputUsage(UT_WorkBuffer &wbuf, bool in_bytes=false);

    /// Returns true if the current session has successfully bound
    /// JE Malloc.
    static bool	usingJEMalloc();

    /// Set our config options for jemalloc. Note that this should be (re)done
    /// after forking since some options may be reset.
    static void configureJEMalloc();
};

#endif /* __UT_ArenaInfo__ */
