//-----------------------------------------------------------------------------
// boost variant/detail/apply_visitor_unary.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003 Eric Friedman
// Copyright (c) 2014 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_DETAIL_APPLY_VISITOR_UNARY_HPP
#define HBOOST_VARIANT_DETAIL_APPLY_VISITOR_UNARY_HPP

#include "hboost/config.hpp"
#include "hboost/detail/workaround.hpp"
#include "hboost/variant/detail/generic_result_type.hpp"

#if HBOOST_WORKAROUND(__EDG__, HBOOST_TESTED_AT(302))
#include "hboost/core/enable_if.hpp"
#include "hboost/mpl/not.hpp"
#include "hboost/type_traits/is_const.hpp"
#endif

#if !defined(HBOOST_NO_CXX14_DECLTYPE_AUTO) && !defined(HBOOST_NO_CXX11_DECLTYPE_N3276)
#   include <hboost/mpl/distance.hpp>
#   include <hboost/mpl/advance.hpp>
#   include <hboost/mpl/deref.hpp>
#   include <hboost/mpl/size.hpp>
#   include <hboost/utility/declval.hpp>
#   include <hboost/core/enable_if.hpp>
#   include "hboost/variant/detail/has_result_type.hpp"
#endif

namespace hboost {

//////////////////////////////////////////////////////////////////////////
// function template apply_visitor(visitor, visitable)
//
// Visits visitable with visitor.
//

//
// nonconst-visitor version:
//

#if !HBOOST_WORKAROUND(__EDG__, HBOOST_TESTED_AT(302))

#   define HBOOST_VARIANT_AUX_APPLY_VISITOR_NON_CONST_RESULT_TYPE(V) \
    HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename V::result_type) \
    /**/

#else // EDG-based compilers

#   define HBOOST_VARIANT_AUX_APPLY_VISITOR_NON_CONST_RESULT_TYPE(V) \
    typename enable_if< \
          mpl::not_< is_const< V > > \
        , HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename V::result_type) \
        >::type \
    /**/

#endif // EDG-based compilers workaround

template <typename Visitor, typename Visitable>
inline
    HBOOST_VARIANT_AUX_APPLY_VISITOR_NON_CONST_RESULT_TYPE(Visitor)
apply_visitor(Visitor& visitor, Visitable& visitable)
{
    return visitable.apply_visitor(visitor);
}

#undef HBOOST_VARIANT_AUX_APPLY_VISITOR_NON_CONST_RESULT_TYPE

//
// const-visitor version:
//

template <typename Visitor, typename Visitable>
inline
    HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename Visitor::result_type)
apply_visitor(const Visitor& visitor, Visitable& visitable)
{
    return visitable.apply_visitor(visitor);
}


#if !defined(HBOOST_NO_CXX14_DECLTYPE_AUTO) && !defined(HBOOST_NO_CXX11_DECLTYPE_N3276)

// C++14
namespace detail { namespace variant {

// This class serves only metaprogramming purposes. none of its methods must be called at runtime!
template <class Visitor, class Variant>
struct result_multideduce1 {
    typedef typename Variant::types                 types;
    typedef typename hboost::mpl::begin<types>::type begin_it;
    typedef typename hboost::mpl::advance<
        begin_it, hboost::mpl::int_<hboost::mpl::size<types>::type::value - 1>
    >::type                                         last_it;

    // For metaprogramming purposes ONLY! Do not use this method (and class) at runtime!
    static Visitor& vis() HBOOST_NOEXCEPT {
        // Functions that work with lambdas must be defined in same translation unit.
        // Because of that, we can not use `hboost::decval<Visitor&>()` here.
        Visitor&(*f)() = 0; // pointer to function
        return f();
    }

    static decltype(auto) deduce_impl(last_it, unsigned /*helper*/) {
        typedef typename hboost::mpl::deref<last_it>::type value_t;
        return vis()( hboost::declval< value_t& >() );
    }

    template <class It>
    static decltype(auto) deduce_impl(It, unsigned helper) {
        typedef typename hboost::mpl::next<It>::type next_t;
        typedef typename hboost::mpl::deref<It>::type value_t;
        if (helper == hboost::mpl::distance<begin_it, It>::type::value) {
            return deduce_impl(next_t(), ++helper);
        }

        return vis()( hboost::declval< value_t& >() );
    }

    static decltype(auto) deduce() {
        return deduce_impl(begin_it(), 0);
    }
};

template <class Visitor, class Variant>
struct result_wrapper1
{
    typedef decltype(result_multideduce1<Visitor, Variant>::deduce()) result_type;

    Visitor& visitor_;
    explicit result_wrapper1(Visitor& visitor) HBOOST_NOEXCEPT
        : visitor_(visitor)
    {}

    template <class T>
    result_type operator()(T& val) const {
        return visitor_(val);
    }
};

}} // namespace detail::variant

template <typename Visitor, typename Visitable>
inline decltype(auto) apply_visitor(Visitor& visitor, Visitable& visitable,
    typename hboost::disable_if<
        hboost::detail::variant::has_result_type<Visitor>
    >::type* = 0)
{
    hboost::detail::variant::result_wrapper1<Visitor, Visitable> cpp14_vis(visitor);
    return visitable.apply_visitor(cpp14_vis);
}

template <typename Visitor, typename Visitable>
inline decltype(auto) apply_visitor(const Visitor& visitor, Visitable& visitable,
    typename hboost::disable_if<
        hboost::detail::variant::has_result_type<Visitor>
    >::type* = 0)
{
    hboost::detail::variant::result_wrapper1<const Visitor, Visitable> cpp14_vis(visitor);
    return visitable.apply_visitor(cpp14_vis);
}

#endif // !defined(HBOOST_NO_CXX14_DECLTYPE_AUTO) && !defined(HBOOST_NO_CXX11_DECLTYPE_N3276)

} // namespace hboost

#endif // HBOOST_VARIANT_DETAIL_APPLY_VISITOR_UNARY_HPP
