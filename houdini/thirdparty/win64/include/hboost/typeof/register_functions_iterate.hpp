// Copyright (C) 2004 Arkadiy Vertleyb
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hboost/typeof/encode_decode_params.hpp>

#define n HBOOST_PP_ITERATION()

// function pointers

template<class V, class R HBOOST_PP_ENUM_TRAILING_PARAMS(n, class P)>
struct encode_type_impl<V, R(*)(HBOOST_PP_ENUM_PARAMS(n, P))>
{
    typedef R HBOOST_PP_CAT(P, n);
    typedef HBOOST_TYPEOF_ENCODE_PARAMS(HBOOST_PP_INC(n), FUN_PTR_ID + n) type;
};

template<class V, class R HBOOST_PP_ENUM_TRAILING_PARAMS(n, class P)>
struct encode_type_impl<V, R(*)(HBOOST_PP_ENUM_PARAMS(n, P) ...)>
{
    typedef R HBOOST_PP_CAT(P, n);
    typedef HBOOST_TYPEOF_ENCODE_PARAMS(HBOOST_PP_INC(n), FUN_VAR_PTR_ID + n) type;
};

template<class Iter>
struct decode_type_impl<hboost::mpl::size_t<FUN_PTR_ID + n>, Iter>
{
    typedef Iter iter0;
    HBOOST_TYPEOF_DECODE_PARAMS(HBOOST_PP_INC(n))
    typedef HBOOST_PP_CAT(p, n)(*type)(HBOOST_PP_ENUM_PARAMS(n, p));
    typedef HBOOST_PP_CAT(iter, HBOOST_PP_INC(n)) iter;
};

template<class Iter>
struct decode_type_impl<hboost::mpl::size_t<FUN_VAR_PTR_ID + n>, Iter>
{
    typedef Iter iter0;
    HBOOST_TYPEOF_DECODE_PARAMS(HBOOST_PP_INC(n))
    typedef HBOOST_PP_CAT(p, n)(*type)(HBOOST_PP_ENUM_PARAMS(n, p) ...);
    typedef HBOOST_PP_CAT(iter, HBOOST_PP_INC(n)) iter;
};

#ifndef HBOOST_TYPEOF_NO_FUNCTION_TYPES

    // function references

    template<class V, class R HBOOST_PP_ENUM_TRAILING_PARAMS(n, class P)>
    struct encode_type_impl<V, R(&)(HBOOST_PP_ENUM_PARAMS(n, P))>
    {
        typedef R HBOOST_PP_CAT(P, n);
        typedef HBOOST_TYPEOF_ENCODE_PARAMS(HBOOST_PP_INC(n), FUN_REF_ID + n) type;
    };

    template<class V, class R HBOOST_PP_ENUM_TRAILING_PARAMS(n, class P)>
    struct encode_type_impl<V, R(&)(HBOOST_PP_ENUM_PARAMS(n, P) ...)>
    {
        typedef R HBOOST_PP_CAT(P, n);
        typedef HBOOST_TYPEOF_ENCODE_PARAMS(HBOOST_PP_INC(n), FUN_VAR_REF_ID + n) type;
    };

    template<class Iter>
    struct decode_type_impl<hboost::mpl::size_t<FUN_REF_ID + n>, Iter>
    {
        typedef Iter iter0;
        HBOOST_TYPEOF_DECODE_PARAMS(HBOOST_PP_INC(n))
        typedef HBOOST_PP_CAT(p, n)(&type)(HBOOST_PP_ENUM_PARAMS(n, p));
        typedef HBOOST_PP_CAT(iter, HBOOST_PP_INC(n)) iter;
    };

    template<class Iter>
    struct decode_type_impl<hboost::mpl::size_t<FUN_VAR_REF_ID + n>, Iter>
    {
        typedef Iter iter0;
        HBOOST_TYPEOF_DECODE_PARAMS(HBOOST_PP_INC(n))
        typedef HBOOST_PP_CAT(p, n)(&type)(HBOOST_PP_ENUM_PARAMS(n, p) ...);
        typedef HBOOST_PP_CAT(iter, HBOOST_PP_INC(n)) iter;
    };

    // functions

    template<class V, class R HBOOST_PP_ENUM_TRAILING_PARAMS(n, class P)>
    struct encode_type_impl<V, R(HBOOST_PP_ENUM_PARAMS(n, P))>
    {
        typedef R HBOOST_PP_CAT(P, n);
        typedef HBOOST_TYPEOF_ENCODE_PARAMS(HBOOST_PP_INC(n), FUN_ID + n) type;
    };

    template<class V, class R HBOOST_PP_ENUM_TRAILING_PARAMS(n, class P)>
    struct encode_type_impl<V, R(HBOOST_PP_ENUM_PARAMS(n, P) ...)>
    {
        typedef R HBOOST_PP_CAT(P, n);
        typedef HBOOST_TYPEOF_ENCODE_PARAMS(HBOOST_PP_INC(n), FUN_VAR_ID + n) type;
    };

    template<class Iter>
    struct decode_type_impl<hboost::mpl::size_t<FUN_ID + n>, Iter>
    {
        typedef Iter iter0;
        HBOOST_TYPEOF_DECODE_PARAMS(HBOOST_PP_INC(n))
        typedef HBOOST_PP_CAT(p, n)(type)(HBOOST_PP_ENUM_PARAMS(n, p));
        typedef HBOOST_PP_CAT(iter, HBOOST_PP_INC(n)) iter;
    };

    template<class Iter>
    struct decode_type_impl<hboost::mpl::size_t<FUN_VAR_ID + n>, Iter>
    {
        typedef Iter iter0;
        HBOOST_TYPEOF_DECODE_PARAMS(HBOOST_PP_INC(n))
        typedef HBOOST_PP_CAT(p, n)(type)(HBOOST_PP_ENUM_PARAMS(n, p) ...);
        typedef HBOOST_PP_CAT(iter, HBOOST_PP_INC(n)) iter;
    };

#endif//HBOOST_TYPEOF_NO_FUNCTION_TYPES

#ifndef HBOOST_TYPEOF_NO_MEMBER_FUNCTION_TYPES
// member functions

#define HBOOST_TYPEOF_qualifier
#define HBOOST_TYPEOF_id MEM_FUN_ID
#include <hboost/typeof/register_mem_functions.hpp>

#define HBOOST_TYPEOF_qualifier const
#define HBOOST_TYPEOF_id CONST_MEM_FUN_ID
#include <hboost/typeof/register_mem_functions.hpp>

#define HBOOST_TYPEOF_qualifier volatile
#define HBOOST_TYPEOF_id VOLATILE_MEM_FUN_ID
#include <hboost/typeof/register_mem_functions.hpp>

#define HBOOST_TYPEOF_qualifier volatile const
#define HBOOST_TYPEOF_id VOLATILE_CONST_MEM_FUN_ID
#include <hboost/typeof/register_mem_functions.hpp>

#undef n
#endif
