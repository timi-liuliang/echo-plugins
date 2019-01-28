// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_CONCEPT_DETAIL_BORLAND_DWA2006429_HPP
# define HBOOST_CONCEPT_DETAIL_BORLAND_DWA2006429_HPP

# include <hboost/preprocessor/cat.hpp>
# include <hboost/concept/detail/backward_compatibility.hpp>

namespace hboost { namespace concepts {

template <class ModelFnPtr>
struct require;

template <class Model>
struct require<void(*)(Model)>
{
    enum { instantiate = sizeof((((Model*)0)->~Model()), 3) };
};

#  define HBOOST_CONCEPT_ASSERT_FN( ModelFnPtr )         \
  enum                                                  \
  {                                                     \
      HBOOST_PP_CAT(hboost_concept_check,__LINE__) =      \
      hboost::concepts::require<ModelFnPtr>::instantiate  \
  }

}} // namespace hboost::concept

#endif // HBOOST_CONCEPT_DETAIL_BORLAND_DWA2006429_HPP
