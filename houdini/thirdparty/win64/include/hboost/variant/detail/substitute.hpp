
#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

//-----------------------------------------------------------------------------
// boost variant/detail/substitute.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_DETAIL_SUBSTITUTE_HPP
#define HBOOST_VARIANT_DETAIL_SUBSTITUTE_HPP

#include "hboost/mpl/aux_/config/ctps.hpp"

#include "hboost/variant/detail/substitute_fwd.hpp"
#include "hboost/variant/variant_fwd.hpp" // for HBOOST_VARIANT_DO_NOT_USE_VARIADIC_TEMPLATES
#include "hboost/mpl/aux_/lambda_arity_param.hpp"
#include "hboost/mpl/aux_/preprocessor/params.hpp"
#include "hboost/mpl/aux_/preprocessor/repeat.hpp"
#include "hboost/mpl/int_fwd.hpp"
#include "hboost/mpl/limits/arity.hpp"
#include "hboost/preprocessor/cat.hpp"
#include "hboost/preprocessor/empty.hpp"
#include "hboost/preprocessor/arithmetic/inc.hpp"
#include "hboost/preprocessor/iterate.hpp"

namespace hboost {
namespace detail { namespace variant {

#if !defined(HBOOST_VARIANT_DETAIL_NO_SUBSTITUTE)

///////////////////////////////////////////////////////////////////////////////
// (detail) metafunction substitute
//
// Substitutes one type for another in the given type expression.
//

//
// primary template
//
template <
      typename T, typename Dest, typename Source
      HBOOST_MPL_AUX_LAMBDA_ARITY_PARAM(
          typename Arity /* = ... (see substitute_fwd.hpp) */
        )
    >
struct substitute
{
    typedef T type;
};

//
// tag substitution specializations
//

#define HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_SUBSTITUTE_TAG(CV_) \
    template <typename Dest, typename Source> \
    struct substitute< \
          CV_ Source \
        , Dest \
        , Source \
          HBOOST_MPL_AUX_LAMBDA_ARITY_PARAM(mpl::int_<-1>) \
        > \
    { \
        typedef CV_ Dest type; \
    }; \
    /**/

HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_SUBSTITUTE_TAG( HBOOST_PP_EMPTY() )
HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_SUBSTITUTE_TAG(const)
HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_SUBSTITUTE_TAG(volatile)
HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_SUBSTITUTE_TAG(const volatile)

#undef HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_SUBSTITUTE_TAG

//
// pointer specializations
//
#define HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_HANDLE_POINTER(CV_) \
    template <typename T, typename Dest, typename Source> \
    struct substitute< \
          T * CV_ \
        , Dest \
        , Source \
          HBOOST_MPL_AUX_LAMBDA_ARITY_PARAM(mpl::int_<-1>) \
        > \
    { \
        typedef typename substitute< \
              T, Dest, Source \
            >::type * CV_ type; \
    }; \
    /**/

HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_HANDLE_POINTER( HBOOST_PP_EMPTY() )
HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_HANDLE_POINTER(const)
HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_HANDLE_POINTER(volatile)
HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_HANDLE_POINTER(const volatile)

#undef HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL_HANDLE_POINTER

//
// reference specializations
//
template <typename T, typename Dest, typename Source>
struct substitute<
      T&
    , Dest
    , Source
      HBOOST_MPL_AUX_LAMBDA_ARITY_PARAM(mpl::int_<-1>)
    >
{
    typedef typename substitute<
          T, Dest, Source
        >::type & type;
};

//
// template expression (i.e., F<...>) specializations
//

#if !defined(HBOOST_VARIANT_DO_NOT_USE_VARIADIC_TEMPLATES)
template <
      template <typename...> class F
    , typename... Ts
    , typename Dest
    , typename Source
      HBOOST_MPL_AUX_LAMBDA_ARITY_PARAM(typename Arity)
    >
struct substitute<
      F<Ts...>
    , Dest
    , Source
      HBOOST_MPL_AUX_LAMBDA_ARITY_PARAM(Arity)
    >
{
    typedef F<typename substitute<
          Ts, Dest, Source
        >::type...> type;
};
#endif // !defined(HBOOST_VARIANT_DO_NOT_USE_VARIADIC_TEMPLATES)

#define HBOOST_VARIANT_AUX_SUBSTITUTE_TYPEDEF_IMPL(N) \
    typedef typename substitute< \
          HBOOST_PP_CAT(U,N), Dest, Source \
        >::type HBOOST_PP_CAT(u,N); \
    /**/

#define HBOOST_VARIANT_AUX_SUBSTITUTE_TYPEDEF(z, N, _) \
    HBOOST_VARIANT_AUX_SUBSTITUTE_TYPEDEF_IMPL( HBOOST_PP_INC(N) ) \
    /**/

#define HBOOST_PP_ITERATION_LIMITS (0,HBOOST_MPL_LIMIT_METAFUNCTION_ARITY)
#define HBOOST_PP_FILENAME_1 "hboost/variant/detail/substitute.hpp"
#include HBOOST_PP_ITERATE()

#undef HBOOST_VARIANT_AUX_SUBSTITUTE_TYPEDEF_IMPL
#undef HBOOST_VARIANT_AUX_SUBSTITUTE_TYPEDEF

#endif // !defined(HBOOST_VARIANT_DETAIL_NO_SUBSTITUTE)

}} // namespace detail::variant
} // namespace hboost

#endif // HBOOST_VARIANT_DETAIL_SUBSTITUTE_HPP

///// iteration, depth == 1

#elif HBOOST_PP_ITERATION_DEPTH() == 1
#define i HBOOST_PP_FRAME_ITERATION(1)

#if i > 0

//
// template specializations
//
template <
      template < HBOOST_MPL_PP_PARAMS(i,typename P) > class T
    , HBOOST_MPL_PP_PARAMS(i,typename U)
    , typename Dest
    , typename Source
    >
struct substitute<
      T< HBOOST_MPL_PP_PARAMS(i,U) >
    , Dest
    , Source
      HBOOST_MPL_AUX_LAMBDA_ARITY_PARAM(mpl::int_<( i )>)
    >
{
private:
    HBOOST_MPL_PP_REPEAT(i, HBOOST_VARIANT_AUX_SUBSTITUTE_TYPEDEF, _)

public:
    typedef T< HBOOST_MPL_PP_PARAMS(i,u) > type;
};

//
// function specializations
//
template <
      typename R
    , HBOOST_MPL_PP_PARAMS(i,typename U)
    , typename Dest
    , typename Source
    >
struct substitute<
      R (*)( HBOOST_MPL_PP_PARAMS(i,U) )
    , Dest
    , Source
      HBOOST_MPL_AUX_LAMBDA_ARITY_PARAM(mpl::int_<-1>)
    >
{
private:
    typedef typename substitute< R, Dest, Source >::type r;
    HBOOST_MPL_PP_REPEAT(i, HBOOST_VARIANT_AUX_SUBSTITUTE_TYPEDEF, _)

public:
    typedef r (*type)( HBOOST_MPL_PP_PARAMS(i,u) );
};

#elif i == 0

//
// zero-arg function specialization
//
template <
      typename R, typename Dest, typename Source
    >
struct substitute<
      R (*)( void )
    , Dest
    , Source
      HBOOST_MPL_AUX_LAMBDA_ARITY_PARAM(mpl::int_<-1>)
    >
{
private:
    typedef typename substitute< R, Dest, Source >::type r;

public:
    typedef r (*type)( void );
};

#endif // i

#undef i
#endif // HBOOST_PP_IS_ITERATING
