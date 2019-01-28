
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_NOT_EMPTY_HPP)
#define HBOOST_VMD_DETAIL_NOT_EMPTY_HPP

#include <hboost/preprocessor/logical/compl.hpp>
#include <hboost/vmd/is_empty.hpp>

#define HBOOST_VMD_DETAIL_NOT_EMPTY(par) \
    HBOOST_PP_COMPL \
        ( \
        HBOOST_VMD_IS_EMPTY(par) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_NOT_EMPTY_HPP */
