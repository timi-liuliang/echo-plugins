//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// @brief Defines Unit Test Framework public API
// ***************************************************************************

#ifndef HBOOST_TEST_UNIT_TEST_SUITE_HPP_071894GER
#define HBOOST_TEST_UNIT_TEST_SUITE_HPP_071894GER

// Boost.Test
#include <hboost/test/framework.hpp>
#include <hboost/test/tree/auto_registration.hpp>
#include <hboost/test/tree/test_case_template.hpp>
#include <hboost/test/tree/global_fixture.hpp>


#include <hboost/test/detail/suppress_warnings.hpp>


#include <hboost/test/detail/pp_variadic.hpp>



//____________________________________________________________________________//

// ************************************************************************** //
// **************    Non-auto (explicit) test case interface   ************** //
// ************************************************************************** //

#define HBOOST_TEST_CASE( test_function )                                   \
hboost::unit_test::make_test_case( hboost::function<void ()>(test_function), \
                                  HBOOST_TEST_STRINGIZE( test_function ),   \
                                  __FILE__, __LINE__ )
#define HBOOST_CLASS_TEST_CASE( test_function, tc_instance )                \
hboost::unit_test::make_test_case( (test_function),                         \
                                  HBOOST_TEST_STRINGIZE( test_function ),   \
                                  __FILE__, __LINE__, tc_instance )

// ************************************************************************** //
// **************               HBOOST_TEST_SUITE               ************** //
// ************************************************************************** //

#define HBOOST_TEST_SUITE( testsuite_name ) \
( new hboost::unit_test::test_suite( testsuite_name, __FILE__, __LINE__ ) )

// ************************************************************************** //
// **************             HBOOST_AUTO_TEST_SUITE            ************** //
// ************************************************************************** //

#define HBOOST_AUTO_TEST_SUITE_WITH_DECOR( suite_name, decorators )      \
namespace suite_name {                                                  \
HBOOST_AUTO_TU_REGISTRAR( suite_name )(                                  \
    HBOOST_STRINGIZE( suite_name ),                                      \
    __FILE__, __LINE__,                                                 \
    decorators );                                                       \
/**/

#define HBOOST_AUTO_TEST_SUITE_NO_DECOR( suite_name )                    \
    HBOOST_AUTO_TEST_SUITE_WITH_DECOR(                                   \
        suite_name,                                                     \
        hboost::unit_test::decorator::collector::instance() )            \
/**/

#if HBOOST_PP_VARIADICS
#define HBOOST_AUTO_TEST_SUITE( ... )                                    \
    HBOOST_TEST_INVOKE_IF_N_ARGS( 1,                                     \
        HBOOST_AUTO_TEST_SUITE_NO_DECOR,                                 \
        HBOOST_AUTO_TEST_SUITE_WITH_DECOR,                               \
        __VA_ARGS__)                                                    \
/**/

#else /* HBOOST_PP_VARIADICS */

#define HBOOST_AUTO_TEST_SUITE( suite_name )                             \
    HBOOST_AUTO_TEST_SUITE_NO_DECOR( suite_name )                        \
/**/


#endif /* HBOOST_PP_VARIADICS */

// ************************************************************************** //
// **************            HBOOST_FIXTURE_TEST_SUITE          ************** //
// ************************************************************************** //

#define HBOOST_FIXTURE_TEST_SUITE_WITH_DECOR(suite_name, F, decorators)  \
    HBOOST_AUTO_TEST_SUITE_WITH_DECOR( suite_name, decorators )          \
typedef F HBOOST_AUTO_TEST_CASE_FIXTURE;                                 \
/**/

#define HBOOST_FIXTURE_TEST_SUITE_NO_DECOR( suite_name, F )              \
    HBOOST_AUTO_TEST_SUITE_NO_DECOR( suite_name )                        \
typedef F HBOOST_AUTO_TEST_CASE_FIXTURE;                                 \
/**/

#if HBOOST_PP_VARIADICS

#define HBOOST_FIXTURE_TEST_SUITE( ... )                                 \
    HBOOST_TEST_INVOKE_IF_N_ARGS( 2,                                     \
        HBOOST_FIXTURE_TEST_SUITE_NO_DECOR,                              \
        HBOOST_FIXTURE_TEST_SUITE_WITH_DECOR,                            \
        __VA_ARGS__)                                                    \
/**/

#else /* HBOOST_PP_VARIADICS */

#define HBOOST_FIXTURE_TEST_SUITE( suite_name, F  )                      \
   HBOOST_FIXTURE_TEST_SUITE_NO_DECOR( suite_name, F )                   \
/**/


#endif /* HBOOST_PP_VARIADICS */


// ************************************************************************** //
// **************           HBOOST_AUTO_TEST_SUITE_END          ************** //
// ************************************************************************** //

#define HBOOST_AUTO_TEST_SUITE_END()                                     \
HBOOST_AUTO_TU_REGISTRAR( HBOOST_JOIN( end_suite, __LINE__ ) )( 1 );      \
}                                                                       \
/**/

// ************************************************************************** //
// **************    HBOOST_AUTO_TEST_CASE_EXPECTED_FAILURES    ************** //
// ************************************************************************** //

/// @deprecated use decorator instead
#define HBOOST_AUTO_TEST_CASE_EXPECTED_FAILURES( test_name, n )          \
HBOOST_TEST_DECORATOR( * hboost::unit_test::expected_failures( n ) )      \
/**/

// ************************************************************************** //
// **************            HBOOST_FIXTURE_TEST_CASE           ************** //
// ************************************************************************** //

#define HBOOST_FIXTURE_TEST_CASE_WITH_DECOR( test_name, F, decorators )  \
struct test_name : public F { void test_method(); };                    \
                                                                        \
static void HBOOST_AUTO_TC_INVOKER( test_name )()                        \
{                                                                       \
    HBOOST_TEST_CHECKPOINT('"' << #test_name << "\" fixture entry.");    \
    test_name t;                                                        \
    HBOOST_TEST_CHECKPOINT('"' << #test_name << "\" entry.");            \
    t.test_method();                                                    \
    HBOOST_TEST_CHECKPOINT('"' << #test_name << "\" exit.");             \
}                                                                       \
                                                                        \
struct HBOOST_AUTO_TC_UNIQUE_ID( test_name ) {};                         \
                                                                        \
HBOOST_AUTO_TU_REGISTRAR( test_name )(                                   \
    hboost::unit_test::make_test_case(                                   \
        &HBOOST_AUTO_TC_INVOKER( test_name ),                            \
        #test_name, __FILE__, __LINE__ ),                               \
        decorators );                                                   \
                                                                        \
void test_name::test_method()                                           \
/**/

#define HBOOST_FIXTURE_TEST_CASE_NO_DECOR( test_name, F )                \
HBOOST_FIXTURE_TEST_CASE_WITH_DECOR( test_name, F,                       \
    hboost::unit_test::decorator::collector::instance() )                \
/**/

#if HBOOST_PP_VARIADICS

#define HBOOST_FIXTURE_TEST_CASE( ... )                                  \
    HBOOST_TEST_INVOKE_IF_N_ARGS( 2,                                     \
        HBOOST_FIXTURE_TEST_CASE_NO_DECOR,                               \
        HBOOST_FIXTURE_TEST_CASE_WITH_DECOR,                             \
         __VA_ARGS__)                                                   \
/**/

#else /* HBOOST_PP_VARIADICS */

#define HBOOST_FIXTURE_TEST_CASE( test_name, F )                         \
     HBOOST_FIXTURE_TEST_CASE_NO_DECOR(test_name, F)                     \
/**/


#endif /* HBOOST_PP_VARIADICS */

// ************************************************************************** //
// **************             HBOOST_AUTO_TEST_CASE             ************** //
// ************************************************************************** //

#define HBOOST_AUTO_TEST_CASE_NO_DECOR( test_name )                      \
    HBOOST_FIXTURE_TEST_CASE_NO_DECOR( test_name,                        \
        HBOOST_AUTO_TEST_CASE_FIXTURE )                                  \
/**/

#define HBOOST_AUTO_TEST_CASE_WITH_DECOR( test_name, decorators )        \
    HBOOST_FIXTURE_TEST_CASE_WITH_DECOR( test_name,                      \
        HBOOST_AUTO_TEST_CASE_FIXTURE, decorators )                      \
/**/

#if HBOOST_PP_VARIADICS

#define HBOOST_AUTO_TEST_CASE( ... )                                     \
    HBOOST_TEST_INVOKE_IF_N_ARGS( 1,                                     \
        HBOOST_AUTO_TEST_CASE_NO_DECOR,                                  \
        HBOOST_AUTO_TEST_CASE_WITH_DECOR,                                \
         __VA_ARGS__)                                                   \
/**/

#else /* HBOOST_PP_VARIADICS */

#define HBOOST_AUTO_TEST_CASE( test_name )                               \
    HBOOST_AUTO_TEST_CASE_NO_DECOR( test_name )                          \
/**/


#endif /* HBOOST_PP_VARIADICS */

// ************************************************************************** //
// **************       HBOOST_FIXTURE_TEST_CASE_TEMPLATE       ************** //
// ************************************************************************** //

#define HBOOST_FIXTURE_TEST_CASE_TEMPLATE( test_name, type_name, TL, F ) \
template<typename type_name>                                            \
struct test_name : public F                                             \
{ void test_method(); };                                                \
                                                                        \
struct HBOOST_AUTO_TC_INVOKER( test_name ) {                             \
    template<typename TestType>                                         \
    static void run( hboost::type<TestType>* = 0 )                       \
    {                                                                   \
        HBOOST_TEST_CHECKPOINT('"' << #test_name <<"\" fixture entry."); \
        test_name<TestType> t;                                          \
        HBOOST_TEST_CHECKPOINT('"' << #test_name << "\" entry.");        \
        t.test_method();                                                \
        HBOOST_TEST_CHECKPOINT('"' << #test_name << "\" exit.");         \
    }                                                                   \
};                                                                      \
                                                                        \
HBOOST_AUTO_TU_REGISTRAR( test_name )(                                   \
    hboost::unit_test::ut_detail::template_test_case_gen<                \
        HBOOST_AUTO_TC_INVOKER( test_name ),TL >(                        \
          HBOOST_STRINGIZE( test_name ), __FILE__, __LINE__ ),           \
    hboost::unit_test::decorator::collector::instance() );               \
                                                                        \
template<typename type_name>                                            \
void test_name<type_name>::test_method()                                \
/**/

// ************************************************************************** //
// **************        HBOOST_AUTO_TEST_CASE_TEMPLATE         ************** //
// ************************************************************************** //

#define HBOOST_AUTO_TEST_CASE_TEMPLATE( test_name, type_name, TL )       \
HBOOST_FIXTURE_TEST_CASE_TEMPLATE( test_name, type_name, TL,             \
    HBOOST_AUTO_TEST_CASE_FIXTURE )                                      \
/**/

// ************************************************************************** //
// **************           HBOOST_TEST_CASE_TEMPLATE           ************** //
// ************************************************************************** //

#define HBOOST_TEST_CASE_TEMPLATE( name, typelist )                      \
    hboost::unit_test::ut_detail::template_test_case_gen<name,typelist>( \
        HBOOST_TEST_STRINGIZE( name ), __FILE__, __LINE__ )              \
/**/

// ************************************************************************** //
// **************      HBOOST_TEST_CASE_TEMPLATE_FUNCTION       ************** //
// ************************************************************************** //

#define HBOOST_TEST_CASE_TEMPLATE_FUNCTION( name, type_name )            \
template<typename type_name>                                            \
void HBOOST_JOIN( name, _impl )( hboost::type<type_name>* );              \
                                                                        \
struct name {                                                           \
    template<typename TestType>                                         \
    static void run( hboost::type<TestType>* frwrd = 0 )                 \
    {                                                                   \
       HBOOST_JOIN( name, _impl )( frwrd );                              \
    }                                                                   \
};                                                                      \
                                                                        \
template<typename type_name>                                            \
void HBOOST_JOIN( name, _impl )( hboost::type<type_name>* )               \
/**/

// ************************************************************************** //
// **************              HBOOST_GLOBAL_FIXTURE            ************** //
// ************************************************************************** //

#define HBOOST_GLOBAL_FIXTURE( F ) \
static hboost::unit_test::ut_detail::global_fixture_impl<F> HBOOST_JOIN( gf_, F ) \
/**/

// ************************************************************************** //
// **************             HBOOST_TEST_DECORATOR             ************** //
// ************************************************************************** //

#define HBOOST_TEST_DECORATOR( D )                                       \
static hboost::unit_test::decorator::collector const&                    \
HBOOST_JOIN(decorator_collector,__LINE__) = D;                           \
/**/

// ************************************************************************** //
// **************         HBOOST_AUTO_TEST_CASE_FIXTURE         ************** //
// ************************************************************************** //

namespace hboost { namespace unit_test { namespace ut_detail {

struct nil_t {};

} // namespace ut_detail
} // unit_test
} // namespace hboost

// Intentionally is in global namespace, so that FIXTURE_TEST_SUITE can reset it in user code.
typedef ::hboost::unit_test::ut_detail::nil_t HBOOST_AUTO_TEST_CASE_FIXTURE;

// ************************************************************************** //
// **************   Auto registration facility helper macros   ************** //
// ************************************************************************** //

#define HBOOST_AUTO_TU_REGISTRAR( test_name )                    \
static hboost::unit_test::ut_detail::auto_test_unit_registrar    \
HBOOST_JOIN( HBOOST_JOIN( test_name, _registrar ), __LINE__ )     \
/**/
#define HBOOST_AUTO_TC_INVOKER( test_name )      HBOOST_JOIN( test_name, _invoker )
#define HBOOST_AUTO_TC_UNIQUE_ID( test_name )    HBOOST_JOIN( test_name, _id )

// ************************************************************************** //
// **************                HBOOST_TEST_MAIN               ************** //
// ************************************************************************** //

#if defined(HBOOST_TEST_MAIN)

#ifdef HBOOST_TEST_ALTERNATIVE_INIT_API
bool init_unit_test()                   {
#else
::hboost::unit_test::test_suite*
init_unit_test_suite( int, char* [] )   {
#endif

#ifdef HBOOST_TEST_MODULE
    using namespace ::hboost::unit_test;
    assign_op( framework::master_test_suite().p_name.value, HBOOST_TEST_STRINGIZE( HBOOST_TEST_MODULE ).trim( "\"" ), 0 );

#endif

#ifdef HBOOST_TEST_ALTERNATIVE_INIT_API
    return true;
}
#else
    return 0;
}
#endif

#endif

//____________________________________________________________________________//

#include <hboost/test/detail/enable_warnings.hpp>


#endif // HBOOST_TEST_UNIT_TEST_SUITE_HPP_071894GER

