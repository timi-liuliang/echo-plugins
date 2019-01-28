/*
 *
 * Copyright (c) 2003
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */
 
 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         regex_traits.hpp
  *   VERSION      see <hboost/version.hpp>
  *   DESCRIPTION: Declares regular expression traits classes.
  */

#ifndef HBOOST_REGEX_TRAITS_HPP_INCLUDED
#define HBOOST_REGEX_TRAITS_HPP_INCLUDED

#ifndef HBOOST_REGEX_CONFIG_HPP
#include <hboost/regex/config.hpp>
#endif
#ifndef HBOOST_REGEX_WORKAROUND_HPP
#include <hboost/regex/v4/regex_workaround.hpp>
#endif
#ifndef HBOOST_REGEX_SYNTAX_TYPE_HPP
#include <hboost/regex/v4/syntax_type.hpp>
#endif
#ifndef HBOOST_REGEX_ERROR_TYPE_HPP
#include <hboost/regex/v4/error_type.hpp>
#endif
#ifndef HBOOST_REGEX_TRAITS_DEFAULTS_HPP_INCLUDED
#include <hboost/regex/v4/regex_traits_defaults.hpp>
#endif
#ifndef HBOOST_NO_STD_LOCALE
#  ifndef HBOOST_CPP_REGEX_TRAITS_HPP_INCLUDED
#     include <hboost/regex/v4/cpp_regex_traits.hpp>
#  endif
#endif
#if !HBOOST_WORKAROUND(__BORLANDC__, < 0x560)
#  ifndef HBOOST_C_REGEX_TRAITS_HPP_INCLUDED
#     include <hboost/regex/v4/c_regex_traits.hpp>
#  endif
#endif
#if defined(_WIN32) && !defined(HBOOST_REGEX_NO_W32)
#  ifndef HBOOST_W32_REGEX_TRAITS_HPP_INCLUDED
#     include <hboost/regex/v4/w32_regex_traits.hpp>
#  endif
#endif
#ifndef HBOOST_REGEX_FWD_HPP_INCLUDED
#include <hboost/regex_fwd.hpp>
#endif

#include "hboost/mpl/has_xxx.hpp"
#include <hboost/static_assert.hpp>

#ifdef HBOOST_MSVC
#pragma warning(push)
#pragma warning(disable: 4103)
#endif
#ifdef HBOOST_HAS_ABI_HEADERS
#  include HBOOST_ABI_PREFIX
#endif
#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

namespace hboost{

template <class charT, class implementationT >
struct regex_traits : public implementationT
{
   regex_traits() : implementationT() {}
};

//
// class regex_traits_wrapper.
// this is what our implementation will actually store;
// it provides default implementations of the "optional"
// interfaces that we support, in addition to the
// required "standard" ones:
//
namespace HBOOST_REGEX_DETAIL_NS{
#if !HBOOST_WORKAROUND(__HP_aCC, < 60000)
HBOOST_MPL_HAS_XXX_TRAIT_DEF(hboost_extensions_tag)
#else
template<class T>
struct has_hboost_extensions_tag
{
   HBOOST_STATIC_CONSTANT(bool, value = false);
};
#endif

template <class BaseT>
struct default_wrapper : public BaseT
{
   typedef typename BaseT::char_type char_type;
   std::string error_string(::hboost::regex_constants::error_type e)const
   {
      return ::hboost::HBOOST_REGEX_DETAIL_NS::get_default_error_string(e);
   }
   ::hboost::regex_constants::syntax_type syntax_type(char_type c)const
   {
      return ((c & 0x7f) == c) ? get_default_syntax_type(static_cast<char>(c)) : ::hboost::regex_constants::syntax_char;
   }
   ::hboost::regex_constants::escape_syntax_type escape_syntax_type(char_type c)const
   {
      return ((c & 0x7f) == c) ? get_default_escape_syntax_type(static_cast<char>(c)) : ::hboost::regex_constants::escape_type_identity;
   }
   int toi(const char_type*& p1, const char_type* p2, int radix)const
   {
      return ::hboost::HBOOST_REGEX_DETAIL_NS::global_toi(p1, p2, radix, *this);
   }
   char_type translate(char_type c, bool icase)const
   {
      return (icase ? this->translate_nocase(c) : this->translate(c));
   }
   char_type translate(char_type c)const
   {
      return BaseT::translate(c);
   }
   char_type tolower(char_type c)const
   {
      return ::hboost::HBOOST_REGEX_DETAIL_NS::global_lower(c);
   }
   char_type toupper(char_type c)const
   {
      return ::hboost::HBOOST_REGEX_DETAIL_NS::global_upper(c);
   }
};

template <class BaseT, bool has_extensions>
struct compute_wrapper_base
{
   typedef BaseT type;
};
#if !HBOOST_WORKAROUND(__HP_aCC, < 60000)
template <class BaseT>
struct compute_wrapper_base<BaseT, false>
{
   typedef default_wrapper<BaseT> type;
};
#else
template <>
struct compute_wrapper_base<c_regex_traits<char>, false>
{
   typedef default_wrapper<c_regex_traits<char> > type;
};
#ifndef HBOOST_NO_WREGEX
template <>
struct compute_wrapper_base<c_regex_traits<wchar_t>, false>
{
   typedef default_wrapper<c_regex_traits<wchar_t> > type;
};
#endif
#endif

} // namespace HBOOST_REGEX_DETAIL_NS

template <class BaseT>
struct regex_traits_wrapper 
   : public ::hboost::HBOOST_REGEX_DETAIL_NS::compute_wrapper_base<
               BaseT, 
               ::hboost::HBOOST_REGEX_DETAIL_NS::has_hboost_extensions_tag<BaseT>::value
            >::type
{
   regex_traits_wrapper(){}
private:
   regex_traits_wrapper(const regex_traits_wrapper&);
   regex_traits_wrapper& operator=(const regex_traits_wrapper&);
};

} // namespace hboost

#ifdef HBOOST_MSVC
#pragma warning(push)
#pragma warning(disable: 4103)
#endif
#ifdef HBOOST_HAS_ABI_HEADERS
#  include HBOOST_ABI_SUFFIX
#endif
#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#endif // include

