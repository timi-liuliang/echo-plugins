//  (C) Copyright Eric Niebler 2004-2005
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
//  Description : this is an abridged version of an excelent HBOOST_FOREACH facility
//  presented by Eric Niebler. I am so fond of it so I can't wait till it
//  going to be accepted into Boost. Also I need version with less number of dependencies
//  and more portable. This version doesn't support rvalues and will reeveluate it's
//  parameters, but should be good enough for my purposes.
// ***************************************************************************

#ifndef HBOOST_TEST_UTILS_FOREACH_HPP
#define HBOOST_TEST_UTILS_FOREACH_HPP

// Boost.Test
#include <hboost/test/detail/config.hpp>

// Boost
#include <hboost/type.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/test/detail/workaround.hpp>

#include <hboost/type_traits/is_const.hpp>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {
namespace for_each {

// ************************************************************************** //
// **************                  static_any                  ************** //
// ************************************************************************** //

struct static_any_base
{
    operator bool() const { return false; }
};

//____________________________________________________________________________//

template<typename Iter>
struct static_any : static_any_base
{
    static_any( Iter const& t ) : m_it( t ) {}

    mutable Iter m_it;
};

//____________________________________________________________________________//

typedef static_any_base const& static_any_t;

//____________________________________________________________________________//

template<typename Iter>
inline Iter&
static_any_cast( static_any_t a, Iter* = 0 )
{
    return static_cast<Iter&>( static_cast<static_any<Iter> const&>( a ).m_it );
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************                   is_const                   ************** //
// ************************************************************************** //

template<typename C>
inline is_const<C>
is_const_coll( C& )
{
    return is_const<C>();
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************                     begin                    ************** //
// ************************************************************************** //

template<typename C>
inline static_any<HBOOST_DEDUCED_TYPENAME C::iterator>
begin( C& t, mpl::false_ )
{
    return static_any<HBOOST_DEDUCED_TYPENAME C::iterator>( t.begin() );
}

//____________________________________________________________________________//

template<typename C>
inline static_any<HBOOST_DEDUCED_TYPENAME C::const_iterator>
begin( C const& t, mpl::true_ )
{
    return static_any<HBOOST_DEDUCED_TYPENAME C::const_iterator>( t.begin() );
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************                      end                     ************** //
// ************************************************************************** //

template<typename C>
inline static_any<HBOOST_DEDUCED_TYPENAME C::iterator>
end( C& t, mpl::false_ )
{
    return static_any<HBOOST_DEDUCED_TYPENAME C::iterator>( t.end() );
}

//____________________________________________________________________________//

template<typename C>
inline static_any<HBOOST_DEDUCED_TYPENAME C::const_iterator>
end( C const& t, mpl::true_ )
{
    return static_any<HBOOST_DEDUCED_TYPENAME C::const_iterator>( t.end() );
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************                      done                    ************** //
// ************************************************************************** //

template<typename C>
inline bool
done( static_any_t cur, static_any_t end, C&, mpl::false_ )
{
    return  static_any_cast<HBOOST_DEDUCED_TYPENAME C::iterator>( cur ) ==
            static_any_cast<HBOOST_DEDUCED_TYPENAME C::iterator>( end );
}

//____________________________________________________________________________//

template<typename C>
inline bool
done( static_any_t cur, static_any_t end, C const&, mpl::true_ )
{
    return  static_any_cast<HBOOST_DEDUCED_TYPENAME C::const_iterator>( cur ) ==
            static_any_cast<HBOOST_DEDUCED_TYPENAME C::const_iterator>( end );
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************                      next                    ************** //
// ************************************************************************** //

template<typename C>
inline void
next( static_any_t cur, C&, mpl::false_ )
{
    ++static_any_cast<HBOOST_DEDUCED_TYPENAME C::iterator>( cur );
}

//____________________________________________________________________________//

template<typename C>
inline void
next( static_any_t cur, C const&, mpl::true_ )
{
    ++static_any_cast<HBOOST_DEDUCED_TYPENAME C::const_iterator>( cur );
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************                      prev                    ************** //
// ************************************************************************** //

template<typename C>
inline void
prev( static_any_t cur, C&, mpl::false_ )
{
    --static_any_cast<HBOOST_DEDUCED_TYPENAME C::iterator>( cur );
}

//____________________________________________________________________________//

template<typename C>
inline void
prev( static_any_t cur, C const&, mpl::true_ )
{
    --static_any_cast<HBOOST_DEDUCED_TYPENAME C::const_iterator>( cur );
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************                      deref                   ************** //
// ************************************************************************** //

template<class RefType,typename C>
inline RefType
deref( static_any_t cur, C&, ::hboost::type<RefType>, mpl::false_ )
{
    return *static_any_cast<HBOOST_DEDUCED_TYPENAME C::iterator>( cur );
}

//____________________________________________________________________________//

template<class RefType,typename C>
inline RefType
deref( static_any_t cur, C const&, ::hboost::type<RefType>, mpl::true_ )
{
    return *static_any_cast<HBOOST_DEDUCED_TYPENAME C::const_iterator>( cur );
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************              HBOOST_TEST_FOREACH              ************** //
// ************************************************************************** //

#define HBOOST_TEST_FE_ANY                   ::hboost::unit_test::for_each::static_any_t
#define HBOOST_TEST_FE_IS_CONST( COL )       ::hboost::unit_test::for_each::is_const_coll( COL )

#define HBOOST_TEST_FE_BEG( COL )            \
    ::hboost::unit_test::for_each::begin(    \
        COL,                                \
        HBOOST_TEST_FE_IS_CONST( COL ) )     \
/**/

#define HBOOST_TEST_FE_END( COL )            \
    ::hboost::unit_test::for_each::end(      \
        COL,                                \
        HBOOST_TEST_FE_IS_CONST( COL ) )     \
/**/

#define HBOOST_TEST_FE_DONE( COL )           \
    ::hboost::unit_test::for_each::done(     \
        HBOOST_TEST_FE_CUR_VAR,              \
        HBOOST_TEST_FE_END_VAR,              \
        COL,                                \
        HBOOST_TEST_FE_IS_CONST( COL ) )     \
/**/

#define HBOOST_TEST_FE_NEXT( COL )           \
    ::hboost::unit_test::for_each::next(     \
        HBOOST_TEST_FE_CUR_VAR,              \
        COL,                                \
        HBOOST_TEST_FE_IS_CONST( COL ) )     \
/**/

#define HBOOST_TEST_FE_PREV( COL )           \
    ::hboost::unit_test::for_each::prev(     \
        HBOOST_TEST_FE_CUR_VAR,              \
        COL,                                \
        HBOOST_TEST_FE_IS_CONST( COL ) )     \
/**/

#define HBOOST_FOREACH_NOOP(COL)             \
    ((void)&(COL))

#define HBOOST_TEST_FE_DEREF( COL, RefType ) \
    ::hboost::unit_test::for_each::deref(    \
        HBOOST_TEST_FE_CUR_VAR,              \
        COL,                                \
        ::hboost::type<RefType >(),          \
        HBOOST_TEST_FE_IS_CONST( COL ) )     \
/**/

#if HBOOST_WORKAROUND( HBOOST_MSVC, == 1310 )
#define HBOOST_TEST_LINE_NUM
#else
#define HBOOST_TEST_LINE_NUM     __LINE__
#endif

#define HBOOST_TEST_FE_CUR_VAR   HBOOST_JOIN( _fe_cur_, HBOOST_TEST_LINE_NUM )
#define HBOOST_TEST_FE_END_VAR   HBOOST_JOIN( _fe_end_, HBOOST_TEST_LINE_NUM )
#define HBOOST_TEST_FE_CON_VAR   HBOOST_JOIN( _fe_con_, HBOOST_TEST_LINE_NUM )

#define HBOOST_TEST_FOREACH( RefType, var, COL )                                             \
if( HBOOST_TEST_FE_ANY HBOOST_TEST_FE_CUR_VAR = HBOOST_TEST_FE_BEG( COL ) ) {} else            \
if( HBOOST_TEST_FE_ANY HBOOST_TEST_FE_END_VAR = HBOOST_TEST_FE_END( COL ) ) {} else            \
for( bool HBOOST_TEST_FE_CON_VAR = true;                                                     \
          HBOOST_TEST_FE_CON_VAR && !HBOOST_TEST_FE_DONE( COL );                              \
          HBOOST_TEST_FE_CON_VAR ? HBOOST_TEST_FE_NEXT( COL ) : HBOOST_FOREACH_NOOP( COL ))    \
                                                                                            \
    if( (HBOOST_TEST_FE_CON_VAR = false, false) ) {} else                                    \
    for( RefType var = HBOOST_TEST_FE_DEREF( COL, RefType );                                 \
         !HBOOST_TEST_FE_CON_VAR; HBOOST_TEST_FE_CON_VAR = true )                             \
/**/

#define HBOOST_TEST_REVERSE_FOREACH( RefType, var, COL )                                     \
if( HBOOST_TEST_FE_ANY HBOOST_TEST_FE_CUR_VAR = HBOOST_TEST_FE_END( COL ) ) {} else            \
if( HBOOST_TEST_FE_ANY HBOOST_TEST_FE_END_VAR = HBOOST_TEST_FE_BEG( COL ) ) {} else            \
for( bool HBOOST_TEST_FE_CON_VAR = true;                                                     \
          HBOOST_TEST_FE_CON_VAR && !HBOOST_TEST_FE_DONE( COL ); )                            \
                                                                                            \
    if( (HBOOST_TEST_FE_CON_VAR = false, false) ) {} else                                    \
    if( (HBOOST_TEST_FE_PREV( COL ), false) ) {} else                                        \
    for( RefType var = HBOOST_TEST_FE_DEREF( COL, RefType );                                 \
         !HBOOST_TEST_FE_CON_VAR; HBOOST_TEST_FE_CON_VAR = true )                             \
/**/

//____________________________________________________________________________//

} // namespace for_each
} // namespace unit_test
} // namespace hboost

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_UTILS_FOREACH_HPP
