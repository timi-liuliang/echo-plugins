/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_BEGIN_IMPL_07172005_0824)
#define HBOOST_FUSION_BEGIN_IMPL_07172005_0824

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/type_traits/is_const.hpp>

namespace hboost { namespace fusion
{
    struct nil_;

    struct cons_tag;

    template <typename Car, typename Cdr>
    struct cons;

    template <typename Cons>
    struct cons_iterator;

    namespace extension
    {
        template <typename Tag>
        struct begin_impl;

        template <>
        struct begin_impl<cons_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef cons_iterator<Sequence> type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type
                call(Sequence& t)
                {
                    return type(t);
                }
            };
        };
    }
}}

#endif
