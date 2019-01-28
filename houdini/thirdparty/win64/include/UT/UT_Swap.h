/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Swap.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_SWAP_H_INCLUDED__
#define __UT_SWAP_H_INCLUDED__

#include <SYS/SYS_Inline.h>
#include <utility>
#include <stddef.h>

/// Generic swap so that it will always work for specializations that use
/// argument-dependent lookup (ADL).
///
/// Contrived specialization example:
/// (note that this is not necessary if MyClass obeys rule of zero)
/// @code
///	class MyClass
///	{
///	    friend void swap(MyClass& a, Myclass &b)
///	    {
///		UTswap(a.myFirstDataMember, b.myFirstDataMember);
///		UTswap(a.mySecondDataMember, b.mySecondDataMember);
///	    }
///	};
/// @endcode
/// @{
template <typename T>
void UTswap(T& a, T& b)
{
    using namespace std;
    swap(a, b);
}
template <typename T, size_t N>
void UTswap(T (&a)[N], T (&b)[N])
{
    using namespace std;
    swap(a, b);
}
template <typename T, size_t S>
inline void
UTswap(T (&a)[S], T (&b)[S], size_t at_most)
{
    if (at_most > S)
	at_most = S;
    for (size_t i = 0; i < at_most; i++)
	UTswap<T>(a[i], b[i]);
}
/// @}

/// Macro for specializing std::swap() and UTswap() for classes that have a
/// swap method.
/// @{
#define UT_SWAPPER_CLASS(T) \
	SYS_FORCE_INLINE void UTswap(T &a, T &b) { a.swap(b); }
#define UT_SWAPPER_TEMPLATE(T) \
	template< typename TA > \
	SYS_FORCE_INLINE void UTswap(T<TA> &a, T<TA> &b) { a.swap(b); }
/// @}

#endif // __UT_SWAP_H_INCLUDED__
