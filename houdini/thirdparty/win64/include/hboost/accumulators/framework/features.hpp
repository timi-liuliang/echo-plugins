///////////////////////////////////////////////////////////////////////////////
// features.hpp
//
//  Copyright 2005 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_ACCUMULATORS_STATISTICS_STATS_HPP_EAN_08_12_2005
#define HBOOST_ACCUMULATORS_STATISTICS_STATS_HPP_EAN_08_12_2005

#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/mpl/vector.hpp>
#include <hboost/accumulators/accumulators_fwd.hpp>

namespace hboost { namespace accumulators
{

///////////////////////////////////////////////////////////////////////////////
// features
//
template<HBOOST_PP_ENUM_PARAMS(HBOOST_ACCUMULATORS_MAX_FEATURES, typename Feature)>
struct features
  : mpl::vector<HBOOST_PP_ENUM_PARAMS(HBOOST_ACCUMULATORS_MAX_FEATURES, Feature)>
{
};

}} // namespace hboost::accumulators

#endif
