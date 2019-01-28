// Copyright (C) 2004 Arkadiy Vertleyb
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_POINTERS_DATA_MEMBERS_HPP_INCLUDED
#define HBOOST_TYPEOF_POINTERS_DATA_MEMBERS_HPP_INCLUDED

#include <hboost/typeof/encode_decode_params.hpp>
#include <hboost/typeof/encode_decode.hpp>

#include HBOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

HBOOST_TYPEOF_BEGIN_ENCODE_NS

enum {PTR_DATA_MEM_ID = HBOOST_TYPEOF_UNIQUE_ID()};

template<class V, class P0, class P1>
struct encode_type_impl<V, P0 P1::*>
{
    typedef HBOOST_TYPEOF_ENCODE_PARAMS(2, PTR_DATA_MEM_ID) type;
};

template<class Iter>
struct decode_type_impl<hboost::mpl::size_t<PTR_DATA_MEM_ID>, Iter>
{
    typedef Iter iter0;
    HBOOST_TYPEOF_DECODE_PARAMS(2)

    template<class T> struct workaround{
        typedef p0 T::* type;
    };
    typedef typename decode_type_impl<hboost::mpl::size_t<PTR_DATA_MEM_ID>, Iter>::template workaround<p1>::type type;
    typedef iter2 iter;
};

HBOOST_TYPEOF_END_ENCODE_NS

#endif//HBOOST_TYPEOF_POINTERS_DATA_MEMBERS_HPP_INCLUDED
