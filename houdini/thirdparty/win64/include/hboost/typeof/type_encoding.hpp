// Copyright (C) 2004 Arkadiy Vertleyb
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_TYPE_ENCODING_HPP_INCLUDED
#define HBOOST_TYPEOF_TYPE_ENCODING_HPP_INCLUDED

#define HBOOST_TYPEOF_REGISTER_TYPE_IMPL(T, Id)                          \
                                                                        \
    template<class V> struct encode_type_impl<V, T >                    \
        : hboost::type_of::push_back<V, hboost::mpl::size_t<Id> >         \
    {};                                                                 \
    template<class Iter> struct decode_type_impl<hboost::mpl::size_t<Id>, Iter> \
    {                                                                   \
        typedef T type;                                                 \
        typedef Iter iter;                                              \
    };

#define HBOOST_TYPEOF_REGISTER_TYPE_EXPLICIT_ID(Type, Id)                \
    HBOOST_TYPEOF_BEGIN_ENCODE_NS                                        \
    HBOOST_TYPEOF_REGISTER_TYPE_IMPL(Type, Id)                           \
    HBOOST_TYPEOF_END_ENCODE_NS

#define HBOOST_TYPEOF_REGISTER_TYPE(Type)                                \
    HBOOST_TYPEOF_REGISTER_TYPE_EXPLICIT_ID(Type, HBOOST_TYPEOF_UNIQUE_ID())

#endif//HBOOST_TYPEOF_TYPE_ENCODING_HPP_INCLUDED
