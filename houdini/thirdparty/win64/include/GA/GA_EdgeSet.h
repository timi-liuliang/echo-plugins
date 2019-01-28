/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:     GA_EdgeSet.h (GA Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __GA_EdgeSet_h__
#define __GA_EdgeSet_h__

#include "GA_Edge.h"
#include <UT/UT_ArraySet.h>

namespace UT {
/// For possible use with UT::ArraySet or UT::ArrayMap
template<typename T,bool DIRECTED>
struct DefaultClearer<GA_EdgeT<T,DIRECTED> >
{
    static void clear(GA_EdgeT<T,DIRECTED> &v)
    {
        v = GA_EdgeT<T,DIRECTED>(T(-1), T(-1));
    }
    static bool isClear(const GA_EdgeT<T,DIRECTED> &v)
    {
        return (v.p0() == T(-1)) && (v.p1() == T(-1));
    }
    static void clearConstruct(GA_EdgeT<T,DIRECTED>*p) {
        clear(*p);
    }
    static const bool clearNeedsDestruction = false;
};
}

/// A helper specialization of UT_Set to support a map keyed by GA_Edge or GA_DirectedEdge
using GA_EdgeSet = UT_ArraySet<GA_Edge>;
using GA_DirectedEdgeSet = UT_ArraySet<GA_DirectedEdge>;

#endif
