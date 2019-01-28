
#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

#ifndef HBOOST_MPL_INHERIT_HPP_INCLUDED
#define HBOOST_MPL_INHERIT_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#if !defined(HBOOST_MPL_PREPROCESSING_MODE)
#   include <hboost/mpl/empty_base.hpp>
#   include <hboost/mpl/aux_/na_spec.hpp>
#   include <hboost/mpl/aux_/lambda_support.hpp>
#endif

#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER inherit.hpp
#   include <hboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <hboost/mpl/limits/arity.hpp>
#   include <hboost/mpl/aux_/preprocessor/params.hpp>
#   include <hboost/mpl/aux_/preprocessor/default_params.hpp>
#   include <hboost/mpl/aux_/preprocessor/enum.hpp>
#   include <hboost/mpl/aux_/config/ctps.hpp>
#   include <hboost/mpl/aux_/config/dtp.hpp>

#   include <hboost/preprocessor/iterate.hpp>
#   include <hboost/preprocessor/dec.hpp>
#   include <hboost/preprocessor/cat.hpp>

namespace hboost { namespace mpl {

// 'inherit<T1,T2,..,Tn>' metafunction; returns an unspecified class type
// produced by public derivation from all metafunction's parameters 
// (T1,T2,..,Tn), except the parameters of 'empty_base' class type; 
// regardless the position and number of 'empty_base' parameters in the 
// metafunction's argument list, derivation from them is always a no-op;
// for instance:
//      inherit<her>::type == her
//      inherit<her,my>::type == struct unspecified : her, my {};
//      inherit<empty_base,her>::type == her
//      inherit<empty_base,her,empty_base,empty_base>::type == her
//      inherit<her,empty_base,my>::type == struct unspecified : her, my {};
//      inherit<empty_base,empty_base>::type == empty_base

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< 
      typename HBOOST_MPL_AUX_NA_PARAM(T1)
    , typename HBOOST_MPL_AUX_NA_PARAM(T2)
    > 
struct inherit2
    : T1, T2
{
    typedef inherit2 type;
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(2, inherit2, (T1,T2))
};

template< typename T1 > 
struct inherit2<T1,empty_base>
{
    typedef T1 type;
    HBOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(2, inherit2, (T1,empty_base))
};

template< typename T2 > 
struct inherit2<empty_base,T2>
{
    typedef T2 type;
    HBOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(2, inherit2, (empty_base,T2))
};

// needed to disambiguate the previous two in case when both 
// T1 and T2 == empty_base
template<> 
struct inherit2<empty_base,empty_base>
{
    typedef empty_base type;
    HBOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(2, inherit2, (empty_base,empty_base))
};

#else

namespace aux {

template< bool C1, bool C2 >
struct inherit2_impl
{
    template< typename Derived, typename T1, typename T2 > struct result_ 
        : T1, T2
    {
        typedef Derived type_;
    };
};

template<>
struct inherit2_impl<false,true>
{
    template< typename Derived, typename T1, typename T2 > struct result_
        : T1
    {
        typedef T1 type_;
    };
};

template<>
struct inherit2_impl<true,false>
{
    template< typename Derived, typename T1, typename T2 > struct result_
        : T2 
    {
        typedef T2 type_;
    };
};

template<>
struct inherit2_impl<true,true>
{
    template< typename Derived, typename T1, typename T2 > struct result_
    {
        typedef T1 type_;
    };
};

} // namespace aux

template< 
      typename HBOOST_MPL_AUX_NA_PARAM(T1)
    , typename HBOOST_MPL_AUX_NA_PARAM(T2)
    > 
struct inherit2
    : aux::inherit2_impl<
          is_empty_base<T1>::value
        , is_empty_base<T2>::value
        >::template result_< inherit2<T1,T2>,T1,T2 >
{
    typedef typename inherit2::type_ type;
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(2, inherit2, (T1,T2))
};

#endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

HBOOST_MPL_AUX_NA_SPEC(2, inherit2)

#define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(3, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY, <hboost/mpl/inherit.hpp>))
#include HBOOST_PP_ITERATE()

}}

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // HBOOST_MPL_INHERIT_HPP_INCLUDED

///// iteration

#else
#define n_ HBOOST_PP_FRAME_ITERATION(1)

template<
      HBOOST_MPL_PP_DEFAULT_PARAMS(n_, typename T, na)
    >
struct HBOOST_PP_CAT(inherit,n_)
    : inherit2<
          typename HBOOST_PP_CAT(inherit,HBOOST_PP_DEC(n_))<
              HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(n_), T)
            >::type
        , HBOOST_PP_CAT(T,n_)
        >
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(
          n_
        , HBOOST_PP_CAT(inherit,n_)
        , (HBOOST_MPL_PP_PARAMS(n_, T))
        )
};

HBOOST_MPL_AUX_NA_SPEC(n_, HBOOST_PP_CAT(inherit,n_))

#if n_ == HBOOST_MPL_LIMIT_METAFUNCTION_ARITY
/// primary template
template<
      HBOOST_MPL_PP_DEFAULT_PARAMS(n_, typename T, empty_base)
    >
struct inherit
    : HBOOST_PP_CAT(inherit,n_)<HBOOST_MPL_PP_PARAMS(n_, T)>
{
};

// 'na' specialization
template<>
struct inherit< HBOOST_MPL_PP_ENUM(5, na) >
{
    template<
#if !defined(HBOOST_MPL_CFG_NO_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
          HBOOST_MPL_PP_DEFAULT_PARAMS(n_, typename T, empty_base)
#else
          HBOOST_MPL_PP_PARAMS(n_, typename T)
#endif
        >
    struct apply
        : inherit< HBOOST_MPL_PP_PARAMS(n_, T) >
    {
    };
};

HBOOST_MPL_AUX_NA_SPEC_LAMBDA(n_, inherit)
HBOOST_MPL_AUX_NA_SPEC_ARITY(n_, inherit)
HBOOST_MPL_AUX_NA_SPEC_TEMPLATE_ARITY(n_, n_, inherit)
#endif

#undef n_
#endif // HBOOST_PP_IS_ITERATING
