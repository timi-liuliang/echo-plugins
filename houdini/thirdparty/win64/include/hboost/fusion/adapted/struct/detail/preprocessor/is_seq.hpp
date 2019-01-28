/*=============================================================================
    HBOOST_PP_VARIADICS version of HBOOST_PP_IS_SEQ inspired from 
    hboost/mpl/aux_/preprocessor/is_seq.hpp, original copyrights goes to :

    Copyright Paul Mensonides 2003
    Copyright Aleksey Gurtovoy 2003-2004

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

==============================================================================*/

#ifndef HBOOST_FUSION_ADAPTED_STRUCT_DETAIL_PREPROCESSOR_IS_SEQ_HPP
#define HBOOST_FUSION_ADAPTED_STRUCT_DETAIL_PREPROCESSOR_IS_SEQ_HPP

#include <hboost/preprocessor/punctuation/paren.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/config/config.hpp>

#if HBOOST_PP_VARIADICS

#define HBOOST_FUSION_PP_IS_SEQ(seq) HBOOST_PP_CAT(HBOOST_FUSION_PP_IS_SEQ_,       \
    HBOOST_FUSION_PP_IS_SEQ_0 seq HBOOST_PP_RPAREN())

#define HBOOST_FUSION_PP_IS_SEQ_0(...)                                           \
    HBOOST_FUSION_PP_IS_SEQ_1(__VA_ARGS__

#define HBOOST_FUSION_PP_IS_SEQ_ALWAYS_0(...)                                    \
    0

#define HBOOST_FUSION_PP_IS_SEQ_HBOOST_FUSION_PP_IS_SEQ_0                         \
    HBOOST_FUSION_PP_IS_SEQ_ALWAYS_0(

#define HBOOST_FUSION_PP_IS_SEQ_HBOOST_FUSION_PP_IS_SEQ_1(...)                    \
    1

#endif // HBOOST_PP_VARIADICS

#endif
