
//  (C) John Maddock 2010. 
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_RVALUE_REFERENCE_HPP_INCLUDED
#define HBOOST_TT_IS_RVALUE_REFERENCE_HPP_INCLUDED

#include <hboost/config.hpp>
#include <hboost/type_traits/integral_constant.hpp>

namespace hboost {

template <class T> struct is_rvalue_reference : public false_type {};
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
template <class T> struct is_rvalue_reference<T&&> : public true_type {};
#endif

} // namespace hboost

#endif // HBOOST_TT_IS_REFERENCE_HPP_INCLUDED

