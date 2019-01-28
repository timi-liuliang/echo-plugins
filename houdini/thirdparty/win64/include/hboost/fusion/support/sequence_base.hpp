/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2007 Tobias Schwinger

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_SEQUENCE_BASE_04182005_0737)
#define HBOOST_FUSION_SEQUENCE_BASE_04182005_0737

#include <hboost/config.hpp>
#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/begin_end_fwd.hpp>

namespace hboost { namespace fusion
{
    namespace detail
    {
        struct from_sequence_convertible_type
        {};
    }

    template <typename Sequence>
    struct sequence_base
    {
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        Sequence const&
        derived() const HBOOST_NOEXCEPT
        {
            return static_cast<Sequence const&>(*this);
        }

        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        Sequence&
        derived() HBOOST_NOEXCEPT
        {
            return static_cast<Sequence&>(*this);
        }

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        operator detail::from_sequence_convertible_type() const HBOOST_NOEXCEPT
        {
            return detail::from_sequence_convertible_type();
        }
    };

    struct fusion_sequence_tag;
}}

namespace hboost { namespace mpl
{
    // Deliberately break mpl::begin, so it doesn't lie that a Fusion sequence
    // is not an MPL sequence by returning mpl::void_.
    // In other words: Fusion Sequences are always MPL Sequences, but they can
    // be incompletely defined.
    template<> struct begin_impl< hboost::fusion::fusion_sequence_tag >;
}}

#endif
