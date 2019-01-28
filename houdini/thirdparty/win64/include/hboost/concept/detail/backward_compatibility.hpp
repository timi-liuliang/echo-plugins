// Copyright David Abrahams 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_CONCEPT_BACKWARD_COMPATIBILITY_DWA200968_HPP
# define HBOOST_CONCEPT_BACKWARD_COMPATIBILITY_DWA200968_HPP

namespace hboost
{
  namespace concepts {}

# if defined(HBOOST_HAS_CONCEPTS) && !defined(HBOOST_CONCEPT_NO_BACKWARD_KEYWORD)
  namespace concept = concepts;
# endif 
} // namespace hboost::concept

#endif // HBOOST_CONCEPT_BACKWARD_COMPATIBILITY_DWA200968_HPP
