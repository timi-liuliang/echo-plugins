/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TimeGate.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_TIMEGATE_H_INCLUDED__
#define __UT_TIMEGATE_H_INCLUDED__

#include "UT_API.h"

namespace UT_TimeGateImpl
{
    UT_API extern volatile int	theClockCount;
    UT_API extern void		startClockThreadIfNeeded();
}

/// Used to reduce the frequency of checks gated by a time out (in
/// milliseconds at resolution of ~100ms). Note that the frequency is not
/// guaranteed, esp. if you try to call tryAcquire() from multiple threads on
/// the same UT_TimeGate object.
template <int TIMEOUT_MS>
class UT_TimeGate
{
public:
    inline	UT_TimeGate()
		    : myTickCount(0)
		{
		    UT_TimeGateImpl::startClockThreadIfNeeded();
		}

    /// Return true if at least TIMEOUT_MS has elapsed from the last time this
    /// method returned true. Resolution is ~100ms.
    inline bool	tryAcquire()
		{
		    const int tick = UT_TimeGateImpl::theClockCount;
		    bool ok = (tick - myTickCount) > ((TIMEOUT_MS + 99) / 100);
		    if (ok)
			myTickCount = tick;
		    return ok;
		}

    /// Return true if at least TIMEOUT_MS has elapsed from the last call to
    /// acquire(), without actually acquiring.
    inline bool	canAcquire()
		{
		    const int tick = UT_TimeGateImpl::theClockCount;
		    return (tick - myTickCount) > ((TIMEOUT_MS + 99) / 100);
		}

    /// Resets the gate so that canAcquire()/tryAcquire() will return false
    /// until at least TIMEOUT_MS has elapsed.
    inline void	acquire()
		{
		    myTickCount = UT_TimeGateImpl::theClockCount;
		}

private:
    // Volatile because multiple threads use the same instance
    volatile int    myTickCount;
};

#endif // __UT_TIMEGATE_H_INCLUDED__
