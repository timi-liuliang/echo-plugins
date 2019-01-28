/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Compare.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_COMPARE_H_INCLUDED__
#define __UT_COMPARE_H_INCLUDED__

#include "UT_API.h"

namespace UT_Compare
{

template <typename T>
using Ternary = int (*)(const T*, const T*);

template <typename T>
class Less
{
public:
    Less(Ternary<T> compare) : myCompare(compare)
	{ }
    bool operator()(const T& lhs, const T& rhs)
	{ return myCompare(&lhs, &rhs) < 0; }
private:
    Ternary<T> myCompare;
};

template <typename T>
class Greater
{
public:
    Greater(Ternary<T> compare) : myCompare(compare)
	{ }
    bool operator()(const T& lhs, const T& rhs)
	{ return myCompare(&lhs, &rhs) > 0; }
private:
    Ternary<T> myCompare;
};

template <typename T>
class Equal
{
public:
    Equal(Ternary<T> compare) : myCompare(compare)
	{ }
    bool operator()(const T& lhs, const T& rhs)
	{ return myCompare(&lhs, &rhs) == 0; }
private:
    Ternary<T> myCompare;
};

} // namespace UT_Compare

/// Adaptor to convert C-style ternary comparators to C++-style bool functors
/// @{
template <typename T>
UT_Compare::Less<T>
UTcompareLess(UT_Compare::Ternary<T> compare)
{
    return UT_Compare::Less<T>(compare);
}

template <typename T>
UT_Compare::Greater<T>
UTcompareGreater(UT_Compare::Ternary<T> compare)
{
    return UT_Compare::Greater<T>(compare);
}

template <typename T>
UT_Compare::Equal<T>
UTcompareEqual(UT_Compare::Ternary<T> compare)
{
    return UT_Compare::Equal<T>(compare);
}
/// @}

#endif // __UT_COMPARE_H_INCLUDED__
