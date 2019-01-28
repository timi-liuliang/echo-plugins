///////////////////////////////////////////////////////////////////////////////
// weight.hpp
//
//  Copyright 2005 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_ACCUMULATORS_FRAMEWORK_PARAMETERS_WEIGHT_HPP_EAN_31_10_2005
#define HBOOST_ACCUMULATORS_FRAMEWORK_PARAMETERS_WEIGHT_HPP_EAN_31_10_2005

#include <hboost/parameter/keyword.hpp>
#include <hboost/accumulators/accumulators_fwd.hpp>

namespace hboost { namespace accumulators
{

// The weight of a single sample
HBOOST_PARAMETER_KEYWORD(tag, weight)
HBOOST_ACCUMULATORS_IGNORE_GLOBAL(weight)

}} // namespace hboost::accumulators

#endif