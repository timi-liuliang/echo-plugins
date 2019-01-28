// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_END_HPP
#define HBOOST_RANGE_END_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/range/config.hpp>

#ifdef HBOOST_NO_FUNCTION_TEMPLATE_ORDERING
#include <hboost/range/detail/end.hpp>
#else

#include <hboost/range/detail/implementation_help.hpp>
#include <hboost/range/iterator.hpp>
#include <hboost/range/const_iterator.hpp>

namespace hboost
{

#if !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
namespace range_detail
{
#endif

        //////////////////////////////////////////////////////////////////////
        // primary template
        //////////////////////////////////////////////////////////////////////
        template< typename C >
        inline HBOOST_DEDUCED_TYPENAME range_iterator<C>::type
        range_end( C& c )
        {
            //
            // If you get a compile-error here, it is most likely because
            // you have not implemented range_begin() properly in
            // the namespace of C
            //
            return c.end();
        }

        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////

        template< typename Iterator >
        inline Iterator range_end( const std::pair<Iterator,Iterator>& p )
        {
            return p.second;
        }

        template< typename Iterator >
        inline Iterator range_end( std::pair<Iterator,Iterator>& p )
        {
            return p.second;
        }

        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////

        template< typename T, std::size_t sz >
        inline const T* range_end( const T (&a)[sz] )
        {
            return range_detail::array_end<T,sz>( a );
        }

        template< typename T, std::size_t sz >
        inline T* range_end( T (&a)[sz] )
        {
            return range_detail::array_end<T,sz>( a );
        }

#if !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
} // namespace 'range_detail'
#endif

namespace range_adl_barrier
{

template< class T >
inline HBOOST_DEDUCED_TYPENAME range_iterator<T>::type end( T& r )
{
#if !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
    using namespace range_detail;
#endif
    return range_end( r );
}

template< class T >
inline HBOOST_DEDUCED_TYPENAME range_iterator<const T>::type end( const T& r )
{
#if !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
    using namespace range_detail;
#endif
    return range_end( r );
}

    } // namespace range_adl_barrier
} // namespace 'boost'

#endif // HBOOST_NO_FUNCTION_TEMPLATE_ORDERING

namespace hboost
{
    namespace range_adl_barrier
    {
        template< class T >
        inline HBOOST_DEDUCED_TYPENAME range_iterator<const T>::type
        const_end( const T& r )
        {
            return hboost::range_adl_barrier::end( r );
        }
    } // namespace range_adl_barrier
    using namespace range_adl_barrier;
} // namespace hboost

#endif

