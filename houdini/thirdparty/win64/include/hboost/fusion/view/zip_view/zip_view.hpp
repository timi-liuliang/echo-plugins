/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_ZIP_VIEW_23012006_0813)
#define HBOOST_FUSION_ZIP_VIEW_23012006_0813

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/support/sequence_base.hpp>
#include <hboost/fusion/support/unused.hpp>
#include <hboost/fusion/iterator/equal_to.hpp>
#include <hboost/fusion/view/detail/strictest_traversal.hpp>
#include <hboost/fusion/view/zip_view/detail/begin_impl.hpp>
#include <hboost/fusion/view/zip_view/detail/end_impl.hpp>
#include <hboost/fusion/view/zip_view/detail/size_impl.hpp>
#include <hboost/fusion/view/zip_view/detail/at_impl.hpp>
#include <hboost/fusion/view/zip_view/detail/value_at_impl.hpp>
#include <hboost/fusion/container/vector/convert.hpp>
#include <hboost/fusion/algorithm/query/find_if.hpp>
#include <hboost/fusion/sequence/intrinsic/end.hpp>
#include <hboost/fusion/sequence/intrinsic/size.hpp>
#include <hboost/fusion/mpl.hpp>
#include <hboost/fusion/algorithm/transformation/remove.hpp>

#include <hboost/mpl/assert.hpp>
#include <hboost/mpl/not.hpp>
#include <hboost/mpl/placeholders.hpp>
#include <hboost/mpl/transform_view.hpp>
#include <hboost/mpl/at.hpp>
#include <hboost/mpl/find_if.hpp>
#include <hboost/mpl/equal_to.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/eval_if.hpp>

#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/type_traits/is_reference.hpp>

#include <hboost/config.hpp>

namespace hboost { namespace fusion {

    namespace detail
    {
        template<typename Sequences>
        struct all_references
            : fusion::result_of::equal_to<typename fusion::result_of::find_if<Sequences, mpl::not_<is_reference<mpl::_> > >::type, typename fusion::result_of::end<Sequences>::type>
        {};

        struct seq_ref_size
        {
            template<typename Params>
            struct result;

            template<typename Seq>
            struct result<seq_ref_size(Seq)>
            {
                static int const high_int = static_cast<int>(
                    (static_cast<unsigned>(~0) >> 1) - 1);

                typedef typename remove_reference<Seq>::type SeqClass;

                typedef typename mpl::eval_if<
                    traits::is_forward<SeqClass>,
                    result_of::size<SeqClass>,
                    mpl::int_<high_int> >::type type;
            };

            // never called, but needed for decltype-based result_of (C++0x)
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
            template<typename Seq>
            HBOOST_FUSION_GPU_ENABLED
            typename result<seq_ref_size(Seq)>::type
            operator()(Seq&&) const;
#endif
        };

        struct poly_min
        {
            template<typename T>
            struct result;

            template<typename Lhs, typename Rhs>
            struct result<poly_min(Lhs, Rhs)>
            {
                typedef typename remove_reference<Lhs>::type lhs;
                typedef typename remove_reference<Rhs>::type rhs;
                typedef typename mpl::min<lhs, rhs>::type type;
            };

            // never called, but needed for decltype-based result_of (C++0x)
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
            template<typename Lhs, typename Rhs>
            HBOOST_FUSION_GPU_ENABLED
            typename result<poly_min(Lhs, Rhs)>::type
            operator()(Lhs&&, Rhs&&) const;
#endif
        };

        template<typename Sequences>
        struct min_size
        {
            typedef typename result_of::transform<Sequences, detail::seq_ref_size>::type sizes;
            typedef typename result_of::fold<sizes, typename result_of::front<sizes>::type, detail::poly_min>::type type;
        };
    }

    struct zip_view_tag;
    struct fusion_sequence_tag;

    template<typename Sequences>
    struct zip_view : sequence_base< zip_view<Sequences> >
    {
        typedef typename result_of::remove<Sequences, unused_type const&>::type real_sequences;
        HBOOST_MPL_ASSERT((detail::all_references<Sequences>));
        typedef typename detail::strictest_traversal<real_sequences>::type category;
        typedef zip_view_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::true_ is_view;
        typedef typename fusion::result_of::as_vector<Sequences>::type sequences;
        typedef typename detail::min_size<real_sequences>::type size;

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        zip_view(
            const Sequences& seqs)
            : sequences_(seqs)
        {};

        sequences sequences_;
    };
}}

#endif
