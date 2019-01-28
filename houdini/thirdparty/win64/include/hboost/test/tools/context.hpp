//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision: 74248 $
//
//  Description : test tools context interfaces
// ***************************************************************************

#ifndef HBOOST_TEST_TOOLS_CONTEXT_HPP_111712GER
#define HBOOST_TEST_TOOLS_CONTEXT_HPP_111712GER

// Boost.Test
#include <hboost/test/utils/lazy_ostream.hpp>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace test_tools {
namespace tt_detail {

// ************************************************************************** //
// **************                 context_frame                ************** //
// ************************************************************************** //

struct HBOOST_TEST_DECL context_frame {
    explicit    context_frame( ::hboost::unit_test::lazy_ostream const& context_descr );
    ~context_frame();

    operator    bool();

private:
    // Data members
    int         m_frame_id;
};

//____________________________________________________________________________//

#define HBOOST_TEST_INFO( context_descr )                                                        \
    ::hboost::unit_test::framework::add_context( HBOOST_TEST_LAZY_MSG( context_descr ) , false )  \
/**/

//____________________________________________________________________________//

#define HBOOST_TEST_CONTEXT( context_descr )                                                     \
    if( ::hboost::test_tools::tt_detail::context_frame HBOOST_JOIN( context_frame_, __LINE__ ) =  \
        ::hboost::test_tools::tt_detail::context_frame( HBOOST_TEST_LAZY_MSG( context_descr ) ) ) \
/**/

//____________________________________________________________________________//

} // namespace tt_detail
} // namespace test_tools
} // namespace hboost

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_TOOLS_CONTEXT_HPP_111712GER
