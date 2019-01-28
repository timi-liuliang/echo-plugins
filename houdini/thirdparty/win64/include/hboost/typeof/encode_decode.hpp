// Copyright (C) 2004 Arkadiy Vertleyb

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// boostinspect:nounnamed

#ifndef HBOOST_TYPEOF_ENCODE_DECODE_HPP_INCLUDED
#define HBOOST_TYPEOF_ENCODE_DECODE_HPP_INCLUDED

#include <hboost/mpl/deref.hpp>
#include <hboost/mpl/next.hpp>

#ifndef HBOOST_TYPEOF_SUPPRESS_UNNAMED_NAMESPACE

#   define HBOOST_TYPEOF_BEGIN_ENCODE_NS namespace { namespace hboost_typeof {
#   define HBOOST_TYPEOF_END_ENCODE_NS }}
#   define HBOOST_TYPEOF_ENCODE_NS_QUALIFIER hboost_typeof

#else

#   define HBOOST_TYPEOF_BEGIN_ENCODE_NS namespace hboost { namespace type_of {
#   define HBOOST_TYPEOF_END_ENCODE_NS }}
#   define HBOOST_TYPEOF_ENCODE_NS_QUALIFIER hboost::type_of

#   define HBOOST_TYPEOF_TEXT "unnamed namespace is off"
#   include <hboost/typeof/message.hpp>

#endif

HBOOST_TYPEOF_BEGIN_ENCODE_NS

template<class V, class Type_Not_Registered_With_Typeof_System>
struct encode_type_impl;

template<class T, class Iter>
struct decode_type_impl
{
    typedef int type;  // MSVC ETI workaround
};

template<class T>
struct decode_nested_template_helper_impl;

HBOOST_TYPEOF_END_ENCODE_NS

namespace hboost { namespace type_of {

    template<class V, class T>
    struct encode_type : HBOOST_TYPEOF_ENCODE_NS_QUALIFIER::encode_type_impl<V, T>
    {};

    template<class Iter>
    struct decode_type : HBOOST_TYPEOF_ENCODE_NS_QUALIFIER::decode_type_impl<
        typename Iter::type,
        typename Iter::next
    >
    {};
}}

#endif//HBOOST_TYPEOF_ENCODE_DECODE_HPP_INCLUDED
