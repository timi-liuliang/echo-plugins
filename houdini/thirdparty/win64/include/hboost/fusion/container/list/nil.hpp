/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005, 2014 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_NIL_04232014_0843)
#define HBOOST_FUSION_NIL_04232014_0843

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/list/cons_fwd.hpp>
#include <hboost/fusion/support/sequence_base.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/mpl/bool.hpp>

namespace hboost { namespace fusion
{
    struct void_;
    struct cons_tag;
    struct forward_traversal_tag;
    struct fusion_sequence_tag;

    struct nil_ : sequence_base<nil_>
    {
        typedef mpl::int_<0> size;
        typedef cons_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::false_ is_view;
        typedef forward_traversal_tag category;
        typedef void_ car_type;
        typedef void_ cdr_type;

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        nil_() HBOOST_NOEXCEPT {}

        template <typename Iterator>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        nil_(Iterator const& /*iter*/, mpl::true_ /*this_is_an_iterator*/) HBOOST_NOEXCEPT
        {}

        template <typename Iterator>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        void assign_from_iter(Iterator const& /*iter*/) HBOOST_NOEXCEPT
        {
        }
    };
}}

#endif

