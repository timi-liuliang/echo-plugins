#ifndef HBOOST_CORE_LIGHTWEIGHT_TEST_HPP
#define HBOOST_CORE_LIGHTWEIGHT_TEST_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER)
# pragma once
#endif

//
//  hboost/core/lightweight_test.hpp - lightweight test library
//
//  Copyright (c) 2002, 2009, 2014 Peter Dimov
//  Copyright (2) Beman Dawes 2010, 2011
//  Copyright (3) Ion Gaztanaga 2013
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//

#include <hboost/assert.hpp>
#include <hboost/current_function.hpp>
#include <hboost/core/no_exceptions_support.hpp>
#include <iostream>

//  IDE's like Visual Studio perform better if output goes to std::cout or
//  some other stream, so allow user to configure output stream:
#ifndef HBOOST_LIGHTWEIGHT_TEST_OSTREAM
# define HBOOST_LIGHTWEIGHT_TEST_OSTREAM std::cerr
#endif

namespace hboost
{

namespace detail
{

struct report_errors_reminder
{
    bool called_report_errors_function;

    report_errors_reminder() : called_report_errors_function(false) {}

    ~report_errors_reminder()
    {
        HBOOST_ASSERT(called_report_errors_function);  // verify report_errors() was called  
    }
};

inline report_errors_reminder& report_errors_remind()
{
    static report_errors_reminder r;
    return r;
}

inline int & test_errors()
{
    static int x = 0;
    report_errors_remind();
    return x;
}

inline void test_failed_impl(char const * expr, char const * file, int line, char const * function)
{
    HBOOST_LIGHTWEIGHT_TEST_OSTREAM
      << file << "(" << line << "): test '" << expr << "' failed in function '"
      << function << "'" << std::endl;
    ++test_errors();
}

inline void error_impl(char const * msg, char const * file, int line, char const * function)
{
    HBOOST_LIGHTWEIGHT_TEST_OSTREAM
      << file << "(" << line << "): " << msg << " in function '"
      << function << "'" << std::endl;
    ++test_errors();
}

inline void throw_failed_impl(char const * excep, char const * file, int line, char const * function)
{
   HBOOST_LIGHTWEIGHT_TEST_OSTREAM
    << file << "(" << line << "): Exception '" << excep << "' not thrown in function '"
    << function << "'" << std::endl;
   ++test_errors();
}

template<class T, class U> inline void test_eq_impl( char const * expr1, char const * expr2,
  char const * file, int line, char const * function, T const & t, U const & u )
{
    if( t == u )
    {
        report_errors_remind();
    }
    else
    {
        HBOOST_LIGHTWEIGHT_TEST_OSTREAM
            << file << "(" << line << "): test '" << expr1 << " == " << expr2
            << "' failed in function '" << function << "': "
            << "'" << t << "' != '" << u << "'" << std::endl;
        ++test_errors();
    }
}

template<class T, class U> inline void test_ne_impl( char const * expr1, char const * expr2,
  char const * file, int line, char const * function, T const & t, U const & u )
{
    if( t != u )
    {
        report_errors_remind();
    }
    else
    {
        HBOOST_LIGHTWEIGHT_TEST_OSTREAM
            << file << "(" << line << "): test '" << expr1 << " != " << expr2
            << "' failed in function '" << function << "': "
            << "'" << t << "' == '" << u << "'" << std::endl;
        ++test_errors();
    }
}

} // namespace detail

inline int report_errors()
{
    hboost::detail::report_errors_remind().called_report_errors_function = true;

    int errors = hboost::detail::test_errors();

    if( errors == 0 )
    {
        HBOOST_LIGHTWEIGHT_TEST_OSTREAM
          << "No errors detected." << std::endl;
        return 0;
    }
    else
    {
        HBOOST_LIGHTWEIGHT_TEST_OSTREAM
          << errors << " error" << (errors == 1? "": "s") << " detected." << std::endl;
        return 1;
    }
}

} // namespace hboost

#define HBOOST_TEST(expr) ((expr)? (void)0: ::hboost::detail::test_failed_impl(#expr, __FILE__, __LINE__, HBOOST_CURRENT_FUNCTION))

#define HBOOST_ERROR(msg) ( ::hboost::detail::error_impl(msg, __FILE__, __LINE__, HBOOST_CURRENT_FUNCTION) )

#define HBOOST_TEST_EQ(expr1,expr2) ( ::hboost::detail::test_eq_impl(#expr1, #expr2, __FILE__, __LINE__, HBOOST_CURRENT_FUNCTION, expr1, expr2) )
#define HBOOST_TEST_NE(expr1,expr2) ( ::hboost::detail::test_ne_impl(#expr1, #expr2, __FILE__, __LINE__, HBOOST_CURRENT_FUNCTION, expr1, expr2) )

#ifndef HBOOST_NO_EXCEPTIONS
   #define HBOOST_TEST_THROWS( EXPR, EXCEP )                    \
      try {                                                    \
         EXPR;                                                 \
         ::hboost::detail::throw_failed_impl                    \
         (#EXCEP, __FILE__, __LINE__, HBOOST_CURRENT_FUNCTION); \
      }                                                        \
      catch(EXCEP const&) {                                    \
      }                                                        \
      catch(...) {                                             \
         ::hboost::detail::throw_failed_impl                    \
         (#EXCEP, __FILE__, __LINE__, HBOOST_CURRENT_FUNCTION); \
      }                                                        \
   //
#else
   #define HBOOST_TEST_THROWS( EXPR, EXCEP )
#endif

#endif // #ifndef HBOOST_CORE_LIGHTWEIGHT_TEST_HPP
