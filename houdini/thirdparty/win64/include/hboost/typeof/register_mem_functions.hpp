// Copyright (C) 2004 Arkadiy Vertleyb
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#include <hboost/typeof/encode_decode_params.hpp>

// member functions

template<class V, class T, class R HBOOST_PP_ENUM_TRAILING_PARAMS(n, class P)> 
struct encode_type_impl<V, R(T::*)(HBOOST_PP_ENUM_PARAMS(n, P)) HBOOST_TYPEOF_qualifier>
{
    typedef R HBOOST_PP_CAT(P, n);
    typedef T HBOOST_PP_CAT(P, HBOOST_PP_INC(n));
    typedef HBOOST_TYPEOF_ENCODE_PARAMS(HBOOST_PP_ADD(n, 2), HBOOST_TYPEOF_id + n) type;
};

template<class Iter>
struct decode_type_impl<hboost::mpl::size_t<HBOOST_TYPEOF_id + n>, Iter>
{
    typedef Iter iter0;
    HBOOST_TYPEOF_DECODE_PARAMS(HBOOST_PP_ADD(n, 2))
    template<class T> struct workaround{
        typedef HBOOST_PP_CAT(p, n)(T::*type)(HBOOST_PP_ENUM_PARAMS(n, p)) HBOOST_TYPEOF_qualifier;
    };
    typedef typename workaround<HBOOST_PP_CAT(p, HBOOST_PP_INC(n))>::type type;
    typedef HBOOST_PP_CAT(iter, HBOOST_PP_ADD(n, 2)) iter;
};

// undef parameters

#undef HBOOST_TYPEOF_id
#undef HBOOST_TYPEOF_qualifier
