
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, 
//  Aleksey Gurtovoy, Howard Hinnant & John Maddock 2000.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

#ifndef HBOOST_TT_DETAIL_IS_MEM_FUN_POINTER_IMPL_HPP_INCLUDED
#define HBOOST_TT_DETAIL_IS_MEM_FUN_POINTER_IMPL_HPP_INCLUDED

#include <hboost/config.hpp>

#if defined(HBOOST_TT_PREPROCESSING_MODE)
//
// Maintenance mode, hide include dependencies
// from trackers:
//
#define PPI <hboost/preprocessor/iterate.hpp>
#include PPI
#undef PPI
#define PPI <hboost/preprocessor/enum_params.hpp>
#include PPI
#undef PPI
#define PPI <hboost/preprocessor/comma_if.hpp>
#include PPI
#undef PPI
#endif

namespace hboost {
namespace type_traits {

template <typename T>
struct is_mem_fun_pointer_impl
{
    HBOOST_STATIC_CONSTANT(bool, value = false);
};

#if !defined(HBOOST_TT_PREPROCESSING_MODE)
// pre-processed code, don't edit, try GNU cpp with 
// cpp -I../../../ -DHBOOST_TT_PREPROCESSING_MODE -x c++ -P filename

template <class R, class T >
struct is_mem_fun_pointer_impl<R (T::*)() > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T >
struct is_mem_fun_pointer_impl<R (T::*)( ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T >
struct is_mem_fun_pointer_impl<R (T::*)() const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T >
struct is_mem_fun_pointer_impl<R (T::*)() volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T >
struct is_mem_fun_pointer_impl<R (T::*)() const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T >
struct is_mem_fun_pointer_impl<R (T::*)( ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T >
struct is_mem_fun_pointer_impl<R (T::*)( ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T >
struct is_mem_fun_pointer_impl<R (T::*)( ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0>
struct is_mem_fun_pointer_impl<R (T::*)( T0) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0>
struct is_mem_fun_pointer_impl<R (T::*)( T0 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0>
struct is_mem_fun_pointer_impl<R (T::*)( T0) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0>
struct is_mem_fun_pointer_impl<R (T::*)( T0) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0>
struct is_mem_fun_pointer_impl<R (T::*)( T0) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0>
struct is_mem_fun_pointer_impl<R (T::*)( T0 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0>
struct is_mem_fun_pointer_impl<R (T::*)( T0 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0>
struct is_mem_fun_pointer_impl<R (T::*)( T0 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23 , class T24>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23 , class T24>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };

#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23 , class T24>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23 , class T24>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23 , class T24>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23 , class T24>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23 , class T24>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T , class T0 , class T1 , class T2 , class T3 , class T4 , class T5 , class T6 , class T7 , class T8 , class T9 , class T10 , class T11 , class T12 , class T13 , class T14 , class T15 , class T16 , class T17 , class T18 , class T19 , class T20 , class T21 , class T22 , class T23 , class T24>
struct is_mem_fun_pointer_impl<R (T::*)( T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
#endif

#else

#undef HBOOST_STATIC_CONSTANT
#define HBOOST_PP_ITERATION_PARAMS_1 \
    (3, (0, 25, "hboost/type_traits/detail/is_mem_fun_pointer_impl.hpp"))
#include HBOOST_PP_ITERATE()

#endif // HBOOST_TT_PREPROCESSING_MODE

} // namespace type_traits
} // namespace hboost

#endif // HBOOST_TT_DETAIL_IS_MEM_FUN_POINTER_IMPL_HPP_INCLUDED

///// iteration

#else
#define HBOOST_PP_COUNTER HBOOST_PP_FRAME_ITERATION(1)

template <class R, class T HBOOST_PP_COMMA_IF(HBOOST_PP_COUNTER) HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,class T)>
struct is_mem_fun_pointer_impl<R (T::*)(HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,T)) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
@#ifndef HBOOST_TT_NO_ELLIPSIS_IN_FUNC_TESTING
template <class R, class T HBOOST_PP_COMMA_IF(HBOOST_PP_COUNTER) HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,class T)>
struct is_mem_fun_pointer_impl<R (T::*)(HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,T) ...) > { HBOOST_STATIC_CONSTANT(bool, value = true); };
@#endif

@#if !defined(HBOOST_TT_NO_CV_FUNC_TEST)
template <class R, class T HBOOST_PP_COMMA_IF(HBOOST_PP_COUNTER) HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,class T)>
struct is_mem_fun_pointer_impl<R (T::*)(HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,T)) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T HBOOST_PP_COMMA_IF(HBOOST_PP_COUNTER) HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,class T)>
struct is_mem_fun_pointer_impl<R (T::*)(HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,T)) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T HBOOST_PP_COMMA_IF(HBOOST_PP_COUNTER) HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,class T)>
struct is_mem_fun_pointer_impl<R (T::*)(HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,T)) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

@#ifndef HBOOST_TT_NO_ELLIPSIS_IN_FUNC_TESTING
template <class R, class T HBOOST_PP_COMMA_IF(HBOOST_PP_COUNTER) HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,class T)>
struct is_mem_fun_pointer_impl<R (T::*)(HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,T) ...) const > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T HBOOST_PP_COMMA_IF(HBOOST_PP_COUNTER) HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,class T)>
struct is_mem_fun_pointer_impl<R (T::*)(HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,T) ...) volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };

template <class R, class T HBOOST_PP_COMMA_IF(HBOOST_PP_COUNTER) HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,class T)>
struct is_mem_fun_pointer_impl<R (T::*)(HBOOST_PP_ENUM_PARAMS(HBOOST_PP_COUNTER,T) ...) const volatile > { HBOOST_STATIC_CONSTANT(bool, value = true); };
@#endif
@#endif

#undef HBOOST_PP_COUNTER
#endif // HBOOST_PP_IS_ITERATING

