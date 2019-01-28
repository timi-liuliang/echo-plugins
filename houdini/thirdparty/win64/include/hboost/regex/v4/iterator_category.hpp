/*
 *
 * Copyright (c) 2002
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         regex_match.hpp
  *   VERSION      see <hboost/version.hpp>
  *   DESCRIPTION: Iterator traits for selecting an iterator type as
  *                an integral constant expression.
  */


#ifndef HBOOST_REGEX_ITERATOR_CATEGORY_HPP
#define HBOOST_REGEX_ITERATOR_CATEGORY_HPP

#include <iterator>
#include <hboost/type_traits/is_convertible.hpp>
#include <hboost/type_traits/is_pointer.hpp>

namespace hboost{
namespace detail{

template <class I>
struct is_random_imp
{
#ifndef HBOOST_NO_STD_ITERATOR_TRAITS
private:
   typedef typename std::iterator_traits<I>::iterator_category cat;
public:
   HBOOST_STATIC_CONSTANT(bool, value = (::hboost::is_convertible<cat*, std::random_access_iterator_tag*>::value));
#else
   HBOOST_STATIC_CONSTANT(bool, value = false);
#endif
};

template <class I>
struct is_random_pointer_imp
{
   HBOOST_STATIC_CONSTANT(bool, value = true);
};

template <bool is_pointer_type>
struct is_random_imp_selector
{
   template <class I>
   struct rebind
   {
      typedef is_random_imp<I> type;
   };
};

template <>
struct is_random_imp_selector<true>
{
   template <class I>
   struct rebind
   {
      typedef is_random_pointer_imp<I> type;
   };
};

}

template <class I>
struct is_random_access_iterator
{
private:
   typedef detail::is_random_imp_selector< ::hboost::is_pointer<I>::value> selector;
   typedef typename selector::template rebind<I> bound_type;
   typedef typename bound_type::type answer;
public:
   HBOOST_STATIC_CONSTANT(bool, value = answer::value);
};

#ifndef HBOOST_NO_INCLASS_MEMBER_INITIALIZATION
template <class I>
const bool is_random_access_iterator<I>::value;
#endif

}

#endif

