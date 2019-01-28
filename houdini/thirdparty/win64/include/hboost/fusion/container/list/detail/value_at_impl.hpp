/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_VALUE_AT_IMPL_07172005_0952)
#define HBOOST_FUSION_VALUE_AT_IMPL_07172005_0952

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/support/detail/access.hpp>
#include <hboost/type_traits/is_const.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/bool.hpp>

namespace hboost { namespace fusion
{
    struct cons_tag;

    namespace extension
    {
        template <typename Tag>
        struct value_at_impl;

        template <>
        struct value_at_impl<cons_tag>
        {
            template <typename Sequence, typename N>
            struct apply
            {
                typedef typename
                    mpl::eval_if<
                        mpl::bool_<N::value == 0>
                      , mpl::identity<typename Sequence::car_type>
                      , apply<typename Sequence::cdr_type, mpl::int_<N::value-1> >
                    >::type
                type;
            };
        };
    }
}}

#endif
