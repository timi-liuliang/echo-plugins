//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision: 74640 $
//
//  Description : defines fixture interface and object makers
// ***************************************************************************

#ifndef HBOOST_TEST_TREE_FIXTURE_HPP_100311GER
#define HBOOST_TEST_TREE_FIXTURE_HPP_100311GER

// Boost.Test
#include <hboost/test/detail/config.hpp>

// Boost
#include <hboost/shared_ptr.hpp>
#include <hboost/scoped_ptr.hpp>
#include <hboost/function/function0.hpp>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {

// ************************************************************************** //
// **************               test_unit_fixture              ************** //
// ************************************************************************** //

class HBOOST_TEST_DECL test_unit_fixture {
public:
    virtual ~test_unit_fixture() {}

    // Fixture interface
    virtual void    setup() = 0;
    virtual void    teardown() = 0;
};

typedef shared_ptr<test_unit_fixture> test_unit_fixture_ptr;

// ************************************************************************** //
// **************              class_based_fixture             ************** //
// ************************************************************************** //

template<typename F, typename Arg=void>
class class_based_fixture : public test_unit_fixture {
public:
    // Constructor
    explicit class_based_fixture( Arg const& arg ) : m_inst(), m_arg( arg ) {}

private:
    // Fixture interface
    virtual void    setup()         { m_inst.reset( new F( m_arg ) ); }
    virtual void    teardown()      { m_inst.reset(); }

    // Data members
    scoped_ptr<F>   m_inst;
    Arg             m_arg;
};

//____________________________________________________________________________//

template<typename F>
class class_based_fixture<F,void> : public test_unit_fixture {
public:
    // Constructor
    class_based_fixture() : m_inst( 0 ) {}

private:
    // Fixture interface
    virtual void    setup()         { m_inst.reset( new F ); }
    virtual void    teardown()      { m_inst.reset(); }

    // Data members
    scoped_ptr<F>   m_inst;
};

//____________________________________________________________________________//

// ************************************************************************** //
// **************            function_based_fixture            ************** //
// ************************************************************************** //

class function_based_fixture : public test_unit_fixture {
public:
    // Constructor
    function_based_fixture( hboost::function<void ()> const& setup_, hboost::function<void ()> const& teardown_ )
    : m_setup( setup_ )
    , m_teardown( teardown_ )
    {
    }

private:
    // Fixture interface
    virtual void                setup()     { if( m_setup ) m_setup(); }
    virtual void                teardown()  { if( m_teardown ) m_teardown(); }

    // Data members
    hboost::function<void ()>    m_setup;
    hboost::function<void ()>    m_teardown;
};

} // namespace unit_test
} // namespace hboost

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_TREE_FIXTURE_HPP_100311GER

