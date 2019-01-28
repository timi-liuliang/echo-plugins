/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_VALUE_AT_IMPL_20060124_2129)
#define HBOOST_FUSION_VALUE_AT_IMPL_20060124_2129

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/convert.hpp>
#include <hboost/fusion/algorithm/transformation/transform.hpp>
#include <hboost/fusion/sequence/intrinsic/value_at.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/fusion/support/unused.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/config.hpp>

namespace hboost { namespace fusion {
    
    struct zip_view_tag;

    namespace detail
    {
        template<typename N>
        struct poly_value_at
        {
            template<typename T>
            struct result;

            template<typename N1, typename Seq>
            struct result<poly_value_at<N1>(Seq)>
                : mpl::eval_if<is_same<Seq, unused_type const&>,
                               mpl::identity<unused_type>,
                               result_of::value_at<typename remove_reference<Seq>::type, N> >
            {};

            // never called, but needed for decltype-based result_of (C++0x)
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
            template<typename Seq>
            HBOOST_FUSION_GPU_ENABLED
            typename result<poly_value_at(Seq)>::type
            operator()(Seq&&) const;
#endif
        };
    }
    
    namespace extension
    {
        template<typename Tag>
        struct value_at_impl;

        template<>
        struct value_at_impl<zip_view_tag>
        {
            template<typename Sequence, typename N>
            struct apply
            {
                typedef typename result_of::transform<
                    typename Sequence::sequences, 
                    detail::poly_value_at<N> >::type values;
                typedef typename result_of::as_vector<values>::type type;
            };
        };
    }
}}

#endif
