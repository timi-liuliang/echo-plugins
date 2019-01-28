
#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

#ifndef HBOOST_MPL_QUOTE_HPP_INCLUDED
#define HBOOST_MPL_QUOTE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2008
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
#   include <hboost/mpl/void.hpp>
#   include <hboost/mpl/aux_/has_type.hpp>
#endif

#include <hboost/mpl/aux_/config/bcc.hpp>
#include <hboost/mpl/aux_/config/ttp.hpp>

#if defined(HBOOST_MPL_CFG_NO_TEMPLATE_TEMPLATE_PARAMETERS) \
    && !defined(HBOOST_MPL_CFG_BCC590_WORKAROUNDS)
#   define HBOOST_MPL_CFG_NO_QUOTE_TEMPLATE
#endif

#if !defined(HBOOST_MPL_CFG_NO_IMPLICIT_METAFUNCTIONS) \
    && defined(HBOOST_MPL_CFG_NO_HAS_XXX)
#   define HBOOST_MPL_CFG_NO_IMPLICIT_METAFUNCTIONS
#endif

#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
 && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER quote.hpp
#   include <hboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <hboost/mpl/limits/arity.hpp>
#   include <hboost/mpl/aux_/preprocessor/params.hpp>
#   include <hboost/mpl/aux_/config/ctps.hpp>
#   include <hboost/mpl/aux_/config/workaround.hpp>

#   include <hboost/preprocessor/iterate.hpp>
#   include <hboost/preprocessor/cat.hpp>

#if !defined(HBOOST_MPL_CFG_NO_QUOTE_TEMPLATE)

namespace hboost { namespace mpl {

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< typename T, bool has_type_ >
struct quote_impl
// GCC has a problem with metafunction forwarding when T is a
// specialization of a template called 'type'.
# if HBOOST_WORKAROUND(__GNUC__, HBOOST_TESTED_AT(4)) \
    && HBOOST_WORKAROUND(__GNUC_MINOR__, HBOOST_TESTED_AT(0)) \
    && HBOOST_WORKAROUND(__GNUC_PATCHLEVEL__, HBOOST_TESTED_AT(2))
{
    typedef typename T::type type;
};
# else 
    : T
{
};
# endif 

template< typename T >
struct quote_impl<T,false>
{
    typedef T type;
};

#else // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template< bool > struct quote_impl
{
    template< typename T > struct result_
        : T
    {
    };
};

template<> struct quote_impl<false>
{
    template< typename T > struct result_
    {
        typedef T type;
    };
};

#endif 

#define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(1, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY, <hboost/mpl/quote.hpp>))
#include HBOOST_PP_ITERATE()

}}

#endif // HBOOST_MPL_CFG_NO_QUOTE_TEMPLATE

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // HBOOST_MPL_QUOTE_HPP_INCLUDED

///// iteration

#else
#define i_ HBOOST_PP_FRAME_ITERATION(1)

template<
      template< HBOOST_MPL_PP_PARAMS(i_, typename P) > class F
    , typename Tag = void_
    >
struct HBOOST_PP_CAT(quote,i_)
{
    template< HBOOST_MPL_PP_PARAMS(i_, typename U) > struct apply
#if defined(HBOOST_MPL_CFG_BCC590_WORKAROUNDS)
    {
        typedef typename quote_impl<
              F< HBOOST_MPL_PP_PARAMS(i_, U) >
            , aux::has_type< F< HBOOST_MPL_PP_PARAMS(i_, U) > >::value
            >::type type;
    };
#elif !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
        : quote_impl<
              F< HBOOST_MPL_PP_PARAMS(i_, U) >
            , aux::has_type< F< HBOOST_MPL_PP_PARAMS(i_, U) > >::value
            >
    {
    };
#else
        : quote_impl< aux::has_type< F< HBOOST_MPL_PP_PARAMS(i_, U) > >::value >
            ::template result_< F< HBOOST_MPL_PP_PARAMS(i_, U) > >
    {
    };
#endif
};

#undef i_
#endif // HBOOST_PP_IS_ITERATING
