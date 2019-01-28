/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_BoundingRectHash.h (UT Library, C++)
 *
 * COMMENTS:	
 */

#ifndef __UT_BoundingBoxHash_h__
#define __UT_BoundingBoxHash_h__

#include "UT_API.h"
#include "UT_BoundingBox.h"
#include <SYS/SYS_Hash.h>

/// @file 
/// Include this if you need to hash UT_BoundingBox

/// Compute UT_BoundingBox hash
template <typename T>
uint64
UT_BoundingBoxT<T>::hash() const
{
    return SYShashRange(begin(), end());
}

#endif
