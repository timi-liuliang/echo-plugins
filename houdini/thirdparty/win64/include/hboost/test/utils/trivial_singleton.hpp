//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : simple helpers for creating cusom output manipulators
// ***************************************************************************

#ifndef HBOOST_TEST_UTILS_TRIVIAL_SIGNLETON_HPP
#define HBOOST_TEST_UTILS_TRIVIAL_SIGNLETON_HPP

// Boost.Test
#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>

// Boost
#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {

// ************************************************************************** //
// **************                   singleton                  ************** //
// ************************************************************************** //

template<typename Derived>
class singleton {
public:
    static Derived& instance() { static Derived the_inst; return the_inst; }

    HBOOST_DELETED_FUNCTION(singleton(singleton const&))
    HBOOST_DELETED_FUNCTION(singleton& operator=(singleton const&))

protected:
    HBOOST_DEFAULTED_FUNCTION(singleton(), {})
    HBOOST_DEFAULTED_FUNCTION(~singleton(), {})
};

//____________________________________________________________________________//

#define HBOOST_TEST_SINGLETON_CONS( type )       \
friend class hboost::unit_test::singleton<type>; \
type() {}                                       \
/**/

#if HBOOST_WORKAROUND(__DECCXX_VER, HBOOST_TESTED_AT(60590042))

#define HBOOST_TEST_SINGLETON_INST( inst ) \
template class unit_test::singleton< HBOOST_JOIN( inst, _t ) > ; \
namespace { HBOOST_JOIN( inst, _t)& inst = HBOOST_JOIN( inst, _t)::instance(); }

#elif defined(__APPLE_CC__) && defined(__GNUC__) && __GNUC__ < 4
#define HBOOST_TEST_SINGLETON_INST( inst ) \
static HBOOST_JOIN( inst, _t)& inst = HBOOST_JOIN (inst, _t)::instance();

#else

#define HBOOST_TEST_SINGLETON_INST( inst ) \
namespace { HBOOST_JOIN( inst, _t)& inst = HBOOST_JOIN( inst, _t)::instance(); }

#endif

//____________________________________________________________________________//

} // namespace unit_test
} // namespace hboost


#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_UTILS_TRIVIAL_SIGNLETON_HPP
