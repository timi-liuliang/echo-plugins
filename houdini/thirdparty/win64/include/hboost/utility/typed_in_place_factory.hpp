// Copyright (C) 2003, Fernando Luis Cacciola Carballal.
// Copyright (C) 2007, Tobias Schwinger.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/optional for documentation.
//
// You are welcome to contact the author at:
//  fernando_cacciola@hotmail.com
//
#ifndef HBOOST_UTILITY_TYPED_INPLACE_FACTORY_04APR2007_HPP
#ifndef HBOOST_PP_IS_ITERATING

#include <hboost/utility/detail/in_place_factory_prefix.hpp>

namespace hboost {

class typed_in_place_factory_base {} ;

#define  HBOOST_PP_ITERATION_LIMITS (0, HBOOST_MAX_INPLACE_FACTORY_ARITY)
#define  HBOOST_PP_FILENAME_1 <hboost/utility/typed_in_place_factory.hpp>
#include HBOOST_PP_ITERATE()

} // namespace hboost

#include <hboost/utility/detail/in_place_factory_suffix.hpp>

#define HBOOST_UTILITY_TYPED_INPLACE_FACTORY_04APR2007_HPP
#else 
#define N HBOOST_PP_ITERATION()

template< class T HBOOST_PP_ENUM_TRAILING_PARAMS(N,class A) >
class HBOOST_PP_CAT(typed_in_place_factory,N) 
  : 
  public typed_in_place_factory_base
{
public:

  typedef T value_type;

  explicit HBOOST_PP_CAT(typed_in_place_factory,N) 
      ( HBOOST_PP_ENUM_BINARY_PARAMS(N, A, const& a) )
#if N > 0
    : HBOOST_PP_ENUM(N, HBOOST_DEFINE_INPLACE_FACTORY_CLASS_MEMBER_INIT, _)
#endif
  {}

  void* apply (void* address) const
  {
    return new(address) T( HBOOST_PP_ENUM_PARAMS(N, m_a) );
  }

  void* apply (void* address, std::size_t n) const
  {
    for(void* next = address = this->apply(address); !! --n;)
      this->apply(next = static_cast<char *>(next) + sizeof(T));
    return address; 
  }

  HBOOST_PP_REPEAT(N, HBOOST_DEFINE_INPLACE_FACTORY_CLASS_MEMBER_DECL, _)
};

template< class T HBOOST_PP_ENUM_TRAILING_PARAMS(N, class A) >
inline HBOOST_PP_CAT(typed_in_place_factory,N)<
    T HBOOST_PP_ENUM_TRAILING_PARAMS(N, A) >
in_place( HBOOST_PP_ENUM_BINARY_PARAMS(N, A, const& a) )
{
  return HBOOST_PP_CAT(typed_in_place_factory,N)< 
      T HBOOST_PP_ENUM_TRAILING_PARAMS(N, A) >( HBOOST_PP_ENUM_PARAMS(N, a) );
}

#undef N
#endif
#endif

