
// Copyright 2005-2011 Daniel James.
// Copyright 2009 Pablo Halpern.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/unordered for documentation

#ifndef HBOOST_UNORDERED_ALLOCATE_HPP
#define HBOOST_UNORDERED_ALLOCATE_HPP

#include <hboost/config.hpp>
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#pragma once
#endif

#include <hboost/unordered/detail/fwd.hpp>
#include <hboost/move/move.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/inc.hpp>
#include <hboost/preprocessor/dec.hpp>
#include <hboost/preprocessor/repetition/enum.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/repetition/repeat_from_to.hpp>
#include <hboost/type_traits/is_class.hpp>
#include <hboost/type_traits/add_lvalue_reference.hpp>
#include <hboost/tuple/tuple.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/utility/addressof.hpp>
#include <hboost/detail/select_type.hpp>
#include <hboost/assert.hpp>
#include <utility>

#if !defined(HBOOST_NO_CXX11_HDR_TUPLE)
#include <tuple>
#endif

#if defined(HBOOST_MSVC)
#pragma warning(push)
#pragma warning(disable:4512) // assignment operator could not be generated.
#pragma warning(disable:4345) // behavior change: an object of POD type
                              // constructed with an initializer of the form ()
                              // will be default-initialized.
#endif

#define HBOOST_UNORDERED_EMPLACE_LIMIT 10

namespace hboost { namespace unordered { namespace detail {

    ////////////////////////////////////////////////////////////////////////////
    // Bits and pieces for implementing traits

    template <typename T> typename hboost::add_lvalue_reference<T>::type make();
    struct choice9 { typedef char (&type)[9]; };
    struct choice8 : choice9 { typedef char (&type)[8]; };
    struct choice7 : choice8 { typedef char (&type)[7]; };
    struct choice6 : choice7 { typedef char (&type)[6]; };
    struct choice5 : choice6 { typedef char (&type)[5]; };
    struct choice4 : choice5 { typedef char (&type)[4]; };
    struct choice3 : choice4 { typedef char (&type)[3]; };
    struct choice2 : choice3 { typedef char (&type)[2]; };
    struct choice1 : choice2 { typedef char (&type)[1]; };
    choice1 choose();

    typedef choice1::type yes_type;
    typedef choice2::type no_type;

    struct private_type
    {
       private_type const &operator,(int) const;
    };

    template <typename T>
    no_type is_private_type(T const&);
    yes_type is_private_type(private_type const&);

    struct convert_from_anything {
        template <typename T>
        convert_from_anything(T const&);
    };

    ////////////////////////////////////////////////////////////////////////////
    // emplace_args
    //
    // Either forwarding variadic arguments, or storing the arguments in
    // emplace_args##n

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

#define HBOOST_UNORDERED_EMPLACE_TEMPLATE typename... Args
#define HBOOST_UNORDERED_EMPLACE_ARGS HBOOST_FWD_REF(Args)... args
#define HBOOST_UNORDERED_EMPLACE_FORWARD hboost::forward<Args>(args)...

#define HBOOST_UNORDERED_EMPLACE_ARGS1(a0) a0
#define HBOOST_UNORDERED_EMPLACE_ARGS2(a0, a1) a0, a1
#define HBOOST_UNORDERED_EMPLACE_ARGS3(a0, a1, a2) a0, a1, a2

#else

#define HBOOST_UNORDERED_EMPLACE_TEMPLATE typename Args
#define HBOOST_UNORDERED_EMPLACE_ARGS Args const& args
#define HBOOST_UNORDERED_EMPLACE_FORWARD args

#define HBOOST_UNORDERED_FWD_PARAM(z, n, a) \
    HBOOST_FWD_REF(HBOOST_PP_CAT(A, n)) HBOOST_PP_CAT(a, n)

#define HBOOST_UNORDERED_CALL_FORWARD(z, i, a) \
    hboost::forward<HBOOST_PP_CAT(A,i)>(HBOOST_PP_CAT(a,i))

#define HBOOST_UNORDERED_EARGS(z, n, _)                                      \
    template <HBOOST_PP_ENUM_PARAMS_Z(z, n, typename A)>                     \
    struct HBOOST_PP_CAT(emplace_args, n)                                    \
    {                                                                       \
        HBOOST_PP_REPEAT_##z(n, HBOOST_UNORDERED_EARGS_MEMBER, _)             \
        HBOOST_PP_CAT(emplace_args, n) (                                     \
            HBOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, Arg, b)                     \
        ) : HBOOST_PP_ENUM_##z(n, HBOOST_UNORDERED_EARGS_INIT, _)             \
        {}                                                                  \
                                                                            \
    };                                                                      \
                                                                            \
    template <HBOOST_PP_ENUM_PARAMS_Z(z, n, typename A)>                     \
    inline HBOOST_PP_CAT(emplace_args, n) <                                  \
        HBOOST_PP_ENUM_PARAMS_Z(z, n, A)                                     \
    > create_emplace_args(                                                  \
        HBOOST_PP_ENUM_##z(n, HBOOST_UNORDERED_FWD_PARAM, b)                  \
    )                                                                       \
    {                                                                       \
        HBOOST_PP_CAT(emplace_args, n) <                                     \
            HBOOST_PP_ENUM_PARAMS_Z(z, n, A)                                 \
        > e(HBOOST_PP_ENUM_PARAMS_Z(z, n, b));                               \
        return e;                                                           \
    }

#define HBOOST_UNORDERED_EMPLACE_ARGS1 create_emplace_args
#define HBOOST_UNORDERED_EMPLACE_ARGS2 create_emplace_args
#define HBOOST_UNORDERED_EMPLACE_ARGS3 create_emplace_args

#if defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)

#define HBOOST_UNORDERED_EARGS_MEMBER(z, n, _)                               \
    typedef HBOOST_FWD_REF(HBOOST_PP_CAT(A, n)) HBOOST_PP_CAT(Arg, n);         \
    HBOOST_PP_CAT(Arg, n) HBOOST_PP_CAT(a, n);

#define HBOOST_UNORDERED_EARGS_INIT(z, n, _)                                 \
    HBOOST_PP_CAT(a, n)(                                                     \
        hboost::forward<HBOOST_PP_CAT(A,n)>(HBOOST_PP_CAT(b, n)))

#else

#define HBOOST_UNORDERED_EARGS_MEMBER(z, n, _)                               \
    typedef typename hboost::add_lvalue_reference<HBOOST_PP_CAT(A, n)>::type  \
        HBOOST_PP_CAT(Arg, n);                                               \
    HBOOST_PP_CAT(Arg, n) HBOOST_PP_CAT(a, n);

#define HBOOST_UNORDERED_EARGS_INIT(z, n, _)                                 \
    HBOOST_PP_CAT(a, n)(HBOOST_PP_CAT(b, n))

#endif

HBOOST_PP_REPEAT_FROM_TO(1, HBOOST_UNORDERED_EMPLACE_LIMIT, HBOOST_UNORDERED_EARGS,
    _)

#undef HBOOST_UNORDERED_DEFINE_EMPLACE_ARGS
#undef HBOOST_UNORDERED_EARGS_MEMBER
#undef HBOOST_UNORDERED_EARGS_INIT

#endif

}}}

////////////////////////////////////////////////////////////////////////////////
//
// Pick which version of allocator_traits to use
//
// 0 = Own partial implementation
// 1 = std::allocator_traits
// 2 = hboost::container::allocator_traits

#if !defined(HBOOST_UNORDERED_USE_ALLOCATOR_TRAITS)
#   if defined(__GXX_EXPERIMENTAL_CXX0X__) && \
            (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7))
#       define HBOOST_UNORDERED_USE_ALLOCATOR_TRAITS 0
#   elif defined(HBOOST_MSVC)
#       if HBOOST_MSVC < 1400
            // Use container's allocator_traits for older versions of Visual
            // C++ as I don't test with them.
#           define HBOOST_UNORDERED_USE_ALLOCATOR_TRAITS 2
#       endif
#   endif
#endif

#if !defined(HBOOST_UNORDERED_USE_ALLOCATOR_TRAITS)
#   define HBOOST_UNORDERED_USE_ALLOCATOR_TRAITS 0
#endif

////////////////////////////////////////////////////////////////////////////////
//
// Some utilities for implementing allocator_traits, but useful elsewhere so
// they're always defined.

#if !defined(HBOOST_NO_CXX11_HDR_TYPE_TRAITS)
#  include <type_traits>
#endif

namespace hboost { namespace unordered { namespace detail {

    ////////////////////////////////////////////////////////////////////////////
    // Integral_constrant, true_type, false_type
    //
    // Uses the standard versions if available.

#if !defined(HBOOST_NO_CXX11_HDR_TYPE_TRAITS)

    using std::integral_constant;
    using std::true_type;
    using std::false_type;

#else

    template <typename T, T Value>
    struct integral_constant { enum { value = Value }; };

    typedef hboost::unordered::detail::integral_constant<bool, true> true_type;
    typedef hboost::unordered::detail::integral_constant<bool, false> false_type;

#endif

    ////////////////////////////////////////////////////////////////////////////
    // Explicitly call a destructor

#if defined(HBOOST_MSVC)
#pragma warning(push)
#pragma warning(disable:4100) // unreferenced formal parameter
#endif

    namespace func {
        template <class T>
        inline void destroy(T* x) {
            x->~T();
        }
    }

#if defined(HBOOST_MSVC)
#pragma warning(pop)
#endif

    ////////////////////////////////////////////////////////////////////////////
    // Expression test mechanism
    //
    // When SFINAE expressions are available, define
    // HBOOST_UNORDERED_HAS_FUNCTION which can check if a function call is
    // supported by a class, otherwise define HBOOST_UNORDERED_HAS_MEMBER which
    // can detect if a class has the specified member, but not that it has the
    // correct type, this is good enough for a passable impression of
    // allocator_traits.

#if !defined(HBOOST_NO_SFINAE_EXPR)

    template <typename T, unsigned int> struct expr_test;
    template <typename T> struct expr_test<T, sizeof(char)> : T {};

#   define HBOOST_UNORDERED_CHECK_EXPRESSION(count, result, expression)      \
        template <typename U>                                               \
        static typename hboost::unordered::detail::expr_test<                \
            HBOOST_PP_CAT(choice, result),                                   \
            sizeof(for_expr_test((                                          \
                (expression),                                               \
            0)))>::type test(                                               \
            HBOOST_PP_CAT(choice, count))

#   define HBOOST_UNORDERED_DEFAULT_EXPRESSION(count, result)                \
        template <typename U>                                               \
        static HBOOST_PP_CAT(choice, result)::type test(                     \
            HBOOST_PP_CAT(choice, count))

#   define HBOOST_UNORDERED_HAS_FUNCTION(name, thing, args, _)               \
    struct HBOOST_PP_CAT(has_, name)                                         \
    {                                                                       \
        template <typename U> static char for_expr_test(U const&);          \
        HBOOST_UNORDERED_CHECK_EXPRESSION(1, 1,                              \
            hboost::unordered::detail::make< thing >().name args);           \
        HBOOST_UNORDERED_DEFAULT_EXPRESSION(2, 2);                           \
                                                                            \
        enum { value = sizeof(test<T>(choose())) == sizeof(choice1::type) };\
    }

#else

    template <typename T> struct identity { typedef T type; };

#   define HBOOST_UNORDERED_CHECK_MEMBER(count, result, name, member)        \
                                                                            \
    typedef typename hboost::unordered::detail::identity<member>::type       \
        HBOOST_PP_CAT(check, count);                                         \
                                                                            \
    template <HBOOST_PP_CAT(check, count) e>                                 \
    struct HBOOST_PP_CAT(test, count) {                                      \
        typedef HBOOST_PP_CAT(choice, result) type;                          \
    };                                                                      \
                                                                            \
    template <class U> static typename                                      \
        HBOOST_PP_CAT(test, count)<&U::name>::type                           \
        test(HBOOST_PP_CAT(choice, count))

#   define HBOOST_UNORDERED_DEFAULT_MEMBER(count, result)                    \
    template <class U> static HBOOST_PP_CAT(choice, result)::type            \
        test(HBOOST_PP_CAT(choice, count))

#   define HBOOST_UNORDERED_HAS_MEMBER(name)                                 \
    struct HBOOST_PP_CAT(has_, name)                                         \
    {                                                                       \
        struct impl {                                                       \
            struct base_mixin { int name; };                                \
            struct base : public T, public base_mixin {};                   \
                                                                            \
            HBOOST_UNORDERED_CHECK_MEMBER(1, 1, name, int base_mixin::*);    \
            HBOOST_UNORDERED_DEFAULT_MEMBER(2, 2);                           \
                                                                            \
            enum { value = sizeof(choice2::type) ==                         \
                sizeof(test<base>(choose()))                                \
            };                                                              \
        };                                                                  \
                                                                            \
        enum { value = impl::value };                                       \
    }

#endif

}}}

////////////////////////////////////////////////////////////////////////////////
//
// Allocator traits
//
// First our implementation, then later light wrappers around the alternatives

#if HBOOST_UNORDERED_USE_ALLOCATOR_TRAITS == 0

#   include <hboost/limits.hpp>
#   include <hboost/utility/enable_if.hpp>
#   include <hboost/pointer_to_other.hpp>
#   if defined(HBOOST_NO_SFINAE_EXPR)
#       include <hboost/type_traits/is_same.hpp>
#   endif

#   if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && \
        !defined(HBOOST_NO_SFINAE_EXPR)
#       define HBOOST_UNORDERED_DETAIL_FULL_CONSTRUCT 1
#   else
#       define HBOOST_UNORDERED_DETAIL_FULL_CONSTRUCT 0
#   endif

namespace hboost { namespace unordered { namespace detail {

    // TODO: Does this match std::allocator_traits<Alloc>::rebind_alloc<T>?
    template <typename Alloc, typename T>
    struct rebind_wrap
    {
        typedef typename Alloc::HBOOST_NESTED_TEMPLATE rebind<T>::other type;
    };

#   if defined(HBOOST_MSVC) && HBOOST_MSVC <= 1400

#       define HBOOST_UNORDERED_DEFAULT_TYPE_TMPLT(tname)                    \
    template <typename Tp, typename Default>                                \
    struct default_type_ ## tname {                                         \
                                                                            \
        template <typename X>                                               \
        static choice1::type test(choice1, typename X::tname* = 0);         \
                                                                            \
        template <typename X>                                               \
        static choice2::type test(choice2, void* = 0);                      \
                                                                            \
        struct DefaultWrap { typedef Default tname; };                      \
                                                                            \
        enum { value = (1 == sizeof(test<Tp>(choose()))) };                 \
                                                                            \
        typedef typename hboost::detail::if_true<value>::                    \
            HBOOST_NESTED_TEMPLATE then<Tp, DefaultWrap>                     \
            ::type::tname type;                                             \
    }

#   else

    template <typename T, typename T2>
    struct sfinae : T2 {};

#       define HBOOST_UNORDERED_DEFAULT_TYPE_TMPLT(tname)                    \
    template <typename Tp, typename Default>                                \
    struct default_type_ ## tname {                                         \
                                                                            \
        template <typename X>                                               \
        static typename hboost::unordered::detail::sfinae<                   \
                typename X::tname, choice1>::type                           \
            test(choice1);                                                  \
                                                                            \
        template <typename X>                                               \
        static choice2::type test(choice2);                                 \
                                                                            \
        struct DefaultWrap { typedef Default tname; };                      \
                                                                            \
        enum { value = (1 == sizeof(test<Tp>(choose()))) };                 \
                                                                            \
        typedef typename hboost::detail::if_true<value>::                    \
            HBOOST_NESTED_TEMPLATE then<Tp, DefaultWrap>                     \
            ::type::tname type;                                             \
    }

#   endif

#   define HBOOST_UNORDERED_DEFAULT_TYPE(T,tname, arg)                   \
    typename default_type_ ## tname<T, arg>::type

    HBOOST_UNORDERED_DEFAULT_TYPE_TMPLT(pointer);
    HBOOST_UNORDERED_DEFAULT_TYPE_TMPLT(const_pointer);
    HBOOST_UNORDERED_DEFAULT_TYPE_TMPLT(void_pointer);
    HBOOST_UNORDERED_DEFAULT_TYPE_TMPLT(const_void_pointer);
    HBOOST_UNORDERED_DEFAULT_TYPE_TMPLT(difference_type);
    HBOOST_UNORDERED_DEFAULT_TYPE_TMPLT(size_type);
    HBOOST_UNORDERED_DEFAULT_TYPE_TMPLT(propagate_on_container_copy_assignment);
    HBOOST_UNORDERED_DEFAULT_TYPE_TMPLT(propagate_on_container_move_assignment);
    HBOOST_UNORDERED_DEFAULT_TYPE_TMPLT(propagate_on_container_swap);

#   if !defined(HBOOST_NO_SFINAE_EXPR)

    template <typename T>
    HBOOST_UNORDERED_HAS_FUNCTION(
        select_on_container_copy_construction, U const, (), 0
    );

    template <typename T>
    HBOOST_UNORDERED_HAS_FUNCTION(
        max_size, U const, (), 0
    );

#       if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

    template <typename T, typename ValueType, typename... Args>
    HBOOST_UNORDERED_HAS_FUNCTION(
    construct, U, (
        hboost::unordered::detail::make<ValueType*>(),
        hboost::unordered::detail::make<Args const>()...), 2
    );

#       else

    template <typename T, typename ValueType>
    HBOOST_UNORDERED_HAS_FUNCTION(
    construct, U, (
        hboost::unordered::detail::make<ValueType*>(),
        hboost::unordered::detail::make<ValueType const>()), 2
    );

#       endif

    template <typename T, typename ValueType>
    HBOOST_UNORDERED_HAS_FUNCTION(
        destroy, U, (hboost::unordered::detail::make<ValueType*>()), 1
    );

#   else

    template <typename T>
    HBOOST_UNORDERED_HAS_MEMBER(select_on_container_copy_construction);

    template <typename T>
    HBOOST_UNORDERED_HAS_MEMBER(max_size);

    template <typename T, typename ValueType>
    HBOOST_UNORDERED_HAS_MEMBER(construct);

    template <typename T, typename ValueType>
    HBOOST_UNORDERED_HAS_MEMBER(destroy);

#   endif

    namespace func
    {

    template <typename Alloc>
    inline Alloc call_select_on_container_copy_construction(const Alloc& rhs,
        typename hboost::enable_if_c<
            hboost::unordered::detail::
            has_select_on_container_copy_construction<Alloc>::value, void*
        >::type = 0)
    {
        return rhs.select_on_container_copy_construction();
    }

    template <typename Alloc>
    inline Alloc call_select_on_container_copy_construction(const Alloc& rhs,
        typename hboost::disable_if_c<
            hboost::unordered::detail::
            has_select_on_container_copy_construction<Alloc>::value, void*
        >::type = 0)
    {
        return rhs;
    }

    template <typename SizeType, typename Alloc>
    inline SizeType call_max_size(const Alloc& a,
        typename hboost::enable_if_c<
            hboost::unordered::detail::has_max_size<Alloc>::value, void*
        >::type = 0)
    {
        return a.max_size();
    }

    template <typename SizeType, typename Alloc>
    inline SizeType call_max_size(const Alloc&, typename hboost::disable_if_c<
            hboost::unordered::detail::has_max_size<Alloc>::value, void*
        >::type = 0)
    {
        return (std::numeric_limits<SizeType>::max)();
    }

    } // namespace func.

    template <typename Alloc>
    struct allocator_traits
    {
        typedef Alloc allocator_type;
        typedef typename Alloc::value_type value_type;

        typedef HBOOST_UNORDERED_DEFAULT_TYPE(Alloc, pointer, value_type*)
            pointer;

        template <typename T>
        struct pointer_to_other : hboost::pointer_to_other<pointer, T> {};

        typedef HBOOST_UNORDERED_DEFAULT_TYPE(Alloc, const_pointer,
            typename pointer_to_other<const value_type>::type)
            const_pointer;

        //typedef HBOOST_UNORDERED_DEFAULT_TYPE(Alloc, void_pointer,
        //    typename pointer_to_other<void>::type)
        //    void_pointer;
        //
        //typedef HBOOST_UNORDERED_DEFAULT_TYPE(Alloc, const_void_pointer,
        //    typename pointer_to_other<const void>::type)
        //    const_void_pointer;

        typedef HBOOST_UNORDERED_DEFAULT_TYPE(Alloc, difference_type,
            std::ptrdiff_t) difference_type;

        typedef HBOOST_UNORDERED_DEFAULT_TYPE(Alloc, size_type, std::size_t)
            size_type;

        // TODO: rebind_alloc and rebind_traits

        static pointer allocate(Alloc& a, size_type n)
            { return a.allocate(n); }

        // I never use this, so I'll just comment it out for now.
        //
        //static pointer allocate(Alloc& a, size_type n,
        //        const_void_pointer hint)
        //    { return DEFAULT_FUNC(allocate, pointer)(a, n, hint); }

        static void deallocate(Alloc& a, pointer p, size_type n)
            { a.deallocate(p, n); }

    public:

#   if HBOOST_UNORDERED_DETAIL_FULL_CONSTRUCT

        template <typename T, typename... Args>
        static typename hboost::enable_if_c<
                hboost::unordered::detail::has_construct<Alloc, T, Args...>
                ::value>::type
            construct(Alloc& a, T* p, HBOOST_FWD_REF(Args)... x)
        {
            a.construct(p, hboost::forward<Args>(x)...);
        }

        template <typename T, typename... Args>
        static typename hboost::disable_if_c<
                hboost::unordered::detail::has_construct<Alloc, T, Args...>
                ::value>::type
            construct(Alloc&, T* p, HBOOST_FWD_REF(Args)... x)
        {
            new ((void*) p) T(hboost::forward<Args>(x)...);
        }

        template <typename T>
        static typename hboost::enable_if_c<
                hboost::unordered::detail::has_destroy<Alloc, T>::value>::type
            destroy(Alloc& a, T* p)
        {
            a.destroy(p);
        }

        template <typename T>
        static typename hboost::disable_if_c<
                hboost::unordered::detail::has_destroy<Alloc, T>::value>::type
            destroy(Alloc&, T* p)
        {
            hboost::unordered::detail::func::destroy(p);
        }

#   elif !defined(HBOOST_NO_SFINAE_EXPR)

        template <typename T>
        static typename hboost::enable_if_c<
                hboost::unordered::detail::has_construct<Alloc, T>::value>::type
            construct(Alloc& a, T* p, T const& x)
        {
            a.construct(p, x);
        }

        template <typename T>
        static typename hboost::disable_if_c<
                hboost::unordered::detail::has_construct<Alloc, T>::value>::type
            construct(Alloc&, T* p, T const& x)
        {
            new ((void*) p) T(x);
        }

        template <typename T>
        static typename hboost::enable_if_c<
                hboost::unordered::detail::has_destroy<Alloc, T>::value>::type
            destroy(Alloc& a, T* p)
        {
            a.destroy(p);
        }

        template <typename T>
        static typename hboost::disable_if_c<
                hboost::unordered::detail::has_destroy<Alloc, T>::value>::type
            destroy(Alloc&, T* p)
        {
            hboost::unordered::detail::func::destroy(p);
        }

#   else

        // If we don't have SFINAE expressions, only call construct for the
        // copy constructor for the allocator's value_type - as that's
        // the only construct method that old fashioned allocators support.

        template <typename T>
        static void construct(Alloc& a, T* p, T const& x,
            typename hboost::enable_if_c<
                    hboost::unordered::detail::has_construct<Alloc, T>::value &&
                    hboost::is_same<T, value_type>::value,
                    void*>::type = 0)
        {
            a.construct(p, x);
        }

        template <typename T>
        static void construct(Alloc&, T* p, T const& x,
            typename hboost::disable_if_c<
                hboost::unordered::detail::has_construct<Alloc, T>::value &&
                hboost::is_same<T, value_type>::value,
                void*>::type = 0)
        {
            new ((void*) p) T(x);
        }

        template <typename T>
        static void destroy(Alloc& a, T* p,
            typename hboost::enable_if_c<
                hboost::unordered::detail::has_destroy<Alloc, T>::value &&
                hboost::is_same<T, value_type>::value,
                void*>::type = 0)
        {
            a.destroy(p);
        }

        template <typename T>
        static void destroy(Alloc&, T* p,
            typename hboost::disable_if_c<
                hboost::unordered::detail::has_destroy<Alloc, T>::value &&
                hboost::is_same<T, value_type>::value,
                void*>::type = 0)
        {
            hboost::unordered::detail::func::destroy(p);
        }

#   endif

        static size_type max_size(const Alloc& a)
        {
            return hboost::unordered::detail::func::
                call_max_size<size_type>(a);
        }

        // Allocator propagation on construction

        static Alloc select_on_container_copy_construction(Alloc const& rhs)
        {
            return hboost::unordered::detail::func::
                call_select_on_container_copy_construction(rhs);
        }

        // Allocator propagation on assignment and swap.
        // Return true if lhs is modified.
        typedef HBOOST_UNORDERED_DEFAULT_TYPE(
            Alloc, propagate_on_container_copy_assignment, false_type)
            propagate_on_container_copy_assignment;
        typedef HBOOST_UNORDERED_DEFAULT_TYPE(
            Alloc,propagate_on_container_move_assignment, false_type)
            propagate_on_container_move_assignment;
        typedef HBOOST_UNORDERED_DEFAULT_TYPE(
            Alloc,propagate_on_container_swap,false_type)
            propagate_on_container_swap;
    };
}}}

#   undef HBOOST_UNORDERED_DEFAULT_TYPE_TMPLT
#   undef HBOOST_UNORDERED_DEFAULT_TYPE

////////////////////////////////////////////////////////////////////////////////
//
// std::allocator_traits

#elif HBOOST_UNORDERED_USE_ALLOCATOR_TRAITS == 1

#   include <memory>

#   define HBOOST_UNORDERED_DETAIL_FULL_CONSTRUCT 1

namespace hboost { namespace unordered { namespace detail {

    template <typename Alloc>
    struct allocator_traits : std::allocator_traits<Alloc> {};

    template <typename Alloc, typename T>
    struct rebind_wrap
    {
        typedef typename std::allocator_traits<Alloc>::
            template rebind_alloc<T> type;
    };
}}}

////////////////////////////////////////////////////////////////////////////////
//
// hboost::container::allocator_traits

#elif HBOOST_UNORDERED_USE_ALLOCATOR_TRAITS == 2

#   include <hboost/container/allocator_traits.hpp>

#   define HBOOST_UNORDERED_DETAIL_FULL_CONSTRUCT 0

namespace hboost { namespace unordered { namespace detail {

    template <typename Alloc>
    struct allocator_traits :
        hboost::container::allocator_traits<Alloc> {};

    template <typename Alloc, typename T>
    struct rebind_wrap :
        hboost::container::allocator_traits<Alloc>::
            template portable_rebind_alloc<T>
    {};

}}}

#else

#error "Invalid HBOOST_UNORDERED_USE_ALLOCATOR_TRAITS value."

#endif


namespace hboost { namespace unordered { namespace detail { namespace func {

    ////////////////////////////////////////////////////////////////////////////
    // call_construct

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

#   if HBOOST_UNORDERED_DETAIL_FULL_CONSTRUCT

    template <typename Alloc, typename T, typename... Args>
    inline void call_construct(Alloc& alloc, T* address,
        HBOOST_FWD_REF(Args)... args)
    {
        hboost::unordered::detail::allocator_traits<Alloc>::construct(alloc,
            address, hboost::forward<Args>(args)...);
    }

    template <typename Alloc, typename T>
    inline void destroy_value_impl(Alloc& alloc, T* x) {
        hboost::unordered::detail::allocator_traits<Alloc>::destroy(alloc, x);
    }


#   else

    template <typename Alloc, typename T, typename... Args>
    inline void call_construct(Alloc&, T* address,
        HBOOST_FWD_REF(Args)... args)
    {
        new((void*) address) T(hboost::forward<Args>(args)...);
    }

    template <typename Alloc, typename T>
    inline void destroy_value_impl(Alloc&, T* x) {
        hboost::unordered::detail::func::destroy(x);
    }


#   endif

#else

    template <typename Alloc, typename T>
    inline void destroy_value_impl(Alloc&, T* x) {
        hboost::unordered::detail::func::destroy(x);
    }

#endif

    ////////////////////////////////////////////////////////////////////////////
    // Construct from tuple
    //
    // Used for piecewise construction.

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

#   define HBOOST_UNORDERED_CONSTRUCT_FROM_TUPLE(n, namespace_)              \
    template<typename Alloc, typename T>                                    \
    void construct_from_tuple(Alloc& alloc, T* ptr, namespace_ tuple<>)     \
    {                                                                       \
        hboost::unordered::detail::func::call_construct(alloc, ptr);         \
    }                                                                       \
                                                                            \
    HBOOST_PP_REPEAT_FROM_TO(1, n,                                           \
        HBOOST_UNORDERED_CONSTRUCT_FROM_TUPLE_IMPL, namespace_)

#   define HBOOST_UNORDERED_CONSTRUCT_FROM_TUPLE_IMPL(z, n, namespace_)      \
    template<typename Alloc, typename T,                                    \
        HBOOST_PP_ENUM_PARAMS_Z(z, n, typename A)>                           \
    void construct_from_tuple(Alloc& alloc, T* ptr,                         \
            namespace_ tuple<HBOOST_PP_ENUM_PARAMS_Z(z, n, A)> const& x)     \
    {                                                                       \
        hboost::unordered::detail::func::call_construct(alloc, ptr,          \
            HBOOST_PP_ENUM_##z(n, HBOOST_UNORDERED_GET_TUPLE_ARG, namespace_) \
        );                                                                  \
    }

#   define HBOOST_UNORDERED_GET_TUPLE_ARG(z, n, namespace_)                  \
    namespace_ get<n>(x)

#elif !defined(__SUNPRO_CC)

#   define HBOOST_UNORDERED_CONSTRUCT_FROM_TUPLE(n, namespace_)              \
    template<typename Alloc, typename T>                                    \
    void construct_from_tuple(Alloc&, T* ptr, namespace_ tuple<>)           \
    {                                                                       \
        new ((void*) ptr) T();                                              \
    }                                                                       \
                                                                            \
    HBOOST_PP_REPEAT_FROM_TO(1, n,                                           \
        HBOOST_UNORDERED_CONSTRUCT_FROM_TUPLE_IMPL, namespace_)

#   define HBOOST_UNORDERED_CONSTRUCT_FROM_TUPLE_IMPL(z, n, namespace_)      \
    template<typename Alloc, typename T,                                    \
        HBOOST_PP_ENUM_PARAMS_Z(z, n, typename A)>                           \
    void construct_from_tuple(Alloc&, T* ptr,                               \
            namespace_ tuple<HBOOST_PP_ENUM_PARAMS_Z(z, n, A)> const& x)     \
    {                                                                       \
        new ((void*) ptr) T(                                                \
            HBOOST_PP_ENUM_##z(n, HBOOST_UNORDERED_GET_TUPLE_ARG, namespace_) \
        );                                                                  \
    }

#   define HBOOST_UNORDERED_GET_TUPLE_ARG(z, n, namespace_)                  \
    namespace_ get<n>(x)

#else

    template <int N> struct length {};

#   define HBOOST_UNORDERED_CONSTRUCT_FROM_TUPLE(n, namespace_)              \
    template<typename Alloc, typename T>                                    \
    void construct_from_tuple_impl(                                         \
            hboost::unordered::detail::func::length<0>, Alloc&, T* ptr,      \
            namespace_ tuple<>)                                             \
    {                                                                       \
        new ((void*) ptr) T();                                              \
    }                                                                       \
                                                                            \
    HBOOST_PP_REPEAT_FROM_TO(1, n,                                           \
        HBOOST_UNORDERED_CONSTRUCT_FROM_TUPLE_IMPL, namespace_)

#   define HBOOST_UNORDERED_CONSTRUCT_FROM_TUPLE_IMPL(z, n, namespace_)      \
    template<typename Alloc, typename T,                                    \
        HBOOST_PP_ENUM_PARAMS_Z(z, n, typename A)>                           \
    void construct_from_tuple_impl(                                         \
            hboost::unordered::detail::func::length<n>, Alloc&, T* ptr,      \
            namespace_ tuple<HBOOST_PP_ENUM_PARAMS_Z(z, n, A)> const& x)     \
    {                                                                       \
        new ((void*) ptr) T(                                                \
            HBOOST_PP_ENUM_##z(n, HBOOST_UNORDERED_GET_TUPLE_ARG, namespace_) \
        );                                                                  \
    }

#   define HBOOST_UNORDERED_GET_TUPLE_ARG(z, n, namespace_)                  \
    namespace_ get<n>(x)

#endif

HBOOST_UNORDERED_CONSTRUCT_FROM_TUPLE(10, hboost::)

#if !defined(__SUNPRO_CC) && !defined(HBOOST_NO_CXX11_HDR_TUPLE)
   HBOOST_UNORDERED_CONSTRUCT_FROM_TUPLE(10, std::)
#endif

#undef HBOOST_UNORDERED_CONSTRUCT_FROM_TUPLE
#undef HBOOST_UNORDERED_CONSTRUCT_FROM_TUPLE_IMPL
#undef HBOOST_UNORDERED_GET_TUPLE_ARG

#if defined(__SUNPRO_CC)

    template <typename Alloc, typename T, typename Tuple>
    void construct_from_tuple(Alloc& alloc, T* ptr, Tuple const& x)
    {
        construct_from_tuple_impl(
            hboost::unordered::detail::func::length<
                hboost::tuples::length<Tuple>::value>(),
            alloc, ptr, x);
    }

#endif

    ////////////////////////////////////////////////////////////////////////////
    // Trait to check for piecewise construction.

    template <typename A0>
    struct use_piecewise {
        static choice1::type test(choice1,
            hboost::unordered::piecewise_construct_t);

        static choice2::type test(choice2, ...);

        enum { value = sizeof(choice1::type) ==
            sizeof(test(choose(), hboost::unordered::detail::make<A0>())) };
    };

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

    ////////////////////////////////////////////////////////////////////////////
    // Construct from variadic parameters

    // For the standard pair constructor.

    template <typename Alloc, typename T, typename... Args>
    inline void construct_value_impl(Alloc& alloc, T* address,
        HBOOST_FWD_REF(Args)... args)
    {
        hboost::unordered::detail::func::call_construct(alloc,
            address, hboost::forward<Args>(args)...);
    }

    // Special case for piece_construct
    //
    // TODO: When possible, it might be better to use std::pair's
    // constructor for std::piece_construct with std::tuple.

    template <typename Alloc, typename A, typename B,
        typename A0, typename A1, typename A2>
    inline typename enable_if<use_piecewise<A0>, void>::type
        construct_value_impl(Alloc& alloc, std::pair<A, B>* address,
            HBOOST_FWD_REF(A0), HBOOST_FWD_REF(A1) a1, HBOOST_FWD_REF(A2) a2)
    {
        hboost::unordered::detail::func::construct_from_tuple(alloc,
            hboost::addressof(address->first), hboost::forward<A1>(a1));
        hboost::unordered::detail::func::construct_from_tuple(alloc,
            hboost::addressof(address->second), hboost::forward<A2>(a2));
    }

#else // HBOOST_NO_CXX11_VARIADIC_TEMPLATES

////////////////////////////////////////////////////////////////////////////////
// Construct from emplace_args

    // Explicitly write out first three overloads for the sake of sane
    // error messages.

    template <typename Alloc, typename T, typename A0>
    inline void construct_value_impl(Alloc&, T* address,
            emplace_args1<A0> const& args)
    {
        new((void*) address) T(hboost::forward<A0>(args.a0));
    }

    template <typename Alloc, typename T, typename A0, typename A1>
    inline void construct_value_impl(Alloc&, T* address,
            emplace_args2<A0, A1> const& args)
    {
        new((void*) address) T(
            hboost::forward<A0>(args.a0),
            hboost::forward<A1>(args.a1)
        );
    }

    template <typename Alloc, typename T, typename A0, typename A1, typename A2>
    inline void construct_value_impl(Alloc&, T* address,
            emplace_args3<A0, A1, A2> const& args)
    {
        new((void*) address) T(
            hboost::forward<A0>(args.a0),
            hboost::forward<A1>(args.a1),
            hboost::forward<A2>(args.a2)
        );
    }

    // Use a macro for the rest.

#define HBOOST_UNORDERED_CONSTRUCT_IMPL(z, num_params, _)                    \
    template <                                                              \
        typename Alloc, typename T,                                         \
        HBOOST_PP_ENUM_PARAMS_Z(z, num_params, typename A)                   \
    >                                                                       \
    inline void construct_value_impl(Alloc&, T* address,                    \
        hboost::unordered::detail::HBOOST_PP_CAT(emplace_args,num_params) <   \
            HBOOST_PP_ENUM_PARAMS_Z(z, num_params, A)                        \
        > const& args)                                                      \
    {                                                                       \
        new((void*) address) T(                                             \
            HBOOST_PP_ENUM_##z(num_params, HBOOST_UNORDERED_CALL_FORWARD,     \
                args.a));                                                   \
    }

    HBOOST_PP_REPEAT_FROM_TO(4, HBOOST_UNORDERED_EMPLACE_LIMIT,
        HBOOST_UNORDERED_CONSTRUCT_IMPL, _)

#undef HBOOST_UNORDERED_CONSTRUCT_IMPL

    // Construct with piece_construct

    template <typename Alloc, typename A, typename B,
        typename A0, typename A1, typename A2>
    inline void construct_value_impl(Alloc& alloc, std::pair<A, B>* address,
            hboost::unordered::detail::emplace_args3<A0, A1, A2> const& args,
            typename enable_if<use_piecewise<A0>, void*>::type = 0)
    {
        hboost::unordered::detail::func::construct_from_tuple(alloc,
            hboost::addressof(address->first), args.a1);
        hboost::unordered::detail::func::construct_from_tuple(alloc,
            hboost::addressof(address->second), args.a2);
    }

#endif // HBOOST_NO_CXX11_VARIADIC_TEMPLATES

}}}}

namespace hboost { namespace unordered { namespace detail {

    ////////////////////////////////////////////////////////////////////////////
    //
    // array_constructor
    //
    // Allocate and construct an array in an exception safe manner, and
    // clean up if an exception is thrown before the container takes charge
    // of it.

    template <typename Allocator>
    struct array_constructor
    {
        typedef hboost::unordered::detail::allocator_traits<Allocator> traits;
        typedef typename traits::pointer pointer;

        Allocator& alloc_;
        pointer ptr_;
        pointer constructed_;
        std::size_t length_;

        array_constructor(Allocator& a)
            : alloc_(a), ptr_(), constructed_(), length_(0)
        {
            constructed_ = pointer();
            ptr_ = pointer();
        }

        ~array_constructor() {
            if (ptr_) {
                for(pointer p = ptr_; p != constructed_; ++p) {
                    hboost::unordered::detail::func::destroy(
                            hboost::addressof(*p));
                }

                traits::deallocate(alloc_, ptr_, length_);
            }
        }

        template <typename V>
        void construct(V const& v, std::size_t l)
        {
            HBOOST_ASSERT(!ptr_);
            length_ = l;
            ptr_ = traits::allocate(alloc_, length_);
            pointer end = ptr_ + static_cast<std::ptrdiff_t>(length_);
            for(constructed_ = ptr_; constructed_ != end; ++constructed_) {
                new ((void*) hboost::addressof(*constructed_)) V(v);
            }
        }

        pointer get() const
        {
            return ptr_;
        }

        pointer release()
        {
            pointer p(ptr_);
            ptr_ = pointer();
            return p;
        }

    private:

        array_constructor(array_constructor const&);
        array_constructor& operator=(array_constructor const&);
    };
}}}

#if defined(HBOOST_MSVC)
#pragma warning(pop)
#endif

#endif
