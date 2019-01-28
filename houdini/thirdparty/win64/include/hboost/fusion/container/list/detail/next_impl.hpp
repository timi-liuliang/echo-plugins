/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_NEXT_IMPL_07172005_0836)
#define HBOOST_FUSION_NEXT_IMPL_07172005_0836

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/iterator/next.hpp>
#include <hboost/fusion/iterator/equal_to.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/type_traits/is_const.hpp>
#include <hboost/type_traits/add_const.hpp>

namespace hboost { namespace fusion
{
    struct cons_iterator_tag;

    template <typename Cons>
    struct cons_iterator;

    namespace extension
    {
        template <typename Tag>
        struct next_impl;

        template <>
        struct next_impl<cons_iterator_tag>
        {
            template <typename Iterator>
            struct apply
            {
                typedef typename Iterator::cons_type cons_type;
                typedef typename cons_type::cdr_type cdr_type;

                typedef cons_iterator<
                    typename mpl::eval_if<
                        is_const<cons_type>
                      , add_const<cdr_type>
                      , mpl::identity<cdr_type>
                    >::type>
                type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type
                call(Iterator const& i)
                {
                    return type(i.cons.cdr);
                }
            };
        };
    }
}}

#endif


