// Copyright (C) 2005 Arkadiy Vertleyb
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_ENCODE_DECODE_PARAMS_HPP_INCLUDED
#define HBOOST_TYPEOF_ENCODE_DECODE_PARAMS_HPP_INCLUDED

#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/repetition/repeat.hpp>

// Assumes iter0 contains initial iterator

#define HBOOST_TYPEOF_DECODE_PARAM(z, n, text)   \
    typedef hboost::type_of::decode_type<iter##n> decode##n;     \
    typedef typename decode##n::type p##n;      \
    typedef typename decode##n::iter HBOOST_PP_CAT(iter, HBOOST_PP_INC(n));

#define HBOOST_TYPEOF_DECODE_PARAMS(n)\
    HBOOST_PP_REPEAT(n, HBOOST_TYPEOF_DECODE_PARAM, ~)

// The P0, P1, ... PN are encoded and added to V 

#define HBOOST_TYPEOF_ENCODE_PARAMS_BEGIN(z, n, text)\
    typename hboost::type_of::encode_type<

#define HBOOST_TYPEOF_ENCODE_PARAMS_END(z, n, text)\
    , HBOOST_PP_CAT(P, n)>::type

#define HBOOST_TYPEOF_ENCODE_PARAMS(n, ID)                   \
    HBOOST_PP_REPEAT(n, HBOOST_TYPEOF_ENCODE_PARAMS_BEGIN, ~) \
    typename hboost::type_of::push_back<V, hboost::mpl::size_t<ID> >::type      \
    HBOOST_PP_REPEAT(n, HBOOST_TYPEOF_ENCODE_PARAMS_END, ~)

#endif//HBOOST_TYPEOF_ENCODE_DECODE_PARAMS_HPP_INCLUDED
