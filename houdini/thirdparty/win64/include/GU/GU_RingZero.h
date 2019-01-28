/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_RingZero.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_RINGZERO_H_INCLUDED__
#define __GU_RINGZERO_H_INCLUDED__

#include "GU_API.h"
#include <SYS/SYS_Types.h>

/// @file
/// Utilities for using the results of GU_Detail::buildRingZeroPoints().

/// Given a ringzero and corresponding ringvalence, determine if the given
/// point is a "border" point which is true if,
/// - For closed faces, the point is on an unshared edge
/// - For open faces, the point is on the end of the curve
static inline bool
GUringZeroIsBorderPoint(exint ring_size, exint ring_valence)
{
    // A boundary point is one which doesn't have a pair of each unique edge.
    // As this is hard to find, we take the simpler, and usually correct,
    // course of checking if the number of unique adjacent edges is one half
    // the number of edges. However, we do an less than test for this to
    // exclude points on non-manifold edges.
    //
    // The (ring_size < 2) test is to ensure that the end points of curves are
    // treated as a border point.
    //
    return (ring_size < 2 || 2*ring_size > ring_valence);
}

#endif // __GU_RINGZERO_H_INCLUDED__
