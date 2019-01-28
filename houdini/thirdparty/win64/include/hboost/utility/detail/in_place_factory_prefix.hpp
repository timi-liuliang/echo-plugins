// Copyright (C) 2003, Fernando Luis Cacciola Carballal.
// Copyright (C) 2007, Tobias Schwinger.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/optional for documentation.
//
// You are welcome to contact the author at:
//  fernando_cacciola@hotmail.com
//
#ifndef HBOOST_UTILITY_DETAIL_INPLACE_FACTORY_PREFIX_04APR2007_HPP
#define HBOOST_UTILITY_DETAIL_INPLACE_FACTORY_PREFIX_04APR2007_HPP

#include <new>
#include <cstddef>
#include <hboost/config.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/punctuation/paren.hpp>
#include <hboost/preprocessor/iteration/iterate.hpp>
#include <hboost/preprocessor/repetition/repeat.hpp>
#include <hboost/preprocessor/repetition/enum.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_params.hpp>

#define HBOOST_DEFINE_INPLACE_FACTORY_CLASS_MEMBER_INIT(z,n,_) HBOOST_PP_CAT(m_a,n) HBOOST_PP_LPAREN() HBOOST_PP_CAT(a,n) HBOOST_PP_RPAREN()
#define HBOOST_DEFINE_INPLACE_FACTORY_CLASS_MEMBER_DECL(z,n,_) HBOOST_PP_CAT(A,n) const& HBOOST_PP_CAT(m_a,n);

#define HBOOST_MAX_INPLACE_FACTORY_ARITY 10

#undef HBOOST_UTILITY_DETAIL_INPLACE_FACTORY_SUFFIX_04APR2007_HPP

#endif

