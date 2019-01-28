/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#pragma once

#ifndef __UT_OPTIONAL_H_INCLUDED__
#define __UT_OPTIONAL_H_INCLUDED__

#include <hboost/optional.hpp>

template <typename T>
using UT_Optional = hboost::optional<T>;

#endif
