// Copyright (C) 2004 Arkadiy Vertleyb
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_MODIFIERS_HPP_INCLUDED
#define HBOOST_TYPEOF_MODIFIERS_HPP_INCLUDED

#include <hboost/typeof/encode_decode.hpp>
#include <hboost/preprocessor/facilities/identity.hpp>

#include HBOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

// modifiers

#define HBOOST_TYPEOF_modifier_support(ID, Fun)\
    template<class V, class T> struct encode_type_impl<V, Fun(T)>\
    {\
        typedef\
            typename hboost::type_of::encode_type<\
            typename hboost::type_of::push_back<\
            V\
            , hboost::mpl::size_t<ID> >::type\
            , T>::type\
            type;\
    };\
    template<class Iter> struct decode_type_impl<hboost::mpl::size_t<ID>, Iter>\
    {\
        typedef hboost::type_of::decode_type<Iter> d1;\
        typedef Fun(typename d1::type) type;\
        typedef typename d1::iter iter;\
    }


#define HBOOST_TYPEOF_const_fun(T) const T
#define HBOOST_TYPEOF_volatile_fun(T) volatile T
#define HBOOST_TYPEOF_volatile_const_fun(T) volatile const T
#define HBOOST_TYPEOF_pointer_fun(T) T*
#define HBOOST_TYPEOF_reference_fun(T) T&

#if defined(__BORLANDC__) && (__BORLANDC__ < 0x600)
//Borland incorrectly handles T const, T const volatile and T volatile.
//It drops the decoration no matter what, so we need to try to handle T* const etc. without loosing the top modifier.
#define HBOOST_TYPEOF_const_pointer_fun(T) T const *
#define HBOOST_TYPEOF_const_reference_fun(T) T const &
#define HBOOST_TYPEOF_volatile_pointer_fun(T) T volatile*
#define HBOOST_TYPEOF_volatile_reference_fun(T) T volatile&
#define HBOOST_TYPEOF_volatile_const_pointer_fun(T) T volatile const *
#define HBOOST_TYPEOF_volatile_const_reference_fun(T) T volatile const &
#endif

HBOOST_TYPEOF_BEGIN_ENCODE_NS

HBOOST_TYPEOF_modifier_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_TYPEOF_const_fun);
HBOOST_TYPEOF_modifier_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_TYPEOF_volatile_fun);
HBOOST_TYPEOF_modifier_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_TYPEOF_volatile_const_fun);
HBOOST_TYPEOF_modifier_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_TYPEOF_pointer_fun);
HBOOST_TYPEOF_modifier_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_TYPEOF_reference_fun);

#if defined(__BORLANDC__) && (__BORLANDC__ < 0x600)
HBOOST_TYPEOF_modifier_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_TYPEOF_const_pointer_fun);
HBOOST_TYPEOF_modifier_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_TYPEOF_const_reference_fun);
HBOOST_TYPEOF_modifier_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_TYPEOF_volatile_pointer_fun);
HBOOST_TYPEOF_modifier_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_TYPEOF_volatile_reference_fun);
HBOOST_TYPEOF_modifier_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_TYPEOF_volatile_const_pointer_fun);
HBOOST_TYPEOF_modifier_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_TYPEOF_volatile_const_reference_fun);
#endif

HBOOST_TYPEOF_END_ENCODE_NS

#undef HBOOST_TYPEOF_modifier_support
#undef HBOOST_TYPEOF_const_fun
#undef HBOOST_TYPEOF_volatile_fun
#undef HBOOST_TYPEOF_volatile_const_fun
#undef HBOOST_TYPEOF_pointer_fun
#undef HBOOST_TYPEOF_reference_fun

#if defined(__BORLANDC__) && (__BORLANDC__ < 0x600)
#undef HBOOST_TYPEOF_const_pointer_fun
#undef HBOOST_TYPEOF_const_reference_fun
#undef HBOOST_TYPEOF_volatile_pointer_fun
#undef HBOOST_TYPEOF_volatile_reference_fun
#undef HBOOST_TYPEOF_volatile_const_pointer_fun
#undef HBOOST_TYPEOF_volatile_const_reference_fun
#endif

// arrays

#define HBOOST_TYPEOF_array_support(ID, Qualifier)\
    template<class V, class T, int N>\
    struct encode_type_impl<V, Qualifier() T[N]>\
    {\
        typedef\
            typename hboost::type_of::encode_type<\
            typename hboost::type_of::push_back<\
            typename hboost::type_of::push_back<\
            V\
            , hboost::mpl::size_t<ID> >::type\
            , hboost::mpl::size_t<N> >::type\
            , T>::type\
        type;\
    };\
    template<class Iter>\
    struct decode_type_impl<hboost::mpl::size_t<ID>, Iter>\
    {\
        enum{n = Iter::type::value};\
        typedef hboost::type_of::decode_type<typename Iter::next> d;\
        typedef typename d::type Qualifier() type[n];\
        typedef typename d::iter iter;\
    }

HBOOST_TYPEOF_BEGIN_ENCODE_NS

HBOOST_TYPEOF_array_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_PP_EMPTY);
HBOOST_TYPEOF_array_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_PP_IDENTITY(const));
HBOOST_TYPEOF_array_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_PP_IDENTITY(volatile));
HBOOST_TYPEOF_array_support(HBOOST_TYPEOF_UNIQUE_ID(), HBOOST_PP_IDENTITY(volatile const));
HBOOST_TYPEOF_END_ENCODE_NS

#undef HBOOST_TYPEOF_array_support

#endif//HBOOST_TYPEOF_MODIFIERS_HPP_INCLUDED
