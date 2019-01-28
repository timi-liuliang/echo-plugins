//  static_assert.hpp  --------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef HBOOST_CHRONO_DETAIL_STATIC_ASSERT_HPP
#define HBOOST_CHRONO_DETAIL_STATIC_ASSERT_HPP

#include <hboost/chrono/config.hpp>

#ifndef HBOOST_NO_CXX11_STATIC_ASSERT
#define HBOOST_CHRONO_STATIC_ASSERT(CND, MSG, TYPES) static_assert(CND,MSG)
#elif defined(HBOOST_CHRONO_USES_STATIC_ASSERT)
#include <hboost/static_assert.hpp>
#define HBOOST_CHRONO_STATIC_ASSERT(CND, MSG, TYPES) HBOOST_STATIC_ASSERT(CND)
#elif defined(HBOOST_CHRONO_USES_MPL_ASSERT)
#include <hboost/mpl/assert.hpp>
#include <hboost/mpl/bool.hpp>
#define HBOOST_CHRONO_STATIC_ASSERT(CND, MSG, TYPES)                                 \
    HBOOST_MPL_ASSERT_MSG(hboost::mpl::bool_< (CND) >::type::value, MSG, TYPES)
#else
//~ #elif defined(HBOOST_CHRONO_USES_ARRAY_ASSERT)
#define HBOOST_CHRONO_STATIC_ASSERT(CND, MSG, TYPES) static char HBOOST_JOIN(hboost_chrono_test_,__LINE__)[(CND)?1:-1]
//~ #define HBOOST_CHRONO_STATIC_ASSERT(CND, MSG, TYPES)
#endif

#endif // HBOOST_CHRONO_DETAIL_STATIC_ASSERT_HPP
