
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_PARENS_HPP)
#define HBOOST_VMD_DETAIL_PARENS_HPP

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>
#include <hboost/preprocessor/variadic/elem.hpp>
#include <hboost/vmd/empty.hpp>
#include <hboost/vmd/detail/mods.hpp>
#include <hboost/vmd/detail/parens_split.hpp>
#include <hboost/vmd/detail/parens_common.hpp>
  
#define HBOOST_VMD_DETAIL_PARENS_PROCESS(vseq) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_PP_IS_BEGIN_PARENS(vseq), \
      HBOOST_VMD_DETAIL_BEGIN_PARENS, \
      HBOOST_VMD_EMPTY \
      ) \
    (vseq) \
/**/

#define HBOOST_VMD_DETAIL_PARENS(...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER \
            ( \
            HBOOST_VMD_DETAIL_NEW_MODS(HBOOST_VMD_ALLOW_AFTER,__VA_ARGS__) \
            ), \
        HBOOST_VMD_DETAIL_PARENS_SPLIT, \
        HBOOST_VMD_DETAIL_PARENS_PROCESS \
        ) \
    (HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__)) \
/**/

#define HBOOST_VMD_DETAIL_PARENS_D(d,...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_MODS_IS_RESULT_AFTER \
            ( \
            HBOOST_VMD_DETAIL_NEW_MODS_D(d,HBOOST_VMD_ALLOW_AFTER,__VA_ARGS__) \
            ), \
        HBOOST_VMD_DETAIL_PARENS_SPLIT, \
        HBOOST_VMD_DETAIL_PARENS_PROCESS \
        ) \
    (HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__)) \
/**/

#endif /* HBOOST_VMD_DETAIL_PARENS_HPP */
