
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_IS_ARRAY_HPP)
#define HBOOST_VMD_DETAIL_IS_ARRAY_HPP

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_tuple.hpp>
#include <hboost/vmd/detail/is_array_common.hpp>

#define HBOOST_VMD_DETAIL_IS_ARRAY(vseq) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_TUPLE(vseq), \
            HBOOST_VMD_DETAIL_IS_ARRAY_SYNTAX, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (vseq) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_ARRAY_D(d,vseq) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_TUPLE(vseq), \
            HBOOST_VMD_DETAIL_IS_ARRAY_SYNTAX_D, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (d,vseq) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_IS_ARRAY_HPP */
