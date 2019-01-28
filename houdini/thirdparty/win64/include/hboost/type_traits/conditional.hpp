//  (C) Copyright John Maddock 2010.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef HBOOST_TT_CONDITIONAL_HPP_INCLUDED
#define HBOOST_TT_CONDITIONAL_HPP_INCLUDED

namespace hboost {

template <bool b, class T, class U> struct conditional { typedef T type; };
template <class T, class U> struct conditional<false, T, U> { typedef U type; };

} // namespace hboost


#endif // HBOOST_TT_CONDITIONAL_HPP_INCLUDED