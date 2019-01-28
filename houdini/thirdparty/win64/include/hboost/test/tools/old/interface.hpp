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
//  Description : contains definition for all test tools in old test toolbox
// ***************************************************************************

#ifndef HBOOST_TEST_TOOLS_OLD_INTERFACE_HPP_111712GER
#define HBOOST_TEST_TOOLS_OLD_INTERFACE_HPP_111712GER

// Boost
#include <hboost/preprocessor/seq/for_each.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/seq/to_tuple.hpp>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

// ************************************************************************** //
// **************                    TOOL BOX                  ************** //
// ************************************************************************** //

// In macros below following argument abbreviations are used:
// P - predicate
// M - message
// S - statement
// E - exception
// L - left argument
// R - right argument
// TL - tool level
// CT - check type
// ARGS - arguments list (as PP sequence)

// frwd_type:
// 0 - args exists and need to be forwarded; call check_frwd
// 1 - args exists, but do not need to be forwarded; call report_assertion directly
// 2 - no arguments; call report_assertion directly

#define HBOOST_TEST_TOOL_PASS_PRED0( P, ARGS ) P
#define HBOOST_TEST_TOOL_PASS_PRED1( P, ARGS ) P HBOOST_PP_SEQ_TO_TUPLE(ARGS)
#define HBOOST_TEST_TOOL_PASS_PRED2( P, ARGS ) P

#define HBOOST_TEST_TOOL_PASS_ARG( r, _, arg ) , arg, HBOOST_STRINGIZE( arg )
#define HBOOST_TEST_TOOL_PASS_ARG_DSCR( r, _, arg ) , HBOOST_STRINGIZE( arg )

#define HBOOST_TEST_TOOL_PASS_ARGS0( ARGS ) \
    HBOOST_PP_SEQ_FOR_EACH( HBOOST_TEST_TOOL_PASS_ARG, _, ARGS )
#define HBOOST_TEST_TOOL_PASS_ARGS1( ARGS ) \
    , HBOOST_PP_SEQ_SIZE(ARGS) HBOOST_PP_SEQ_FOR_EACH( HBOOST_TEST_TOOL_PASS_ARG_DSCR, _, ARGS )
#define HBOOST_TEST_TOOL_PASS_ARGS2( ARGS ) \
    , 0

#define HBOOST_TEST_TOOL_IMPL( frwd_type, P, assertion_descr, TL, CT, ARGS )     \
do {                                                                            \
    HBOOST_TEST_PASSPOINT();                                                     \
    ::hboost::test_tools::tt_detail::                                            \
    HBOOST_PP_IF( frwd_type, report_assertion, check_frwd ) (                    \
        HBOOST_JOIN( HBOOST_TEST_TOOL_PASS_PRED, frwd_type )( P, ARGS ),          \
        HBOOST_TEST_LAZY_MSG( assertion_descr ),                                 \
        HBOOST_TEST_L(__FILE__),                                                 \
        static_cast<std::size_t>(__LINE__),                                     \
        ::hboost::test_tools::tt_detail::TL,                                     \
        ::hboost::test_tools::tt_detail::CT                                      \
        HBOOST_JOIN( HBOOST_TEST_TOOL_PASS_ARGS, frwd_type )( ARGS ) );           \
} while( ::hboost::test_tools::tt_detail::dummy_cond() )                         \
/**/

//____________________________________________________________________________//

#define HBOOST_WARN( P )                     HBOOST_TEST_TOOL_IMPL( 2, \
    (P), HBOOST_TEST_STRINGIZE( P ), WARN, CHECK_PRED, _ )
#define HBOOST_CHECK( P )                    HBOOST_TEST_TOOL_IMPL( 2, \
    (P), HBOOST_TEST_STRINGIZE( P ), CHECK, CHECK_PRED, _ )
#define HBOOST_REQUIRE( P )                  HBOOST_TEST_TOOL_IMPL( 2, \
    (P), HBOOST_TEST_STRINGIZE( P ), REQUIRE, CHECK_PRED, _ )

//____________________________________________________________________________//

#define HBOOST_WARN_MESSAGE( P, M )          HBOOST_TEST_TOOL_IMPL( 2, (P), M, WARN, CHECK_MSG, _ )
#define HBOOST_CHECK_MESSAGE( P, M )         HBOOST_TEST_TOOL_IMPL( 2, (P), M, CHECK, CHECK_MSG, _ )
#define HBOOST_REQUIRE_MESSAGE( P, M )       HBOOST_TEST_TOOL_IMPL( 2, (P), M, REQUIRE, CHECK_MSG, _ )

//____________________________________________________________________________//

#define HBOOST_ERROR( M )                    HBOOST_CHECK_MESSAGE( false, M )
#define HBOOST_FAIL( M )                     HBOOST_REQUIRE_MESSAGE( false, M )

//____________________________________________________________________________//

#define HBOOST_CHECK_THROW_IMPL( S, E, P, postfix, TL )                                   \
do {                                                                                    \
    try {                                                                               \
        HBOOST_TEST_PASSPOINT();                                                         \
        S;                                                                              \
        HBOOST_TEST_TOOL_IMPL( 2, false, "exception " HBOOST_STRINGIZE(E) " expected but not raised", \
                              TL, CHECK_MSG, _ );                                       \
    } catch( E const& ex ) {                                                            \
        ::hboost::unit_test::ut_detail::ignore_unused_variable_warning( ex );            \
        HBOOST_TEST_TOOL_IMPL( 2, P, \
                              "exception \"" HBOOST_STRINGIZE( E )"\" raised as expected" postfix,           \
                              TL, CHECK_MSG, _  );                                      \
    }                                                                                   \
} while( ::hboost::test_tools::tt_detail::dummy_cond() )                                 \
/**/

//____________________________________________________________________________//

#define HBOOST_WARN_THROW( S, E )            HBOOST_CHECK_THROW_IMPL( S, E, true, "", WARN )
#define HBOOST_CHECK_THROW( S, E )           HBOOST_CHECK_THROW_IMPL( S, E, true, "", CHECK )
#define HBOOST_REQUIRE_THROW( S, E )         HBOOST_CHECK_THROW_IMPL( S, E, true, "", REQUIRE )

//____________________________________________________________________________//

#define HBOOST_WARN_EXCEPTION( S, E, P )     HBOOST_CHECK_THROW_IMPL( S, E, P( ex ), \
              ": validation on the raised exception through predicate \"" HBOOST_STRINGIZE(P) "\"", WARN )
#define HBOOST_CHECK_EXCEPTION( S, E, P )    HBOOST_CHECK_THROW_IMPL( S, E, P( ex ), \
              ": validation on the raised exception through predicate \"" HBOOST_STRINGIZE(P) "\"", CHECK )
#define HBOOST_REQUIRE_EXCEPTION( S, E, P )  HBOOST_CHECK_THROW_IMPL( S, E, P( ex ), \
              ": validation on the raised exception through predicate \"" HBOOST_STRINGIZE(P) "\"", REQUIRE )

//____________________________________________________________________________//

#define HBOOST_CHECK_NO_THROW_IMPL( S, TL )                                              \
do {                                                                                    \
    try {                                                                               \
        S;                                                                              \
        HBOOST_TEST_TOOL_IMPL( 2, true, "no exceptions thrown by " HBOOST_STRINGIZE( S ), \
                              TL, CHECK_MSG, _ );                                       \
    } catch( ... ) {                                                                    \
        HBOOST_TEST_TOOL_IMPL( 2, false, "unexpected exception thrown by " HBOOST_STRINGIZE( S ),    \
                              TL, CHECK_MSG, _ );                                       \
    }                                                                                   \
} while( ::hboost::test_tools::tt_detail::dummy_cond() )                                 \
/**/

#define HBOOST_WARN_NO_THROW( S )            HBOOST_CHECK_NO_THROW_IMPL( S, WARN )
#define HBOOST_CHECK_NO_THROW( S )           HBOOST_CHECK_NO_THROW_IMPL( S, CHECK )
#define HBOOST_REQUIRE_NO_THROW( S )         HBOOST_CHECK_NO_THROW_IMPL( S, REQUIRE )

//____________________________________________________________________________//

#define HBOOST_WARN_EQUAL( L, R )            HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::equal_impl_frwd(), "", WARN, CHECK_EQUAL, (L)(R) )
#define HBOOST_CHECK_EQUAL( L, R )           HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::equal_impl_frwd(), "", CHECK, CHECK_EQUAL, (L)(R) )
#define HBOOST_REQUIRE_EQUAL( L, R )         HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::equal_impl_frwd(), "", REQUIRE, CHECK_EQUAL, (L)(R) )

//____________________________________________________________________________//

#define HBOOST_WARN_NE( L, R )               HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::ne_impl(), "", WARN, CHECK_NE, (L)(R) )
#define HBOOST_CHECK_NE( L, R )              HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::ne_impl(), "", CHECK, CHECK_NE, (L)(R) )
#define HBOOST_REQUIRE_NE( L, R )            HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::ne_impl(), "", REQUIRE, CHECK_NE, (L)(R) )

//____________________________________________________________________________//

#define HBOOST_WARN_LT( L, R )               HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::lt_impl(), "", WARN, CHECK_LT, (L)(R) )
#define HBOOST_CHECK_LT( L, R )              HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::lt_impl(), "", CHECK, CHECK_LT, (L)(R) )
#define HBOOST_REQUIRE_LT( L, R )            HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::lt_impl(), "", REQUIRE, CHECK_LT, (L)(R) )

//____________________________________________________________________________//

#define HBOOST_WARN_LE( L, R )               HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::le_impl(), "", WARN, CHECK_LE, (L)(R) )
#define HBOOST_CHECK_LE( L, R )              HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::le_impl(), "", CHECK, CHECK_LE, (L)(R) )
#define HBOOST_REQUIRE_LE( L, R )            HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::le_impl(), "", REQUIRE, CHECK_LE, (L)(R) )

//____________________________________________________________________________//

#define HBOOST_WARN_GT( L, R )               HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::gt_impl(), "", WARN, CHECK_GT, (L)(R) )
#define HBOOST_CHECK_GT( L, R )              HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::gt_impl(), "", CHECK, CHECK_GT, (L)(R) )
#define HBOOST_REQUIRE_GT( L, R )            HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::gt_impl(), "", REQUIRE, CHECK_GT, (L)(R) )

//____________________________________________________________________________//

#define HBOOST_WARN_GE( L, R )               HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::ge_impl(), "", WARN, CHECK_GE, (L)(R) )
#define HBOOST_CHECK_GE( L, R )              HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::ge_impl(), "", CHECK, CHECK_GE, (L)(R) )
#define HBOOST_REQUIRE_GE( L, R )            HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::tt_detail::ge_impl(), "", REQUIRE, CHECK_GE, (L)(R) )

//____________________________________________________________________________//

#define HBOOST_WARN_CLOSE( L, R, T )         HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::check_is_close_t(), "", WARN, CHECK_CLOSE, (L)(R)(::hboost::math::fpc::percent_tolerance(T)) )
#define HBOOST_CHECK_CLOSE( L, R, T )        HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::check_is_close_t(), "", CHECK, CHECK_CLOSE, (L)(R)(::hboost::math::fpc::percent_tolerance(T)) )
#define HBOOST_REQUIRE_CLOSE( L, R, T )      HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::check_is_close_t(), "", REQUIRE, CHECK_CLOSE, (L)(R)(::hboost::math::fpc::percent_tolerance(T)) )

//____________________________________________________________________________//

#define HBOOST_WARN_CLOSE_FRACTION(L, R, T)  HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::check_is_close_t(), "", WARN, CHECK_CLOSE_FRACTION, (L)(R)(T) )
#define HBOOST_CHECK_CLOSE_FRACTION(L, R, T) HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::check_is_close_t(), "", CHECK, CHECK_CLOSE_FRACTION, (L)(R)(T) )
#define HBOOST_REQUIRE_CLOSE_FRACTION(L,R,T) HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::check_is_close_t(), "", REQUIRE, CHECK_CLOSE_FRACTION, (L)(R)(T) )

//____________________________________________________________________________//

#define HBOOST_WARN_SMALL( FPV, T )          HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::check_is_small_t(), "", WARN, CHECK_SMALL, (FPV)(T) )
#define HBOOST_CHECK_SMALL( FPV, T )         HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::check_is_small_t(), "", CHECK, CHECK_SMALL, (FPV)(T) )
#define HBOOST_REQUIRE_SMALL( FPV, T )       HBOOST_TEST_TOOL_IMPL( 0, \
    ::hboost::test_tools::check_is_small_t(), "", REQUIRE, CHECK_SMALL, (FPV)(T) )

//____________________________________________________________________________//

#define HBOOST_WARN_PREDICATE( P, ARGS )     HBOOST_TEST_TOOL_IMPL( 0, \
    P, HBOOST_TEST_STRINGIZE( P ), WARN, CHECK_PRED_WITH_ARGS, ARGS )
#define HBOOST_CHECK_PREDICATE( P, ARGS )    HBOOST_TEST_TOOL_IMPL( 0, \
    P, HBOOST_TEST_STRINGIZE( P ), CHECK, CHECK_PRED_WITH_ARGS, ARGS )
#define HBOOST_REQUIRE_PREDICATE( P, ARGS )  HBOOST_TEST_TOOL_IMPL( 0, \
    P, HBOOST_TEST_STRINGIZE( P ), REQUIRE, CHECK_PRED_WITH_ARGS, ARGS )

//____________________________________________________________________________//

#define HBOOST_WARN_EQUAL_COLLECTIONS( L_begin, L_end, R_begin, R_end )                  \
    HBOOST_TEST_TOOL_IMPL( 1, ::hboost::test_tools::tt_detail::equal_coll_impl(),         \
        "", WARN, CHECK_EQUAL_COLL, (L_begin)(L_end)(R_begin)(R_end) )                  \
/**/
#define HBOOST_CHECK_EQUAL_COLLECTIONS( L_begin, L_end, R_begin, R_end )                 \
    HBOOST_TEST_TOOL_IMPL( 1, ::hboost::test_tools::tt_detail::equal_coll_impl(),         \
        "", CHECK, CHECK_EQUAL_COLL, (L_begin)(L_end)(R_begin)(R_end) )                 \
/**/
#define HBOOST_REQUIRE_EQUAL_COLLECTIONS( L_begin, L_end, R_begin, R_end )               \
    HBOOST_TEST_TOOL_IMPL( 1, ::hboost::test_tools::tt_detail::equal_coll_impl(),         \
        "", REQUIRE, CHECK_EQUAL_COLL, (L_begin)(L_end)(R_begin)(R_end) )               \
/**/

//____________________________________________________________________________//

#define HBOOST_WARN_BITWISE_EQUAL( L, R )    HBOOST_TEST_TOOL_IMPL( 1, \
    ::hboost::test_tools::tt_detail::bitwise_equal_impl(), "", WARN, CHECK_BITWISE_EQUAL, (L)(R) )
#define HBOOST_CHECK_BITWISE_EQUAL( L, R )   HBOOST_TEST_TOOL_IMPL( 1, \
    ::hboost::test_tools::tt_detail::bitwise_equal_impl(), "", CHECK, CHECK_BITWISE_EQUAL, (L)(R) )
#define HBOOST_REQUIRE_BITWISE_EQUAL( L, R ) HBOOST_TEST_TOOL_IMPL( 1, \
    ::hboost::test_tools::tt_detail::bitwise_equal_impl(), "", REQUIRE, CHECK_BITWISE_EQUAL, (L)(R) )

//____________________________________________________________________________//

#define HBOOST_IS_DEFINED( symb ) ::hboost::test_tools::tt_detail::is_defined_impl( #symb, HBOOST_STRINGIZE(= symb) )

//____________________________________________________________________________//

#ifdef HBOOST_TEST_NO_NEW_TOOLS

#define HBOOST_TEST_WARN( P )                HBOOST_WARN( P )
#define HBOOST_TEST_CHECK( P )               HBOOST_CHECK( P )
#define HBOOST_TEST_REQUIRE( P )             HBOOST_REQUIRE( P )

#define HBOOST_TEST( P )                     HBOOST_CHECK( P )

#endif

//____________________________________________________________________________//

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_TOOLS_OLD_INTERFACE_HPP_111712GER
