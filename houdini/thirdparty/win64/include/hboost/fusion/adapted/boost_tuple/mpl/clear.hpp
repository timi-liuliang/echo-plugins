/*=============================================================================
    Copyright (c) 2012 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_CLEAR_10172012_0100)
#define HBOOST_FUSION_CLEAR_10172012_0100

#include <hboost/mpl/identity.hpp>
#include <hboost/fusion/adapted/hboost_tuple/tag_of.hpp>

namespace hboost { namespace fusion { namespace detail {

  template <typename Tag>
  struct clear;

  template <>
  struct clear<hboost_tuple_tag> : mpl::identity<hboost::tuple<> > {};

}}}

#endif
