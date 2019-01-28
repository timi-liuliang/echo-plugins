
#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

#ifndef HBOOST_MPL_AUX778076_ADVANCE_BACKWARD_HPP_INCLUDED
#define HBOOST_MPL_AUX778076_ADVANCE_BACKWARD_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#if !defined(HBOOST_MPL_PREPROCESSING_MODE)
#   include <hboost/mpl/prior.hpp>
#   include <hboost/mpl/apply_wrap.hpp>
#endif

#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if    !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER advance_backward.hpp
#   include <hboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <hboost/mpl/limits/unrolling.hpp>
#   include <hboost/mpl/aux_/nttp_decl.hpp>
#   include <hboost/mpl/aux_/config/eti.hpp>

#   include <hboost/preprocessor/iterate.hpp>
#   include <hboost/preprocessor/cat.hpp>
#   include <hboost/preprocessor/inc.hpp>

namespace hboost { namespace mpl { namespace aux {

// forward declaration
template< HBOOST_MPL_AUX_NTTP_DECL(long, N) > struct advance_backward;

#   define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, HBOOST_MPL_LIMIT_UNROLLING, <hboost/mpl/aux_/advance_backward.hpp>))
#   include HBOOST_PP_ITERATE()

// implementation for N that exceeds HBOOST_MPL_LIMIT_UNROLLING
template< HBOOST_MPL_AUX_NTTP_DECL(long, N) >
struct advance_backward
{
    template< typename Iterator > struct apply
    {
        typedef typename apply_wrap1<
              advance_backward<HBOOST_MPL_LIMIT_UNROLLING>
            , Iterator
            >::type chunk_result_;

        typedef typename apply_wrap1<
              advance_backward<(
                (N - HBOOST_MPL_LIMIT_UNROLLING) < 0
                    ? 0
                    : N - HBOOST_MPL_LIMIT_UNROLLING
                    )>
            , chunk_result_
            >::type type;
    };
};

}}}

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // HBOOST_MPL_AUX778076_ADVANCE_BACKWARD_HPP_INCLUDED

///// iteration, depth == 1

// For gcc 4.4 compatability, we must include the
// HBOOST_PP_ITERATION_DEPTH test inside an #else clause.
#else // HBOOST_PP_IS_ITERATING
#if HBOOST_PP_ITERATION_DEPTH() == 1
#define i_ HBOOST_PP_FRAME_ITERATION(1)

template<>
struct advance_backward< HBOOST_PP_FRAME_ITERATION(1) >
{
    template< typename Iterator > struct apply
    {
        typedef Iterator iter0;

#if i_ > 0
#   define HBOOST_PP_ITERATION_PARAMS_2 \
    (3,(1, HBOOST_PP_FRAME_ITERATION(1), <hboost/mpl/aux_/advance_backward.hpp>))
#   include HBOOST_PP_ITERATE()
#endif

        typedef HBOOST_PP_CAT(iter,HBOOST_PP_FRAME_ITERATION(1)) type;
    };

#if defined(HBOOST_MPL_CFG_MSVC_60_ETI_BUG)
    /// ETI workaround
    template<> struct apply<int>
    {
        typedef int type;
    };
#endif
};

#undef i_

///// iteration, depth == 2

#elif HBOOST_PP_ITERATION_DEPTH() == 2

#   define AUX778076_ITER_0 HBOOST_PP_CAT(iter,HBOOST_PP_DEC(HBOOST_PP_FRAME_ITERATION(2)))
#   define AUX778076_ITER_1 HBOOST_PP_CAT(iter,HBOOST_PP_FRAME_ITERATION(2))

        typedef typename prior<AUX778076_ITER_0>::type AUX778076_ITER_1;
        
#   undef AUX778076_ITER_1
#   undef AUX778076_ITER_0

#endif // HBOOST_PP_ITERATION_DEPTH()
#endif // HBOOST_PP_IS_ITERATING
