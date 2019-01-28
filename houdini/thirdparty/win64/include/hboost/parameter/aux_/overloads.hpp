// Copyright David Abrahams, Daniel Wallin 2003. Use, modification and 
// distribution is subject to the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

// This file generates overloads in this format:
//
//     template<class A0, class A1>
//     typename mpl::apply_wrap1<
//         aux::make_arg_list<
//             PS0,A0
//           , aux::make_arg_list<
//                 PS1,A1
//               , mpl::identity<aux::empty_arg_list>
//             >
//         >
//      , unnamed_list
//     >::type
//     operator()(A0 const& a0, A1 const& a1) const
//     {
//         typedef typename mpl::apply_wrap1<
//             aux::make_arg_list<
//                 PS0,A0
//               , aux::make_arg_list<
//                     PS1,A1
//                   , mpl::identity<aux::empty_arg_list>
//                 >
//             >
//         >::type arg_tuple;
//
//         return arg_tuple(
//             a0
//           , a1
//           , aux::void_()
//             ...
//         );
//     }
//

#if !defined(HBOOST_PP_IS_ITERATING)
# error Boost.Parameters - do not include this file!
#endif

#define N HBOOST_PP_ITERATION()

#define HBOOST_PARAMETER_open_list(z, n, text) \
    aux::item< \
        HBOOST_PP_CAT(PS, n), HBOOST_PP_CAT(A, n)

#define HBOOST_PARAMETER_close_list(z, n, text) > 

#define HBOOST_PARAMETER_arg_list(n) \
    aux::make_arg_list< \
        HBOOST_PP_ENUM(N, HBOOST_PARAMETER_open_list, _) \
      , void_ \
        HBOOST_PP_REPEAT(N, HBOOST_PARAMETER_close_list, _) \
      , deduced_list \
      , aux::tag_keyword_arg \
    >

#define HBOOST_PARAMETER_arg_pack_init(z, n, limit) \
    HBOOST_PP_CAT(a, HBOOST_PP_SUB(limit,n))

template<HBOOST_PP_ENUM_PARAMS(N, class A)>
typename mpl::first<
    typename HBOOST_PARAMETER_arg_list(N)::type
>::type
operator()(HBOOST_PP_ENUM_BINARY_PARAMS(N, A, & a)) const
{
    typedef typename HBOOST_PARAMETER_arg_list(N)::type result;

    typedef typename mpl::first<result>::type result_type;
    typedef typename mpl::second<result>::type error;
    error();

    return result_type(
        HBOOST_PP_ENUM(N, HBOOST_PARAMETER_arg_pack_init, HBOOST_PP_DEC(N))
        HBOOST_PP_ENUM_TRAILING_PARAMS(
            HBOOST_PP_SUB(HBOOST_PARAMETER_MAX_ARITY, N)
          , aux::void_reference() HBOOST_PP_INTERCEPT
        ));
}

#undef HBOOST_PARAMETER_arg_list
#undef HBOOST_PARAMETER_open_list
#undef HBOOST_PARAMETER_close_list
#undef N

