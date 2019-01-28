
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_SEQUENCE_SIZE_HPP)
#define HBOOST_VMD_DETAIL_SEQUENCE_SIZE_HPP

#include <hboost/preprocessor/array/size.hpp>
#include <hboost/vmd/detail/sequence_to_array.hpp>

#define HBOOST_VMD_DETAIL_SEQUENCE_SIZE(vseq) \
    HBOOST_PP_ARRAY_SIZE(HBOOST_VMD_DETAIL_SEQUENCE_TO_ARRAY(vseq)) \
/**/

#define HBOOST_VMD_DETAIL_SEQUENCE_SIZE_D(d,vseq) \
    HBOOST_PP_ARRAY_SIZE(HBOOST_VMD_DETAIL_SEQUENCE_TO_ARRAY_D(d,vseq)) \
/**/

#endif /* HBOOST_VMD_DETAIL_SEQUENCE_SIZE_HPP */
