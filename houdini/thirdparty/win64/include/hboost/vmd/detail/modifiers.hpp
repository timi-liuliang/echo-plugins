
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_MODIFIERS_HPP)
#define HBOOST_VMD_DETAIL_MODIFIERS_HPP

#include <hboost/vmd/detail/match_single_identifier.hpp>

#define HBOOST_VMD_REGISTER_HBOOST_VMD_RETURN_NO_TYPE (HBOOST_VMD_RETURN_NO_TYPE)
#define HBOOST_VMD_REGISTER_HBOOST_VMD_RETURN_TYPE (HBOOST_VMD_RETURN_TYPE)
#define HBOOST_VMD_REGISTER_HBOOST_VMD_RETURN_TYPE_TUPLE (HBOOST_VMD_RETURN_TYPE_TUPLE)
#define HBOOST_VMD_REGISTER_HBOOST_VMD_RETURN_TYPE_ARRAY (HBOOST_VMD_RETURN_TYPE_ARRAY)
#define HBOOST_VMD_REGISTER_HBOOST_VMD_RETURN_TYPE_LIST  (HBOOST_VMD_RETURN_TYPE_LIST)

#define HBOOST_VMD_REGISTER_HBOOST_VMD_RETURN_AFTER (HBOOST_VMD_RETURN_AFTER)
#define HBOOST_VMD_REGISTER_HBOOST_VMD_RETURN_NO_AFTER (HBOOST_VMD_RETURN_NO_AFTER)
#define HBOOST_VMD_REGISTER_HBOOST_VMD_RETURN_ONLY_AFTER (HBOOST_VMD_RETURN_ONLY_AFTER)

#define HBOOST_VMD_REGISTER_HBOOST_VMD_RETURN_INDEX (HBOOST_VMD_RETURN_INDEX)
#define HBOOST_VMD_REGISTER_HBOOST_VMD_RETURN_NO_INDEX (HBOOST_VMD_RETURN_NO_INDEX)

#define HBOOST_VMD_DETECT_HBOOST_VMD_RETURN_NO_TYPE_HBOOST_VMD_RETURN_NO_TYPE
#define HBOOST_VMD_DETECT_HBOOST_VMD_RETURN_TYPE_HBOOST_VMD_RETURN_TYPE
#define HBOOST_VMD_DETECT_HBOOST_VMD_RETURN_TYPE_TUPLE_HBOOST_VMD_RETURN_TYPE_TUPLE
#define HBOOST_VMD_DETECT_HBOOST_VMD_RETURN_TYPE_ARRAY_HBOOST_VMD_RETURN_TYPE_ARRAY
#define HBOOST_VMD_DETECT_HBOOST_VMD_RETURN_TYPE_LIST_HBOOST_VMD_RETURN_TYPE_LIST

#define HBOOST_VMD_DETECT_HBOOST_VMD_RETURN_AFTER_HBOOST_VMD_RETURN_AFTER
#define HBOOST_VMD_DETECT_HBOOST_VMD_RETURN_NO_AFTER_HBOOST_VMD_RETURN_NO_AFTER
#define HBOOST_VMD_DETECT_HBOOST_VMD_RETURN_ONLY_AFTER_HBOOST_VMD_RETURN_ONLY_AFTER

#define HBOOST_VMD_DETECT_HBOOST_VMD_RETURN_INDEX_HBOOST_VMD_RETURN_INDEX
#define HBOOST_VMD_DETECT_HBOOST_VMD_RETURN_NO_INDEX_HBOOST_VMD_RETURN_NO_INDEX

#define HBOOST_VMD_REGISTER_HBOOST_VMD_ALLOW_ALL  (HBOOST_VMD_ALLOW_ALL)
#define HBOOST_VMD_REGISTER_HBOOST_VMD_ALLOW_RETURN  (HBOOST_VMD_ALLOW_RETURN)
#define HBOOST_VMD_REGISTER_HBOOST_VMD_ALLOW_AFTER  (HBOOST_VMD_ALLOW_AFTER)
#define HBOOST_VMD_REGISTER_HBOOST_VMD_ALLOW_INDEX  (HBOOST_VMD_ALLOW_INDEX)

#define HBOOST_VMD_DETECT_HBOOST_VMD_ALLOW_ALL_HBOOST_VMD_ALLOW_ALL
#define HBOOST_VMD_DETECT_HBOOST_VMD_ALLOW_RETURN_HBOOST_VMD_ALLOW_RETURN
#define HBOOST_VMD_DETECT_HBOOST_VMD_ALLOW_AFTER_HBOOST_VMD_ALLOW_AFTER
#define HBOOST_VMD_DETECT_HBOOST_VMD_ALLOW_INDEX_HBOOST_VMD_ALLOW_INDEX

#define HBOOST_VMD_DETAIL_IS_RETURN_TYPE_TUPLE(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_RETURN_TYPE_TUPLE) \
/**/

#define HBOOST_VMD_DETAIL_IS_RETURN_TYPE_ARRAY(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_RETURN_TYPE_ARRAY) \
/**/

#define HBOOST_VMD_DETAIL_IS_RETURN_TYPE_LIST(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_RETURN_TYPE_LIST) \
/**/

#define HBOOST_VMD_DETAIL_IS_RETURN_TYPE(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_RETURN_TYPE) \
/**/

#define HBOOST_VMD_DETAIL_IS_RETURN_NO_TYPE(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_RETURN_NO_TYPE) \
/**/

#define HBOOST_VMD_DETAIL_IS_RETURN_AFTER(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_RETURN_AFTER) \
/**/

#define HBOOST_VMD_DETAIL_IS_RETURN_ONLY_AFTER(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_RETURN_ONLY_AFTER) \
/**/

#define HBOOST_VMD_DETAIL_IS_RETURN_NO_AFTER(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_RETURN_NO_AFTER) \
/**/

#define HBOOST_VMD_DETAIL_IS_RETURN_INDEX(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_RETURN_INDEX) \
/**/

#define HBOOST_VMD_DETAIL_IS_RETURN_NO_INDEX(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_RETURN_NO_INDEX) \
/**/

#define HBOOST_VMD_DETAIL_IS_ALLOW_ALL(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_ALLOW_ALL) \
/**/

#define HBOOST_VMD_DETAIL_IS_ALLOW_RETURN(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_ALLOW_RETURN) \
/**/

#define HBOOST_VMD_DETAIL_IS_ALLOW_AFTER(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_ALLOW_AFTER) \
/**/

#define HBOOST_VMD_DETAIL_IS_ALLOW_INDEX(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_ALLOW_INDEX) \
/**/

#endif /* HBOOST_VMD_DETAIL_MODIFIERS_HPP */