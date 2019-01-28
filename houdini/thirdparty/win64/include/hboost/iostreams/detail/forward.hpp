// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_DETAIL_FORWARD_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_FORWARD_HPP_INCLUDED   

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif                  
 
#include <hboost/config.hpp> // HBOOST_MSVC, HBOOST_NO_SFINAE
#include <hboost/detail/workaround.hpp>
#include <hboost/iostreams/detail/config/limits.hpp>
#include <hboost/iostreams/detail/push_params.hpp>
#include <hboost/preprocessor/arithmetic/dec.hpp>
#include <hboost/preprocessor/arithmetic/inc.hpp>
#include <hboost/preprocessor/punctuation/comma_if.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/repeat_from_to.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/type_traits/is_same.hpp>

//------Macros for defining forwarding constructors and open overloads--------//
    
//
// Macro: HBOOST_IOSTREAMS_FORWARD(class, impl, device, params, args)
// Description: Defines constructors and overloads of 'open' which construct
//      a device using the specified argument list and pass it to the specified
//      helper function
//   class - The class name
//   impl - The helper function
//   device - The device type
//   params - The list of formal parameters trailing the device parameter in
//     the helper function's signature
//   params - The list of arguments passed to the helper function, following the
//     device argument
//
#define HBOOST_IOSTREAMS_FORWARD(class, impl, device, params, args) \
    class(const device& t params()) \
    { this->impl(::hboost::iostreams::detail::wrap(t) args()); } \
    class(device& t params()) \
    { this->impl(::hboost::iostreams::detail::wrap(t) args()); } \
    class(const ::hboost::reference_wrapper<device>& ref params()) \
    { this->impl(ref args()); } \
    void open(const device& t params()) \
    { this->impl(::hboost::iostreams::detail::wrap(t) args()); } \
    void open(device& t params()) \
    { this->impl(::hboost::iostreams::detail::wrap(t) args()); } \
    void open(const ::hboost::reference_wrapper<device>& ref params()) \
    { this->impl(ref args()); } \
    HBOOST_PP_REPEAT_FROM_TO( \
        1, HBOOST_PP_INC(HBOOST_IOSTREAMS_MAX_FORWARDING_ARITY), \
        HBOOST_IOSTREAMS_FORWARDING_CTOR, (class, impl, device) \
    ) \
    HBOOST_PP_REPEAT_FROM_TO( \
        1, HBOOST_PP_INC(HBOOST_IOSTREAMS_MAX_FORWARDING_ARITY), \
        HBOOST_IOSTREAMS_FORWARDING_FN, (class, impl, device) \
    ) \
    /**/
#if !HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
# define HBOOST_IOSTREAMS_FORWARDING_CTOR_I(z, n, tuple) \
    template< typename U100 HBOOST_PP_COMMA_IF(HBOOST_PP_DEC(n)) \
              HBOOST_PP_ENUM_PARAMS_Z(z, HBOOST_PP_DEC(n), typename U) > \
    HBOOST_PP_TUPLE_ELEM(3, 0, tuple) \
    ( U100& u100 HBOOST_PP_COMMA_IF(HBOOST_PP_DEC(n)) \
      HBOOST_PP_ENUM_BINARY_PARAMS_Z(z, HBOOST_PP_DEC(n), const U, &u) \
      HBOOST_IOSTREAMS_DISABLE_IF_SAME(U100, HBOOST_PP_TUPLE_ELEM(3, 2, tuple))) \
    { this->HBOOST_PP_TUPLE_ELEM(3, 1, tuple) \
      ( HBOOST_PP_TUPLE_ELEM(3, 2, tuple) \
        ( u100 HBOOST_PP_COMMA_IF(HBOOST_PP_DEC(n)) \
          HBOOST_PP_ENUM_PARAMS_Z(z, HBOOST_PP_DEC(n), u)) ); } \
    /**/
# define HBOOST_IOSTREAMS_FORWARDING_FN_I(z, n, tuple) \
    template< typename U100 HBOOST_PP_COMMA_IF(HBOOST_PP_DEC(n)) \
              HBOOST_PP_ENUM_PARAMS_Z(z, HBOOST_PP_DEC(n), typename U) > \
    void open \
    ( U100& u100 HBOOST_PP_COMMA_IF(HBOOST_PP_DEC(n)) \
      HBOOST_PP_ENUM_BINARY_PARAMS_Z(z, HBOOST_PP_DEC(n), const U, &u) \
      HBOOST_IOSTREAMS_DISABLE_IF_SAME(U100, HBOOST_PP_TUPLE_ELEM(3, 2, tuple))) \
    { this->HBOOST_PP_TUPLE_ELEM(3, 1, tuple) \
      ( u100 HBOOST_PP_COMMA_IF(HBOOST_PP_DEC(n)) \
        HBOOST_PP_ENUM_PARAMS_Z(z, HBOOST_PP_DEC(n), u) ); } \
    /**/
#else
# define HBOOST_IOSTREAMS_FORWARDING_CTOR_I(z, n, tuple)
# define HBOOST_IOSTREAMS_FORWARDING_FN_I(z, n, tuple)
#endif
#define HBOOST_IOSTREAMS_FORWARDING_CTOR(z, n, tuple) \
    template<HBOOST_PP_ENUM_PARAMS_Z(z, n, typename U)> \
    HBOOST_PP_TUPLE_ELEM(3, 0, tuple) \
    (HBOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, const U, &u) \
      HBOOST_IOSTREAMS_DISABLE_IF_SAME(U0, HBOOST_PP_TUPLE_ELEM(3, 2, tuple))) \
    { this->HBOOST_PP_TUPLE_ELEM(3, 1, tuple) \
      ( HBOOST_PP_TUPLE_ELEM(3, 2, tuple) \
        (HBOOST_PP_ENUM_PARAMS_Z(z, n, u)) ); } \
    HBOOST_IOSTREAMS_FORWARDING_CTOR_I(z, n, tuple) \
    /**/
#define HBOOST_IOSTREAMS_FORWARDING_FN(z, n, tuple) \
    template<HBOOST_PP_ENUM_PARAMS_Z(z, n, typename U)> \
    void open(HBOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, const U, &u) \
      HBOOST_IOSTREAMS_DISABLE_IF_SAME(U0, HBOOST_PP_TUPLE_ELEM(3, 2, tuple))) \
    { this->HBOOST_PP_TUPLE_ELEM(3, 1, tuple) \
      ( HBOOST_PP_TUPLE_ELEM(3, 2, tuple) \
        (HBOOST_PP_ENUM_PARAMS_Z(z, n, u)) ); } \
    HBOOST_IOSTREAMS_FORWARDING_FN_I(z, n, tuple) \
    /**/

// Disable forwarding constructors if first parameter type is the same
// as the device type
#if !defined(HBOOST_NO_SFINAE) && \
    !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x592))
# define HBOOST_IOSTREAMS_DISABLE_IF_SAME(device, param) \
    , typename hboost::disable_if< hboost::is_same<device, param> >::type* = 0 \
    /**/
#else 
# define HBOOST_IOSTREAMS_DISABLE_IF_SAME(device, param)
#endif

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_FORWARD_HPP_INCLUDED
