
// Copyright Peter Dimov 2001-2002
// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//

// Preprocessed version of "hboost/mpl/arg.hpp" header
// -- DO NOT modify by hand!

HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN
template<> struct arg< -1 >
{
    HBOOST_STATIC_CONSTANT(int, value  = -1);
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          typename U1 = na, typename U2 = na, typename U3 = na
        , typename U4 = na, typename U5 = na
        >
    struct apply
    {
        typedef U1 type;
        HBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

template<> struct arg<1>
{
    HBOOST_STATIC_CONSTANT(int, value  = 1);
    typedef arg<2> next;
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          typename U1 = na, typename U2 = na, typename U3 = na
        , typename U4 = na, typename U5 = na
        >
    struct apply
    {
        typedef U1 type;
        HBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

template<> struct arg<2>
{
    HBOOST_STATIC_CONSTANT(int, value  = 2);
    typedef arg<3> next;
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          typename U1 = na, typename U2 = na, typename U3 = na
        , typename U4 = na, typename U5 = na
        >
    struct apply
    {
        typedef U2 type;
        HBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

template<> struct arg<3>
{
    HBOOST_STATIC_CONSTANT(int, value  = 3);
    typedef arg<4> next;
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          typename U1 = na, typename U2 = na, typename U3 = na
        , typename U4 = na, typename U5 = na
        >
    struct apply
    {
        typedef U3 type;
        HBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

template<> struct arg<4>
{
    HBOOST_STATIC_CONSTANT(int, value  = 4);
    typedef arg<5> next;
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          typename U1 = na, typename U2 = na, typename U3 = na
        , typename U4 = na, typename U5 = na
        >
    struct apply
    {
        typedef U4 type;
        HBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

template<> struct arg<5>
{
    HBOOST_STATIC_CONSTANT(int, value  = 5);
    typedef arg<6> next;
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          typename U1 = na, typename U2 = na, typename U3 = na
        , typename U4 = na, typename U5 = na
        >
    struct apply
    {
        typedef U5 type;
        HBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

HBOOST_MPL_AUX_NONTYPE_ARITY_SPEC(1,int, arg)

HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE
