//  Copyright 2010 John Maddock

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef HBOOST_TYPE_TRAITS_EXT_ADD_LVALUE_REFERENCE__HPP
#define HBOOST_TYPE_TRAITS_EXT_ADD_LVALUE_REFERENCE__HPP

#include <hboost/type_traits/add_reference.hpp>

namespace hboost{

template <class T> struct add_lvalue_reference
{
   typedef typename hboost::add_reference<T>::type type; 
};

#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
template <class T> struct add_lvalue_reference<T&&>
{
   typedef T& type;
};
#endif

}

#endif  // HBOOST_TYPE_TRAITS_EXT_ADD_LVALUE_REFERENCE__HPP
