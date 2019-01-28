// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_DETAIL_WRAP_UNWRAP_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_WRAP_UNWRAP_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              

#include <hboost/config.hpp>                             // SFINAE, MSVC.
#include <hboost/detail/workaround.hpp>
#include <hboost/iostreams/detail/enable_if_stream.hpp>
#include <hboost/iostreams/traits_fwd.hpp>               // is_std_io.
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/ref.hpp>

namespace hboost { namespace iostreams { namespace detail {
                    
//------------------Definition of wrap/unwrap traits--------------------------//

template<typename T>
struct wrapped_type 
    : mpl::if_<is_std_io<T>, reference_wrapper<T>, T>
    { };

template<typename T>
struct unwrapped_type 
    : unwrap_reference<T>
    { };

template<typename T>
struct unwrap_ios 
    : mpl::eval_if<
          is_std_io<T>, 
          unwrap_reference<T>, 
          mpl::identity<T>
      >
    { };

//------------------Definition of wrap----------------------------------------//

#ifndef HBOOST_NO_SFINAE //----------------------------------------------------//
    template<typename T>
    inline T wrap(const T& t HBOOST_IOSTREAMS_DISABLE_IF_STREAM(T)) 
    { return t; }

    template<typename T>
    inline typename wrapped_type<T>::type
    wrap(T& t HBOOST_IOSTREAMS_ENABLE_IF_STREAM(T)) { return hboost::ref(t); }
#else // #ifndef HBOOST_NO_SFINAE //-------------------------------------------//
    template<typename T>
    inline typename wrapped_type<T>::type // BCC 5.x needs namespace qualification.
    wrap_impl(const T& t, mpl::true_) { return hboost::ref(const_cast<T&>(t)); }

    template<typename T>
    inline typename wrapped_type<T>::type // BCC 5.x needs namespace qualification.
    wrap_impl(T& t, mpl::true_) { return hboost::ref(t); }

    template<typename T>
    inline typename wrapped_type<T>::type 
    wrap_impl(const T& t, mpl::false_) { return t; }

    template<typename T>
    inline typename wrapped_type<T>::type 
    wrap_impl(T& t, mpl::false_) { return t; }

    template<typename T>
    inline typename wrapped_type<T>::type 
    wrap(const T& t) { return wrap_impl(t, is_std_io<T>()); }

    template<typename T>
    inline typename wrapped_type<T>::type 
    wrap(T& t) { return wrap_impl(t, is_std_io<T>()); }
#endif // #ifndef HBOOST_NO_SFINAE //------------------------------------------//

//------------------Definition of unwrap--------------------------------------//

#if !HBOOST_WORKAROUND(HBOOST_MSVC, < 1310) //----------------------------------//

template<typename T>
typename unwrapped_type<T>::type& 
unwrap(const reference_wrapper<T>& ref) { return ref.get(); }

template<typename T>
typename unwrapped_type<T>::type& unwrap(T& t) { return t; }

template<typename T>
const typename unwrapped_type<T>::type& unwrap(const T& t) { return t; }

#else // #if !HBOOST_WORKAROUND(HBOOST_MSVC, < 1310) //-------------------------//

// Since unwrap is a potential bottleneck, we avoid runtime tag dispatch.
template<bool IsRefWrap>
struct unwrap_impl;

template<>
struct unwrap_impl<true> {
    template<typename T>
    static typename unwrapped_type<T>::type& unwrap(const T& t) 
    { return t.get(); }
};

template<>
struct unwrap_impl<false> {
    template<typename T>
    static typename unwrapped_type<T>::type& unwrap(const T& t) 
    { return const_cast<T&>(t); }
};

template<typename T>
typename unwrapped_type<T>::type& 
unwrap(const T& t) 
{ return unwrap_impl<is_reference_wrapper<T>::value>::unwrap(t); }

#endif // #if !HBOOST_WORKAROUND(HBOOST_MSVC, < 1310) //------------------------//

} } } // End namespaces detail, iostreams, boost.

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_WRAP_UNWRAP_HPP_INCLUDED
