//-----------------------------------------------------------------------------
// boost variant/detail/substitute_fwd.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_DETAIL_SUBSTITUTE_FWD_HPP
#define HBOOST_VARIANT_DETAIL_SUBSTITUTE_FWD_HPP

#include "hboost/mpl/aux_/lambda_arity_param.hpp"
#include "hboost/mpl/aux_/template_arity.hpp"
#include "hboost/mpl/int_fwd.hpp"


///////////////////////////////////////////////////////////////////////////////
// HBOOST_VARIANT_DETAIL_NO_SUBSTITUTE
//
// Defined if 'substitute' is not implementable on the current compiler.
//

#include "hboost/mpl/aux_/config/ctps.hpp"
#include "hboost/mpl/aux_/config/ttp.hpp"

#if defined(HBOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS) \
 && !defined(HBOOST_VARIANT_DETAIL_NO_SUBSTITUTE)
#   define HBOOST_VARIANT_DETAIL_NO_SUBSTITUTE
#endif

namespace hboost {
namespace detail { namespace variant {

#if !defined(HBOOST_VARIANT_DETAIL_NO_SUBSTITUTE)

///////////////////////////////////////////////////////////////////////////////
// metafunction substitute
//
// Substitutes one type for another in the given type expression.
//
template <
      typename T, typename Dest, typename Source
      HBOOST_MPL_AUX_LAMBDA_ARITY_PARAM(
          typename Arity = mpl::int_< mpl::aux::template_arity<T>::value >
        )
    >
struct substitute;

#endif // !defined(HBOOST_VARIANT_DETAIL_NO_SUBSTITUTE)

}} // namespace detail::variant
} // namespace hboost

#endif // HBOOST_VARIANT_DETAIL_SUBSTITUTE_FWD_HPP
