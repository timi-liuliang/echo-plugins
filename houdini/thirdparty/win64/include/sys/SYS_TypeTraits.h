/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_TypeTraits.h (SYS Library, C++)
 *
 * COMMENTS:	Type trait utilities
 */

#ifndef __SYS_TYPETRAITS_H_INCLUDED__
#define __SYS_TYPETRAITS_H_INCLUDED__

#include "SYS_Inline.h"
#include <type_traits>

/// @file

/// Check whether a given type is plain-old-data (POD)
template <typename T>
static constexpr SYS_FORCE_INLINE bool SYSisPOD()
{
    return std::is_pod<T>::value;
}

/// Check whether a type is arithmetic (integer or floating-point type).
template <typename T>
class SYS_IsArithmetic : public std::is_arithmetic<T>
{
};

/// Check whether a type is an integral type.
template <typename T>
class SYS_IsIntegral : public std::is_integral<T>
{
};

/// Check whether a type is a floating point type.
template <typename T>
class SYS_IsFloatingPoint : public std::is_floating_point<T>
{
};

/// Check whether an arithmetic type is signed.
template <typename T>
class SYS_IsSigned : public std::is_signed<T>
{
};

/// Check whether two types are the same
/// @{
template <typename T1, typename T2>
class SYS_IsSame : public std::is_same<T1, T2>
{
};

template <typename T1, typename T2>
static constexpr SYS_FORCE_INLINE bool SYSisSame()
{
    return SYS_IsSame<T1, T2>::value;
}
/// @}

/// Check whether a given type is a pointer
/// @{

template <typename T> class SYS_IsPointer
{ public: static const bool value = false; };

template <typename U> class SYS_IsPointer<U*>
{ public: static const bool value = true; };

template <typename T>
static constexpr SYS_FORCE_INLINE bool SYSisPointer()
{
    return SYS_IsPointer<T>::value;
}

/// @}

/// Choose whether to make a type const or not with a template bool.
/// This allows a set of types to be made const or not with a single
/// bool template, instead of having a separate template parameter for
/// each type.
/// @{

template<typename T,bool CONST> class SYS_ConstType
{ public: typedef T type; };

template<typename T> class SYS_ConstType<T,true>
{ public: typedef const T type; };

/// @}

/// Choose a type from two alternatives based on a bool.
/// It choses the first type if false, and the second type if true.
/// @{

template<typename T0,typename T1,bool IST1> class SYS_SelectType
{ public: typedef T0 type; };

template<typename T0,typename T1> class SYS_SelectType<T0,T1,true>
{ public: typedef T1 type; };

/// @}

/// Hides a function overload or template specialization based on a
/// compile-time boolean.
/// {@
template <bool B, class T = void>
class SYS_EnableIf : public std::enable_if<B, T>
{
};

template <bool B, class T = void>
class SYS_DisableIf : public SYS_EnableIf<!B, T>
{
};
/// @}

// For consistency, include this for all platforms even though we're only using
// stdlib.h for _countof()
#include <stdlib.h>


/// Return the number of elements of a static array
/// @{

#ifdef _MSC_VER

#define SYScountof(array) (_countof(array))

#else

template <typename T, size_t N>
char (*SYScountOfHelper(T(&array_arg)[N]))[N];

#define SYScountof(array) sizeof(*SYScountOfHelper(array))

#endif

/// @}

#endif // __SYS_TYPETRAITS_H_INCLUDED__
