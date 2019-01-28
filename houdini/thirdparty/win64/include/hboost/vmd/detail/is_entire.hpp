
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_IS_ENTIRE_HPP)
#define HBOOST_VMD_DETAIL_IS_ENTIRE_HPP

#include <hboost/preprocessor/logical/bitand.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/detail/not_empty.hpp>

#define HBOOST_VMD_DETAIL_IS_ENTIRE(tuple) \
    HBOOST_PP_BITAND \
        ( \
        HBOOST_VMD_DETAIL_NOT_EMPTY(HBOOST_PP_TUPLE_ELEM(0,tuple)), \
        HBOOST_VMD_IS_EMPTY(HBOOST_PP_TUPLE_ELEM(1,tuple)) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_IS_ENTIRE_HPP */
