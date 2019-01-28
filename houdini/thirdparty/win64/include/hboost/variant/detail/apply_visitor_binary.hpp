//-----------------------------------------------------------------------------
// boost variant/detail/apply_visitor_binary.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003 Eric Friedman
// Copyright (c) 2014 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_DETAIL_APPLY_VISITOR_BINARY_HPP
#define HBOOST_VARIANT_DETAIL_APPLY_VISITOR_BINARY_HPP

#include "hboost/config.hpp"
#include "hboost/detail/workaround.hpp"
#include "hboost/variant/detail/generic_result_type.hpp"

#include "hboost/variant/detail/apply_visitor_unary.hpp"

#if HBOOST_WORKAROUND(__EDG__, HBOOST_TESTED_AT(302))
#include "hboost/utility/enable_if.hpp"
#include "hboost/mpl/not.hpp"
#include "hboost/type_traits/is_const.hpp"
#endif


#if !defined(HBOOST_NO_CXX14_DECLTYPE_AUTO) && !defined(HBOOST_NO_CXX11_DECLTYPE_N3276)
#   include "hboost/variant/detail/has_result_type.hpp"
#endif

namespace hboost {

//////////////////////////////////////////////////////////////////////////
// function template apply_visitor(visitor, visitable1, visitable2)
//
// Visits visitable1 and visitable2 such that their values (which we
// shall call x and y, respectively) are used as arguments in the
// expression visitor(x, y).
//

namespace detail { namespace variant {

template <typename Visitor, typename Value1>
class apply_visitor_binary_invoke
{
public: // visitor typedefs

    typedef typename Visitor::result_type
        result_type;

private: // representation

    Visitor& visitor_;
    Value1& value1_;

public: // structors

    apply_visitor_binary_invoke(Visitor& visitor, Value1& value1) HBOOST_NOEXCEPT
        : visitor_(visitor)
        , value1_(value1)
    {
    }

public: // visitor interfaces

    template <typename Value2>
        HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(result_type)
    operator()(Value2& value2)
    {
        return visitor_(value1_, value2);
    }

private:
    apply_visitor_binary_invoke& operator=(const apply_visitor_binary_invoke&);
};

template <typename Visitor, typename Visitable2>
class apply_visitor_binary_unwrap
{
public: // visitor typedefs

    typedef typename Visitor::result_type
        result_type;

private: // representation

    Visitor& visitor_;
    Visitable2& visitable2_;

public: // structors

    apply_visitor_binary_unwrap(Visitor& visitor, Visitable2& visitable2) HBOOST_NOEXCEPT
        : visitor_(visitor)
        , visitable2_(visitable2)
    {
    }

public: // visitor interfaces

    template <typename Value1>
        HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(result_type)
    operator()(Value1& value1)
    {
        apply_visitor_binary_invoke<
              Visitor
            , Value1
            > invoker(visitor_, value1);

        return hboost::apply_visitor(invoker, visitable2_);
    }

private:
    apply_visitor_binary_unwrap& operator=(const apply_visitor_binary_unwrap&);

};

}} // namespace detail::variant

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

template <typename Visitor, typename Visitable1, typename Visitable2>
inline
    HBOOST_VARIANT_AUX_APPLY_VISITOR_NON_CONST_RESULT_TYPE(Visitor)
apply_visitor(
      Visitor& visitor
    , Visitable1& visitable1, Visitable2& visitable2
    )
{
    ::hboost::detail::variant::apply_visitor_binary_unwrap<
          Visitor, Visitable2
        > unwrapper(visitor, visitable2);

    return hboost::apply_visitor(unwrapper, visitable1);
}

#undef HBOOST_VARIANT_AUX_APPLY_VISITOR_NON_CONST_RESULT_TYPE

//
// const-visitor version:
//

template <typename Visitor, typename Visitable1, typename Visitable2>
inline
    HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(
          typename Visitor::result_type
        )
apply_visitor(
      const Visitor& visitor
    , Visitable1& visitable1, Visitable2& visitable2
    )
{
    ::hboost::detail::variant::apply_visitor_binary_unwrap<
          const Visitor, Visitable2
        > unwrapper(visitor, visitable2);

    return hboost::apply_visitor(unwrapper, visitable1);
}


#if !defined(HBOOST_NO_CXX14_DECLTYPE_AUTO) && !defined(HBOOST_NO_CXX11_DECLTYPE_N3276)

//////////////////////////////////////////////////////////////////////////
// function template apply_visitor(visitor, visitable1, visitable2)
//
// C++14 part.
//

namespace detail { namespace variant {

template <typename Visitor, typename Value1>
class apply_visitor_binary_invoke_cpp14
{
    Visitor& visitor_;
    Value1& value1_;

public: // structors

    apply_visitor_binary_invoke_cpp14(Visitor& visitor, Value1& value1) HBOOST_NOEXCEPT
        : visitor_(visitor)
        , value1_(value1)
    {
    }

public: // visitor interfaces

    template <typename Value2>
    decltype(auto) operator()(Value2& value2)
    {
        return visitor_(value1_, value2);
    }

private:
    apply_visitor_binary_invoke_cpp14& operator=(const apply_visitor_binary_invoke_cpp14&);
};

template <typename Visitor, typename Visitable2>
class apply_visitor_binary_unwrap_cpp14
{
    Visitor& visitor_;
    Visitable2& visitable2_;

public: // structors

    apply_visitor_binary_unwrap_cpp14(Visitor& visitor, Visitable2& visitable2) HBOOST_NOEXCEPT
        : visitor_(visitor)
        , visitable2_(visitable2)
    {
    }

public: // visitor interfaces

    template <typename Value1>
    decltype(auto) operator()(Value1& value1)
    {
        apply_visitor_binary_invoke_cpp14<
              Visitor
            , Value1
            > invoker(visitor_, value1);

        return hboost::apply_visitor(invoker, visitable2_);
    }

private:
    apply_visitor_binary_unwrap_cpp14& operator=(const apply_visitor_binary_unwrap_cpp14&);
};

}} // namespace detail::variant

template <typename Visitor, typename Visitable1, typename Visitable2>
inline decltype(auto) apply_visitor(Visitor& visitor, Visitable1& visitable1, Visitable2& visitable2,
    typename hboost::disable_if<
        hboost::detail::variant::has_result_type<Visitor>
    >::type* = 0)
{
    ::hboost::detail::variant::apply_visitor_binary_unwrap_cpp14<
          Visitor, Visitable2
        > unwrapper(visitor, visitable2);

    return hboost::apply_visitor(unwrapper, visitable1);
}

template <typename Visitor, typename Visitable1, typename Visitable2>
inline decltype(auto) apply_visitor(const Visitor& visitor, Visitable1& visitable1, Visitable2& visitable2,
    typename hboost::disable_if<
        hboost::detail::variant::has_result_type<Visitor>
    >::type* = 0)
{
    ::hboost::detail::variant::apply_visitor_binary_unwrap_cpp14<
          const Visitor, Visitable2
        > unwrapper(visitor, visitable2);

    return hboost::apply_visitor(unwrapper, visitable1);
}

#endif // !defined(HBOOST_NO_CXX14_DECLTYPE_AUTO) && !defined(HBOOST_NO_CXX11_DECLTYPE_N3276)

} // namespace hboost

#endif // HBOOST_VARIANT_DETAIL_APPLY_VISITOR_BINARY_HPP
