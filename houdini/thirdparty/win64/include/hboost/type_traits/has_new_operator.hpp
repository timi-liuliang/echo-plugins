
//  (C) Copyright Runar Undheim, Robert Ramey & John Maddock 2008.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_HAS_NEW_OPERATOR_HPP_INCLUDED
#define HBOOST_TT_HAS_NEW_OPERATOR_HPP_INCLUDED

#include <new> // std::nothrow_t
#include <cstddef> // std::size_t
#include <hboost/type_traits/integral_constant.hpp>
#include <hboost/type_traits/detail/yes_no_type.hpp>
#include <hboost/detail/workaround.hpp>

#if defined(new) 
#  if HBOOST_WORKAROUND(HBOOST_MSVC, >= 1310)
#     define HBOOST_TT_AUX_MACRO_NEW_DEFINED
#     pragma push_macro("new")
#     undef new
#  else
#     error "Sorry but you can't include this header if 'new' is defined as a macro."
#  endif
#endif

namespace hboost {
namespace detail {
    template <class U, U x> 
    struct test;

    template <typename T>
    struct has_new_operator_impl {
        template<class U>
        static type_traits::yes_type check_sig1(
            U*, 
            test<
            void *(*)(std::size_t),
                &U::operator new
            >* = NULL
        );
        template<class U>
        static type_traits::no_type check_sig1(...);

        template<class U>
        static type_traits::yes_type check_sig2(
            U*, 
            test<
            void *(*)(std::size_t, const std::nothrow_t&),
                &U::operator new
            >* = NULL
        );
        template<class U>
        static type_traits::no_type check_sig2(...);

        template<class U>
        static type_traits::yes_type check_sig3(
            U*, 
            test<
            void *(*)(std::size_t, void*),
                &U::operator new
            >* = NULL
        );
        template<class U>
        static type_traits::no_type check_sig3(...);


        template<class U>
        static type_traits::yes_type check_sig4(
            U*, 
            test<
            void *(*)(std::size_t),
                &U::operator new[]
            >* = NULL
        );
        template<class U>
        static type_traits::no_type check_sig4(...);

        template<class U>
        static type_traits::yes_type check_sig5(
            U*, 
            test<
            void *(*)(std::size_t, const std::nothrow_t&),
                &U::operator new[]
            >* = NULL
        );
        template<class U>
        static type_traits::no_type check_sig5(...);

        template<class U>
        static type_traits::yes_type check_sig6(
            U*, 
            test<
            void *(*)(std::size_t, void*),
                &U::operator new[]
            >* = NULL
        );
        template<class U>
        static type_traits::no_type check_sig6(...);

        // GCC2 won't even parse this template if we embed the computation
        // of s1 in the computation of value.
        #ifdef __GNUC__
            HBOOST_STATIC_CONSTANT(unsigned, s1 = sizeof(has_new_operator_impl<T>::template check_sig1<T>(0)));
            HBOOST_STATIC_CONSTANT(unsigned, s2 = sizeof(has_new_operator_impl<T>::template check_sig2<T>(0)));
            HBOOST_STATIC_CONSTANT(unsigned, s3 = sizeof(has_new_operator_impl<T>::template check_sig3<T>(0)));
            HBOOST_STATIC_CONSTANT(unsigned, s4 = sizeof(has_new_operator_impl<T>::template check_sig4<T>(0)));
            HBOOST_STATIC_CONSTANT(unsigned, s5 = sizeof(has_new_operator_impl<T>::template check_sig5<T>(0)));
            HBOOST_STATIC_CONSTANT(unsigned, s6 = sizeof(has_new_operator_impl<T>::template check_sig6<T>(0)));
        #else
            #if HBOOST_WORKAROUND(HBOOST_MSVC_FULL_VER, >= 140050000)
                #pragma warning(push)
                #pragma warning(disable:6334)
            #endif

            HBOOST_STATIC_CONSTANT(unsigned, s1 = sizeof(check_sig1<T>(0)));
            HBOOST_STATIC_CONSTANT(unsigned, s2 = sizeof(check_sig2<T>(0)));
            HBOOST_STATIC_CONSTANT(unsigned, s3 = sizeof(check_sig3<T>(0)));
            HBOOST_STATIC_CONSTANT(unsigned, s4 = sizeof(check_sig4<T>(0)));
            HBOOST_STATIC_CONSTANT(unsigned, s5 = sizeof(check_sig5<T>(0)));
            HBOOST_STATIC_CONSTANT(unsigned, s6 = sizeof(check_sig6<T>(0)));

            #if HBOOST_WORKAROUND(HBOOST_MSVC_FULL_VER, >= 140050000)
                #pragma warning(pop)
            #endif
        #endif
        HBOOST_STATIC_CONSTANT(bool, value = 
            (s1 == sizeof(type_traits::yes_type)) ||
            (s2 == sizeof(type_traits::yes_type)) ||
            (s3 == sizeof(type_traits::yes_type)) ||
            (s4 == sizeof(type_traits::yes_type)) ||
            (s5 == sizeof(type_traits::yes_type)) ||
            (s6 == sizeof(type_traits::yes_type))
        );
    };
} // namespace detail

template <class T> struct has_new_operator : public integral_constant<bool, ::hboost::detail::has_new_operator_impl<T>::value>{};

} // namespace hboost

#if defined(HBOOST_TT_AUX_MACRO_NEW_DEFINED)
#  pragma pop_macro("new")
#endif

#endif // HBOOST_TT_HAS_NEW_OPERATOR_HPP_INCLUDED
