//  add_rvalue_reference.hpp  ---------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef HBOOST_TYPE_TRAITS_EXT_ADD_RVALUE_REFERENCE__HPP
#define HBOOST_TYPE_TRAITS_EXT_ADD_RVALUE_REFERENCE__HPP

#include <hboost/config.hpp>

//----------------------------------------------------------------------------//

#include <hboost/type_traits/is_void.hpp>
#include <hboost/type_traits/is_reference.hpp>

//----------------------------------------------------------------------------//
//                                                                            //
//                           C++03 implementation of                          //
//             20.9.7.2 Reference modifications [meta.trans.ref]              //
//                          Written by Vicente J. Botet Escriba               //
//                                                                            //
// If T names an object or function type then the member typedef type
// shall name T&&; otherwise, type shall name T. [ Note: This rule reflects
// the semantics of reference collapsing. For example, when a type T names
// a type T1&, the type add_rvalue_reference<T>::type is not an rvalue
// reference. -end note ]
//----------------------------------------------------------------------------//

namespace hboost {

namespace type_traits_detail {

    template <typename T, bool b>
    struct add_rvalue_reference_helper
    { typedef T   type; };

#if !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)
    template <typename T>
    struct add_rvalue_reference_helper<T, true>
    {
        typedef T&&   type;
    };
#endif

    template <typename T>
    struct add_rvalue_reference_imp
    {
       typedef typename hboost::type_traits_detail::add_rvalue_reference_helper
                  <T, (is_void<T>::value == false && is_reference<T>::value == false) >::type type;
    };

}

template <class T> struct add_rvalue_reference
{
   typedef typename hboost::type_traits_detail::add_rvalue_reference_imp<T>::type type;
};

}  // namespace hboost

#endif  // HBOOST_TYPE_TRAITS_EXT_ADD_RVALUE_REFERENCE__HPP

