
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_IS_TYPE_TYPE_HPP)
#define HBOOST_VMD_DETAIL_IS_TYPE_TYPE_HPP

#include <hboost/vmd/detail/type_registration.hpp>
#include <hboost/vmd/detail/match_single_identifier.hpp>

/*

    Check to see if the 'id' is HBOOST_VMD_TYPE_TYPE
    
    Returns 1 if it is, else 0

*/

#define HBOOST_VMD_DETAIL_IS_TYPE_TYPE(id) \
    HBOOST_VMD_DETAIL_MATCH_SINGLE_IDENTIFIER(id,HBOOST_VMD_TYPE_TYPE) \
/**/

#endif /* HBOOST_VMD_DETAIL_IS_TYPE_TYPE_HPP */
