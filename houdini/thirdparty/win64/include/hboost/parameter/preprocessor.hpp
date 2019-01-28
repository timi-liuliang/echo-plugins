// Copyright Daniel Wallin 2006. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PARAMETER_PREPROCESSOR_060206_HPP
# define HBOOST_PARAMETER_PREPROCESSOR_060206_HPP

# include <hboost/parameter/parameters.hpp>
# include <hboost/parameter/binding.hpp>
# include <hboost/parameter/match.hpp>

# include <hboost/parameter/aux_/parenthesized_type.hpp>
# include <hboost/parameter/aux_/cast.hpp>
# include <hboost/parameter/aux_/preprocessor/flatten.hpp>

# include <hboost/preprocessor/repetition/repeat_from_to.hpp>
# include <hboost/preprocessor/comparison/equal.hpp>
# include <hboost/preprocessor/control/if.hpp>
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/control/expr_if.hpp>
# include <hboost/preprocessor/repetition/enum_params.hpp>
# include <hboost/preprocessor/repetition/enum_binary_params.hpp>
# include <hboost/preprocessor/repetition/enum_trailing.hpp>
# include <hboost/preprocessor/seq/first_n.hpp>
# include <hboost/preprocessor/seq/for_each_product.hpp>
# include <hboost/preprocessor/seq/for_each_i.hpp> 
# include <hboost/preprocessor/tuple/elem.hpp> 
# include <hboost/preprocessor/tuple/eat.hpp>
# include <hboost/preprocessor/seq/fold_left.hpp>
# include <hboost/preprocessor/seq/push_back.hpp>
# include <hboost/preprocessor/seq/size.hpp>
# include <hboost/preprocessor/seq/enum.hpp>
# include <hboost/preprocessor/seq/push_back.hpp>

# include <hboost/preprocessor/detail/is_nullary.hpp>

# include <hboost/mpl/always.hpp>
# include <hboost/mpl/apply_wrap.hpp>

namespace hboost { namespace parameter { namespace aux {

#  if ! defined(HBOOST_NO_SFINAE) && ! HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x592))

// Given Match, which is "void x" where x is an argument matching
// criterion, extract a corresponding MPL predicate.
template <class Match>
struct unwrap_predicate;

// Match anything
template <>
struct unwrap_predicate<void*>
{
    typedef mpl::always<mpl::true_> type;
};

#if HBOOST_WORKAROUND(__SUNPRO_CC, HBOOST_TESTED_AT(0x580))

typedef void* voidstar;

// A matching predicate is explicitly specified
template <class Predicate>
struct unwrap_predicate<voidstar (Predicate)>
{
    typedef Predicate type;
};

#else

// A matching predicate is explicitly specified
template <class Predicate>
struct unwrap_predicate<void *(Predicate)>
{
    typedef Predicate type;
};

#endif 


// A type to which the argument is supposed to be convertible is
// specified
template <class Target>
struct unwrap_predicate<void (Target)>
{
    typedef is_convertible<mpl::_, Target> type;
};

// Recast the ParameterSpec's nested match metafunction as a free metafunction
template <
    class Parameters
  , HBOOST_PP_ENUM_BINARY_PARAMS(
        HBOOST_PARAMETER_MAX_ARITY, class A, = hboost::parameter::void_ HBOOST_PP_INTERCEPT
    )
>
struct match
  : Parameters::template match<
        HBOOST_PP_ENUM_PARAMS(HBOOST_PARAMETER_MAX_ARITY, A)
    >
{};
# endif 

# undef false_

template <
    class Parameters
  , HBOOST_PP_ENUM_BINARY_PARAMS(
        HBOOST_PARAMETER_MAX_ARITY, class A, = hboost::parameter::void_ HBOOST_PP_INTERCEPT
    )
>
struct argument_pack
{
    typedef typename make_arg_list<
        typename HBOOST_PARAMETER_build_arg_list(
            HBOOST_PARAMETER_MAX_ARITY, make_items, typename Parameters::parameter_spec, A
        )::type
      , typename Parameters::deduced_list
      , tag_keyword_arg
      , mpl::false_
    >::type result;
    typedef typename mpl::first<result>::type type;
};

// Works around VC6 problem where it won't accept rvalues.
template <class T>
T& as_lvalue(T& value, long)
{
    return value;
}

template <class T>
T const& as_lvalue(T const& value, int)
{
    return value;
}


# if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))

template <class Predicate, class T, class Args>
struct apply_predicate
{
    HBOOST_MPL_ASSERT((
        mpl::and_<mpl::false_,T>
    ));

    typedef typename mpl::if_<
        typename mpl::apply2<Predicate,T,Args>::type
      , char
      , int
    >::type type;
};

template <class P>
struct funptr_predicate
{
    static P p;

    template <class T, class Args, class P0>
    static typename apply_predicate<P0,T,Args>::type
    check_predicate(type<T>, Args*, void**(*)(P0));

    template <class T, class Args, class P0>
    static typename mpl::if_<
        is_convertible<T,P0>
      , char
      , int
     >::type check_predicate(type<T>, Args*, void*(*)(P0));

    template <class T, class Args>
    struct apply
    {
        HBOOST_STATIC_CONSTANT(bool, result = 
            sizeof(check_predicate(hboost::type<T>(), (Args*)0, &p)) == 1
        );

        typedef mpl::bool_<apply<T,Args>::result> type;
    };
};

template <>
struct funptr_predicate<void**>
  : mpl::always<mpl::true_>
{};

# endif

}}} // namespace hboost::parameter::aux

# if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
// From Paul Mensonides
#  define HBOOST_PARAMETER_IS_NULLARY(x) \
    HBOOST_PP_SPLIT(1, HBOOST_PARAMETER_IS_NULLARY_C x HBOOST_PP_COMMA() 0) \
    /**/
#  define HBOOST_PARAMETER_IS_NULLARY_C() \
    ~, 1 HBOOST_PP_RPAREN() \
    HBOOST_PP_TUPLE_EAT(2) HBOOST_PP_LPAREN() ~ \
    /**/
# else
#  define HBOOST_PARAMETER_IS_NULLARY(x) HBOOST_PP_IS_NULLARY(x)
# endif

# define HBOOST_PARAMETER_MEMBER_FUNCTION_CHECK_STATIC_static ()
# define HBOOST_PARAMETER_MEMBER_FUNCTION_IS_STATIC(name) \
    HBOOST_PARAMETER_IS_NULLARY( \
        HBOOST_PP_CAT(HBOOST_PARAMETER_MEMBER_FUNCTION_CHECK_STATIC_,name) \
    )

# if !defined(HBOOST_MSVC)
#  define HBOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC_static
#  define HBOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC(name) \
    HBOOST_PP_CAT(HBOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC_, name)
# else
// Workaround for MSVC preprocessor.
//
// When stripping static from "static f", msvc will produce
// " f". The leading whitespace doesn't go away when pasting
// the token with something else, so this thing is a hack to
// strip the whitespace.
#  define HBOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC_static (
#  define HBOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC_AUX(name) \
    HBOOST_PP_CAT(HBOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC_, name))
#  define HBOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC(name) \
    HBOOST_PP_SEQ_HEAD( \
        HBOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC_AUX(name) \
    )
# endif

# define HBOOST_PARAMETER_MEMBER_FUNCTION_STATIC(name) \
    HBOOST_PP_EXPR_IF( \
        HBOOST_PARAMETER_MEMBER_FUNCTION_IS_STATIC(name) \
      , static \
    )

# define HBOOST_PARAMETER_MEMBER_FUNCTION_NAME(name) \
    HBOOST_PP_IF( \
        HBOOST_PARAMETER_MEMBER_FUNCTION_IS_STATIC(name) \
      , HBOOST_PARAMETER_MEMBER_FUNCTION_STRIP_STATIC \
      , name HBOOST_PP_TUPLE_EAT(1) \
    )(name)

// Calculates [begin, end) arity range.

# define HBOOST_PARAMETER_ARITY_RANGE_M_optional(state) state
# define HBOOST_PARAMETER_ARITY_RANGE_M_deduced_optional(state) state
# define HBOOST_PARAMETER_ARITY_RANGE_M_required(state) HBOOST_PP_INC(state)
# define HBOOST_PARAMETER_ARITY_RANGE_M_deduced_required(state) HBOOST_PP_INC(state)

# define HBOOST_PARAMETER_ARITY_RANGE_M(s, state, x) \
    HBOOST_PP_CAT( \
        HBOOST_PARAMETER_ARITY_RANGE_M_ \
      , HBOOST_PARAMETER_FN_ARG_QUALIFIER(x) \
    )(state)
/**/

# define HBOOST_PARAMETER_ARITY_RANGE(args) \
    ( \
        HBOOST_PP_SEQ_FOLD_LEFT(HBOOST_PARAMETER_ARITY_RANGE_M, 0, args) \
      , HBOOST_PP_INC(HBOOST_PP_SEQ_SIZE(args)) \
    )
/**/

// Accessor macros for the argument specs tuple.
# define HBOOST_PARAMETER_FN_ARG_QUALIFIER(x) \
    HBOOST_PP_TUPLE_ELEM(4,0,x)
/**/

# define HBOOST_PARAMETER_FN_ARG_NAME(x) \
    HBOOST_PP_TUPLE_ELEM(4,1,x)
/**/

# define HBOOST_PARAMETER_FN_ARG_PRED(x) \
    HBOOST_PP_TUPLE_ELEM(4,2,x)
/**/

# define HBOOST_PARAMETER_FN_ARG_DEFAULT(x) \
    HBOOST_PP_TUPLE_ELEM(4,3,x)
/**/

# define HBOOST_PARAMETETER_FUNCTION_EAT_KEYWORD_QUALIFIER_out(x)
# define HBOOST_PARAMETETER_FUNCTION_EAT_KEYWORD_QUALIFIER_in_out(x)

// Returns 1 if x is either "out(k)" or "in_out(k)".
# define HBOOST_PARAMETER_FUNCTION_IS_KEYWORD_QUALIFIER(x) \
    HBOOST_PP_IS_EMPTY( \
        HBOOST_PP_CAT(HBOOST_PARAMETETER_FUNCTION_EAT_KEYWORD_QUALIFIER_, x) \
    ) \
/**/

# define HBOOST_PARAMETETER_FUNCTION_GET_KEYWORD_QUALIFIER_out(x) x
# define HBOOST_PARAMETETER_FUNCTION_GET_KEYWORD_QUALIFIER_in_out(x) x
# define HBOOST_PARAMETER_FUNCTION_KEYWORD_GET(x) \
    HBOOST_PP_CAT(HBOOST_PARAMETETER_FUNCTION_GET_KEYWORD_QUALIFIER_, x)
/**/

// Returns the keyword of x, where x is either a keyword qualifier
// or a keyword.
//
//   k => k
//   out(k) => k
//   in_out(k) => k
//
# define HBOOST_PARAMETER_FUNCTION_KEYWORD(x) \
    HBOOST_PP_IF( \
        HBOOST_PARAMETER_FUNCTION_IS_KEYWORD_QUALIFIER(x) \
      , HBOOST_PARAMETER_FUNCTION_KEYWORD_GET \
      , x HBOOST_PP_TUPLE_EAT(1) \
    )(x)
/**/

# define HBOOST_PARAMETER_FN_ARG_KEYWORD(x) \
    HBOOST_PARAMETER_FUNCTION_KEYWORD( \
        HBOOST_PARAMETER_FN_ARG_NAME(x) \
    )

// Builds forwarding functions.

# define HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION_TEMPLATE_Z(z, n) \
    template<HBOOST_PP_ENUM_PARAMS_Z(z, n, class ParameterArgumentType)>
/**/

# if ! defined(HBOOST_NO_SFINAE) && ! HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x592))
#  define HBOOST_PARAMETER_FUNCTION_FWD_MATCH_Z(z, name, parameters, n) \
    , typename hboost::parameter::aux::match< \
          parameters, HBOOST_PP_ENUM_PARAMS(n, ParameterArgumentType) \
      >::type = parameters()
# else
#  define HBOOST_PARAMETER_FUNCTION_FWD_MATCH_Z(z, name, parameters, n)
# endif
/**/

# define HBOOST_PARAMETER_FUNCTION_PARAMETERS_NAME(base) \
    HBOOST_PP_CAT( \
        hboost_param_parameters_ \
      , HBOOST_PP_CAT(__LINE__, HBOOST_PARAMETER_MEMBER_FUNCTION_NAME(base)) \
    )

// Produce a name for a result type metafunction for the function
// named base
# define HBOOST_PARAMETER_FUNCTION_RESULT_NAME(base) \
    HBOOST_PP_CAT( \
        hboost_param_result_ \
      , HBOOST_PP_CAT(__LINE__,HBOOST_PARAMETER_MEMBER_FUNCTION_NAME(base)) \
    )

// Can't do hboost_param_impl_ ## basee because base might start with an underscore
// daniel: what? how is that relevant? the reason for using CAT() is to make sure
// base is expanded. i'm not sure we need to here, but it's more stable to do it.
# define HBOOST_PARAMETER_IMPL(base) \
    HBOOST_PP_CAT(hboost_param_impl,HBOOST_PARAMETER_MEMBER_FUNCTION_NAME(base))

# define HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION00(z, n, r, data, elem) \
    HBOOST_PP_IF( \
        n \
      , HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION_TEMPLATE_Z, HBOOST_PP_TUPLE_EAT(2) \
    )(z,n) \
    HBOOST_PARAMETER_MEMBER_FUNCTION_STATIC(HBOOST_PP_TUPLE_ELEM(7,3,data)) \
    inline \
    HBOOST_PP_EXPR_IF(n, typename) \
        HBOOST_PARAMETER_FUNCTION_RESULT_NAME(HBOOST_PP_TUPLE_ELEM(7,3,data))<   \
        HBOOST_PP_EXPR_IF(n, typename) \
        hboost::parameter::aux::argument_pack< \
            HBOOST_PARAMETER_FUNCTION_PARAMETERS_NAME(HBOOST_PP_TUPLE_ELEM(7,3,data)) \
            HBOOST_PP_COMMA_IF(n) \
            HBOOST_PP_IF( \
                n, HBOOST_PP_SEQ_ENUM, HBOOST_PP_TUPLE_EAT(1) \
            )(elem) \
        >::type \
    >::type \
    HBOOST_PARAMETER_MEMBER_FUNCTION_NAME(HBOOST_PP_TUPLE_ELEM(7,3,data))( \
        HBOOST_PP_IF( \
            n \
          , HBOOST_PP_SEQ_FOR_EACH_I_R \
          , HBOOST_PP_TUPLE_EAT(4) \
        )( \
            r \
          , HBOOST_PARAMETER_FUNCTION_ARGUMENT \
          , ~ \
          , elem \
        ) \
        HBOOST_PP_IF(n, HBOOST_PARAMETER_FUNCTION_FWD_MATCH_Z, HBOOST_PP_TUPLE_EAT(4))( \
            z \
          , HBOOST_PP_TUPLE_ELEM(7,3,data) \
          , HBOOST_PARAMETER_FUNCTION_PARAMETERS_NAME(HBOOST_PP_TUPLE_ELEM(7,3,data)) \
          , n \
        ) \
    ) HBOOST_PP_EXPR_IF(HBOOST_PP_TUPLE_ELEM(7,4,data), const) \
    { \
        return HBOOST_PARAMETER_IMPL(HBOOST_PP_TUPLE_ELEM(7,3,data))( \
            HBOOST_PARAMETER_FUNCTION_PARAMETERS_NAME(HBOOST_PP_TUPLE_ELEM(7,3,data))()( \
                HBOOST_PP_ENUM_PARAMS_Z(z, n, a) \
            ) \
        ); \
    }
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION0(r, data, elem) \
    HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION00( \
        HBOOST_PP_TUPLE_ELEM(7,0,data) \
      , HBOOST_PP_TUPLE_ELEM(7,1,data) \
      , r \
      , data \
      , elem \
    )
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION_ARITY_0(z, n, data) \
    HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION00( \
        z, n, HBOOST_PP_DEDUCE_R() \
      , (z, n, HBOOST_PP_TUPLE_REM(5) data) \
      , ~ \
    )
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION_ARITY_N(z, n, data) \
    HBOOST_PP_SEQ_FOR_EACH( \
        HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION0 \
      , (z, n, HBOOST_PP_TUPLE_REM(5) data) \
      , HBOOST_PP_SEQ_FOR_EACH_PRODUCT( \
            HBOOST_PARAMETER_FUNCTION_FWD_PRODUCT \
          , HBOOST_PP_SEQ_FIRST_N( \
                n, HBOOST_PP_TUPLE_ELEM(5,3,data) \
            ) \
        ) \
    )
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION(z, n, data) \
    HBOOST_PP_IF( \
        n \
      , HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION_ARITY_N \
      , HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION_ARITY_0 \
    )(z,n,data) \
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_FUNCTIONS0( \
    result,name,args,const_,combinations,range \
) \
    HBOOST_PP_REPEAT_FROM_TO( \
        HBOOST_PP_TUPLE_ELEM(2,0,range), HBOOST_PP_TUPLE_ELEM(2,1,range) \
      , HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION \
      , (result,name,const_,combinations,HBOOST_PP_TUPLE_ELEM(2,1,range)) \
    )
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_FUNCTIONS(result,name,args, const_, combinations) \
    HBOOST_PARAMETER_FUNCTION_FWD_FUNCTIONS0( \
        result, name, args, const_, combinations, HBOOST_PARAMETER_ARITY_RANGE(args) \
    )
/**/

// Builds hboost::parameter::parameters<> specialization
#  define HBOOST_PARAMETER_FUNCTION_PARAMETERS_QUALIFIER_optional(tag) \
    optional<tag

#  define HBOOST_PARAMETER_FUNCTION_PARAMETERS_QUALIFIER_required(tag) \
    required<tag

#  define HBOOST_PARAMETER_FUNCTION_PARAMETERS_QUALIFIER_deduced_optional(tag) \
    optional<hboost::parameter::deduced<tag>

#  define HBOOST_PARAMETER_FUNCTION_PARAMETERS_QUALIFIER_deduced_required(tag) \
    required<hboost::parameter::deduced<tag>

# if !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))

#  define HBOOST_PARAMETER_FUNCTION_PARAMETERS_M(r,tag_namespace,i,elem) \
    HBOOST_PP_COMMA_IF(i) \
    hboost::parameter::HBOOST_PP_CAT( \
        HBOOST_PARAMETER_FUNCTION_PARAMETERS_QUALIFIER_ \
      , HBOOST_PARAMETER_FN_ARG_QUALIFIER(elem) \
    )( \
        tag_namespace::HBOOST_PARAMETER_FUNCTION_KEYWORD( \
            HBOOST_PARAMETER_FN_ARG_KEYWORD(elem) \
        ) \
    ) \
      , typename hboost::parameter::aux::unwrap_predicate< \
            void HBOOST_PARAMETER_FN_ARG_PRED(elem) \
        >::type \
    >
# elif HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
#  define HBOOST_PARAMETER_FUNCTION_PARAMETERS_M(r,tag_namespace,i,elem) \
    HBOOST_PP_COMMA_IF(i) \
    hboost::parameter::HBOOST_PP_CAT( \
        HBOOST_PARAMETER_FUNCTION_PARAMETERS_QUALIFIER_ \
      , HBOOST_PARAMETER_FN_ARG_QUALIFIER(elem) \
    )( \
        tag_namespace::HBOOST_PARAMETER_FUNCTION_KEYWORD( \
            HBOOST_PARAMETER_FN_ARG_KEYWORD(elem) \
        ) \
    ) \
      , hboost::mpl::always<hboost::mpl::true_> \
    >
# endif

# define HBOOST_PARAMETER_FUNCTION_PARAMETERS(tag_namespace, base, args)             \
    template <class BoostParameterDummy>                                            \
    struct HBOOST_PP_CAT(                                                            \
            HBOOST_PP_CAT(hboost_param_params_, __LINE__)                             \
          , HBOOST_PARAMETER_MEMBER_FUNCTION_NAME(base)                              \
    ) : hboost::parameter::parameters<                                               \
            HBOOST_PP_SEQ_FOR_EACH_I(                                                \
                HBOOST_PARAMETER_FUNCTION_PARAMETERS_M, tag_namespace, args          \
            )                                                                       \
        >                                                                           \
    {};                                                                             \
                                                                                    \
    typedef HBOOST_PP_CAT( \
            HBOOST_PP_CAT(hboost_param_params_, __LINE__) \
          , HBOOST_PARAMETER_MEMBER_FUNCTION_NAME(base) \
    )<int>

// Defines result type metafunction
# define HBOOST_PARAMETER_FUNCTION_RESULT_ARG(z, _, i, x) \
    HBOOST_PP_COMMA_IF(i) class HBOOST_PP_TUPLE_ELEM(3,1,x)
/**/

# define HBOOST_PARAMETER_FUNCTION_RESULT_(result, name, args)                                   \
    template <class Args>                                                                       \
    struct HBOOST_PARAMETER_FUNCTION_RESULT_NAME(name)                                           \
    {                                                                                           \
        typedef typename HBOOST_PARAMETER_PARENTHESIZED_TYPE(result) type;                       \
    };

// Defines implementation function
# define HBOOST_PARAMETER_FUNCTION_IMPL_HEAD(name)           \
    template <class Args>                                   \
    typename HBOOST_PARAMETER_FUNCTION_RESULT_NAME(name)<    \
       Args                                                 \
    >::type HBOOST_PARAMETER_IMPL(name)(Args const& args)

# define HBOOST_PARAMETER_FUNCTION_IMPL_FWD(name) \
    HBOOST_PARAMETER_FUNCTION_IMPL_HEAD(name);
/**/

# define HBOOST_PARAMETER_FUNCTION_SPLIT_ARG_required(state, arg) \
    ( \
        HBOOST_PP_INC(HBOOST_PP_TUPLE_ELEM(4, 0, state)) \
      , HBOOST_PP_SEQ_PUSH_BACK(HBOOST_PP_TUPLE_ELEM(4, 1, state), arg) \
      , HBOOST_PP_TUPLE_ELEM(4, 2, state) \
      , HBOOST_PP_TUPLE_ELEM(4, 3, state) \
    )

# define HBOOST_PARAMETER_FUNCTION_SPLIT_ARG_deduced_required(state, arg) \
    HBOOST_PARAMETER_FUNCTION_SPLIT_ARG_required(state, arg)

# define HBOOST_PARAMETER_FUNCTION_SPLIT_ARG_optional(state, arg) \
    ( \
        HBOOST_PP_TUPLE_ELEM(4, 0, state) \
      , HBOOST_PP_TUPLE_ELEM(4, 1, state) \
      , HBOOST_PP_INC(HBOOST_PP_TUPLE_ELEM(4, 2, state)) \
      , HBOOST_PP_SEQ_PUSH_BACK(HBOOST_PP_TUPLE_ELEM(4, 3, state), arg) \
    )

# define HBOOST_PARAMETER_FUNCTION_SPLIT_ARG_deduced_optional(state, arg) \
    HBOOST_PARAMETER_FUNCTION_SPLIT_ARG_optional(state, arg)

# define HBOOST_PARAMETER_FUNCTION_SPLIT_ARG(s, state, arg) \
    HBOOST_PP_CAT( \
        HBOOST_PARAMETER_FUNCTION_SPLIT_ARG_ \
      , HBOOST_PARAMETER_FN_ARG_QUALIFIER(arg) \
    )(state, arg)

// Returns (required_count, required, optional_count, optionals) tuple
# define HBOOST_PARAMETER_FUNCTION_SPLIT_ARGS(args) \
    HBOOST_PP_SEQ_FOLD_LEFT( \
        HBOOST_PARAMETER_FUNCTION_SPLIT_ARG \
      , (0,HBOOST_PP_SEQ_NIL, 0,HBOOST_PP_SEQ_NIL) \
      , args \
    )

# define HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_ARG_NAME(keyword) \
    HBOOST_PP_CAT(HBOOST_PP_CAT(keyword,_),type)

// Helpers used as parameters to HBOOST_PARAMETER_FUNCTION_DEFAULT_ARGUMENTS.
# define HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_TEMPLATE_ARG(r, _, arg) \
    , class HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_ARG_NAME( \
              HBOOST_PARAMETER_FN_ARG_KEYWORD(arg) \
      )

# define HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_ARG(r, _, arg) \
    , HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_ARG_NAME( \
              HBOOST_PARAMETER_FN_ARG_KEYWORD(arg) \
      )& HBOOST_PARAMETER_FN_ARG_KEYWORD(arg)

# define HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_PARAMETER(r, _, arg) \
    , HBOOST_PARAMETER_FN_ARG_KEYWORD(arg)

// Produces a name for the dispatch functions.
# define HBOOST_PARAMETER_FUNCTION_DEFAULT_NAME(name) \
    HBOOST_PP_CAT( \
        hboost_param_default_ \
      , HBOOST_PP_CAT(__LINE__, HBOOST_PARAMETER_MEMBER_FUNCTION_NAME(name)) \
    )

// Helper macro used below to produce lists based on the keyword argument
// names. macro is applied to every element. n is the number of
// optional arguments that should be included.
# define HBOOST_PARAMETER_FUNCTION_DEFAULT_ARGUMENTS(macro, n, split_args) \
    HBOOST_PP_SEQ_FOR_EACH( \
        macro \
      , ~ \
      , HBOOST_PP_TUPLE_ELEM(4,1,split_args) \
    ) \
    HBOOST_PP_SEQ_FOR_EACH( \
        macro \
      , ~ \
      , HBOOST_PP_SEQ_FIRST_N( \
          HBOOST_PP_SUB(HBOOST_PP_TUPLE_ELEM(4,2,split_args), n) \
        , HBOOST_PP_TUPLE_ELEM(4,3,split_args) \
        ) \
    )

// Generates a keyword | default expression.
# define HBOOST_PARAMETER_FUNCTION_DEFAULT_EVAL_DEFAULT(arg, tag_namespace) \
    hboost::parameter::keyword< \
        tag_namespace::HBOOST_PARAMETER_FN_ARG_KEYWORD(arg) \
    >::instance | hboost::parameter::aux::use_default_tag()

# define HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_GET_ARG(arg, tag_ns) \
    HBOOST_PARAMETER_FUNCTION_CAST( \
        args[ \
            HBOOST_PARAMETER_FUNCTION_DEFAULT_EVAL_DEFAULT( \
                arg, tag_ns \
            ) \
        ] \
      , HBOOST_PARAMETER_FN_ARG_PRED(arg) \
      , Args \
    )

# define HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_BODY(name, n, split_args, tag_namespace) \
    { \
        return HBOOST_PARAMETER_FUNCTION_DEFAULT_NAME(name)( \
            (ResultType(*)())0 \
          , args \
          , 0L \
            HBOOST_PARAMETER_FUNCTION_DEFAULT_ARGUMENTS( \
                HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_PARAMETER \
              , n \
              , split_args \
            ) \
          , HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_GET_ARG( \
                HBOOST_PP_SEQ_ELEM( \
                    HBOOST_PP_SUB(HBOOST_PP_TUPLE_ELEM(4,2,split_args), n) \
                  , HBOOST_PP_TUPLE_ELEM(4,3,split_args) \
                ) \
              , tag_namespace \
            ) \
        ); \
    }

# define HBOOST_PARAMETER_FUNCTION_DEFAULT_EVAL_ACTUAL_DEFAULT(arg) \
    HBOOST_PARAMETER_FUNCTION_CAST( \
        hboost::parameter::aux::as_lvalue(HBOOST_PARAMETER_FN_ARG_DEFAULT(arg), 0L) \
      , HBOOST_PARAMETER_FN_ARG_PRED(arg) \
      , Args \
    )

# define HBOOST_PARAMETER_FUNCTION_DEFAULT_EVAL_DEFAULT_BODY(name, n, split_args, tag_ns, const_) \
    template < \
        class ResultType \
      , class Args \
        HBOOST_PARAMETER_FUNCTION_DEFAULT_ARGUMENTS( \
            HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_TEMPLATE_ARG \
          , HBOOST_PP_INC(n) \
          , split_args \
        ) \
    > \
    HBOOST_PARAMETER_MEMBER_FUNCTION_STATIC(name) \
    ResultType HBOOST_PARAMETER_FUNCTION_DEFAULT_NAME(name)( \
        ResultType(*)() \
      , Args const& args \
      , long \
        HBOOST_PARAMETER_FUNCTION_DEFAULT_ARGUMENTS( \
            HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_ARG \
          , HBOOST_PP_INC(n) \
          , split_args \
        ) \
      , hboost::parameter::aux::use_default_tag \
    ) HBOOST_PP_EXPR_IF(const_, const) \
    { \
        return HBOOST_PARAMETER_FUNCTION_DEFAULT_NAME(name)( \
            (ResultType(*)())0 \
          , args \
          , 0L \
            HBOOST_PARAMETER_FUNCTION_DEFAULT_ARGUMENTS( \
                HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_PARAMETER \
              , HBOOST_PP_INC(n) \
              , split_args \
            ) \
          , HBOOST_PARAMETER_FUNCTION_DEFAULT_EVAL_ACTUAL_DEFAULT( \
                HBOOST_PP_SEQ_ELEM( \
                    HBOOST_PP_SUB(HBOOST_PP_TUPLE_ELEM(4,2,split_args), HBOOST_PP_INC(n)) \
                  , HBOOST_PP_TUPLE_ELEM(4,3,split_args) \
                ) \
            ) \
        ); \
    }

// Produces a forwarding layer in the default evaluation machine.
//
// data is a tuple:
//
//   (name, split_args)
//
// Where name is the base name of the function, and split_args is a tuple:
//
//   (required_count, required_args, optional_count, required_args)
//


// defines the actual function body for HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION below.
# define HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION0(z, n, data) \
    template < \
        class ResultType \
      , class Args \
        HBOOST_PARAMETER_FUNCTION_DEFAULT_ARGUMENTS( \
            HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_TEMPLATE_ARG \
          , n \
          , HBOOST_PP_TUPLE_ELEM(5,1,data) \
        ) \
    > \
    HBOOST_PARAMETER_MEMBER_FUNCTION_STATIC(HBOOST_PP_TUPLE_ELEM(5,0,data)) \
    ResultType HBOOST_PARAMETER_FUNCTION_DEFAULT_NAME(HBOOST_PP_TUPLE_ELEM(5,0,data))( \
        ResultType(*)() \
      , Args const& args \
      , int \
        HBOOST_PARAMETER_FUNCTION_DEFAULT_ARGUMENTS( \
            HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_ARG \
          , n \
          , HBOOST_PP_TUPLE_ELEM(5,1,data) \
        ) \
    ) HBOOST_PP_EXPR_IF(HBOOST_PP_TUPLE_ELEM(5,2,data), const) \
    HBOOST_PP_IF( \
        n \
      , HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_BODY \
      , ; HBOOST_PP_TUPLE_EAT(4) \
    )( \
        HBOOST_PP_TUPLE_ELEM(5,0,data) \
      , n \
      , HBOOST_PP_TUPLE_ELEM(5,1,data) \
      , HBOOST_PP_TUPLE_ELEM(5,3,data) \
    )

# define HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION(z, n, data) \
    HBOOST_PP_IF( \
        HBOOST_PP_AND( \
            HBOOST_PP_NOT(n) \
          , HBOOST_PP_TUPLE_ELEM(5,4,data) \
        ) \
      , HBOOST_PP_TUPLE_EAT(3) \
      , HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION0 \
    )(z, n, data) \
    HBOOST_PP_IF( \
        HBOOST_PP_EQUAL(n, HBOOST_PP_TUPLE_ELEM(4,2,HBOOST_PP_TUPLE_ELEM(5,1,data))) \
      , HBOOST_PP_TUPLE_EAT(5) \
      , HBOOST_PARAMETER_FUNCTION_DEFAULT_EVAL_DEFAULT_BODY \
    )( \
        HBOOST_PP_TUPLE_ELEM(5,0,data) \
      , n \
      , HBOOST_PP_TUPLE_ELEM(5,1,data) \
      , HBOOST_PP_TUPLE_ELEM(5,3,data) \
      , HBOOST_PP_TUPLE_ELEM(5,2,data) \
    )

# define HBOOST_PARAMETER_FUNCTION_DEFAULT_GET_ARG(r, tag_ns, arg) \
    , HBOOST_PARAMETER_FUNCTION_CAST( \
          args[ \
              hboost::parameter::keyword<tag_ns::HBOOST_PARAMETER_FN_ARG_KEYWORD(arg)>::instance \
          ] \
        , HBOOST_PARAMETER_FN_ARG_PRED(arg) \
        , Args \
      )

// Generates the function template that recives a ArgumentPack, and then
// goes on to call the layers of overloads generated by 
// HBOOST_PARAMETER_FUNCTION_DEFAULT_LAYER.
# define HBOOST_PARAMETER_FUNCTION_INITIAL_DISPATCH_FUNCTION(name, split_args, const_, tag_ns) \
    template <class Args> \
    typename HBOOST_PARAMETER_FUNCTION_RESULT_NAME(name)<Args>::type \
    HBOOST_PARAMETER_MEMBER_FUNCTION_STATIC(name) \
    HBOOST_PARAMETER_IMPL(name)(Args const& args) HBOOST_PP_EXPR_IF(const_, const) \
    { \
        return HBOOST_PARAMETER_FUNCTION_DEFAULT_NAME(name)( \
            (typename HBOOST_PARAMETER_FUNCTION_RESULT_NAME(name)<Args>::type(*)())0 \
          , args \
          , 0L \
 \
            HBOOST_PP_SEQ_FOR_EACH( \
                HBOOST_PARAMETER_FUNCTION_DEFAULT_GET_ARG \
              , tag_ns \
              , HBOOST_PP_TUPLE_ELEM(4,1,split_args) \
            ) \
 \
        ); \
    }

// Helper for HBOOST_PARAMETER_FUNCTION_DEFAULT_LAYER below.
# define HBOOST_PARAMETER_FUNCTION_DEFAULT_LAYER_AUX( \
    name, split_args, skip_fwd_decl, const_, tag_namespace \
  ) \
    HBOOST_PP_REPEAT_FROM_TO( \
        0 \
      , HBOOST_PP_INC(HBOOST_PP_TUPLE_ELEM(4, 2, split_args)) \
      , HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION \
      , (name, split_args, const_, tag_namespace, skip_fwd_decl) \
    ) \
 \
    HBOOST_PARAMETER_FUNCTION_INITIAL_DISPATCH_FUNCTION(name, split_args, const_, tag_namespace) \
\
    template < \
        class ResultType \
      , class Args \
        HBOOST_PARAMETER_FUNCTION_DEFAULT_ARGUMENTS( \
            HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_TEMPLATE_ARG \
          , 0 \
          , split_args \
        ) \
    > \
    HBOOST_PARAMETER_MEMBER_FUNCTION_STATIC(name) \
    ResultType HBOOST_PARAMETER_FUNCTION_DEFAULT_NAME(name)( \
        ResultType(*)() \
      , Args const& \
      , int \
        HBOOST_PARAMETER_FUNCTION_DEFAULT_ARGUMENTS( \
            HBOOST_PARAMETER_FUNCTION_DEFAULT_FUNCTION_ARG \
          , 0 \
          , split_args \
        ) \
    ) HBOOST_PP_EXPR_IF(const_, const)

// Generates a bunch of forwarding functions that each extract
// one more argument.
# define HBOOST_PARAMETER_FUNCTION_DEFAULT_LAYER(name, args, skip_fwd_decl, const_, tag_ns) \
    HBOOST_PARAMETER_FUNCTION_DEFAULT_LAYER_AUX( \
        name, HBOOST_PARAMETER_FUNCTION_SPLIT_ARGS(args), skip_fwd_decl, const_, tag_ns \
    )
/**/

// Defines the result metafunction and the parameters specialization.
# define HBOOST_PARAMETER_FUNCTION_HEAD(result, name, tag_namespace, args)   \
      HBOOST_PARAMETER_FUNCTION_RESULT_(result, name, args)                   \
                                                                            \
          HBOOST_PARAMETER_FUNCTION_PARAMETERS(tag_namespace, name, args)    \
          HBOOST_PARAMETER_FUNCTION_PARAMETERS_NAME(name);                   \

// Helper for HBOOST_PARAMETER_FUNCTION below.
# define HBOOST_PARAMETER_FUNCTION_AUX(result, name, tag_namespace, args)    \
    HBOOST_PARAMETER_FUNCTION_HEAD(result, name, tag_namespace, args)         \
    HBOOST_PARAMETER_FUNCTION_IMPL_HEAD(name); \
\
    HBOOST_PARAMETER_FUNCTION_FWD_FUNCTIONS(                                  \
        result, name, args, 0                                                \
      , HBOOST_PARAMETER_FUNCTION_FWD_COMBINATIONS(args)                      \
    )                                                                        \
                                                                             \
    HBOOST_PARAMETER_FUNCTION_DEFAULT_LAYER(name, args, 0, 0, tag_namespace)

// Defines a Boost.Parameter enabled function with the new syntax.
# define HBOOST_PARAMETER_FUNCTION(result, name, tag_namespace, args)    \
    HBOOST_PARAMETER_FUNCTION_AUX(                                       \
        result, name, tag_namespace                                      \
      , HBOOST_PARAMETER_FLATTEN(3, 2, 3, args)                           \
    )                                                                    \
/**/

// Defines a Boost.Parameter enabled function.
# define HBOOST_PARAMETER_BASIC_FUNCTION_AUX(result, name, tag_namespace, args)    \
    HBOOST_PARAMETER_FUNCTION_HEAD(result, name, tag_namespace, args)        \
                                                                            \
    HBOOST_PARAMETER_FUNCTION_IMPL_FWD(name)                                 \
                                                                            \
    HBOOST_PARAMETER_FUNCTION_FWD_FUNCTIONS(                                 \
        result, name, args, 0                                               \
      , HBOOST_PARAMETER_FUNCTION_FWD_COMBINATIONS(args)                     \
    )                                                                       \
                                                                            \
    HBOOST_PARAMETER_FUNCTION_IMPL_HEAD(name)

# define HBOOST_PARAMETER_BASIC_FUNCTION(result, name, tag_namespace, args)  \
    HBOOST_PARAMETER_BASIC_FUNCTION_AUX(                                     \
        result, name, tag_namespace                                     \
      , HBOOST_PARAMETER_FLATTEN(2, 2, 3, args)                          \
    )                                                                   \
/**/

// Defines a Boost.Parameter enabled member function.
# define HBOOST_PARAMETER_BASIC_MEMBER_FUNCTION_AUX(result, name, tag_namespace, args, const_) \
    HBOOST_PARAMETER_FUNCTION_HEAD(result, name, tag_namespace, args)                    \
                                                                                        \
    HBOOST_PARAMETER_FUNCTION_FWD_FUNCTIONS(                                             \
        result, name, args, const_                                                      \
      , HBOOST_PARAMETER_FUNCTION_FWD_COMBINATIONS(args)                                 \
    )                                                                                   \
                                                                                        \
    HBOOST_PARAMETER_FUNCTION_IMPL_HEAD(name) HBOOST_PP_EXPR_IF(const_, const)            \
/**/

# define HBOOST_PARAMETER_BASIC_MEMBER_FUNCTION(result, name, tag_namespace, args) \
    HBOOST_PARAMETER_BASIC_MEMBER_FUNCTION_AUX( \
        result, name, tag_namespace \
      , HBOOST_PARAMETER_FLATTEN(2, 2, 3, args) \
      , 0 \
    )
/**/

# define HBOOST_PARAMETER_BASIC_CONST_MEMBER_FUNCTION(result, name, tag_namespace, args) \
    HBOOST_PARAMETER_BASIC_MEMBER_FUNCTION_AUX( \
        result, name, tag_namespace \
      , HBOOST_PARAMETER_FLATTEN(2, 2, 3, args) \
      , 1 \
    )
/**/



# define HBOOST_PARAMETER_MEMBER_FUNCTION_AUX(result, name, tag_namespace, const_, args)    \
    HBOOST_PARAMETER_FUNCTION_HEAD(result, name, tag_namespace, args)         \
\
    HBOOST_PARAMETER_FUNCTION_FWD_FUNCTIONS(                                  \
        result, name, args, const_                                           \
      , HBOOST_PARAMETER_FUNCTION_FWD_COMBINATIONS(args)                      \
    )                                                                        \
                                                                             \
    HBOOST_PARAMETER_FUNCTION_DEFAULT_LAYER(name, args, 1, const_, tag_namespace)

// Defines a Boost.Parameter enabled function with the new syntax.
# define HBOOST_PARAMETER_MEMBER_FUNCTION(result, name, tag_namespace, args)    \
    HBOOST_PARAMETER_MEMBER_FUNCTION_AUX(                                       \
        result, name, tag_namespace, 0                                     \
      , HBOOST_PARAMETER_FLATTEN(3, 2, 3, args)                           \
    )                                                                    \
/**/

# define HBOOST_PARAMETER_CONST_MEMBER_FUNCTION(result, name, tag_namespace, args)    \
    HBOOST_PARAMETER_MEMBER_FUNCTION_AUX(                                       \
        result, name, tag_namespace, 1                                     \
      , HBOOST_PARAMETER_FLATTEN(3, 2, 3, args)                           \
    )                                                                    \
/**/

// Defines a Boost.Parameter enabled constructor.

# define HBOOST_PARAMETER_FUNCTION_ARGUMENT(r, _, i, elem) \
    HBOOST_PP_COMMA_IF(i) elem& HBOOST_PP_CAT(a, i)
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTOR00(z, n, r, data, elem) \
    HBOOST_PP_IF( \
        n \
      , HBOOST_PARAMETER_FUNCTION_FWD_FUNCTION_TEMPLATE_Z, HBOOST_PP_TUPLE_EAT(2) \
    )(z, n) \
    HBOOST_PP_EXPR_IF(HBOOST_PP_EQUAL(n,1), explicit) \
    HBOOST_PP_TUPLE_ELEM(6,2,data)( \
        HBOOST_PP_IF( \
            n \
          , HBOOST_PP_SEQ_FOR_EACH_I_R \
          , HBOOST_PP_TUPLE_EAT(4) \
        )( \
            r \
          , HBOOST_PARAMETER_FUNCTION_ARGUMENT \
          , ~ \
          , elem \
        ) \
        HBOOST_PP_IF(n, HBOOST_PARAMETER_FUNCTION_FWD_MATCH_Z, HBOOST_PP_TUPLE_EAT(4))( \
            z \
          , HBOOST_PP_TUPLE_ELEM(6,3,data) \
          , HBOOST_PP_CAT(constructor_parameters, __LINE__) \
          , n \
        ) \
    ) \
      : HBOOST_PARAMETER_PARENTHESIZED_TYPE(HBOOST_PP_TUPLE_ELEM(6,3,data)) ( \
            HBOOST_PP_CAT(constructor_parameters, __LINE__)()( \
                HBOOST_PP_ENUM_PARAMS_Z(z, n, a) \
            ) \
        ) \
    {}
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTOR0(r, data, elem) \
    HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTOR00( \
        HBOOST_PP_TUPLE_ELEM(6,0,data) \
      , HBOOST_PP_TUPLE_ELEM(6,1,data) \
      , r \
      , data \
      , elem \
    )
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_PRODUCT(r, product) \
    (product)
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTOR_ARITY_0(z, n, data) \
    HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTOR00( \
        z, n, HBOOST_PP_DEDUCE_R() \
      , (z, n, HBOOST_PP_TUPLE_REM(4) data) \
      , ~ \
    )
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTOR_ARITY_N(z, n, data) \
    HBOOST_PP_SEQ_FOR_EACH( \
        HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTOR0 \
      , (z, n, HBOOST_PP_TUPLE_REM(4) data) \
      , HBOOST_PP_SEQ_FOR_EACH_PRODUCT( \
            HBOOST_PARAMETER_FUNCTION_FWD_PRODUCT \
          , HBOOST_PP_SEQ_FIRST_N( \
                n, HBOOST_PP_TUPLE_ELEM(4,2,data) \
            ) \
        ) \
    )
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTOR(z, n, data) \
    HBOOST_PP_IF( \
        n \
      , HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTOR_ARITY_N \
      , HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTOR_ARITY_0 \
    )(z,n,data) \
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTORS0(class_,base,args,combinations,range) \
    HBOOST_PP_REPEAT_FROM_TO( \
        HBOOST_PP_TUPLE_ELEM(2,0,range), HBOOST_PP_TUPLE_ELEM(2,1,range) \
      , HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTOR \
      , (class_,base,combinations,HBOOST_PP_TUPLE_ELEM(2,1,range)) \
    )
/**/

# define HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTORS(class_,base,args,combinations) \
    HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTORS0( \
        class_, base, args, combinations, HBOOST_PARAMETER_ARITY_RANGE(args) \
    )
/**/

# define HBOOST_PARAMETER_CONSTRUCTOR_AUX(class_, base, tag_namespace, args) \
    HBOOST_PARAMETER_FUNCTION_PARAMETERS(tag_namespace, ctor, args)          \
        HBOOST_PP_CAT(constructor_parameters, __LINE__); \
\
    HBOOST_PARAMETER_FUNCTION_FWD_CONSTRUCTORS( \
        class_, base, args \
      , HBOOST_PARAMETER_FUNCTION_FWD_COMBINATIONS(args) \
    ) \
/**/

# define HBOOST_PARAMETER_CONSTRUCTOR(class_, base, tag_namespace, args) \
    HBOOST_PARAMETER_CONSTRUCTOR_AUX( \
        class_, base, tag_namespace \
      , HBOOST_PARAMETER_FLATTEN(2, 2, 3, args) \
    )
/**/

# ifndef HBOOST_NO_FUNCTION_TEMPLATE_ORDERING
#  define HBOOST_PARAMETER_FUNCTION_FWD_COMBINATION(r, _, i, elem) \
    (HBOOST_PP_IF( \
        HBOOST_PARAMETER_FUNCTION_IS_KEYWORD_QUALIFIER( \
            HBOOST_PARAMETER_FN_ARG_NAME(elem) \
        ) \
      , (const ParameterArgumentType ## i)(ParameterArgumentType ## i) \
      , (const ParameterArgumentType ## i) \
    ))
// No partial ordering. This feature doesn't work.
# else
#  define HBOOST_PARAMETER_FUNCTION_FWD_COMBINATION(r, _, i, elem) \
    (HBOOST_PP_IF( \
        HBOOST_PARAMETER_FUNCTION_IS_KEYWORD_QUALIFIER( \
            HBOOST_PARAMETER_FN_ARG_NAME(elem) \
        ) \
      , (ParameterArgumentType ## i) \
      , (const ParameterArgumentType ## i) \
    ))
# endif

# define HBOOST_PARAMETER_FUNCTION_FWD_COMBINATIONS(args) \
    HBOOST_PP_SEQ_FOR_EACH_I(HBOOST_PARAMETER_FUNCTION_FWD_COMBINATION, ~, args)

#endif // HBOOST_PARAMETER_PREPROCESSOR_060206_HPP

