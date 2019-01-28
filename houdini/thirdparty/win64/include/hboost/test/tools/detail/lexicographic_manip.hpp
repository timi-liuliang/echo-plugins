//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! Lexicographic comparison manipulator implementation
// ***************************************************************************

#ifndef HBOOST_TEST_TOOLS_DETAIL_LEXICOGRAPHIC_MANIP_HPP_050815GER
#define HBOOST_TEST_TOOLS_DETAIL_LEXICOGRAPHIC_MANIP_HPP_050815GER

// Boost Test
#include <hboost/test/tools/detail/fwd.hpp>
#include <hboost/test/tools/detail/indirections.hpp>

#include <hboost/test/tools/assertion.hpp>
#include <hboost/test/tools/collection_comparison_op.hpp>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace test_tools {

// ************************************************************************** //
// **************      per element comparison manipulator      ************** //
// ************************************************************************** //

//! Lexicographic comparison manipulator, for containers
struct lexicographic {};

//____________________________________________________________________________//

inline int
operator<<( unit_test::lazy_ostream const&, lexicographic )   { return 0; }

//____________________________________________________________________________//

namespace tt_detail {

template<typename T1, typename T2, typename OP>
inline assertion_result
operator<<(assertion_evaluate_t<assertion::binary_expr<T1,T2,OP> > const& ae, lexicographic )
{
    typedef typename OP::elem_op elem_op;
    return assertion::op::lexicographic_compare<elem_op>( ae.m_e.lhs().value(), ae.m_e.rhs() );
}

//____________________________________________________________________________//

inline check_type
operator<<( assertion_type const&, lexicographic )
{
    return CHECK_BUILT_ASSERTION;
}

//____________________________________________________________________________//

} // namespace tt_detail
} // namespace test_tools
} // namespace hboost

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_TOOLS_DETAIL_LEXICOGRAPHIC_MANIP_HPP_050815GER
