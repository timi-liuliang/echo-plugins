/*=============================================================================
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HBOOST_FUSION_ALGORITHM_ITERATION_FOLD_FWD_HPP
#define HBOOST_FUSION_ALGORITHM_ITERATION_FOLD_FWD_HPP

namespace hboost { namespace fusion
{
    namespace result_of
    {
        template<typename Seq, typename State, typename F>
        struct fold;
    }

    template<typename Seq, typename State, typename F>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::fold<
        Seq
      , State const
      , F
    >::type
    fold(Seq& seq, State const& state, F f);

    template<typename Seq, typename State, typename F>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::fold<
        Seq const
      , State const
      , F
    >::type
    fold(Seq const& seq, State const& state, F f);

    template<typename Seq, typename State, typename F>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::fold<
        Seq
      , State
      , F
    >::type
    fold(Seq& seq, State& state, F f);

    template<typename Seq, typename State, typename F>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::fold<
        Seq const
      , State
      , F
    >::type
    fold(Seq const& seq, State& state, F f);
}}

#endif
