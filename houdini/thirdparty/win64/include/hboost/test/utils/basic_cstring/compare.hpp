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
//  Description : class basic_cstring comparisons implementation
// ***************************************************************************

#ifndef HBOOST_TEST_UTILS_BASIC_CSTRING_COMPARE_HPP
#define HBOOST_TEST_UTILS_BASIC_CSTRING_COMPARE_HPP

// Boost.Test
#include <hboost/test/utils/basic_cstring/basic_cstring.hpp>

// STL
#include <functional>
#include <cctype>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

# if defined(HBOOST_NO_STDC_NAMESPACE) && !HBOOST_WORKAROUND(__BORLANDC__, <= 0x570)
namespace std { using ::toupper; }
# endif

namespace hboost {

namespace unit_test {

// ************************************************************************** //
// **************                case_ins_compare              ************** //
// ************************************************************************** //

namespace ut_detail {

template<class CharT>
struct case_ins
{
    static bool         eq( CharT c1, CharT c2 ) { return (std::toupper)( c1 ) == (std::toupper)( c2 ); }
    static bool         lt( CharT c1, CharT c2 ) { return (std::toupper)( c1 ) <  (std::toupper)( c2 ); }

    static int          compare( CharT const* s1, CharT const* s2, std::size_t n )
    {
        for( std::size_t i = 0; i < n; ++i ) {
            if( !eq( s1[i], s2[i] ) )
                return lt( s1[i], s2[i] ) ? -1 : 1;
        }
        return 0;
    }
};

} // namespace ut_detail

// ************************************************************************** //
// **************                  case_ins_eq                 ************** //
// ************************************************************************** //

template<class CharT>
inline bool
case_ins_eq( basic_cstring<CharT> x, basic_cstring<CharT> y )
{
    return x.size() == y.size() && ut_detail::case_ins<CharT>::compare( x.begin(), y.begin(), x.size() ) == 0;
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************                 case_ins_less                ************** //
// ************************************************************************** //

template<class CharT>
class case_ins_less : public std::binary_function<basic_cstring<CharT>,basic_cstring<CharT>,bool>
{
public:
    bool operator()( basic_cstring<CharT> x, basic_cstring<CharT> y ) const
    {
        return x.size() != y.size()
                ? x.size() < y.size()
                : ut_detail::case_ins<CharT>::compare( x.begin(), y.begin(), x.size() ) < 0;
    }
};

//____________________________________________________________________________//

// ************************************************************************** //
// **************                 operators <,>                ************** //
// ************************************************************************** //

template<class CharT>
inline bool
operator <( hboost::unit_test::basic_cstring<CharT> const& x,
            hboost::unit_test::basic_cstring<CharT> const& y )
{
    typedef typename hboost::unit_test::basic_cstring<CharT>::traits_type traits_type;
    return x.size() != y.size()
            ? x.size() < y.size()
            : traits_type::compare( x.begin(), y.begin(), x.size() ) < 0;
}

//____________________________________________________________________________//

template<class CharT>
inline bool
operator <=( hboost::unit_test::basic_cstring<CharT> const& x,
            hboost::unit_test::basic_cstring<CharT> const& y )
{
    return !(y < x);
}

//____________________________________________________________________________//

template<class CharT>
inline bool
operator >( hboost::unit_test::basic_cstring<CharT> const& x,
            hboost::unit_test::basic_cstring<CharT> const& y )
{
    return y < x;
}

//____________________________________________________________________________//

template<class CharT>
inline bool
operator >=( hboost::unit_test::basic_cstring<CharT> const& x,
            hboost::unit_test::basic_cstring<CharT> const& y )
{
    return !(x < y);
}

//____________________________________________________________________________//

} // namespace unit_test

} // namespace hboost

//____________________________________________________________________________//

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_BASIC_CSTRING_COMPARE_HPP_071894GER
