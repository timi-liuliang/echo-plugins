// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_CONCEPT_USAGE_DWA2006919_HPP
# define HBOOST_CONCEPT_USAGE_DWA2006919_HPP

# include <hboost/concept/assert.hpp>
# include <hboost/detail/workaround.hpp>
# include <hboost/concept/detail/backward_compatibility.hpp>

namespace hboost { namespace concepts { 

template <class Model>
struct usage_requirements
{
    ~usage_requirements() { ((Model*)0)->~Model(); }
};

#  if HBOOST_WORKAROUND(__GNUC__, <= 3)

#   define HBOOST_CONCEPT_USAGE(model)                                    \
      model(); /* at least 2.96 and 3.4.3 both need this :( */           \
      HBOOST_CONCEPT_ASSERT((hboost::concepts::usage_requirements<model>)); \
      ~model()

#  else

#   define HBOOST_CONCEPT_USAGE(model)                                    \
      HBOOST_CONCEPT_ASSERT((hboost::concepts::usage_requirements<model>)); \
      ~model()

#  endif

}} // namespace hboost::concepts

#endif // HBOOST_CONCEPT_USAGE_DWA2006919_HPP
