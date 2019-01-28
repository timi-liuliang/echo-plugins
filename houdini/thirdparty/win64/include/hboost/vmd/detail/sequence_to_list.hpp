
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_SEQUENCE_TO_LIST_HPP)
#define HBOOST_VMD_DETAIL_SEQUENCE_TO_LIST_HPP

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/vmd/detail/modifiers.hpp>
#include <hboost/vmd/detail/mods.hpp>
#include <hboost/vmd/detail/not_empty.hpp>
#include <hboost/vmd/detail/sequence_elem.hpp>

#define HBOOST_VMD_DETAIL_SEQUENCE_TO_LIST(...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_NOT_EMPTY(HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_VSEQ(__VA_ARGS__)), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_PROCESS_TUPLE \
            ( \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_VSEQ(__VA_ARGS__), \
            , \
            HBOOST_VMD_TYPE_LIST, \
            HBOOST_VMD_DETAIL_NEW_MODS(HBOOST_VMD_ALLOW_RETURN,__VA_ARGS__) \
            ), \
        HBOOST_PP_NIL \
        ) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_TO_LIST_D(d,...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_NOT_EMPTY(HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_VSEQ(__VA_ARGS__)), \
        HBOOST_VMD_DETAIL_SEQUENCE_ELEM_PROCESS_TUPLE_D \
            ( \
            d, \
            HBOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_VSEQ(__VA_ARGS__), \
            , \
            HBOOST_VMD_TYPE_LIST, \
            HBOOST_VMD_DETAIL_NEW_MODS_D(d,HBOOST_VMD_ALLOW_RETURN,__VA_ARGS__) \
            ), \
        HBOOST_PP_NIL \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_SEQUENCE_TO_LIST_HPP */
