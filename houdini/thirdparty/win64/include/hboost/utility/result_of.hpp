// Boost result_of library

//  Copyright Douglas Gregor 2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org/libs/utility
#ifndef HBOOST_RESULT_OF_HPP
#define HBOOST_RESULT_OF_HPP

#include <hboost/config.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/iteration/iterate.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/repetition/enum_shifted_params.hpp>
#include <hboost/preprocessor/facilities/intercept.hpp>
#include <hboost/detail/workaround.hpp>
#include <hboost/mpl/has_xxx.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/or.hpp>
#include <hboost/type_traits/is_class.hpp>
#include <hboost/type_traits/is_pointer.hpp>
#include <hboost/type_traits/is_member_function_pointer.hpp>
#include <hboost/type_traits/remove_cv.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/utility/declval.hpp>
#include <hboost/utility/enable_if.hpp>

#ifndef HBOOST_RESULT_OF_NUM_ARGS
#  define HBOOST_RESULT_OF_NUM_ARGS 16
#endif

// Use the decltype-based version of result_of by default if the compiler
// supports N3276 <http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2011/n3276.pdf>.
// The user can force the choice by defining HBOOST_RESULT_OF_USE_DECLTYPE,
// HBOOST_RESULT_OF_USE_TR1, or HBOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK but not more than one!
#if (defined(HBOOST_RESULT_OF_USE_DECLTYPE) && defined(HBOOST_RESULT_OF_USE_TR1)) || \
    (defined(HBOOST_RESULT_OF_USE_DECLTYPE) && defined(HBOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK)) || \
    (defined(HBOOST_RESULT_OF_USE_TR1) && defined(HBOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK))
#  error More than one of HBOOST_RESULT_OF_USE_DECLTYPE, HBOOST_RESULT_OF_USE_TR1 and \
  HBOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK cannot be defined at the same time.
#endif

#if defined(HBOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK) && defined(HBOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE)
#  error Cannot fallback to decltype if HBOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE is not defined.
#endif

#ifndef HBOOST_RESULT_OF_USE_TR1
#  ifndef HBOOST_RESULT_OF_USE_DECLTYPE
#    ifndef HBOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK
#      ifndef HBOOST_NO_CXX11_DECLTYPE_N3276 // this implies !defined(HBOOST_NO_CXX11_DECLTYPE)
#        define HBOOST_RESULT_OF_USE_DECLTYPE
#      else
#        define HBOOST_RESULT_OF_USE_TR1
#      endif
#    endif
#  endif
#endif

namespace hboost {

template<typename F> struct result_of;
template<typename F> struct tr1_result_of; // a TR1-style implementation of result_of

#if !defined(HBOOST_NO_SFINAE)
namespace detail {

HBOOST_MPL_HAS_XXX_TRAIT_DEF(result_type)

// Work around a nvcc bug by only defining has_result when it's needed.
#ifdef HBOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK
HBOOST_MPL_HAS_XXX_TEMPLATE_DEF(result)
#endif

template<typename F, typename FArgs, bool HasResultType> struct tr1_result_of_impl;

template<typename F> struct cpp0x_result_of;

#ifdef HBOOST_NO_SFINAE_EXPR

// There doesn't seem to be any other way to turn this off such that the presence of
// the user-defined operator,() below doesn't cause spurious warning all over the place,
// so unconditionally turn it off.
#if HBOOST_MSVC
#  pragma warning(disable: 4913) // user defined binary operator ',' exists but no overload could convert all operands, default built-in binary operator ',' used
#endif

struct result_of_private_type {};

struct result_of_weird_type {
  friend result_of_private_type operator,(result_of_private_type, result_of_weird_type);
};

typedef char result_of_yes_type;      // sizeof(result_of_yes_type) == 1
typedef char (&result_of_no_type)[2]; // sizeof(result_of_no_type)  == 2

template<typename T>
result_of_no_type result_of_is_private_type(T const &);
result_of_yes_type result_of_is_private_type(result_of_private_type);

template<typename C>
struct result_of_callable_class : C {
    result_of_callable_class();
    typedef result_of_private_type const &(*pfn_t)(...);
    operator pfn_t() const volatile;
};

template<typename C>
struct result_of_wrap_callable_class {
  typedef result_of_callable_class<C> type;
};

template<typename C>
struct result_of_wrap_callable_class<C const> {
  typedef result_of_callable_class<C> const type;
};

template<typename C>
struct result_of_wrap_callable_class<C volatile> {
  typedef result_of_callable_class<C> volatile type;
};

template<typename C>
struct result_of_wrap_callable_class<C const volatile> {
  typedef result_of_callable_class<C> const volatile type;
};

template<typename C>
struct result_of_wrap_callable_class<C &> {
  typedef typename result_of_wrap_callable_class<C>::type &type;
};

template<typename F, bool TestCallability = true> struct cpp0x_result_of_impl;

#else // HBOOST_NO_SFINAE_EXPR

template<typename T>
struct result_of_always_void
{
  typedef void type;
};

template<typename F, typename Enable = void> struct cpp0x_result_of_impl {};

#endif // HBOOST_NO_SFINAE_EXPR

template<typename F>
struct result_of_void_impl
{
  typedef void type;
};

template<typename R>
struct result_of_void_impl<R (*)(void)>
{
  typedef R type;
};

template<typename R>
struct result_of_void_impl<R (&)(void)>
{
  typedef R type;
};

// Determine the return type of a function pointer or pointer to member.
template<typename F, typename FArgs>
struct result_of_pointer
  : tr1_result_of_impl<typename remove_cv<F>::type, FArgs, false> { };

template<typename F, typename FArgs>
struct tr1_result_of_impl<F, FArgs, true>
{
  typedef typename F::result_type type;
};

template<typename FArgs>
struct is_function_with_no_args : mpl::false_ {};

template<typename F>
struct is_function_with_no_args<F(void)> : mpl::true_ {};

template<typename F, typename FArgs>
struct result_of_nested_result : F::template result<FArgs>
{};

template<typename F, typename FArgs>
struct tr1_result_of_impl<F, FArgs, false>
  : mpl::if_<is_function_with_no_args<FArgs>,
             result_of_void_impl<F>,
             result_of_nested_result<F, FArgs> >::type
{};

} // end namespace detail

#define HBOOST_PP_ITERATION_PARAMS_1 (3,(0,HBOOST_RESULT_OF_NUM_ARGS,<hboost/utility/detail/result_of_iterate.hpp>))
#include HBOOST_PP_ITERATE()

#else
#  define HBOOST_NO_RESULT_OF 1
#endif

}

#endif // HBOOST_RESULT_OF_HPP
