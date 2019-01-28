
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_IS_NUMBER_HPP)
#define HBOOST_VMD_DETAIL_IS_NUMBER_HPP

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/detail/equal_type.hpp>
#include <hboost/vmd/detail/identifier_type.hpp>
#include <hboost/vmd/detail/is_identifier.hpp>
#include <hboost/vmd/detail/number_registration.hpp>

#define HBOOST_VMD_DETAIL_IS_NUMBER_TYPE(vseq) \
    HBOOST_VMD_DETAIL_EQUAL_TYPE \
        ( \
        HBOOST_VMD_TYPE_NUMBER, \
        HBOOST_VMD_DETAIL_IDENTIFIER_TYPE(vseq) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IS_NUMBER(vseq) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_IS_IDENTIFIER_SINGLE(vseq), \
            HBOOST_VMD_DETAIL_IS_NUMBER_TYPE, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (vseq) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_IS_NUMBER_HPP */
