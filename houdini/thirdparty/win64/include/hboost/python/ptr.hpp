#ifndef HBOOST_PTR_DWA20020601_HPP
# define HBOOST_PTR_DWA20020601_HPP

# include <hboost/python/detail/prefix.hpp>
// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Based on hboost/ref.hpp, thus:
//  Copyright (C) 1999, 2000 Jaakko Jarvi (jaakko.jarvi@cs.utu.fi)
//  Copyright (C) 2001 Peter Dimov

# pragma once

# include <hboost/config.hpp>
# include <hboost/mpl/bool.hpp>

namespace hboost { namespace python {

template<class Ptr> class pointer_wrapper
{ 
 public:
    typedef Ptr type;
    
    explicit pointer_wrapper(Ptr x): p_(x) {}
    operator Ptr() const { return p_; }
    Ptr get() const { return p_; }
 private:
    Ptr p_;
};

template<class T>
inline pointer_wrapper<T> ptr(T t)
{ 
    return pointer_wrapper<T>(t);
}

template<typename T>
class is_pointer_wrapper
    : public mpl::false_
{
};

template<typename T>
class is_pointer_wrapper<pointer_wrapper<T> >
    : public mpl::true_
{
};

template<typename T>
class unwrap_pointer
{
 public:
    typedef T type;
};

template<typename T>
class unwrap_pointer<pointer_wrapper<T> >
{
 public:
    typedef T type;
};

}} // namespace hboost::python

#endif // #ifndef HBOOST_PTR_DWA20020601_HPP