/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_ITERATOR_FACADE_09252006_1011)
#define HBOOST_FUSION_ITERATOR_FACADE_09252006_1011

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/support/iterator_base.hpp>
#include <hboost/fusion/iterator/detail/advance.hpp>
#include <hboost/fusion/iterator/detail/distance.hpp>
#include <hboost/fusion/support/category_of.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/mpl/assert.hpp>
#include <hboost/mpl/if.hpp>

namespace hboost { namespace fusion
{
    struct iterator_facade_tag;

    template <typename Derived, typename Category>
    struct iterator_facade : iterator_base<Derived>
    {
        typedef iterator_facade_tag fusion_tag;
        typedef Derived derived_type;
        typedef Category category;

        // default implementation
        template <typename I1, typename I2>
        struct equal_to // default implementation
            : is_same<
                typename I1::derived_type
              , typename I2::derived_type
            >
        {};

        // default implementation
        template <typename Iterator, typename N>
        struct advance :
            mpl::if_c<
                (N::value > 0)
              , advance_detail::forward<Iterator, N::value>
              , advance_detail::backward<Iterator, N::value>
            >::type
        {
            HBOOST_MPL_ASSERT_NOT((traits::is_random_access<Iterator>));
        };

        // default implementation
        template <typename First, typename Last>
        struct distance :
            distance_detail::linear_distance<First, Last>
        {};
    };
}}

#ifdef HBOOST_FUSION_WORKAROUND_FOR_LWG_2408
namespace std
{
    template <typename Derived, typename Category>
    struct iterator_traits< ::hboost::fusion::iterator_facade<Derived, Category> >
    { };
}
#endif

#endif
