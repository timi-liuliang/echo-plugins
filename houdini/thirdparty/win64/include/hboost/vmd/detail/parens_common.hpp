
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_PARENS_COMMON_HPP)
#define HBOOST_VMD_DETAIL_PARENS_COMMON_HPP

#include <hboost/preprocessor/facilities/expand.hpp>
#include <hboost/preprocessor/punctuation/paren.hpp>
#include <hboost/vmd/empty.hpp>
  
#define HBOOST_VMD_DETAIL_BEGIN_PARENS_EXP2(...) ( __VA_ARGS__ ) HBOOST_VMD_EMPTY HBOOST_PP_LPAREN()
#define HBOOST_VMD_DETAIL_BEGIN_PARENS_EXP1(vseq) HBOOST_VMD_DETAIL_BEGIN_PARENS_EXP2 vseq HBOOST_PP_RPAREN()
#define HBOOST_VMD_DETAIL_BEGIN_PARENS(vseq) HBOOST_PP_EXPAND(HBOOST_VMD_DETAIL_BEGIN_PARENS_EXP1(vseq))

#define HBOOST_VMD_DETAIL_AFTER_PARENS_DATA(vseq) HBOOST_VMD_EMPTY vseq
#define HBOOST_VMD_DETAIL_SPLIT_PARENS(vseq) (HBOOST_VMD_DETAIL_BEGIN_PARENS(vseq),HBOOST_VMD_DETAIL_AFTER_PARENS_DATA(vseq))

#endif /* HBOOST_VMD_DETAIL_PARENS_COMMON_HPP */
