///////////////////////////////////////////////////////////////////////////////
/// \file repeat.hpp
/// Contains macros to ease the generation of repetitious code constructs
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_REPEAT_HPP_EAN_11_24_2008
#define HBOOST_PROTO_REPEAT_HPP_EAN_11_24_2008

#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/facilities/intercept.hpp>
#include <hboost/preprocessor/repetition/enum.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/iteration/local.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/proto/proto_fwd.hpp> // for HBOOST_PROTO_MAX_ARITY

////////////////////////////////////////////
/// INTERNAL ONLY
#define HBOOST_PROTO_ref_a_aux(Z, N, DATA)\
  hboost::ref(HBOOST_PP_CAT(proto_a, N))

/// \brief Generates a sequence like <tt>typename A0, typename A1, ...</tt>
///
#define HBOOST_PROTO_typename_A(N)\
  HBOOST_PP_ENUM_PARAMS(N, typename proto_A)

/// \brief Generates a sequence like <tt>A0 const &, A1 const &, ...</tt>
///
#define HBOOST_PROTO_A_const_ref(N)\
  HBOOST_PP_ENUM_BINARY_PARAMS(N, proto_A, const & HBOOST_PP_INTERCEPT)

/// \brief Generates a sequence like <tt>A0 &, A1 &, ...</tt>
///
#define HBOOST_PROTO_A_ref(N)\
  HBOOST_PP_ENUM_BINARY_PARAMS(N, proto_A, & HBOOST_PP_INTERCEPT)

/// \brief Generates a sequence like <tt>A0, A1, ...</tt>
///
#define HBOOST_PROTO_A(N)\
  HBOOST_PP_ENUM_PARAMS(N, proto_A)

/// \brief Generates a sequence like <tt>A0 const, A1 const, ...</tt>
///
#define HBOOST_PROTO_A_const(N)\
  HBOOST_PP_ENUM_PARAMS(N, const proto_A)

/// \brief Generates a sequence like <tt>A0 const &a0, A1 const &a0, ...</tt>
///
#define HBOOST_PROTO_A_const_ref_a(N)\
  HBOOST_PP_ENUM_BINARY_PARAMS(N, proto_A, const &proto_a)

/// \brief Generates a sequence like <tt>A0 &a0, A1 &a0, ...</tt>
///
#define HBOOST_PROTO_A_ref_a(N)\
  HBOOST_PP_ENUM_BINARY_PARAMS(N, proto_A, &proto_a)

/// \brief Generates a sequence like <tt>hboost::ref(a0), hboost::ref(a1), ...</tt>
///
#define HBOOST_PROTO_ref_a(N)\
  HBOOST_PP_ENUM(N, HBOOST_PROTO_ref_a_aux, ~)

/// \brief Generates a sequence like <tt>a0, a1, ...</tt>
///
#define HBOOST_PROTO_a(N)\
  HBOOST_PP_ENUM_PARAMS(N, proto_a)

////////////////////////////////////////////
/// INTERNAL ONLY
#define HBOOST_PROTO_invoke(Z, N, DATA)\
  HBOOST_PP_TUPLE_ELEM(5,0,DATA)(N, HBOOST_PP_TUPLE_ELEM(5,1,DATA), HBOOST_PP_TUPLE_ELEM(5,2,DATA), HBOOST_PP_TUPLE_ELEM(5,3,DATA), HBOOST_PP_TUPLE_ELEM(5,4,DATA))

/// \brief Repeatedly invoke the specified macro.
///
/// HBOOST_PROTO_REPEAT_FROM_TO_EX() is used generate the kind of repetitive code that is typical
/// of EDSLs built with Proto. HBOOST_PROTO_REPEAT_FROM_TO_EX(FROM, TO, MACRO, typename_A, A, A_a, a)  is equivalent to:
///
/// \code
/// MACRO(FROM, typename_A, A, A_a, a)
/// MACRO(FROM+1, typename_A, A, A_a, a)
/// ...
/// MACRO(TO-1, typename_A, A, A_a, a)
/// \endcode
#define HBOOST_PROTO_REPEAT_FROM_TO_EX(FROM, TO, MACRO, typename_A, A, A_a, a)\
  HBOOST_PP_REPEAT_FROM_TO(FROM, TO, HBOOST_PROTO_invoke, (MACRO, typename_A, A, A_a, a))

/// \brief Repeatedly invoke the specified macro.
///
/// HBOOST_PROTO_REPEAT_FROM_TO() is used generate the kind of repetitive code that is typical
/// of EDSLs built with Proto. HBOOST_PROTO_REPEAT_FROM_TO(FROM, TO, MACRO)  is equivalent to:
///
/// \code
/// MACRO(FROM, HBOOST_PROTO_typename_A, HBOOST_PROTO_A_const_ref, HBOOST_PROTO_A_const_ref_a, HBOOST_PROTO_ref_a)
/// MACRO(FROM+1, HBOOST_PROTO_typename_A, HBOOST_PROTO_A_const_ref, HBOOST_PROTO_A_const_ref_a, HBOOST_PROTO_ref_a)
/// ...
/// MACRO(TO-1, HBOOST_PROTO_typename_A, HBOOST_PROTO_A_const_ref, HBOOST_PROTO_A_const_ref_a, HBOOST_PROTO_ref_a)
/// \endcode
///
/// Example:
///
/** \code

// Generate HBOOST_PROTO_MAX_ARITY-1 overloads of the
// following construct() function template.
#define M0(N, typename_A, A_const_ref, A_const_ref_a, ref_a)      \
template<typename T, typename_A(N)>                               \
typename proto::result_of::make_expr<                             \
    proto::tag::function                                          \
  , construct_helper<T>                                           \
  , A_const_ref(N)                                                \
>::type const                                                     \
construct(A_const_ref_a(N))                                       \
{                                                                 \
    return proto::make_expr<                                      \
        proto::tag::function                                      \
    >(                                                            \
        construct_helper<T>()                                     \
      , ref_a(N)                                                  \
    );                                                            \
}
HBOOST_PROTO_REPEAT_FROM_TO(1, HBOOST_PROTO_MAX_ARITY, M0)
#undef M0

\endcode
**/
/// The above invocation of HBOOST_PROTO_REPEAT_FROM_TO()  will generate
/// the following code:
///
/// \code
/// template<typename T, typename A0>
/// typename proto::result_of::make_expr<
///     proto::tag::function
///   , construct_helper<T>
///  , A0 const &
/// >::type const
/// construct(A0 const & a0)
/// {
///     return proto::make_expr<
///         proto::tag::function
///     >(
///         construct_helper<T>()
///       , hboost::ref(a0)
///     );
/// }
///
/// template<typename T, typename A0, typename A1>
/// typename proto::result_of::make_expr<
///     proto::tag::function
///   , construct_helper<T>
///   , A0 const &
///   , A1 const &
/// >::type const
/// construct(A0 const & a0, A1 const & a1)
/// {
///     return proto::make_expr<
///         proto::tag::function
///     >(
///         construct_helper<T>()
///       , hboost::ref(a0)
///       , hboost::ref(a1)
///     );
/// }
///
/// // ... and so on, up to HBOOST_PROTO_MAX_ARITY-1 arguments ...
/// \endcode
#define HBOOST_PROTO_REPEAT_FROM_TO(FROM, TO, MACRO)\
  HBOOST_PROTO_REPEAT_FROM_TO_EX(FROM, TO, MACRO, HBOOST_PROTO_typename_A, HBOOST_PROTO_A_const_ref, HBOOST_PROTO_A_const_ref_a, HBOOST_PROTO_ref_a)

/// \brief Repeatedly invoke the specified macro.
///
/// HBOOST_PROTO_REPEAT_EX() is used generate the kind of repetitive code that is typical
/// of EDSLs built with Proto. HBOOST_PROTO_REPEAT_EX(MACRO, typename_A, A, A_a, a)  is equivalent to:
///
/// \code
/// MACRO(1, typename_A, A, A_a, a)
/// MACRO(2, typename_A, A, A_a, a)
/// ...
/// MACRO(HBOOST_PROTO_MAX_ARITY, typename_A, A, A_a, a)
/// \endcode
#define HBOOST_PROTO_REPEAT_EX(MACRO, typename_A, A, A_a, a)\
  HBOOST_PROTO_REPEAT_FROM_TO_EX(1, HBOOST_PP_INC(HBOOST_PROTO_MAX_ARITY), MACRO, HBOOST_PROTO_typename_A, HBOOST_PROTO_A_const_ref, HBOOST_PROTO_A_const_ref_a, HBOOST_PROTO_ref_a)

/// \brief Repeatedly invoke the specified macro.
///
/// HBOOST_PROTO_REPEAT() is used generate the kind of repetitive code that is typical
/// of EDSLs built with Proto. HBOOST_PROTO_REPEAT(MACRO)  is equivalent to:
///
/// \code
/// MACRO(1, HBOOST_PROTO_typename_A, HBOOST_PROTO_A_const_ref, HBOOST_PROTO_A_const_ref_a, HBOOST_PROTO_ref_a)
/// MACRO(2, HBOOST_PROTO_typename_A, HBOOST_PROTO_A_const_ref, HBOOST_PROTO_A_const_ref_a, HBOOST_PROTO_ref_a)
/// ...
/// MACRO(HBOOST_PROTO_MAX_ARITY, HBOOST_PROTO_typename_A, HBOOST_PROTO_A_const_ref, HBOOST_PROTO_A_const_ref_a, HBOOST_PROTO_ref_a)
/// \endcode
#define HBOOST_PROTO_REPEAT(MACRO)\
  HBOOST_PROTO_REPEAT_FROM_TO(1, HBOOST_PP_INC(HBOOST_PROTO_MAX_ARITY), MACRO)

/// \brief Repeatedly invoke the specified macro.
///
/// HBOOST_PROTO_LOCAL_ITERATE() is used generate the kind of repetitive code that is typical
/// of EDSLs built with Proto. This macro causes the user-defined macro HBOOST_PROTO_LOCAL_MACRO to
/// be expanded with values in the range specified by HBOOST_PROTO_LOCAL_LIMITS.
///
/// Usage:
///
/// \code
/// #include HBOOST_PROTO_LOCAL_ITERATE()
/// \endcode
///
/// Example:
///
/** \code

// Generate HBOOST_PROTO_MAX_ARITY-1 overloads of the
// following construct() function template.
#define HBOOST_PROTO_LOCAL_MACRO(N, typename_A, A_const_ref,       \
  A_const_ref_a, ref_a)                                           \
template<typename T, typename_A(N)>                               \
typename proto::result_of::make_expr<                             \
    proto::tag::function                                          \
  , construct_helper<T>                                           \
  , A_const_ref(N)                                                \
>::type const                                                     \
construct(A_const_ref_a(N))                                       \
{                                                                 \
    return proto::make_expr<                                      \
        proto::tag::function                                      \
    >(                                                            \
        construct_helper<T>()                                     \
      , ref_a(N)                                                  \
    );                                                            \
}
#define HBOOST_PROTO_LOCAL_LIMITS (1, HBOOST_PP_DEC(HBOOST_PROTO_MAX_ARITY))
#include HBOOST_PROTO_LOCAL_ITERATE()

\endcode
**/
/// The above inclusion of HBOOST_PROTO_LOCAL_ITERATE() will generate
/// the following code:
///
/// \code
/// template<typename T, typename A0>
/// typename proto::result_of::make_expr<
///     proto::tag::function
///   , construct_helper<T>
///  , A0 const &
/// >::type const
/// construct(A0 const & a0)
/// {
///     return proto::make_expr<
///         proto::tag::function
///     >(
///         construct_helper<T>()
///       , hboost::ref(a0)
///     );
/// }
///
/// template<typename T, typename A0, typename A1>
/// typename proto::result_of::make_expr<
///     proto::tag::function
///   , construct_helper<T>
///   , A0 const &
///   , A1 const &
/// >::type const
/// construct(A0 const & a0, A1 const & a1)
/// {
///     return proto::make_expr<
///         proto::tag::function
///     >(
///         construct_helper<T>()
///       , hboost::ref(a0)
///       , hboost::ref(a1)
///     );
/// }
///
/// // ... and so on, up to HBOOST_PROTO_MAX_ARITY-1 arguments ...
/// \endcode
///
/// If HBOOST_PROTO_LOCAL_LIMITS is not defined by the user, it defaults
/// to (1, HBOOST_PROTO_MAX_ARITY)
///
/// At each iteration, HBOOST_PROTO_LOCAL_MACRO is invoked with the current
/// iteration number and the following 4 macro parameters:
///
/// \li HBOOST_PROTO_LOCAL_typename_A
/// \li HBOOST_PROTO_LOCAL_A
/// \li HBOOST_PROTO_LOCAL_A_a
/// \li HBOOST_PROTO_LOCAL_a
///
/// If these macros are not defined by the user, they default respectively to:
///
/// \li HBOOST_PROTO_typename_A
/// \li HBOOST_PROTO_A_const_ref
/// \li HBOOST_PROTO_A_const_ref_a
/// \li HBOOST_PROTO_ref_a
///
/// After including HBOOST_PROTO_LOCAL_ITERATE(), the following macros are
/// automatically undefined:
///
/// \li HBOOST_PROTO_LOCAL_MACRO
/// \li HBOOST_PROTO_LOCAL_LIMITS
/// \li HBOOST_PROTO_LOCAL_typename_A
/// \li HBOOST_PROTO_LOCAL_A
/// \li HBOOST_PROTO_LOCAL_A_a
/// \li HBOOST_PROTO_LOCAL_a
#define HBOOST_PROTO_LOCAL_ITERATE() <hboost/proto/detail/local.hpp>

#endif
