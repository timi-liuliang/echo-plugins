//
//! Copyright (c) 2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef HBOOST_NUMERIC_CAST_TRAITS_HPP
#define HBOOST_NUMERIC_CAST_TRAITS_HPP

#include <hboost/numeric/conversion/converter_policies.hpp>

namespace hboost { namespace numeric {

    template <typename Target, typename Source, typename EnableIf = void>
    struct numeric_cast_traits
    {
        typedef def_overflow_handler    overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<Source>           rounding_policy;
    };

}}//namespace hboost::numeric;

#if !defined( HBOOST_NUMERIC_CONVERSION_RELAX_BUILT_IN_CAST_TRAITS )
#include <hboost/cstdint.hpp>
#include <hboost/numeric/conversion/detail/numeric_cast_traits.hpp>
#endif//!defined HBOOST_NUMERIC_CONVERSION_RELAX_BUILT_IN_CAST_TRAITS

#endif//HBOOST_NUMERIC_CAST_TRAITS_HPP
