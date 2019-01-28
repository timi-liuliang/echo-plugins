
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_IS_EMPTY_ARRAY_HPP)
#define HBOOST_VMD_DETAIL_IS_EMPTY_ARRAY_HPP

#include <hboost/preprocessor/array/size.hpp>
#include <hboost/preprocessor/logical/not.hpp>

#define HBOOST_VMD_DETAIL_IS_EMPTY_ARRAY_SIZE(array) \
    HBOOST_PP_NOT \
        ( \
        HBOOST_PP_ARRAY_SIZE(array) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_IS_EMPTY_ARRAY_HPP */
