
#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

#ifndef HBOOST_MPL_AUX_LAMBDA_NO_CTPS_HPP_INCLUDED
#define HBOOST_MPL_AUX_LAMBDA_NO_CTPS_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
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
#   include <hboost/mpl/is_placeholder.hpp>
#   include <hboost/mpl/if.hpp>
#   include <hboost/mpl/identity.hpp>
#   include <hboost/mpl/bool.hpp>
#   include <hboost/mpl/aux_/na_spec.hpp>
#   include <hboost/mpl/aux_/lambda_support.hpp>
#   include <hboost/mpl/aux_/template_arity.hpp>
#   include <hboost/mpl/aux_/value_wknd.hpp>
#endif

#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if    !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER lambda_no_ctps.hpp
#   include <hboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <hboost/mpl/limits/arity.hpp>
#   include <hboost/mpl/aux_/preprocessor/params.hpp>
#   include <hboost/mpl/aux_/preprocessor/default_params.hpp>
#   include <hboost/mpl/aux_/preprocessor/repeat.hpp>
#   include <hboost/mpl/aux_/preprocessor/enum.hpp>
#   include <hboost/mpl/aux_/config/msvc.hpp>
#   include <hboost/mpl/aux_/config/workaround.hpp>

#   include <hboost/preprocessor/comma_if.hpp>
#   include <hboost/preprocessor/iterate.hpp>
#   include <hboost/preprocessor/inc.hpp>
#   include <hboost/preprocessor/cat.hpp>

namespace hboost { namespace mpl {

#   define AUX778076_LAMBDA_PARAMS(i_, param) \
    HBOOST_MPL_PP_PARAMS(i_, param) \
    /**/

namespace aux {

#define n_ HBOOST_MPL_LIMIT_METAFUNCTION_ARITY
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
#undef n_

template< typename Arity > struct lambda_impl
{
    template< typename T, typename Tag, typename Protect > struct result_
    {
        typedef T type;
        typedef is_placeholder<T> is_le;
    };
};

#define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(1, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY, <hboost/mpl/aux_/lambda_no_ctps.hpp>))
#include HBOOST_PP_ITERATE()

} // namespace aux

template<
      typename T
    , typename Tag
    , typename Protect
    >
struct lambda
{
    /// Metafunction forwarding confuses MSVC 6.x
    typedef typename aux::template_arity<T>::type arity_;
    typedef typename aux::lambda_impl<arity_>
        ::template result_< T,Tag,Protect > l_;

    typedef typename l_::type type;
    typedef typename l_::is_le is_le;
    
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(3, lambda, (T, Tag, Protect))
};

HBOOST_MPL_AUX_NA_SPEC2(1, 3, lambda)

template<
      typename T
    >
struct is_lambda_expression
    : lambda<T>::is_le
{
};

#   undef AUX778076_LAMBDA_PARAMS

}}

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // HBOOST_MPL_AUX_LAMBDA_NO_CTPS_HPP_INCLUDED

///// iteration, depth == 1

#else

#define i_ HBOOST_PP_FRAME_ITERATION(1)

#   define AUX778076_LAMBDA_TYPEDEF(unused, i_, F) \
    typedef lambda< \
          typename F::HBOOST_PP_CAT(arg,HBOOST_PP_INC(i_)) \
        , Tag \
        , false_ \
        > HBOOST_PP_CAT(l,HBOOST_PP_INC(i_)); \
    /**/

#   define AUX778076_IS_LE_TYPEDEF(unused, i_, unused2) \
    typedef typename HBOOST_PP_CAT(l,HBOOST_PP_INC(i_))::is_le \
        HBOOST_PP_CAT(is_le,HBOOST_PP_INC(i_)); \
    /**/

#   define AUX778076_IS_LAMBDA_EXPR(unused, i_, unused2) \
    HBOOST_PP_COMMA_IF(i_) \
    HBOOST_MPL_AUX_MSVC_VALUE_WKND(HBOOST_PP_CAT(is_le,HBOOST_PP_INC(i_)))::value \
    /**/

#   define AUX778076_LAMBDA_RESULT(unused, i_, unused2) \
    , typename HBOOST_PP_CAT(l,HBOOST_PP_INC(i_))::type \
    /**/

template<> struct lambda_impl< int_<i_> >
{
    template< typename F, typename Tag, typename Protect > struct result_
    {
        HBOOST_MPL_PP_REPEAT(i_, AUX778076_LAMBDA_TYPEDEF, F)
        HBOOST_MPL_PP_REPEAT(i_, AUX778076_IS_LE_TYPEDEF, unused)

        typedef aux::lambda_or<
              HBOOST_MPL_PP_REPEAT(i_, AUX778076_IS_LAMBDA_EXPR, unused)
            > is_le;

        typedef HBOOST_PP_CAT(bind,i_)<
              typename F::rebind
            HBOOST_MPL_PP_REPEAT(i_, AUX778076_LAMBDA_RESULT, unused)
            > bind_;

        typedef typename if_<
              is_le
            , if_< Protect, mpl::protect<bind_>, bind_ >
            , identity<F>
            >::type type_;
    
        typedef typename type_::type type;
    };
};

#   undef AUX778076_LAMBDA_RESULT
#   undef AUX778076_IS_LAMBDA_EXPR
#   undef AUX778076_IS_LE_TYPEDEF
#   undef AUX778076_LAMBDA_TYPEDEF

#undef i_

#endif // HBOOST_PP_IS_ITERATING
