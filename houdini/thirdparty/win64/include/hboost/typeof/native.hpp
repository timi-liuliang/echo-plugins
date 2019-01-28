// Copyright (C) 2006 Arkadiy Vertleyb
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_NATIVE_HPP_INCLUDED
#define HBOOST_TYPEOF_NATIVE_HPP_INCLUDED

#ifndef MSVC_TYPEOF_HACK

#ifdef HBOOST_NO_SFINAE

namespace hboost { namespace type_of {

    template<class T> 
        T& ensure_obj(const T&);

}}

#else

#include <hboost/type_traits/is_function.hpp> 
#include <hboost/utility/enable_if.hpp>

namespace hboost { namespace type_of {
# ifdef HBOOST_NO_SFINAE
    template<class T> 
    T& ensure_obj(const T&);
# else
    template<typename T>
        typename enable_if<is_function<T>, T&>::type
        ensure_obj(T&);

    template<typename T>
        typename disable_if<is_function<T>, T&>::type
        ensure_obj(const T&);
# endif
}}

#endif//HBOOST_NO_SFINAE

#define HBOOST_TYPEOF(expr) HBOOST_TYPEOF_KEYWORD(hboost::type_of::ensure_obj(expr))
#define HBOOST_TYPEOF_TPL HBOOST_TYPEOF

#define HBOOST_TYPEOF_NESTED_TYPEDEF_TPL(name,expr) \
    struct name {\
        typedef HBOOST_TYPEOF_TPL(expr) type;\
    };

#define HBOOST_TYPEOF_NESTED_TYPEDEF(name,expr) \
    struct name {\
        typedef HBOOST_TYPEOF(expr) type;\
    };

#endif//MSVC_TYPEOF_HACK

#define HBOOST_TYPEOF_REGISTER_TYPE(x)
#define HBOOST_TYPEOF_REGISTER_TEMPLATE(x, params)

#endif//HBOOST_TYPEOF_NATIVE_HPP_INCLUDED

