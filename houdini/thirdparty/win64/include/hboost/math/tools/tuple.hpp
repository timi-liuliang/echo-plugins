//  (C) Copyright John Maddock 2010.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_MATH_TUPLE_HPP_INCLUDED
#  define HBOOST_MATH_TUPLE_HPP_INCLUDED
#  include <hboost/config.hpp>

#ifndef HBOOST_NO_CXX11_HDR_TUPLE

#include <tuple>

namespace hboost{ namespace math{

using ::std::tuple;

// [6.1.3.2] Tuple creation functions
using ::std::ignore;
using ::std::make_tuple;
using ::std::tie;
using ::std::get;

// [6.1.3.3] Tuple helper classes
using ::std::tuple_size;
using ::std::tuple_element;

}}

#elif (defined(__BORLANDC__) && (__BORLANDC__ <= 0x600)) || defined(__IBMCPP__)

#include <hboost/tuple/tuple.hpp>
#include <hboost/tuple/tuple_comparison.hpp>
#include <hboost/type_traits/integral_constant.hpp>

namespace hboost{ namespace math{

using ::hboost::tuple;

// [6.1.3.2] Tuple creation functions
using ::hboost::tuples::ignore;
using ::hboost::make_tuple;
using ::hboost::tie;

// [6.1.3.3] Tuple helper classes
template <class T> 
struct tuple_size 
   : public ::hboost::integral_constant
   < ::std::size_t, ::hboost::tuples::length<T>::value>
{};

template < int I, class T>
struct tuple_element
{
   typedef typename hboost::tuples::element<I,T>::type type;
};

#if !HBOOST_WORKAROUND(__BORLANDC__, < 0x0582)
// [6.1.3.4] Element access
using ::hboost::get;
#endif

} } // namespaces

#else

#include <hboost/fusion/include/tuple.hpp>
#include <hboost/fusion/include/std_pair.hpp>

namespace hboost{ namespace math{

using ::hboost::fusion::tuple;

// [6.1.3.2] Tuple creation functions
using ::hboost::fusion::ignore;
using ::hboost::fusion::make_tuple;
using ::hboost::fusion::tie;
using ::hboost::fusion::get;

// [6.1.3.3] Tuple helper classes
using ::hboost::fusion::tuple_size;
using ::hboost::fusion::tuple_element;

}}

#endif

#endif


