// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_DETAIL_PUSH_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_PUSH_HPP_INCLUDED 

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif                    
 
#include <hboost/config.hpp> // HBOOST_MSVC.
#include <hboost/detail/workaround.hpp>
#include <hboost/iostreams/categories.hpp>
#include <hboost/iostreams/categories.hpp>
#include <hboost/iostreams/detail/adapter/range_adapter.hpp>
#include <hboost/iostreams/detail/config/wide_streams.hpp>
#include <hboost/iostreams/detail/enable_if_stream.hpp>   
#include <hboost/iostreams/pipeline.hpp>   
#include <hboost/iostreams/detail/push_params.hpp>   
#include <hboost/iostreams/detail/resolve.hpp>
#include <hboost/mpl/bool.hpp>   
#include <hboost/preprocessor/cat.hpp> 
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/static_assert.hpp>
#include <hboost/type_traits/is_convertible.hpp>

//
// Macro: HBOOST_IOSTREAMS_DEFINE_PUSH_CONSTRUCTOR(name, mode, ch, helper).
// Description: Defines overloads with name 'name' which forward to a function
//      'helper' which takes a filter or devide by const reference.
//
#define HBOOST_IOSTREAMS_DEFINE_PUSH_CONSTRUCTOR(name, mode, ch, helper) \
    HBOOST_IOSTREAMS_DEFINE_PUSH_IMPL(name, mode, ch, helper, 0, ?) \
    /**/

//
// Macro: HBOOST_IOSTREAMS_DEFINE_PUSH(name, mode, ch, helper).
// Description: Defines constructors which forward to a function
//      'helper' which takes a filter or device by const reference.
//
#define HBOOST_IOSTREAMS_DEFINE_PUSH(name, mode, ch, helper) \
    HBOOST_IOSTREAMS_DEFINE_PUSH_IMPL(name, mode, ch, helper, 1, void) \
    /**/

//--------------------Definition of HBOOST_IOSTREAMS_DEFINE_PUSH_IMPL----------//
          
#define HBOOST_IOSTREAMS_ADAPT_STREAM(mode, ch, arg, helper, has_return) \
    this->helper( ::hboost::iostreams::detail::resolve<mode, ch>(arg) \
                  HBOOST_IOSTREAMS_PUSH_ARGS() ); \
    /**/

#if !HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300) && \
    !HBOOST_WORKAROUND(__BORLANDC__, < 0x600) \
    /**/
# ifndef HBOOST_IOSTREAMS_NO_STREAM_TEMPLATES
#  define HBOOST_IOSTREAMS_DEFINE_PUSH_IMPL(name, mode, ch, helper, has_return, result) \
    template<typename CharType, typename TraitsType> \
    HBOOST_PP_IIF(has_return, result, explicit) \
    name(::std::basic_streambuf<CharType, TraitsType>& sb HBOOST_IOSTREAMS_PUSH_PARAMS()) \
    { HBOOST_IOSTREAMS_ADAPT_STREAM(mode, ch, sb, helper, has_return); } \
    template<typename CharType, typename TraitsType> \
    HBOOST_PP_IIF(has_return, result, explicit) \
    name(::std::basic_istream<CharType, TraitsType>& is HBOOST_IOSTREAMS_PUSH_PARAMS()) \
    { HBOOST_STATIC_ASSERT((!is_convertible<mode, output>::value)); \
      HBOOST_IOSTREAMS_ADAPT_STREAM(mode, ch, is, helper, has_return); } \
    template<typename CharType, typename TraitsType> \
    HBOOST_PP_IIF(has_return, result, explicit) \
    name(::std::basic_ostream<CharType, TraitsType>& os HBOOST_IOSTREAMS_PUSH_PARAMS()) \
    { HBOOST_STATIC_ASSERT((!is_convertible<mode, input>::value)); \
      HBOOST_IOSTREAMS_ADAPT_STREAM(mode, ch, os, helper, has_return); } \
    template<typename CharType, typename TraitsType> \
    HBOOST_PP_IIF(has_return, result, explicit) \
    name(::std::basic_iostream<CharType, TraitsType>& io HBOOST_IOSTREAMS_PUSH_PARAMS()) \
    { HBOOST_IOSTREAMS_ADAPT_STREAM(mode, ch, io, helper, has_return); } \
    template<typename Iter> \
    HBOOST_PP_IIF(has_return, result, explicit) \
    name(const iterator_range<Iter>& rng HBOOST_IOSTREAMS_PUSH_PARAMS()) \
    { HBOOST_PP_EXPR_IF(has_return, return) \
    this->helper( ::hboost::iostreams::detail::range_adapter< \
                      mode, iterator_range<Iter> \
                  >(rng) \
                  HBOOST_IOSTREAMS_PUSH_ARGS() ); } \
    template<typename Pipeline, typename Concept> \
    HBOOST_PP_IIF(has_return, result, explicit) \
    name(const ::hboost::iostreams::pipeline<Pipeline, Concept>& p) \
    { p.push(*this); } \
    template<typename T> \
    HBOOST_PP_IIF(has_return, result, explicit) \
    name(const T& t HBOOST_IOSTREAMS_PUSH_PARAMS() HBOOST_IOSTREAMS_DISABLE_IF_STREAM(T)) \
    { this->helper( ::hboost::iostreams::detail::resolve<mode, ch>(t) \
                    HBOOST_IOSTREAMS_PUSH_ARGS() ); } \
    /**/
# else // # ifndef HBOOST_IOSTREAMS_NO_STREAM_TEMPLATES
#  define HBOOST_IOSTREAMS_DEFINE_PUSH_IMPL(name, mode, ch, helper, has_return, result) \
    HBOOST_PP_IF(has_return, result, explicit) \
    name(::std::streambuf& sb HBOOST_IOSTREAMS_PUSH_PARAMS()) \
    { HBOOST_IOSTREAMS_ADAPT_STREAM(mode, ch, sb, helper, has_return); } \
    HBOOST_PP_IF(has_return, result, explicit) \
    name(::std::istream& is HBOOST_IOSTREAMS_PUSH_PARAMS()) \
    { HBOOST_STATIC_ASSERT((!is_convertible<mode, output>::value)); \
      HBOOST_IOSTREAMS_ADAPT_STREAM(mode, ch, is, helper, has_return); } \
    HBOOST_PP_IF(has_return, result, explicit) \
    name(::std::ostream& os HBOOST_IOSTREAMS_PUSH_PARAMS()) \
    { HBOOST_STATIC_ASSERT((!is_convertible<mode, input>::value)); \
      HBOOST_IOSTREAMS_ADAPT_STREAM(mode, ch, os, helper, has_return); } \
    HBOOST_PP_IF(has_return, result, explicit) \
    name(::std::iostream& io HBOOST_IOSTREAMS_PUSH_PARAMS()) \
    { HBOOST_IOSTREAMS_ADAPT_STREAM(mode, ch, io, helper, has_return); } \
    template<typename Iter> \
    HBOOST_PP_IF(has_return, result, explicit) \
    name(const iterator_range<Iter>& rng HBOOST_IOSTREAMS_PUSH_PARAMS()) \
    { HBOOST_PP_EXPR_IF(has_return, return) \
    this->helper( ::hboost::iostreams::detail::range_adapter< \
                      mode, iterator_range<Iter> \
                  >(rng) \
                  HBOOST_IOSTREAMS_PUSH_ARGS() ); } \
    template<typename Pipeline, typename Concept> \
    HBOOST_PP_IF(has_return, result, explicit) \
    name(const ::hboost::iostreams::pipeline<Pipeline, Concept>& p) \
    { p.push(*this); } \
    template<typename T> \
    HBOOST_PP_EXPR_IF(has_return, result) \
    name(const T& t HBOOST_IOSTREAMS_PUSH_PARAMS() HBOOST_IOSTREAMS_DISABLE_IF_STREAM(T)) \
    { this->helper( ::hboost::iostreams::detail::resolve<mode, ch>(t) \
                    HBOOST_IOSTREAMS_PUSH_ARGS() ); } \
    /**/
# endif // # ifndef HBOOST_IOSTREAMS_NO_STREAM_TEMPLATES
#else // #if VC6, VC7.0, Borland 5.x
# define HBOOST_IOSTREAMS_DEFINE_PUSH_IMPL(name, mode, ch, helper, has_return, result) \
    template<typename T> \
    void HBOOST_PP_CAT(name, _msvc_impl) \
    ( ::hboost::mpl::true_, const T& t HBOOST_IOSTREAMS_PUSH_PARAMS() ) \
    { t.push(*this); } \
    template<typename T> \
    void HBOOST_PP_CAT(name, _msvc_impl) \
    ( ::hboost::mpl::false_, const T& t HBOOST_IOSTREAMS_PUSH_PARAMS() ) \
    { this->helper( ::hboost::iostreams::detail::resolve<mode, ch>(t) \
                    HBOOST_IOSTREAMS_PUSH_ARGS() ); } \
    template<typename T> \
    HBOOST_PP_IF(has_return, result, explicit) \
    name(const T& t HBOOST_IOSTREAMS_PUSH_PARAMS()) \
    { \
        this->HBOOST_PP_CAT(name, _msvc_impl) \
              ( ::hboost::iostreams::detail::is_pipeline<T>(), \
                t HBOOST_IOSTREAMS_PUSH_ARGS() ); \
    } \
    /**/
#endif // #if VC6, VC7.0, Borland 5.x

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_PUSH_HPP_INCLUDED
