/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_BoundingRectHash.h (UT Library, C++)
 *
 * COMMENTS:	
 */

#ifndef __UT_BoundingRectHash_H__
#define __UT_BoundingRectHash_H__

#include "UT_API.h"
#include "UT_BoundingRect.h"
#include <SYS/SYS_Hash.h>

/// @file 
/// Include this if you need to hash UT_BoundingRect

/// Compute UT_BoundingRect hash
template <typename T>
uint64
UT_BoundingRectT<T>::hash() const
{
    return SYShashRange(begin(), end());
}

#endif
