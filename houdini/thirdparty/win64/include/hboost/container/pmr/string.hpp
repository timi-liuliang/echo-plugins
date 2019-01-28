//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2015-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_PMR_STRING_HPP
#define HBOOST_CONTAINER_PMR_STRING_HPP

#if defined (_MSC_VER)
#  pragma once 
#endif

#include <hboost/container/string.hpp>
#include <hboost/container/pmr/polymorphic_allocator.hpp>

namespace hboost {
namespace container {
namespace pmr {

#if !defined(HBOOST_NO_CXX11_TEMPLATE_ALIASES)

template <class CharT, class Traits = std::char_traits<CharT>  >
using basic_string =
   hboost::container::basic_string<CharT, Traits, polymorphic_allocator<CharT> >;

#endif

template <class CharT, class Traits = std::char_traits<CharT> >
struct basic_string_of
{
   typedef hboost::container::basic_string
      <CharT, Traits, polymorphic_allocator<CharT> > type;
};

typedef basic_string_of<char>::type    string;

typedef basic_string_of<wchar_t>::type wstring;

}  //namespace pmr {
}  //namespace container {
}  //namespace hboost {

#endif   //HBOOST_CONTAINER_PMR_STRING_HPP
