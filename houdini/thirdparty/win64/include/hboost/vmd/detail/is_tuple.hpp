
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_IS_TUPLE_HPP)
#define HBOOST_VMD_DETAIL_IS_TUPLE_HPP

#include <hboost/vmd/detail/is_entire.hpp>
#include <hboost/vmd/detail/parens_split.hpp>
  
#define HBOOST_VMD_DETAIL_IS_TUPLE(vseq) \
    HBOOST_VMD_DETAIL_IS_ENTIRE \
        ( \
        HBOOST_VMD_DETAIL_PARENS_SPLIT(vseq) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_IS_TUPLE_HPP */
