/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_JOINT_VIEW_07162005_0140)
#define HBOOST_FUSION_JOINT_VIEW_07162005_0140

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/view/joint_view/joint_view_fwd.hpp>
#include <hboost/fusion/support/detail/access.hpp>
#include <hboost/fusion/support/is_view.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/sequence/intrinsic/end.hpp>
#include <hboost/fusion/sequence/intrinsic/size.hpp>
#include <hboost/fusion/view/joint_view/joint_view_iterator.hpp>
#include <hboost/fusion/view/joint_view/detail/begin_impl.hpp>
#include <hboost/fusion/view/joint_view/detail/end_impl.hpp>
#include <hboost/fusion/support/sequence_base.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/plus.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/inherit.hpp>
#include <hboost/mpl/identity.hpp>

namespace hboost { namespace fusion
{
    struct joint_view_tag;
    struct forward_traversal_tag;
    struct fusion_sequence_tag;

    template <typename Sequence1, typename Sequence2>
    struct joint_view : sequence_base<joint_view<Sequence1, Sequence2> >
    {
        typedef joint_view_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef typename
            mpl::eval_if<
                mpl::and_<
                    traits::is_associative<Sequence1>
                  , traits::is_associative<Sequence2>
                >
              , mpl::inherit2<forward_traversal_tag,associative_tag>
              , mpl::identity<forward_traversal_tag>
            >::type
        category;
        typedef mpl::true_ is_view;

        typedef typename result_of::begin<Sequence1>::type first_type;
        typedef typename result_of::end<Sequence1>::type last_type;
        typedef typename result_of::begin<Sequence2>::type concat_type;
        typedef typename result_of::end<Sequence2>::type concat_last_type;
        typedef typename mpl::int_<
            result_of::size<Sequence1>::value + result_of::size<Sequence2>::value>
        size;

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        joint_view(Sequence1& in_seq1, Sequence2& in_seq2)
            : seq1(in_seq1)
            , seq2(in_seq2)
        {}

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        first_type first() const { return fusion::begin(seq1); }
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        concat_type concat() const { return fusion::begin(seq2); }
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        concat_last_type concat_last() const { return fusion::end(seq2); }

    private:
        // silence MSVC warning C4512: assignment operator could not be generated
        joint_view& operator= (joint_view const&);

        typename mpl::if_<traits::is_view<Sequence1>, Sequence1, Sequence1&>::type seq1;
        typename mpl::if_<traits::is_view<Sequence2>, Sequence2, Sequence2&>::type seq2;
    };
}}

#endif


