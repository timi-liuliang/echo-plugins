//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//!@file
//!@brief contains wrappers, which allows to build Boost.Test with no exception
// ***************************************************************************

#ifndef HBOOST_TEST_DETAIL_THROW_EXCEPTION_HPP
#define HBOOST_TEST_DETAIL_THROW_EXCEPTION_HPP

// Boost
#include <hboost/config.hpp> // HBOOST_NO_EXCEPTION

#ifdef HBOOST_NO_EXCEPTION
// C RUNTIME
#include <stdlib.h>

#endif

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {
namespace ut_detail {

#ifdef HBOOST_NO_EXCEPTIONS

template<typename E>
HBOOST_NORETURN inline void
throw_exception(E const& e) { abort(); }

#define HBOOST_TEST_I_TRY
#define HBOOST_TEST_I_CATCH( T, var ) for(T const& var = *(T*)0; false;)
#define HBOOST_TEST_I_CATCH0( T ) if(0)
#define HBOOST_TEST_I_CATCHALL() if(0)
#define HBOOST_TEST_I_RETHROW

#else

template<typename E>
HBOOST_NORETURN inline void
throw_exception(E const& e) { throw e; }

#define HBOOST_TEST_I_TRY try
#define HBOOST_TEST_I_CATCH( T, var ) catch( T const& var )
#define HBOOST_TEST_I_CATCH0( T ) catch( T const& )
#define HBOOST_TEST_I_CATCHALL() catch(...)
#define HBOOST_TEST_I_RETHROW throw
#endif

//____________________________________________________________________________//

#define HBOOST_TEST_I_THROW( E ) unit_test::ut_detail::throw_exception( E )
#define HBOOST_TEST_I_THROW( E ) unit_test::ut_detail::throw_exception( E )
#define HBOOST_TEST_I_ASSRT( cond, ex ) if( cond ) {} else HBOOST_TEST_I_THROW( ex )


} // namespace ut_detail
} // namespace unit_test
} // namespace hboost

//____________________________________________________________________________//

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_DETAIL_THROW_EXCEPTION_HPP
