/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_SWAP_20070501_1956)
#define HBOOST_FUSION_SWAP_20070501_1956

#include <hboost/fusion/support/config.hpp>
#include <algorithm>

#include <hboost/fusion/support/is_sequence.hpp>
#include <hboost/fusion/view/zip_view.hpp>
#include <hboost/fusion/algorithm/iteration/for_each.hpp>
#include <hboost/fusion/sequence/intrinsic/front.hpp>
#include <hboost/fusion/sequence/intrinsic/back.hpp>
#include <hboost/core/enable_if.hpp>
#include <hboost/mpl/and.hpp>

namespace hboost { namespace fusion {

    namespace result_of
    {
        template<typename Seq1, typename Seq2>
        struct swap
            : enable_if<mpl::and_<
                  traits::is_sequence<Seq1>,
                  traits::is_sequence<Seq2>
              > > {};
    }

    namespace detail
    {
        struct swap
        {
            template<typename Elem>
            struct result
            {
                typedef void type;
            };

            template<typename Elem>
            HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
            void operator()(Elem const& e) const
            {
                using std::swap;
                swap(front(e), back(e));
            }
        };
    }

    template<typename Seq1, typename Seq2>
    HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::swap<Seq1, Seq2>::type
    swap(Seq1& lhs, Seq2& rhs)
    {
        typedef vector<Seq1&, Seq2&> references;
        for_each(zip_view<references>(references(lhs, rhs)), detail::swap());
    }
}}

#endif
