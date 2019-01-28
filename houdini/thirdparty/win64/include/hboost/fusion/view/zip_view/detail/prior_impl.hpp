/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_PRIOR_IMPL_20060124_2006)
#define HBOOST_FUSION_PRIOR_IMPL_20060124_2006

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/view/zip_view/zip_view_iterator_fwd.hpp>
#include <hboost/fusion/iterator/prior.hpp>
#include <hboost/fusion/algorithm/transformation/transform.hpp>
#include <hboost/fusion/support/unused.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/type_traits/remove_const.hpp>

namespace hboost { namespace fusion {

    struct zip_view_iterator_tag;
    
    namespace detail
    {
        struct poly_prior
        {
            template<typename Sig>
            struct result;

            template<typename It>
            struct result<poly_prior(It)>
            {
                typedef typename remove_const<
                    typename remove_reference<It>::type>::type it;
                typedef typename mpl::eval_if<is_same<it, unused_type>,
                    mpl::identity<unused_type>,
                    result_of::prior<it> >::type type;
            };

            template<typename It>
            HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
            typename result<poly_prior(It)>::type
            operator()(const It& it) const
            {
                return fusion::prior(it);
            }

            HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
            unused_type operator()(unused_type const&) const
            {
                return unused_type();
            }
        };
    }

    namespace extension
    {
        template<typename Tag>
        struct prior_impl;

        template<>
        struct prior_impl<zip_view_iterator_tag>
        {
            template<typename Iterator>
            struct apply
            {
                typedef zip_view_iterator<
                    typename result_of::transform<typename Iterator::iterators, detail::poly_prior>::type,
                    typename Iterator::category> type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type
                call(Iterator const& it)

                {
                    return type(
                        fusion::transform(it.iterators_, detail::poly_prior()));
                }
            };
        };
    }
}}

#endif
