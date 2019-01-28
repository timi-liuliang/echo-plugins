// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// #include guards intentionally disabled.
// #ifndef HBOOST_DETAIL_FUNCTION_N_DWA2006514_HPP
// # define HBOOST_DETAIL_FUNCTION_N_DWA2006514_HPP

#include <hboost/mpl/void.hpp>
#include <hboost/mpl/apply.hpp>

#include <hboost/preprocessor/control/if.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/punctuation/comma_if.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_params.hpp>
#include <hboost/preprocessor/repetition/repeat.hpp>
#include <hboost/preprocessor/seq/fold_left.hpp>
#include <hboost/preprocessor/seq/seq.hpp>
#include <hboost/preprocessor/seq/for_each.hpp>
#include <hboost/preprocessor/seq/for_each_i.hpp>
#include <hboost/preprocessor/seq/for_each_product.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/type_traits/add_const.hpp>
#include <hboost/type_traits/remove_reference.hpp>

namespace hboost { namespace detail {

# define HBOOST_DETAIL_default_arg(z, n, _)                                      \
    typedef mpl::void_ HBOOST_PP_CAT(arg, n);

# define HBOOST_DETAIL_function_arg(z, n, _)                                     \
    typedef typename remove_reference<                                          \
        typename add_const< HBOOST_PP_CAT(A, n) >::type                          \
    >::type HBOOST_PP_CAT(arg, n);

#define HBOOST_DETAIL_cat_arg_counts(s, state, n)                                \
    HBOOST_PP_IF(                                                                \
        n                                                                       \
      , HBOOST_PP_CAT(state, HBOOST_PP_CAT(_, n))                                 \
      , state                                                                   \
    )                                                                           \
    /**/

#define function_name                                                           \
    HBOOST_PP_SEQ_FOLD_LEFT(                                                     \
        HBOOST_DETAIL_cat_arg_counts                                             \
      , HBOOST_PP_CAT(function, HBOOST_PP_SEQ_HEAD(args))                         \
      , HBOOST_PP_SEQ_TAIL(args)(0)                                              \
    )                                                                           \
    /**/

template<typename F>
struct function_name
{
    HBOOST_PP_REPEAT(
        HBOOST_MPL_LIMIT_METAFUNCTION_ARITY
      , HBOOST_DETAIL_default_arg
      , ~
    )

    template<typename Signature>
    struct result {};

#define HBOOST_DETAIL_function_result(r, _, n)                                   \
    template<typename This HBOOST_PP_ENUM_TRAILING_PARAMS(n, typename A)>        \
    struct result<This(HBOOST_PP_ENUM_PARAMS(n, A))>                             \
    {                                                                           \
        HBOOST_PP_REPEAT(n, HBOOST_DETAIL_function_arg, ~)                        \
        typedef                                                                 \
            typename HBOOST_PP_CAT(mpl::apply, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY)<\
                F                                                               \
                HBOOST_PP_ENUM_TRAILING_PARAMS(                                  \
                    HBOOST_MPL_LIMIT_METAFUNCTION_ARITY                          \
                  , arg                                                         \
                )                                                               \
            >::type                                                             \
        impl;                                                                   \
        typedef typename impl::result_type type;                                \
    };                                                                          \
    /**/

    HBOOST_PP_SEQ_FOR_EACH(HBOOST_DETAIL_function_result, _, args)

# define arg_type(r, _, i, is_const)                                            \
    HBOOST_PP_COMMA_IF(i) HBOOST_PP_CAT(A, i) HBOOST_PP_CAT(const_if, is_const) &

# define result_(r, n, constness)                                               \
    typename result<                                                            \
        function_name(                                                          \
            HBOOST_PP_SEQ_FOR_EACH_I_R(r, arg_type, ~, constness)                \
        )                                                                       \
    >                                                                           \
    /**/

# define param(r, _, i, is_const) HBOOST_PP_COMMA_IF(i)                          \
    HBOOST_PP_CAT(A, i) HBOOST_PP_CAT(const_if, is_const) & HBOOST_PP_CAT(x, i)

# define param_list(r, n, constness)                                            \
    HBOOST_PP_SEQ_FOR_EACH_I_R(r, param, ~, constness)

# define call_operator(r, constness)                                            \
    template<HBOOST_PP_ENUM_PARAMS(HBOOST_PP_SEQ_SIZE(constness), typename A)>    \
        result_(r, HBOOST_PP_SEQ_SIZE(constness), constness)::type               \
    operator ()( param_list(r, HBOOST_PP_SEQ_SIZE(constness), constness) ) const \
    {                                                                           \
        typedef result_(r, HBOOST_PP_SEQ_SIZE(constness), constness)::impl impl; \
        return impl()(HBOOST_PP_ENUM_PARAMS(HBOOST_PP_SEQ_SIZE(constness), x));   \
    }                                                                           \
    /**/

# define const_if0
# define const_if1 const

# define bits(z, n, _) ((0)(1))

# define gen_operator(r, _, n)                                                  \
    HBOOST_PP_SEQ_FOR_EACH_PRODUCT_R(                                            \
        r                                                                       \
      , call_operator                                                           \
      , HBOOST_PP_REPEAT(n, bits, ~)                                             \
    )                                                                           \
    /**/

    HBOOST_PP_SEQ_FOR_EACH(
        gen_operator
      , ~
      , args
    )

# undef bits
# undef const_if1
# undef const_if0
# undef call_operator
# undef param_list
# undef param
# undef result_
# undef default_
# undef arg_type
# undef gen_operator
# undef function_name

# undef args
};

}} // namespace hboost::detail

//#endif // HBOOST_DETAIL_FUNCTION_N_DWA2006514_HPP
