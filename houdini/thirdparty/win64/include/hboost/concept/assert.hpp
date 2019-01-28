// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_CONCEPT_ASSERT_DWA2006430_HPP
# define HBOOST_CONCEPT_ASSERT_DWA2006430_HPP

# include <hboost/config.hpp>
# include <hboost/detail/workaround.hpp>

// The old protocol used a constraints() member function in concept
// checking classes.  If the compiler supports SFINAE, we can detect
// that function and seamlessly support the old concept checking
// classes.  In this release, backward compatibility with the old
// concept checking classes is enabled by default, where available.
// The old protocol is deprecated, though, and backward compatibility
// will no longer be the default in the next release.

# if !defined(HBOOST_NO_OLD_CONCEPT_SUPPORT)                                         \
    && !defined(HBOOST_NO_SFINAE)                                                    \
                                                                                    \
    && !(HBOOST_WORKAROUND(__GNUC__, == 3) && HBOOST_WORKAROUND(__GNUC_MINOR__, < 4))

// Note: gcc-2.96 through 3.3.x have some SFINAE, but no ability to
// check for the presence of particularmember functions.

#  define HBOOST_OLD_CONCEPT_SUPPORT

# endif

# ifdef HBOOST_MSVC
#  include <hboost/concept/detail/msvc.hpp>
# elif HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
#  include <hboost/concept/detail/borland.hpp>
# else 
#  include <hboost/concept/detail/general.hpp>
# endif

  // Usage, in class or function context:
  //
  //     HBOOST_CONCEPT_ASSERT((UnaryFunctionConcept<F,bool,int>));
  //
# define HBOOST_CONCEPT_ASSERT(ModelInParens) \
    HBOOST_CONCEPT_ASSERT_FN(void(*)ModelInParens)

#endif // HBOOST_CONCEPT_ASSERT_DWA2006430_HPP
