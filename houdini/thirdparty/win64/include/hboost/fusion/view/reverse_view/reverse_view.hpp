/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_REVERSE_VIEW_07202005_0836)
#define FUSION_REVERSE_VIEW_07202005_0836

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/support/detail/access.hpp>
#include <hboost/fusion/support/is_view.hpp>
#include <hboost/fusion/support/category_of.hpp>
#include <hboost/fusion/view/reverse_view/reverse_view_iterator.hpp>
#include <hboost/fusion/view/reverse_view/detail/begin_impl.hpp>
#include <hboost/fusion/view/reverse_view/detail/end_impl.hpp>
#include <hboost/fusion/view/reverse_view/detail/at_impl.hpp>
#include <hboost/fusion/view/reverse_view/detail/value_at_impl.hpp>
#include <hboost/fusion/support/sequence_base.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/sequence/intrinsic/end.hpp>
#include <hboost/fusion/sequence/intrinsic/size.hpp>
#include <hboost/type_traits/is_base_of.hpp>
#include <hboost/static_assert.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/inherit.hpp>
#include <hboost/mpl/identity.hpp>

namespace hboost { namespace fusion
{
    struct reverse_view_tag;
    struct fusion_sequence_tag;

    template <typename Sequence>
    struct reverse_view : sequence_base<reverse_view<Sequence> >
    {
        typedef reverse_view_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::true_ is_view;

        typedef Sequence seq_type;
        typedef typename traits::category_of<Sequence>::type category;
        typedef typename result_of::begin<Sequence>::type first_type;
        typedef typename result_of::end<Sequence>::type last_type;
        typedef typename result_of::size<Sequence>::type size;

        HBOOST_STATIC_ASSERT((
            is_base_of<
                bidirectional_traversal_tag
              , typename traits::category_of<first_type>::type>::value));

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        reverse_view(Sequence& in_seq)
            : seq(in_seq)
        {}

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        first_type first() const { return fusion::begin(seq); }
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        last_type last() const { return fusion::end(seq); }
        typename mpl::if_<traits::is_view<Sequence>, Sequence, Sequence&>::type seq;

    private:
        // silence MSVC warning C4512: assignment operator could not be generated
        reverse_view& operator= (reverse_view const&);
    };
}}

#endif


