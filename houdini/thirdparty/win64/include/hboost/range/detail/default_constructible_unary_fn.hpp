// Boost.Range library
//
//  Copyright Neil Groves 2014. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_DETAIL_DEFAULT_CONSTRUCTIBLE_UNARY_FN_HPP_INCLUDED
#define HBOOST_RANGE_DETAIL_DEFAULT_CONSTRUCTIBLE_UNARY_FN_HPP_INCLUDED

#include <hboost/optional/optional.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/type_traits/has_trivial_constructor.hpp>

namespace hboost
{
    namespace range_detail
    {

template<typename F, typename R>
class default_constructible_unary_fn_wrapper
{
public:
    typedef R result_type;

    default_constructible_unary_fn_wrapper()
    {
    }
    default_constructible_unary_fn_wrapper(const F& source)
        : m_impl(source)
    {
    }
    template<typename Arg>
    R operator()(const Arg& arg) const
    {
        HBOOST_ASSERT(m_impl);
        return (*m_impl)(arg);
    }
    template<typename Arg>
    R operator()(Arg& arg) const
    {
        HBOOST_ASSERT(m_impl);
        return (*m_impl)(arg);
    }
private:
    hboost::optional<F> m_impl;
};

template<typename F, typename R>
struct default_constructible_unary_fn_gen
{
    typedef typename hboost::mpl::if_<
        hboost::has_trivial_default_constructor<F>,
        F,
        default_constructible_unary_fn_wrapper<F,R>
    >::type type;
};

    } // namespace range_detail
} // namespace hboost

#endif // include guard
