//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision: 81247 $
//
//  Description : contains definition for all test tools in test toolbox
// ***************************************************************************

#ifndef HBOOST_TEST_TOOLS_INTERFACE_HPP_111712GER
#define HBOOST_TEST_TOOLS_INTERFACE_HPP_111712GER

// Boost.Test
#include <hboost/test/unit_test_log.hpp>
#ifdef HBOOST_TEST_TOOLS_DEBUGGABLE
#include <hboost/test/debug.hpp>
#endif
#ifdef HBOOST_NO_CXX11_AUTO_DECLARATIONS
#include <hboost/test/tools/detail/expression_holder.hpp>
#endif

#include <hboost/test/detail/pp_variadic.hpp>

#ifdef HBOOST_TEST_NO_OLD_TOOLS
#include <hboost/preprocessor/seq/to_tuple.hpp>

#include <iterator>
#endif // HBOOST_TEST_NO_OLD_TOOLS

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

// ************************************************************************** //
// **************               HBOOST_TEST_<level>             ************** //
// ************************************************************************** //

#define HBOOST_TEST_BUILD_ASSERTION( P )             \
    (::hboost::test_tools::assertion::seed()->*P)    \
/**/

//____________________________________________________________________________//

// Implementation based on direct predicate evaluation
#define HBOOST_TEST_TOOL_DIRECT_IMPL( P, level, M )              \
do {                                                            \
    ::hboost::test_tools::assertion_result res = (P);            \
    report_assertion(                                           \
        res,                                                    \
        HBOOST_TEST_LAZY_MSG( M ),                               \
        HBOOST_TEST_L(__FILE__),                                 \
        static_cast<std::size_t>(__LINE__),                     \
        ::hboost::test_tools::tt_detail::level,                  \
        ::hboost::test_tools::tt_detail::CHECK_MSG,              \
        0 );                                                    \
} while( ::hboost::test_tools::tt_detail::dummy_cond() )         \
/**/

//____________________________________________________________________________//

// Implementation based on expression template construction
#define HBOOST_TEST_TOOL_ET_IMPL( P, level )                     \
do {                                                            \
    HBOOST_TEST_PASSPOINT();                                     \
                                                                \
    ::hboost::test_tools::tt_detail::                            \
    report_assertion(                                           \
      HBOOST_TEST_BUILD_ASSERTION( P ).evaluate(),               \
      HBOOST_TEST_LAZY_MSG( HBOOST_TEST_STRINGIZE( P ) ),         \
      HBOOST_TEST_L(__FILE__),                                   \
      static_cast<std::size_t>(__LINE__),                       \
      ::hboost::test_tools::tt_detail::level,                    \
      ::hboost::test_tools::tt_detail::CHECK_BUILT_ASSERTION,    \
      0 );                                                      \
} while( ::hboost::test_tools::tt_detail::dummy_cond() )         \
/**/

//____________________________________________________________________________//

// Implementation based on expression template construction with extra tool arguments
#define HBOOST_TEST_TOOL_ET_IMPL_EX( P, level, arg )             \
do {                                                            \
    HBOOST_TEST_PASSPOINT();                                     \
                                                                \
    ::hboost::test_tools::tt_detail::                            \
    report_assertion(                                           \
      ::hboost::test_tools::tt_detail::assertion_evaluate(       \
        HBOOST_TEST_BUILD_ASSERTION( P ) )                       \
          << arg,                                               \
      ::hboost::test_tools::tt_detail::assertion_text(           \
          HBOOST_TEST_LAZY_MSG( HBOOST_TEST_STRINGIZE(P) ),       \
          HBOOST_TEST_LAZY_MSG( arg ) ),                         \
      HBOOST_TEST_L(__FILE__),                                   \
      static_cast<std::size_t>(__LINE__),                       \
      ::hboost::test_tools::tt_detail::level,                    \
      ::hboost::test_tools::tt_detail::assertion_type()          \
          << arg,                                               \
      0 );                                                      \
} while( ::hboost::test_tools::tt_detail::dummy_cond() )         \
/**/

//____________________________________________________________________________//

#ifdef HBOOST_TEST_TOOLS_UNDER_DEBUGGER

#define HBOOST_TEST_TOOL_UNIV( level, P )                                    \
    HBOOST_TEST_TOOL_DIRECT_IMPL( P, level, HBOOST_TEST_STRINGIZE( P ) )      \
/**/

#define HBOOST_TEST_TOOL_UNIV_EX( level, P, ... )                            \
    HBOOST_TEST_TOOL_UNIV( level, P )                                        \
/**/

#elif defined(HBOOST_TEST_TOOLS_DEBUGGABLE)

#define HBOOST_TEST_TOOL_UNIV( level, P )                                    \
do {                                                                        \
    if( ::hboost::debug::under_debugger() )                                  \
        HBOOST_TEST_TOOL_DIRECT_IMPL( P, level, HBOOST_TEST_STRINGIZE( P ) ); \
    else                                                                    \
        HBOOST_TEST_TOOL_ET_IMPL( P, level );                                \
} while( ::hboost::test_tools::tt_detail::dummy_cond() )                     \
/**/

#define HBOOST_TEST_TOOL_UNIV_EX( level, P, ... )                            \
    HBOOST_TEST_TOOL_UNIV( level, P )                                        \
/**/

#else

#define HBOOST_TEST_TOOL_UNIV( level, P )                                    \
    HBOOST_TEST_TOOL_ET_IMPL( P, level )                                     \
/**/

#define HBOOST_TEST_TOOL_UNIV_EX( level, P, ... )                            \
    HBOOST_TEST_TOOL_ET_IMPL_EX( P, level, __VA_ARGS__ )                     \
/**/

#endif

//____________________________________________________________________________//

#define HBOOST_TEST_WARN( ... )              HBOOST_TEST_INVOKE_IF_N_ARGS(    \
    2, HBOOST_TEST_TOOL_UNIV, HBOOST_TEST_TOOL_UNIV_EX, WARN, __VA_ARGS__ )   \
/**/
#define HBOOST_TEST_CHECK( ... )             HBOOST_TEST_INVOKE_IF_N_ARGS(    \
    2, HBOOST_TEST_TOOL_UNIV, HBOOST_TEST_TOOL_UNIV_EX, CHECK, __VA_ARGS__ )  \
/**/
#define HBOOST_TEST_REQUIRE( ... )           HBOOST_TEST_INVOKE_IF_N_ARGS(    \
    2, HBOOST_TEST_TOOL_UNIV, HBOOST_TEST_TOOL_UNIV_EX, REQUIRE, __VA_ARGS__ )\
/**/

#define HBOOST_TEST( ... )                   HBOOST_TEST_INVOKE_IF_N_ARGS(    \
    2, HBOOST_TEST_TOOL_UNIV, HBOOST_TEST_TOOL_UNIV_EX, CHECK, __VA_ARGS__ )  \
/**/

//____________________________________________________________________________//

#define HBOOST_TEST_ERROR( M )               HBOOST_CHECK_MESSAGE( false, M )
#define HBOOST_TEST_FAIL( M )                HBOOST_REQUIRE_MESSAGE( false, M )

//____________________________________________________________________________//

#define HBOOST_TEST_IS_DEFINED( symb ) ::hboost::test_tools::tt_detail::is_defined_impl( symb, HBOOST_STRINGIZE(= symb) )

//____________________________________________________________________________//

#ifdef HBOOST_TEST_NO_OLD_TOOLS

#ifdef HBOOST_TEST_TOOLS_UNDER_DEBUGGER

#define HBOOST_CHECK_THROW_IMPL(S, E, TL, Ppassed, Mpassed, Pcaught, Mcaught)\
do { try {                                                                  \
    S;                                                                      \
    HBOOST_TEST_TOOL_DIRECT_IMPL( Ppassed, TL, Mpassed );                    \
} catch( E ) {                                                              \
    HBOOST_TEST_TOOL_DIRECT_IMPL( Pcaught, TL, Mcaught );                    \
}} while( ::hboost::test_tools::tt_detail::dummy_cond() )                    \
/**/

#elif defined(HBOOST_TEST_TOOLS_DEBUGGABLE)

#define HBOOST_CHECK_THROW_IMPL(S, E, TL, Ppassed, Mpassed, Pcaught, Mcaught)\
do { try {                                                                  \
    if( ::hboost::debug::under_debugger() )                                  \
        HBOOST_TEST_PASSPOINT();                                             \
    S;                                                                      \
    HBOOST_TEST_TOOL_DIRECT_IMPL( Ppassed, TL, Mpassed );                    \
} catch( E ) {                                                              \
    HBOOST_TEST_TOOL_DIRECT_IMPL( Pcaught, TL, Mcaught );                    \
}} while( ::hboost::test_tools::tt_detail::dummy_cond() )                    \
/**/

#else

#define HBOOST_CHECK_THROW_IMPL(S, E, TL, Ppassed, Mpassed, Pcaught, Mcaught)\
do { try {                                                                  \
    HBOOST_TEST_PASSPOINT();                                                 \
    S;                                                                      \
    HBOOST_TEST_TOOL_DIRECT_IMPL( Ppassed, TL, Mpassed );                    \
} catch( E ) {                                                              \
    HBOOST_TEST_TOOL_DIRECT_IMPL( Pcaught, TL, Mcaught );                    \
}} while( ::hboost::test_tools::tt_detail::dummy_cond() )                    \
/**/

#endif

//____________________________________________________________________________//

#define HBOOST_WARN_THROW( S, E )                                            \
    HBOOST_CHECK_THROW_IMPL(S, E const&, WARN,                               \
            false, "exception " HBOOST_STRINGIZE(E) " is expected",          \
            true , "exception " HBOOST_STRINGIZE(E) " is caught" )           \
/**/
#define HBOOST_CHECK_THROW( S, E )                                           \
    HBOOST_CHECK_THROW_IMPL(S, E const&, CHECK,                              \
            false, "exception " HBOOST_STRINGIZE(E) " is expected",          \
            true , "exception " HBOOST_STRINGIZE(E) " is caught" )           \
/**/
#define HBOOST_REQUIRE_THROW( S, E )                                         \
    HBOOST_CHECK_THROW_IMPL(S, E const&, REQUIRE,                            \
            false, "exception " HBOOST_STRINGIZE(E) " is expected",          \
            true , "exception " HBOOST_STRINGIZE(E) " is caught" )           \
/**/

//____________________________________________________________________________//

#define HBOOST_WARN_EXCEPTION( S, E, P )                                     \
    HBOOST_CHECK_THROW_IMPL(S, E const& ex, WARN,                            \
            false, "exception " HBOOST_STRINGIZE(E) " is expected",          \
            P(ex), "incorrect exception " HBOOST_STRINGIZE(E) " is caught" ) \
/**/
#define HBOOST_CHECK_EXCEPTION( S, E, P )                                    \
    HBOOST_CHECK_THROW_IMPL(S, E const& ex, CHECK,                           \
            false, "exception " HBOOST_STRINGIZE(E) " is expected",          \
            P(ex), "incorrect exception " HBOOST_STRINGIZE(E) " is caught" ) \
/**/
#define HBOOST_REQUIRE_EXCEPTION( S, E, P )                                  \
    HBOOST_CHECK_THROW_IMPL(S, E const& ex, REQUIRE,                         \
            false, "exception " HBOOST_STRINGIZE(E) " is expected",          \
            P(ex), "incorrect exception " HBOOST_STRINGIZE(E) " is caught" ) \
/**/

//____________________________________________________________________________//

#define HBOOST_WARN_NO_THROW( S )                                            \
    HBOOST_CHECK_THROW_IMPL(S, ..., WARN,                                    \
            true , "no exceptions thrown by " HBOOST_STRINGIZE( S ),         \
            false, "exception thrown by " HBOOST_STRINGIZE( S ) )            \
/**/
#define HBOOST_CHECK_NO_THROW( S )                                           \
    HBOOST_CHECK_THROW_IMPL(S, ..., CHECK,                                   \
            true , "no exceptions thrown by " HBOOST_STRINGIZE( S ),         \
            false, "exception thrown by " HBOOST_STRINGIZE( S ) )            \
/**/
#define HBOOST_REQUIRE_NO_THROW( S )                                         \
    HBOOST_CHECK_THROW_IMPL(S, ..., REQUIRE,                                 \
            true , "no exceptions thrown by " HBOOST_STRINGIZE( S ),         \
            false, "exception thrown by " HBOOST_STRINGIZE( S ) )            \
/**/

//____________________________________________________________________________//

#define HBOOST_WARN_MESSAGE( P, M )          HBOOST_TEST_TOOL_DIRECT_IMPL( P, WARN, M )
#define HBOOST_CHECK_MESSAGE( P, M )         HBOOST_TEST_TOOL_DIRECT_IMPL( P, CHECK, M )
#define HBOOST_REQUIRE_MESSAGE( P, M )       HBOOST_TEST_TOOL_DIRECT_IMPL( P, REQUIRE, M )

//____________________________________________________________________________//

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////   DEPRECATED TOOLS   /////////////////////////////

#define HBOOST_WARN( P )                     HBOOST_TEST_WARN( P )
#define HBOOST_CHECK( P )                    HBOOST_TEST_CHECK( P )
#define HBOOST_REQUIRE( P )                  HBOOST_TEST_REQUIRE( P )

//____________________________________________________________________________//

#define HBOOST_ERROR( M )                    HBOOST_TEST_ERROR( M )
#define HBOOST_FAIL( M )                     HBOOST_TEST_FAIL( M )

//____________________________________________________________________________//

#define HBOOST_WARN_EQUAL( L, R )            HBOOST_TEST_WARN( L == R )
#define HBOOST_CHECK_EQUAL( L, R )           HBOOST_TEST_CHECK( L == R )
#define HBOOST_REQUIRE_EQUAL( L, R )         HBOOST_TEST_REQUIRE( L == R )

#define HBOOST_WARN_NE( L, R )               HBOOST_TEST_WARN( L != R )
#define HBOOST_CHECK_NE( L, R )              HBOOST_TEST_CHECK( L != R )
#define HBOOST_REQUIRE_NE( L, R )            HBOOST_TEST_REQUIRE( L != R )

#define HBOOST_WARN_LT( L, R )               HBOOST_TEST_WARN( L < R )
#define HBOOST_CHECK_LT( L, R )              HBOOST_TEST_CHECK( L < R )
#define HBOOST_REQUIRE_LT( L, R )            HBOOST_TEST_REQUIRE( L < R )

#define HBOOST_WARN_LE( L, R )               HBOOST_TEST_WARN( L <= R )
#define HBOOST_CHECK_LE( L, R )              HBOOST_TEST_CHECK( L <= R )
#define HBOOST_REQUIRE_LE( L, R )            HBOOST_TEST_REQUIRE( L <= R )

#define HBOOST_WARN_GT( L, R )               HBOOST_TEST_WARN( L > R )
#define HBOOST_CHECK_GT( L, R )              HBOOST_TEST_CHECK( L > R )
#define HBOOST_REQUIRE_GT( L, R )            HBOOST_TEST_REQUIRE( L > R )

#define HBOOST_WARN_GE( L, R )               HBOOST_TEST_WARN( L >= R )
#define HBOOST_CHECK_GE( L, R )              HBOOST_TEST_CHECK( L >= R )
#define HBOOST_REQUIRE_GE( L, R )            HBOOST_TEST_REQUIRE( L >= R )

//____________________________________________________________________________//

#define HBOOST_WARN_CLOSE( L, R, T )         HBOOST_TEST_WARN( L == R, T % ::hboost::test_tools::tolerance() )
#define HBOOST_CHECK_CLOSE( L, R, T )        HBOOST_TEST_CHECK( L == R, T % ::hboost::test_tools::tolerance() )
#define HBOOST_REQUIRE_CLOSE( L, R, T )      HBOOST_TEST_REQUIRE( L == R, T % ::hboost::test_tools::tolerance() )

#define HBOOST_WARN_CLOSE_FRACTION(L, R, T)  HBOOST_TEST_WARN( L == R, ::hboost::test_tools::tolerance( T ) )
#define HBOOST_CHECK_CLOSE_FRACTION(L, R, T) HBOOST_TEST_CHECK( L == R, ::hboost::test_tools::tolerance( T ) )
#define HBOOST_REQUIRE_CLOSE_FRACTION(L,R,T) HBOOST_TEST_REQUIRE( L == R, ::hboost::test_tools::tolerance( T ) )

#define HBOOST_WARN_SMALL( FPV, T )          HBOOST_TEST_WARN( FPV == 0., ::hboost::test_tools::tolerance( T ) )
#define HBOOST_CHECK_SMALL( FPV, T )         HBOOST_TEST_CHECK( FPV == 0., ::hboost::test_tools::tolerance( T ) )
#define HBOOST_REQUIRE_SMALL( FPV, T )       HBOOST_TEST_REQUIRE( FPV == 0., ::hboost::test_tools::tolerance( T ) )

//____________________________________________________________________________//

#define HBOOST_WARN_EQUAL_COLLECTIONS( L_begin, L_end, R_begin, R_end )              \
    HBOOST_TEST_WARN( ::hboost::test_tools::tt_detail::make_it_pair(L_begin, L_end) ==\
                     ::hboost::test_tools::tt_detail::make_it_pair(R_begin, R_end),  \
                     ::hboost::test_tools::per_element() )                           \
/**/

#define HBOOST_CHECK_EQUAL_COLLECTIONS( L_begin, L_end, R_begin, R_end )              \
    HBOOST_TEST_CHECK( ::hboost::test_tools::tt_detail::make_it_pair(L_begin, L_end) ==\
                      ::hboost::test_tools::tt_detail::make_it_pair(R_begin, R_end),  \
                      ::hboost::test_tools::per_element() )                           \
/**/

#define HBOOST_REQUIRE_EQUAL_COLLECTIONS( L_begin, L_end, R_begin, R_end )              \
    HBOOST_TEST_REQUIRE( ::hboost::test_tools::tt_detail::make_it_pair(L_begin, L_end) ==\
                        ::hboost::test_tools::tt_detail::make_it_pair(R_begin, R_end),  \
                        ::hboost::test_tools::per_element() )                           \
/**/

//____________________________________________________________________________//

#define HBOOST_WARN_BITWISE_EQUAL( L, R )    HBOOST_TEST_WARN( L == R, ::hboost::test_tools::bitwise() )
#define HBOOST_CHECK_BITWISE_EQUAL( L, R )   HBOOST_TEST_CHECK( L == R, ::hboost::test_tools::bitwise() )
#define HBOOST_REQUIRE_BITWISE_EQUAL( L, R ) HBOOST_TEST_REQUIRE( L == R, ::hboost::test_tools::bitwise() )

//____________________________________________________________________________//

#define HBOOST_WARN_PREDICATE( P, ARGS )     HBOOST_TEST_WARN( P HBOOST_PP_SEQ_TO_TUPLE(ARGS) )
#define HBOOST_CHECK_PREDICATE( P, ARGS )    HBOOST_TEST_CHECK( P HBOOST_PP_SEQ_TO_TUPLE(ARGS) )
#define HBOOST_REQUIRE_PREDICATE( P, ARGS )  HBOOST_TEST_REQUIRE( P HBOOST_PP_SEQ_TO_TUPLE(ARGS) )

//____________________________________________________________________________//

#define HBOOST_IS_DEFINED( symb ) ::hboost::test_tools::tt_detail::is_defined_impl( #symb, HBOOST_STRINGIZE(= symb) )

//____________________________________________________________________________//

#endif // HBOOST_TEST_NO_OLD_TOOLS

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_TOOLS_INTERFACE_HPP_111712GER
