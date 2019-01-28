// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2005-2007 Jonathan Turkanis
// (C) Copyright David Abrahams 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_DETAIL_IS_DEREFERENCEABLE_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_IS_DEREFERENCEABLE_HPP_INCLUDED

# include <hboost/type_traits/remove_cv.hpp>
# include <hboost/mpl/aux_/lambda_support.hpp>
# include <hboost/mpl/bool.hpp>
# include <hboost/detail/workaround.hpp>

namespace hboost { namespace iostreams { namespace detail { 

// is_dereferenceable<T> metafunction
//
// Requires: Given x of type T&, if the expression *x is well-formed
// it must have complete type; otherwise, it must neither be ambiguous
// nor violate access.

// This namespace ensures that ADL doesn't mess things up.
namespace is_dereferenceable_
{
  // a type returned from operator* when no increment is found in the
  // type's own namespace
  struct tag {};
  
  // any soaks up implicit conversions and makes the following
  // operator* less-preferred than any other such operator that
  // might be found via ADL.
  struct any { template <class T> any(T const&); };

  // This is a last-resort operator* for when none other is found
  tag operator*(any const&);

# if HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3202)) \
    || HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300)
#  define HBOOST_comma(a,b) (a)
# else 
  // In case an operator++ is found that returns void, we'll use ++x,0
  tag operator,(tag,int);  
#  define HBOOST_comma(a,b) (a,b)
# endif 
  
  // two check overloads help us identify which operator++ was picked
  char (& check_increment(tag) )[2];
  
  template <class T>
  char check_increment(T const&);
  
  template <class T>
  struct impl
  {
      static typename hboost::remove_cv<T>::type& x;

      HBOOST_STATIC_CONSTANT(
          bool
        , value = sizeof(is_dereferenceable_::check_increment(HBOOST_comma(*x,0))) == 1
      );
  };
}

# undef HBOOST_comma

template<typename T> 
struct is_dereferenceable 
    : public ::hboost::integral_constant<bool, is_dereferenceable_::impl<T>::value >
{ 
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_dereferenceable,(T))
};

} } 


} // End namespaces detail, iostreams, boost.

#endif // HBOOST_IOSTREAMS_DETAIL_IS_DEREFERENCEABLE_HPP_INCLUDED
