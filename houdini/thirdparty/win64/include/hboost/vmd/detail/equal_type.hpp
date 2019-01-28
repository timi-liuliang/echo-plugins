
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_EQUAL_TYPE_HPP)
#define HBOOST_VMD_DETAIL_EQUAL_TYPE_HPP

#include <hboost/vmd/detail/match_single_identifier.hpp>
#include <hboost/vmd/detail/type_registration.hpp>

#define HBOOST_VMD_DETAIL_EQUAL_TYPE(type1,type2) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(type1,type2) \
/**/

#define HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,type1,type2) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(type1,type2) \
/**/

#endif /* HBOOST_VMD_DETAIL_EQUAL_TYPE_HPP */