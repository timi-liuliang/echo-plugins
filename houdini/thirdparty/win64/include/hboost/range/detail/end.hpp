// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_DETAIL_END_HPP
#define HBOOST_RANGE_DETAIL_END_HPP

#include <hboost/config.hpp> // HBOOST_MSVC
#include <hboost/detail/workaround.hpp>

#include <hboost/range/detail/implementation_help.hpp>
#include <hboost/range/iterator.hpp>
#include <hboost/range/detail/common.hpp>

namespace hboost
{
    namespace range_detail
    {
        template< typename T >
        struct range_end;

        //////////////////////////////////////////////////////////////////////
        // default
        //////////////////////////////////////////////////////////////////////

        template<>
        struct range_end<std_container_>
        {
            template< typename C >
            static HBOOST_RANGE_DEDUCED_TYPENAME range_iterator<C>::type
            fun( C& c )
            {
                return c.end();
            };
        };

        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////

        template<>
        struct range_end<std_pair_>
        {
            template< typename P >
            static HBOOST_RANGE_DEDUCED_TYPENAME range_iterator<P>::type
            fun( const P& p )
            {
                return p.second;
            }
        };

        //////////////////////////////////////////////////////////////////////
        // array
        //////////////////////////////////////////////////////////////////////

        template<>
        struct range_end<array_>
        {
            template<typename T>
            static HBOOST_RANGE_DEDUCED_TYPENAME remove_extent<T>::type* fun(T& t)
            {
                return t + remove_extent<T>::size;
            }
        };

    } // namespace 'range_detail'

    namespace range_adl_barrier
    {
        template< typename C >
        inline HBOOST_RANGE_DEDUCED_TYPENAME range_iterator<C>::type
        end( C& c )
        {
            return range_detail::range_end< HBOOST_RANGE_DEDUCED_TYPENAME range_detail::range<C>::type >::fun( c );
        }
    } // namespace range_adl_barrier

} // namespace 'boost'

#endif
