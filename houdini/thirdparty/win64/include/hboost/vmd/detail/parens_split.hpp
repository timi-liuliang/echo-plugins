
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_PARENS_SPLIT_HPP)
#define HBOOST_VMD_DETAIL_PARENS_SPLIT_HPP

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>
#include <hboost/vmd/detail/empty_result.hpp>
#include <hboost/vmd/detail/parens_common.hpp>
  
#define HBOOST_VMD_DETAIL_PARENS_SPLIT(vseq) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_PP_IS_BEGIN_PARENS(vseq), \
      HBOOST_VMD_DETAIL_SPLIT_PARENS, \
      HBOOST_VMD_DETAIL_EMPTY_RESULT \
      ) \
    (vseq) \
/**/
#endif /* HBOOST_VMD_DETAIL_PARENS_SPLIT_HPP */
