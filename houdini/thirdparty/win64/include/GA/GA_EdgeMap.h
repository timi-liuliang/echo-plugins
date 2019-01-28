/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:     GA_EdgeMap.h (GA Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __GA_EdgeMap_h__
#define __GA_EdgeMap_h__

#include "GA_Edge.h"
#include "GA_EdgeSet.h"
#include <UT/UT_ArrayMap.h>

/// A helper specialization of UT_Map to support a map keyed by GA_Edge or GA_DirectedEdge
template <typename ToType>
using GA_EdgeMap = UT_ArrayMap<GA_Edge, ToType>;

template <typename ToType>
using GA_DirectedEdgeMap = UT_ArrayMap<GA_DirectedEdge, ToType>;

#endif
