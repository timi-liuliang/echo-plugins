/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_MakePtrConst.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_MAKEPTRCONST_H_INCLUDED__
#define __UT_MAKEPTRCONST_H_INCLUDED__

/// @file
/// UT_MakePtrConst adds const to a pointer.
///
/// For example, it will turn "int*" into "const int*" 
/// 
/// NOTE:   This template will NOT WORK for FUNCTION POINTER types.
///	    The previous incarnation of this code used hboost::mpl to handle
///	    this case but gcc 3.3.6 would periodically crash.
///
/// Usage: 
/// @code
///	typedef typename UT_MakePtrConst<int *>::type ConstIntPtr;
/// @endcode
///
/// It is safe to use this when the pointer type argument is
/// already const. In that case, the resulting type will be 
/// equivalent to the input type.

template< typename V >
struct UT_MakePtrConst;

template< typename V >
struct UT_MakePtrConst<V*>
{
    typedef const V* type;
};

#endif // __UT_MAKEPTRCONST_H_INCLUDED__
