
#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

#ifndef HBOOST_MPL_AUX_FULL_LAMBDA_HPP_INCLUDED
#define HBOOST_MPL_AUX_FULL_LAMBDA_HPP_INCLUDED

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
#   include <hboost/mpl/lambda_fwd.hpp>
#   include <hboost/mpl/bind_fwd.hpp>
#   include <hboost/mpl/protect.hpp>
#   include <hboost/mpl/quote.hpp>
#   include <hboost/mpl/arg.hpp>
#   include <hboost/mpl/bool.hpp>
#   include <hboost/mpl/int_fwd.hpp>
#   include <hboost/mpl/aux_/template_arity.hpp>
#   include <hboost/mpl/aux_/na_spec.hpp>
#   include <hboost/mpl/aux_/config/ttp.hpp>
#   if defined(HBOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING)
#       include <hboost/mpl/if.hpp>
#   endif
#endif

#include <hboost/mpl/aux_/lambda_arity_param.hpp>
#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER full_lambda.hpp
#   include <hboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <hboost/mpl/limits/arity.hpp>
#   include <hboost/mpl/aux_/preprocessor/default_params.hpp>
#   include <hboost/mpl/aux_/preprocessor/params.hpp>
#   include <hboost/mpl/aux_/preprocessor/enum.hpp>
#   include <hboost/mpl/aux_/preprocessor/repeat.hpp>
#   include <hboost/mpl/aux_/config/dmc_ambiguous_ctps.hpp>

#   include <hboost/preprocessor/iterate.hpp>
#   include <hboost/preprocessor/comma_if.hpp>
#   include <hboost/preprocessor/inc.hpp>
#   include <hboost/preprocessor/cat.hpp>

namespace hboost { namespace mpl {

// local macros, #undef-ined at the end of the header
#   define AUX778076_LAMBDA_PARAMS(i_, param) \
    HBOOST_MPL_PP_PARAMS(i_, param) \
    /**/

#   define AUX778076_BIND_PARAMS(param) \
    HBOOST_MPL_PP_PARAMS( \
          HBOOST_MPL_LIMIT_METAFUNCTION_ARITY \
        , param \
        ) \
    /**/

#   define AUX778076_BIND_N_PARAMS(i_, param) \
    HBOOST_PP_COMMA_IF(i_) \
    HBOOST_MPL_PP_PARAMS(i_, param) \
    /**/

#   define AUX778076_ARITY_PARAM(param) \
    HBOOST_MPL_AUX_LAMBDA_ARITY_PARAM(param) \
    /**/


#define n_ HBOOST_MPL_LIMIT_METAFUNCTION_ARITY
namespace aux {

template<
      HBOOST_MPL_PP_DEFAULT_PARAMS(n_,bool C,false)
    >
struct lambda_or
    : true_
{
};

template<>
struct lambda_or< HBOOST_MPL_PP_ENUM(n_,false) >
    : false_
{
};

} // namespace aux
#undef n_

template<
      typename T
    , typename Tag
    AUX778076_ARITY_PARAM(typename Arity)
    >
struct lambda
{
    typedef false_ is_le;
    typedef T result_;
    typedef T type;
};

template<
      typename T
    >
struct is_lambda_expression
    : lambda<T>::is_le
{
};


template< int N, typename Tag >
struct lambda< arg<N>,Tag AUX778076_ARITY_PARAM(int_<-1>) >
{
    typedef true_ is_le;
    typedef mpl::arg<N> result_; // qualified for the sake of MIPSpro 7.41
    typedef mpl::protect<result_> type; 
};


#define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY, <hboost/mpl/aux_/full_lambda.hpp>))
#include HBOOST_PP_ITERATE()

/// special case for 'protect'
template< typename T, typename Tag >
struct lambda< mpl::protect<T>,Tag AUX778076_ARITY_PARAM(int_<1>) >
{
    typedef false_ is_le;
    typedef mpl::protect<T> result_;
    typedef result_ type;
};

/// specializations for the main 'bind' form
template<
      typename F, AUX778076_BIND_PARAMS(typename T)
    , typename Tag
    >
struct lambda<
          bind<F,AUX778076_BIND_PARAMS(T)>
        , Tag
        AUX778076_ARITY_PARAM(int_<HBOOST_PP_INC(HBOOST_MPL_LIMIT_METAFUNCTION_ARITY)>)
        >
{
    typedef false_ is_le;
    typedef bind<F, AUX778076_BIND_PARAMS(T)> result_;
    typedef result_ type;
};


#if defined(HBOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING)

template<
      typename F
    , typename Tag1
    , typename Tag2
    , typename Arity
    >
struct lambda<
          lambda<F,Tag1,Arity>
        , Tag2
        , int_<3>
        >
{
    typedef lambda< F,Tag2 > l1;
    typedef lambda< Tag1,Tag2 > l2;

    typedef typename l1::is_le is_le;
    typedef bind1< quote1<aux::template_arity>, typename l1::result_ > arity_;
    typedef lambda< typename if_<is_le,arity_,Arity>::type,Tag2 > l3;
    
    typedef aux::le_result3<is_le, Tag2, mpl::lambda, l1, l2, l3> le_result_;
    typedef typename le_result_::result_ result_;
    typedef typename le_result_::type type;
};

#elif !defined(HBOOST_MPL_CFG_DMC_AMBIGUOUS_CTPS)

/// workaround for MWCW 8.3+/EDG < 303, leads to ambiguity on Digital Mars
template<
      typename F, typename Tag1, typename Tag2
    >
struct lambda<
          lambda< F,Tag1 >
        , Tag2
        >
{
    typedef lambda< F,Tag2 > l1;
    typedef lambda< Tag1,Tag2 > l2;
    
    typedef typename l1::is_le is_le;
    typedef aux::le_result2<is_le, Tag2, mpl::lambda, l1, l2> le_result_;
    typedef typename le_result_::result_ result_;
    typedef typename le_result_::type type;
};

#endif

#   undef AUX778076_ARITY_PARAM
#   undef AUX778076_BIND_N_PARAMS
#   undef AUX778076_BIND_PARAMS
#   undef AUX778076_LAMBDA_PARAMS

#if !defined(HBOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING)
HBOOST_MPL_AUX_NA_SPEC(2, lambda)
#else
HBOOST_MPL_AUX_NA_SPEC2(2, 3, lambda)
#endif

}}

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // HBOOST_MPL_AUX_FULL_LAMBDA_HPP_INCLUDED

///// iteration, depth == 1

// For gcc 4.4 compatability, we must include the
// HBOOST_PP_ITERATION_DEPTH test inside an #else clause.
#else // HBOOST_PP_IS_ITERATING
#if HBOOST_PP_ITERATION_DEPTH() == 1
#define i_ HBOOST_PP_FRAME_ITERATION(1)

#if i_ > 0

namespace aux {

#   define AUX778076_RESULT(unused, i_, T) \
    HBOOST_PP_COMMA_IF(i_) \
    typename HBOOST_PP_CAT(T, HBOOST_PP_INC(i_))::result_ \
    /**/

#   define AUX778076_TYPE(unused, i_, T) \
    HBOOST_PP_COMMA_IF(i_) \
    typename HBOOST_PP_CAT(T, HBOOST_PP_INC(i_))::type \
    /**/

template<
      typename IsLE, typename Tag
    , template< AUX778076_LAMBDA_PARAMS(i_, typename P) > class F
    , AUX778076_LAMBDA_PARAMS(i_, typename L)
    >
struct HBOOST_PP_CAT(le_result,i_)
{
    typedef F<
          HBOOST_MPL_PP_REPEAT(i_, AUX778076_TYPE, L)
        > result_;
    
    typedef result_ type;
};

template<
      typename Tag
    , template< AUX778076_LAMBDA_PARAMS(i_, typename P) > class F
    , AUX778076_LAMBDA_PARAMS(i_, typename L)
    >
struct HBOOST_PP_CAT(le_result,i_)< true_,Tag,F,AUX778076_LAMBDA_PARAMS(i_, L) >
{
    typedef HBOOST_PP_CAT(bind,i_)<
          HBOOST_PP_CAT(quote,i_)<F,Tag>
        , HBOOST_MPL_PP_REPEAT(i_, AUX778076_RESULT, L)
        > result_;

    typedef mpl::protect<result_> type;
};

#   undef AUX778076_TYPE
#   undef AUX778076_RESULT

} // namespace aux


#   define AUX778076_LAMBDA_TYPEDEF(unused, i_, T) \
    typedef lambda< HBOOST_PP_CAT(T, HBOOST_PP_INC(i_)), Tag > \
        HBOOST_PP_CAT(l,HBOOST_PP_INC(i_)); \
/**/

#   define AUX778076_IS_LE_TYPEDEF(unused, i_, unused2) \
    typedef typename HBOOST_PP_CAT(l,HBOOST_PP_INC(i_))::is_le \
        HBOOST_PP_CAT(is_le,HBOOST_PP_INC(i_)); \
/**/

#   define AUX778076_IS_LAMBDA_EXPR(unused, i_, unused2) \
    HBOOST_PP_COMMA_IF(i_) \
    HBOOST_PP_CAT(is_le,HBOOST_PP_INC(i_))::value \
/**/

template<
      template< AUX778076_LAMBDA_PARAMS(i_, typename P) > class F
    , AUX778076_LAMBDA_PARAMS(i_, typename T)
    , typename Tag
    >
struct lambda< 
          F<AUX778076_LAMBDA_PARAMS(i_, T)>
        , Tag
        AUX778076_ARITY_PARAM(int_<i_>)
        >
{
    HBOOST_MPL_PP_REPEAT(i_, AUX778076_LAMBDA_TYPEDEF, T)
    HBOOST_MPL_PP_REPEAT(i_, AUX778076_IS_LE_TYPEDEF, unused)

    typedef typename aux::lambda_or<
          HBOOST_MPL_PP_REPEAT(i_, AUX778076_IS_LAMBDA_EXPR, unused)
        >::type is_le;

    typedef aux::HBOOST_PP_CAT(le_result,i_)<
          is_le, Tag, F, AUX778076_LAMBDA_PARAMS(i_, l)
        > le_result_;
    
    typedef typename le_result_::result_ result_;
    typedef typename le_result_::type type;
};


#   undef AUX778076_IS_LAMBDA_EXPR
#   undef AUX778076_IS_LE_TYPEDEF
#   undef AUX778076_LAMBDA_TYPEDEF

#endif // i_ > 0

template<
      typename F AUX778076_BIND_N_PARAMS(i_, typename T)
    , typename Tag
    >
struct lambda<
          HBOOST_PP_CAT(bind,i_)<F AUX778076_BIND_N_PARAMS(i_, T)>
        , Tag
        AUX778076_ARITY_PARAM(int_<HBOOST_PP_INC(i_)>)
        >
{
    typedef false_ is_le;
    typedef HBOOST_PP_CAT(bind,i_)<
          F
        AUX778076_BIND_N_PARAMS(i_, T)
        > result_;
        
    typedef result_ type;
};

#undef i_
#endif // HBOOST_PP_ITERATION_DEPTH()
#endif // HBOOST_PP_IS_ITERATING
