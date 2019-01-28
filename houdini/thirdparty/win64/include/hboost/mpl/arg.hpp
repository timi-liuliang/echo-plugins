
#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

#ifndef HBOOST_MPL_ARG_HPP_INCLUDED
#define HBOOST_MPL_ARG_HPP_INCLUDED

// Copyright Peter Dimov 2001-2002
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
#   include <hboost/mpl/arg_fwd.hpp>
#   include <hboost/mpl/aux_/na.hpp>
#   include <hboost/mpl/aux_/na_assert.hpp>
#   include <hboost/mpl/aux_/arity_spec.hpp>
#   include <hboost/mpl/aux_/arg_typedef.hpp>
#endif

#include <hboost/mpl/aux_/config/static_constant.hpp>
#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER arg.hpp
#   include <hboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <hboost/mpl/limits/arity.hpp>
#   include <hboost/mpl/aux_/preprocessor/default_params.hpp>
#   include <hboost/mpl/aux_/preprocessor/params.hpp>
#   include <hboost/mpl/aux_/config/lambda.hpp>
#   include <hboost/mpl/aux_/config/dtp.hpp>
#   include <hboost/mpl/aux_/nttp_decl.hpp>

#   include <hboost/preprocessor/iterate.hpp>
#   include <hboost/preprocessor/inc.hpp>
#   include <hboost/preprocessor/cat.hpp>

HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

// local macro, #undef-ined at the end of the header
#if !defined(HBOOST_MPL_CFG_NO_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
#   define AUX778076_ARG_N_DEFAULT_PARAMS(param,value) \
    HBOOST_MPL_PP_DEFAULT_PARAMS( \
          HBOOST_MPL_LIMIT_METAFUNCTION_ARITY \
        , param \
        , value \
        ) \
    /**/
#else
#   define AUX778076_ARG_N_DEFAULT_PARAMS(param,value) \
    HBOOST_MPL_PP_PARAMS( \
          HBOOST_MPL_LIMIT_METAFUNCTION_ARITY \
        , param \
        ) \
    /**/
#endif

#define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY, <hboost/mpl/arg.hpp>))
#include HBOOST_PP_ITERATE()


#   undef AUX778076_ARG_N_DEFAULT_PARAMS

HBOOST_MPL_AUX_NONTYPE_ARITY_SPEC(1,int,arg)

HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // HBOOST_MPL_ARG_HPP_INCLUDED

///// iteration

#else
#define i_ HBOOST_PP_FRAME_ITERATION(1)

#if i_ > 0

template<> struct arg<i_>
{
    HBOOST_STATIC_CONSTANT(int, value = i_);
    typedef arg<HBOOST_PP_INC(i_)> next;
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          AUX778076_ARG_N_DEFAULT_PARAMS(typename U, na)
        >
    struct apply
    {
        typedef HBOOST_PP_CAT(U,i_) type;
        HBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

#else

template<> struct arg<-1>
{
    HBOOST_STATIC_CONSTANT(int, value = -1);
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    HBOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          AUX778076_ARG_N_DEFAULT_PARAMS(typename U, na)
        >
    struct apply
    {
        typedef U1 type;
        HBOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

#endif // i_ > 0

#undef i_
#endif // HBOOST_PP_IS_ITERATING
