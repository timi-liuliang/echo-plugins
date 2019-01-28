//  (C) Copyright Ion Gaztanaga 2014.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_COPY_ASSIGNABLE_HPP_INCLUDED
#define HBOOST_TT_IS_COPY_ASSIGNABLE_HPP_INCLUDED

#include <hboost/config.hpp>
#include <hboost/type_traits/detail/yes_no_type.hpp>
#include <hboost/type_traits/is_base_and_derived.hpp>
#include <hboost/noncopyable.hpp>

#if !defined(HBOOST_NO_CXX11_DELETED_FUNCTIONS) && !defined(HBOOST_NO_CXX11_DECLTYPE) \
   && !defined(HBOOST_INTEL_CXX_VERSION) && \
      !(defined(HBOOST_MSVC) && _MSC_VER == 1800)
#define HBOOST_TT_CXX11_IS_COPY_ASSIGNABLE
#include <hboost/type_traits/declval.hpp>
#else
   //For compilers without decltype
   #include <hboost/type_traits/is_const.hpp>
   #include <hboost/type_traits/is_array.hpp>
   #include <hboost/type_traits/add_reference.hpp>
   #include <hboost/type_traits/remove_reference.hpp>
#endif

namespace hboost {

namespace detail{

template <bool DerivedFromNoncopyable, class T>
struct is_copy_assignable_impl2 {

// Intel compiler has problems with SFINAE for copy constructors and deleted functions:
//
// error: function *function_name* cannot be referenced -- it is a deleted function
// static hboost::type_traits::yes_type test(T1&, decltype(T1(hboost::declval<T1&>()))* = 0);
//                                                        ^ 
//
// MSVC 12.0 (Visual 2013) has problems when the copy constructor has been deleted. See:
// https://connect.microsoft.com/VisualStudio/feedback/details/800328/std-is-copy-constructible-is-broken
#if defined(HBOOST_TT_CXX11_IS_COPY_ASSIGNABLE)
    typedef hboost::type_traits::yes_type yes_type;
    typedef hboost::type_traits::no_type  no_type;

    template <class U>
    static decltype(::hboost::declval<U&>() = ::hboost::declval<const U&>(), yes_type() ) test(int);

    template <class>
    static no_type test(...);

    static const bool value = sizeof(test<T>(0)) == sizeof(yes_type);

#else
    static HBOOST_DEDUCED_TYPENAME hboost::add_reference<T>::type produce();

    template <class T1>
    static hboost::type_traits::no_type test(T1&, typename T1::hboost_move_no_copy_constructor_or_assign* = 0);

    static hboost::type_traits::yes_type test(...);
    // If you see errors like this:
    //
    //      `'T::operator=(const T&)' is private`
    //      `hboost/type_traits/is_copy_assignable.hpp:NN:M: error: within this context`
    //
    // then you are trying to call that macro for a structure defined like that:
    //
    //      struct T {
    //          ...
    //      private:
    //          T & operator=(const T &);
    //          ...
    //      };
    //
    // To fix that you must modify your structure:
    //
    //      // C++03 and C++11 version
    //      struct T: private hboost::noncopyable {
    //          ...
    //      private:
    //          T & operator=(const T &);
    //          ...
    //      };
    //
    //      // C++11 version
    //      struct T {
    //          ...
    //      private:
    //          T& operator=(const T &) = delete;
    //          ...
    //      };
    HBOOST_STATIC_CONSTANT(bool, value = (
            sizeof(test(produce())) == sizeof(hboost::type_traits::yes_type)
    ));
   #endif
};

template <class T>
struct is_copy_assignable_impl2<true, T> {
    HBOOST_STATIC_CONSTANT(bool, value = false);
};

template <class T>
struct is_copy_assignable_impl {

#if !defined(HBOOST_TT_CXX11_IS_COPY_ASSIGNABLE)
    //For compilers without decltype, at least return false on const types, arrays
    //types derived from hboost::noncopyable and types defined as HBOOST_MOVEABLE_BUT_NOT_COPYABLE
    typedef HBOOST_DEDUCED_TYPENAME hboost::remove_reference<T>::type unreferenced_t;
    HBOOST_STATIC_CONSTANT(bool, value = (
        hboost::detail::is_copy_assignable_impl2<
            hboost::is_base_and_derived<hboost::noncopyable, T>::value
            || hboost::is_const<unreferenced_t>::value || hboost::is_array<unreferenced_t>::value
            ,T
        >::value
    ));
    #else
    HBOOST_STATIC_CONSTANT(bool, value = (
        hboost::detail::is_copy_assignable_impl2<
            hboost::is_base_and_derived<hboost::noncopyable, T>::value,T
        >::value
    ));
    #endif
};

} // namespace detail

template <class T> struct is_copy_assignable : public integral_constant<bool, ::hboost::detail::is_copy_assignable_impl<T>::value>{};
template <> struct is_copy_assignable<void> : public false_type{};
#ifndef HBOOST_NO_CV_VOID_SPECIALIZATIONS
template <> struct is_copy_assignable<void const> : public false_type{};
template <> struct is_copy_assignable<void const volatile> : public false_type{};
template <> struct is_copy_assignable<void volatile> : public false_type{};
#endif

} // namespace hboost

#endif // HBOOST_TT_IS_COPY_ASSIGNABLE_HPP_INCLUDED
