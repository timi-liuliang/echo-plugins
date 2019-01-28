/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_VALUE_OF_IMPL_20060124_2147)
#define HBOOST_FUSION_VALUE_OF_IMPL_20060124_2147

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/convert.hpp>
#include <hboost/fusion/algorithm/transformation/transform.hpp>
#include <hboost/fusion/iterator/value_of.hpp>
#include <hboost/mpl/placeholders.hpp>
#include <hboost/fusion/support/unused.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/config.hpp>

namespace hboost { namespace fusion
{
    struct zip_view_iterator_tag;

    namespace detail
    {
        struct poly_value_of
        {
            template<typename T>
            struct result;

            template<typename It>
            struct result<poly_value_of(It)>
                : mpl::eval_if<is_same<It, unused_type>,
                               mpl::identity<unused_type>,
                               result_of::value_of<It> >
            {};

            // never called, but needed for decltype-based result_of (C++0x)
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
            template<typename It>
            HBOOST_FUSION_GPU_ENABLED
            typename result<poly_value_of(It)>::type
            operator()(It&&) const;
#endif
        };
    }

    namespace extension
    {
        template<typename Tag>
        struct value_of_impl;

        template<>
        struct value_of_impl<zip_view_iterator_tag>
        {
            template<typename Iterator>
            struct apply
            {
                typedef typename result_of::transform<
                    typename Iterator::iterators,
                    detail::poly_value_of>::type values;

                typedef typename result_of::as_vector<values>::type type;
            };
        };
    }
}}

#endif
