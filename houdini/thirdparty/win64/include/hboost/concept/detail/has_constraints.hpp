// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_CONCEPT_DETAIL_HAS_CONSTRAINTS_DWA2006429_HPP
# define HBOOST_CONCEPT_DETAIL_HAS_CONSTRAINTS_DWA2006429_HPP

# include <hboost/mpl/bool.hpp>
# include <hboost/detail/workaround.hpp>
# include <hboost/concept/detail/backward_compatibility.hpp>

namespace hboost { namespace concepts {

namespace detail
{ 

// Here we implement the metafunction that detects whether a
// constraints metafunction exists
  typedef char yes;
  typedef char (&no)[2];

  template <class Model, void (Model::*)()>
  struct wrap_constraints {};
    
#if HBOOST_WORKAROUND(__SUNPRO_CC, <= 0x580) || defined(__CUDACC__)
  // Work around the following bogus error in Sun Studio 11, by
  // turning off the has_constraints function entirely:
  //    Error: complex expression not allowed in dependent template
  //    argument expression
  inline no has_constraints_(...);
#else
  template <class Model>
  inline yes has_constraints_(Model*, wrap_constraints<Model,&Model::constraints>* = 0);
  inline no has_constraints_(...);
#endif
}

// This would be called "detail::has_constraints," but it has a strong
// tendency to show up in error messages.
template <class Model>
struct not_satisfied
{
    HBOOST_STATIC_CONSTANT(
        bool
      , value = sizeof( detail::has_constraints_((Model*)0) ) == sizeof(detail::yes) );
    typedef mpl::bool_<value> type;
};

}} // namespace hboost::concepts::detail

#endif // HBOOST_CONCEPT_DETAIL_HAS_CONSTRAINTS_DWA2006429_HPP
