
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef HBOOST_FT_DETAIL_TO_SEQUENCE_HPP_INCLUDED
#define HBOOST_FT_DETAIL_TO_SEQUENCE_HPP_INCLUDED

#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/is_sequence.hpp>
#include <hboost/mpl/placeholders.hpp>
#include <hboost/type_traits/add_reference.hpp>

#include <hboost/function_types/is_callable_builtin.hpp>

namespace hboost { namespace function_types { namespace detail {

// wrap first arguments in components, if callable builtin type
template<typename T>
struct to_sequence
{
  typedef typename
   mpl::eval_if
   < is_callable_builtin<T>
   , to_sequence< components<T> >
   , mpl::identity< T >
   >::type
  type;
};

// reduce template instantiations, if possible
template<typename T, typename U>
struct to_sequence< components<T,U> > 
{
  typedef typename components<T,U>::types type;
};

} } } // namespace ::hboost::function_types::detail

#endif

