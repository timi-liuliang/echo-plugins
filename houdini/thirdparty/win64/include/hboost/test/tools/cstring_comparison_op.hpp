//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//!@file
//!@brief C string comparison with enhanced reporting
// ***************************************************************************

#ifndef HBOOST_TEST_TOOLS_CSTRING_COMPARISON_OP_HPP_050815GER
#define HBOOST_TEST_TOOLS_CSTRING_COMPARISON_OP_HPP_050815GER

// Boost.Test
#include <hboost/test/tools/assertion.hpp>

#include <hboost/test/utils/is_cstring.hpp>
#include <hboost/test/utils/basic_cstring/compare.hpp>

// Boost
#include <hboost/utility/enable_if.hpp>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace test_tools {
namespace assertion {
namespace op {



// ************************************************************************** //
// **************               string_compare                 ************** //
// ************************************************************************** //

#define DEFINE_CSTRING_COMPARISON( oper, name, rev )                \
template<typename Lhs,typename Rhs>                                 \
struct name<Lhs,Rhs,typename hboost::enable_if_c<                    \
    unit_test::is_cstring<Lhs>::value &&                            \
    unit_test::is_cstring<Rhs>::value>::type> {                     \
    typedef typename hboost::add_const<                              \
                typename remove_pointer<                            \
                    typename decay<Lhs>::type>::type>::type         \
        lhs_char_type;                                              \
    typedef typename hboost::add_const<                              \
                typename remove_pointer<                            \
                    typename decay<Rhs>::type>::type>::type         \
        rhs_char_type;                                              \
public:                                                             \
    typedef assertion_result result_type;                           \
                                                                    \
    static bool                                                     \
    eval( Lhs const& lhs, Rhs const& rhs)                           \
    {                                                               \
        return unit_test::basic_cstring<lhs_char_type>(lhs) oper    \
               unit_test::basic_cstring<rhs_char_type>(rhs);        \
    }                                                               \
                                                                    \
    template<typename PrevExprType>                                 \
    static void                                                     \
    report( std::ostream&       ostr,                               \
            PrevExprType const& lhs,                                \
            Rhs const&          rhs)                                \
    {                                                               \
        lhs.report( ostr );                                         \
        ostr << revert()                                            \
             << tt_detail::print_helper( rhs );                     \
    }                                                               \
                                                                    \
    static char const* revert()                                     \
    { return " " #rev " "; }                                        \
};                                                                  \
/**/

HBOOST_TEST_FOR_EACH_COMP_OP( DEFINE_CSTRING_COMPARISON )
#undef DEFINE_CSTRING_COMPARISON

//____________________________________________________________________________//

} // namespace op
} // namespace assertion
} // namespace test_tools
} // namespace hboost

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_TOOLS_CSTRING_COMPARISON_OP_HPP_050815GER

