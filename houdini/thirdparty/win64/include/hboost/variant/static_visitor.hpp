//-----------------------------------------------------------------------------
// boost variant/static_visitor.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_STATIC_VISITOR_HPP
#define HBOOST_VARIANT_STATIC_VISITOR_HPP

#include "hboost/config.hpp"
#include "hboost/detail/workaround.hpp"

#include "hboost/mpl/if.hpp"
#include "hboost/type_traits/is_base_and_derived.hpp"

#include <hboost/type_traits/integral_constant.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

namespace hboost {

//////////////////////////////////////////////////////////////////////////
// class template static_visitor
//
// An empty base class that typedefs the return type of a deriving static
// visitor. The class is analogous to std::unary_function in this role.
//

namespace detail {

    struct is_static_visitor_tag { };

    typedef void static_visitor_default_return;

} // namespace detail

template <typename R = ::hboost::detail::static_visitor_default_return>
class static_visitor
    : public detail::is_static_visitor_tag
{
public: // typedefs

    typedef R result_type;

protected: // for use as base class only
#if !defined(HBOOST_NO_CXX11_DEFAULTED_FUNCTIONS) && !defined(HBOOST_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS)
    static_visitor() = default;
    ~static_visitor() = default;
#else
    static_visitor()  HBOOST_NOEXCEPT { }
    ~static_visitor()  HBOOST_NOEXCEPT { }
#endif
};

//////////////////////////////////////////////////////////////////////////
// metafunction is_static_visitor
//
// Value metafunction indicates whether the specified type derives from
// static_visitor<...>.
//
// NOTE #1: This metafunction does NOT check whether the specified type
//  fulfills the requirements of the StaticVisitor concept.
//
// NOTE #2: This template never needs to be specialized!
//

namespace detail {

template <typename T>
struct is_static_visitor_impl
{
    HBOOST_STATIC_CONSTANT(bool, value = 
        (::hboost::is_base_and_derived< 
            detail::is_static_visitor_tag,
            T
        >::value));
};

} // namespace detail

template< typename T > struct is_static_visitor
	: public ::hboost::integral_constant<bool,(::hboost::detail::is_static_visitor_impl<T>::value)>
{
public:
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_static_visitor,(T))
};

} // namespace hboost

#endif // HBOOST_VARIANT_STATIC_VISITOR_HPP
