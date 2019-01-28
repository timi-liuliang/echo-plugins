/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_StdUtil.h (UT Library, C++)
 *
 * COMMENTS:
 */

/// @file Adaptors between UT and std classes

#ifndef __UT_STDUTIL_H_INCLUDED__
#define __UT_STDUTIL_H_INCLUDED__

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_FixedVector.h"
#include "UT_StringHolder.h"
#include "UT_WorkArgs.h"
#include <SYS/SYS_Types.h>

#include <string>
#include <vector>

template <typename T>
static inline void
UTarrayFromStdVector(UT_Array<T> &dst, const std::vector<T> &src)
{
    dst.clear();
    for (exint i = 0, n = src.size(); i < n; ++i)
	dst.append(src[i]);
}

template <typename T>
static inline void
UTarrayToStdVector(const UT_Array<T> &src, std::vector<T> &dst)
{
    dst.clear();
    for (exint i = 0, n = src.size(); i < n; ++i)
	dst.push_back(src[i]);
}

inline void
UTarrayFromStdVectorOfStrings(
	UT_Array<UT_StringHolder> &dst, const std::vector<std::string> &src)
{
    dst.clear();
    for (exint i = 0, n = src.size(); i < n; ++i)
	dst.append(src[i]);
}

static inline void
UTarrayToStdVectorOfStrings(
	const UT_Array<UT_StringHolder> &src, std::vector<std::string> &dst)
{
    dst.clear();
    for (exint i = 0, n = src.size(); i < n; ++i)
	dst.push_back(src[i].c_str());
}

static inline void
UTargsToStringVector(const UT_WorkArgs &src, std::vector<std::string> &dst)
{
    dst.clear();
    for (exint i = 0, n = src.getArgc(); i < n; ++i)
	dst.push_back(src.getArg(i));
}

template <typename T, exint SIZE, bool INSTANTIATED>
static inline std::vector<T>
UTasStdVector(const UT_FixedVector<T,SIZE,INSTANTIATED> &src)
{
    std::vector<T> dst(SIZE);
    for (exint i = 0; i < SIZE; ++i)
	dst[i] = src[i];
    return dst;
}

#endif // __UT_STDUTIL_H_INCLUDED__
