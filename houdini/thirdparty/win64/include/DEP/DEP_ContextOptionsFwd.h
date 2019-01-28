/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DEP_ContextOptionsFwd.h (DEP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DEP_CONTEXTOPTIONSFWD_H_INCLUDED__
#define __DEP_CONTEXTOPTIONSFWD_H_INCLUDED__

template <typename T> class UT_Array;
template <typename T> class UT_COWReadHandle;
template <typename T> class UT_COWTransientReadHandle;
template <typename T> class UT_COWWriteHandle;
template <typename T> class UT_COWHandle;

class DEP_ContextOptions;
using DEP_ContextOptionsHandle = UT_COWHandle<DEP_ContextOptions>;
using DEP_ContextOptionsReadHandle = UT_COWReadHandle<DEP_ContextOptions>;
using DEP_ContextOptionsWriteHandle = UT_COWWriteHandle<DEP_ContextOptions>;
using DEP_ContextOptionsStack = UT_Array<DEP_ContextOptionsHandle>;

#endif // __DEP_CONTEXTOPTIONSFWD_H_INCLUDED__
