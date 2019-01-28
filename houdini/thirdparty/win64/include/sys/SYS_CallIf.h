/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_TypeTraits.h (SYS Library, C++)
 *
 * COMMENTS:	Wrapper for Boost.TypeTraits
 */

#ifndef __SYS_CALLIF_H_INCLUDED__
#define __SYS_CALLIF_H_INCLUDED__

/// @file

#include "SYS_Compiler.h"
#include "SYS_Inline.h"

#if SYS_HAS_CXX14
  #define SYS_CALLIF_AUTO auto
#else
  #define SYS_CALLIF_AUTO 
#endif

/// This function is for use with lambdas to force the compiler to
/// not even try compiling or inlining function calls when a compile-time
/// condition is false.
///
/// Usage: SYS_CallIf<CONDITION>::call([&](SYS_CALLIF_AUTO) { ... });
///
/// @note Your lambda must have exactly 1 parameter of type SYS_CALLIF_AUTO
/// @{
template<bool CONDITION>
struct SYS_CallIf
{
    template<typename FUNCTOR>
    static SYS_FORCE_INLINE void call(FUNCTOR functor)
    {
        // No call at all if CONDITION is false
    }
};

template<>
struct SYS_CallIf<true>
{
#if SYS_HAS_CXX14
    template<typename FUNCTOR>
    static SYS_FORCE_INLINE void call(FUNCTOR functor)
    {
	struct dummy {};
        functor(dummy());
    }
#else
    template<typename FUNCTOR>
    static SYS_FORCE_INLINE void call(FUNCTOR functor)
    {
        functor();
    }
#endif
};
/// @}

#endif // __SYS_CALLIF_H_INCLUDED__
