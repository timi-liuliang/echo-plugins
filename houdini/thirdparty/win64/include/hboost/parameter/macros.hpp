// Copyright David Abrahams, Daniel Wallin 2003. Use, modification and 
// distribution is subject to the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PARAMETER_MACROS_050412_HPP
#define HBOOST_PARAMETER_MACROS_050412_HPP

#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/repetition/repeat_from_to.hpp>
#include <hboost/preprocessor/arithmetic/inc.hpp>
#include <hboost/preprocessor/logical/bool.hpp>
#include <hboost/preprocessor/punctuation/comma_if.hpp>
#include <hboost/preprocessor/control/expr_if.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/detail/workaround.hpp>

#define HBOOST_PARAMETER_FUN_TEMPLATE_HEAD1(n) \
    template<HBOOST_PP_ENUM_PARAMS(n, class T)>

#define HBOOST_PARAMETER_FUN_TEMPLATE_HEAD0(n)

#if ! defined(HBOOST_NO_SFINAE) && ! HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x592)) 

# define HBOOST_PARAMETER_MATCH_TYPE(n, param)           \
            HBOOST_PP_EXPR_IF(n, typename) param::match  \
            <                                           \
                HBOOST_PP_ENUM_PARAMS(n, T)              \
            >::type 

#else

# define HBOOST_PARAMETER_MATCH_TYPE(n, param) param

#endif

#define HBOOST_PARAMETER_FUN_DECL(z, n, params)                                      \
                                                                                    \
    HBOOST_PP_CAT(HBOOST_PARAMETER_FUN_TEMPLATE_HEAD, HBOOST_PP_BOOL(n))(n)            \
                                                                                    \
    HBOOST_PP_TUPLE_ELEM(3, 0, params)                                               \
        HBOOST_PP_TUPLE_ELEM(3, 1, params)(                                          \
            HBOOST_PP_ENUM_BINARY_PARAMS(n, T, const& p)                             \
            HBOOST_PP_COMMA_IF(n)                                                    \
            HBOOST_PARAMETER_MATCH_TYPE(n,HBOOST_PP_TUPLE_ELEM(3, 2, params))         \
            kw = HBOOST_PP_TUPLE_ELEM(3, 2, params)()                                \
        )                                                                           \
    {                                                                               \
        return HBOOST_PP_CAT(HBOOST_PP_TUPLE_ELEM(3, 1, params), _with_named_params)( \
            kw(HBOOST_PP_ENUM_PARAMS(n, p))                                          \
        );                                                                          \
    }

// Generates:
//
// template<class Params>
// ret name ## _with_named_params(Params const&);
//
// template<class T0>
// ret name(T0 const& p0, typename parameters::match<T0>::type kw = parameters())
// {
//     return name ## _with_named_params(kw(p0));
// }
//
// template<class T0, ..., class TN>
// ret name(T0 const& p0, ..., TN const& PN
//    , typename parameters::match<T0, ..., TN>::type kw = parameters())
// {
//     return name ## _with_named_params(kw(p0, ..., pN));
// }
//
// template<class Params>
// ret name ## _with_named_params(Params const&)
//
// lo and hi determines the min and max arity of the generated functions.

#define HBOOST_PARAMETER_FUN(ret, name, lo, hi, parameters)                          \
                                                                                    \
    template<class Params>                                                          \
    ret HBOOST_PP_CAT(name, _with_named_params)(Params const& p);                    \
                                                                                    \
    HBOOST_PP_REPEAT_FROM_TO(                                                        \
        lo, HBOOST_PP_INC(hi), HBOOST_PARAMETER_FUN_DECL, (ret, name, parameters))    \
                                                                                    \
    template<class Params>                                                          \
    ret HBOOST_PP_CAT(name, _with_named_params)(Params const& p)

#define HBOOST_PARAMETER_MEMFUN(ret, name, lo, hi, parameters)                       \
                                                                                    \
    HBOOST_PP_REPEAT_FROM_TO(                                                        \
        lo, HBOOST_PP_INC(hi), HBOOST_PARAMETER_FUN_DECL, (ret, name, parameters))    \
                                                                                    \
    template<class Params>                                                          \
    ret HBOOST_PP_CAT(name, _with_named_params)(Params const& p)

#endif // HBOOST_PARAMETER_MACROS_050412_HPP

