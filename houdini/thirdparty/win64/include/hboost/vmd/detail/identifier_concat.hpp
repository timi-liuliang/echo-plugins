
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_IDENTIFIER_CONCAT_HPP)
#define HBOOST_VMD_DETAIL_IDENTIFIER_CONCAT_HPP

#include <hboost/preprocessor/cat.hpp>
#include <hboost/vmd/detail/idprefix.hpp>

#define HBOOST_VMD_DETAIL_IDENTIFIER_CONCATENATE(vseq) \
    HBOOST_PP_CAT \
        ( \
        HBOOST_VMD_DETAIL_IDENTIFIER_REGISTRATION_PREFIX, \
        vseq \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_IDENTIFIER_CONCAT_HPP */
