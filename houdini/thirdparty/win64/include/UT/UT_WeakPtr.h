/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_WeakPtr.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_WeakPtr__
#define __UT_WeakPtr__

#include <memory>

template <typename T>
using UT_WeakPtr = std::weak_ptr<T>;

#endif // __UT_WeakPtr__
