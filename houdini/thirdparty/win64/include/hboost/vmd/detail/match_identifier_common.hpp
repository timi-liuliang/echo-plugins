
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_COMMON_HPP)
#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_COMMON_HPP

#include <hboost/preprocessor/cat.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/detail/idprefix.hpp>

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_OP_CREATE_ID_RESULT(id,keyid) \
    HBOOST_PP_CAT \
        ( \
        HBOOST_VMD_DETAIL_IDENTIFIER_DETECTION_PREFIX, \
        HBOOST_PP_CAT \
            ( \
            keyid, \
            HBOOST_PP_CAT \
                ( \
                _, \
                id \
                ) \
            ) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_OP_CMP_IDS(id,keyid) \
    HBOOST_VMD_IS_EMPTY \
        ( \
        HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_OP_CREATE_ID_RESULT(id,keyid) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_MATCH_IDENTIFIER_COMMON_HPP */
