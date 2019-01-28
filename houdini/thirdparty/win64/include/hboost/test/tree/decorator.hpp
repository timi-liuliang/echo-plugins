//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision: 62016 $
//
//  Description : defines decorators to be using with auto registered test units
// ***************************************************************************

#ifndef HBOOST_TEST_TREE_DECORATOR_HPP_091911GER
#define HBOOST_TEST_TREE_DECORATOR_HPP_091911GER

// Boost.Test
#include <hboost/test/detail/config.hpp>
#include <hboost/test/detail/global_typedef.hpp>

#include <hboost/test/tree/fixture.hpp>

#include <hboost/test/tools/assertion_result.hpp>

#include <hboost/test/utils/basic_cstring/basic_cstring.hpp>
#include <hboost/test/utils/trivial_singleton.hpp>

// Boost
#include <hboost/shared_ptr.hpp>
#include <hboost/function/function0.hpp>
#include <hboost/function/function1.hpp>

#include <hboost/test/detail/suppress_warnings.hpp>

// STL
#include <vector>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {

class test_unit;

namespace decorator {

// ************************************************************************** //
// **************             decorator::collector             ************** //
// ************************************************************************** //

class base;
typedef hboost::shared_ptr<base> base_ptr;

class HBOOST_TEST_DECL collector : public singleton<collector> {
public:
    collector&              operator*( base const& d );

    void                    store_in( test_unit& tu );

    void                    reset();

private:
    HBOOST_TEST_SINGLETON_CONS( collector )

    // Data members
    std::vector<base_ptr>   m_tu_decorators;
};

// ************************************************************************** //
// **************               decorator::base                ************** //
// ************************************************************************** //

class HBOOST_TEST_DECL base {
public:
    // composition interface
    collector&              operator*() const;

    // application interface
    virtual void            apply( test_unit& tu ) = 0;

    // deep cloning interface
    virtual base_ptr        clone() const = 0;

protected:
    virtual ~base() {}
};

// ************************************************************************** //
// **************               decorator::label               ************** //
// ************************************************************************** //

class HBOOST_TEST_DECL label : public decorator::base {
public:
    explicit                label( const_string l ) : m_label( l ) {}

private:
    // decorator::base interface
    virtual void            apply( test_unit& tu );
    virtual base_ptr        clone() const { return base_ptr(new label( m_label )); }

    // Data members
    const_string            m_label;
};

// ************************************************************************** //
// **************         decorator::expected_failures         ************** //
// ************************************************************************** //

class HBOOST_TEST_DECL expected_failures : public decorator::base {
public:
    explicit                expected_failures( counter_t ef ) : m_exp_fail( ef ) {}

private:
    // decorator::base interface
    virtual void            apply( test_unit& tu );
    virtual base_ptr        clone() const { return base_ptr(new expected_failures( m_exp_fail )); }

    // Data members
    counter_t               m_exp_fail;
};

// ************************************************************************** //
// **************              decorator::timeout              ************** //
// ************************************************************************** //

class HBOOST_TEST_DECL timeout : public decorator::base {
public:
    explicit                timeout( unsigned t ) : m_timeout( t ) {}

private:
    // decorator::base interface
    virtual void            apply( test_unit& tu );
    virtual base_ptr        clone() const { return base_ptr(new timeout( m_timeout )); }

    // Data members
    unsigned                m_timeout;
};

// ************************************************************************** //
// **************            decorator::description            ************** //
// ************************************************************************** //

class HBOOST_TEST_DECL description : public decorator::base {
public:
    explicit                description( const_string descr ) : m_description( descr ) {}

private:
    // decorator::base interface
    virtual void            apply( test_unit& tu );
    virtual base_ptr        clone() const { return base_ptr(new description( m_description )); }

    // Data members
    const_string            m_description;
};

// ************************************************************************** //
// **************            decorator::depends_on             ************** //
// ************************************************************************** //

class HBOOST_TEST_DECL depends_on : public decorator::base {
public:
    explicit                depends_on( const_string dependency ) : m_dependency( dependency ) {}

private:
    // decorator::base interface
    virtual void            apply( test_unit& tu );
    virtual base_ptr        clone() const { return base_ptr(new depends_on( m_dependency )); }

    // Data members
    const_string            m_dependency;
};

// ************************************************************************** //
// **************    decorator::enable_if/enabled/disabled     ************** //
// ************************************************************************** //

class HBOOST_TEST_DECL enable_if_impl : public decorator::base {
protected:
    void                    apply_impl( test_unit& tu, bool condition );
};

template<bool condition>
class enable_if : public enable_if_impl {
private:
    // decorator::base interface
    virtual void            apply( test_unit& tu )   { this->apply_impl( tu, condition ); }
    virtual base_ptr        clone() const            { return base_ptr(new enable_if<condition>()); }
};

typedef enable_if<true> enabled;
typedef enable_if<false> disabled;

// ************************************************************************** //
// **************              decorator::fixture              ************** //
// ************************************************************************** //

class HBOOST_TEST_DECL fixture_t : public decorator::base {
public:
    // Constructor
    explicit                fixture_t( test_unit_fixture_ptr impl ) : m_impl( impl ) {}

private:
    // decorator::base interface
    virtual void            apply( test_unit& tu );
    virtual base_ptr        clone() const { return base_ptr(new fixture_t( m_impl )); }

    // Data members
    test_unit_fixture_ptr m_impl;
};

//____________________________________________________________________________//

template<typename F>
inline fixture_t
fixture()
{
    return fixture_t( test_unit_fixture_ptr( new unit_test::class_based_fixture<F>() ) );
}

//____________________________________________________________________________//

template<typename F, typename Arg>
inline fixture_t
fixture( Arg const& arg )
{
    return fixture_t( test_unit_fixture_ptr( new unit_test::class_based_fixture<F,Arg>( arg ) ) );
}

//____________________________________________________________________________//

inline fixture_t
fixture( hboost::function<void()> const& setup, hboost::function<void()> const& teardown = hboost::function<void()>() )
{
    return fixture_t( test_unit_fixture_ptr( new unit_test::function_based_fixture( setup, teardown ) ) );
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************            decorator::depends_on             ************** //
// ************************************************************************** //

class HBOOST_TEST_DECL precondition : public decorator::base {
public:
    typedef hboost::function<test_tools::assertion_result (test_unit_id)>   predicate_t;

    explicit                precondition( predicate_t p ) : m_precondition( p ) {}

private:
    // decorator::base interface
    virtual void            apply( test_unit& tu );
    virtual base_ptr        clone() const { return base_ptr(new precondition( m_precondition )); }

    // Data members
    predicate_t             m_precondition;
};

} // namespace decorator

using decorator::label;
using decorator::expected_failures;
using decorator::timeout;
using decorator::description;
using decorator::depends_on;
using decorator::enable_if;
using decorator::enabled;
using decorator::disabled;
using decorator::fixture;
using decorator::precondition;

} // namespace unit_test
} // namespace hboost

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_TREE_DECORATOR_HPP_091911GER
