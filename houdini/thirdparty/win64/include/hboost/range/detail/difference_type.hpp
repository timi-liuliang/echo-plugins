// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_DETAIL_DIFFERENCE_TYPE_HPP
#define HBOOST_RANGE_DETAIL_DIFFERENCE_TYPE_HPP

#include <hboost/range/detail/common.hpp>
#include <hboost/iterator/iterator_traits.hpp>

//////////////////////////////////////////////////////////////////////////////
// missing partial specialization  workaround.
//////////////////////////////////////////////////////////////////////////////

namespace hboost 
{
    namespace range_detail 
    {        
        template< typename T >
        struct range_difference_type_;

        template<>
        struct range_difference_type_<std_container_>
        {
            template< typename C >
            struct pts
            {
                typedef HBOOST_DEDUCED_TYPENAME C::difference_type type;
            };
        };

        template<>
        struct range_difference_type_<std_pair_>
        {
            template< typename P >
            struct pts
            {
                typedef HBOOST_RANGE_DEDUCED_TYPENAME hboost::iterator_difference< HBOOST_DEDUCED_TYPENAME P::first_type>::type type;                
            };
        };

        template<>
        struct range_difference_type_<array_>
        {
            template< typename A >
            struct pts
            {
                typedef std::ptrdiff_t type;
            };
        };

        template<>
        struct range_difference_type_<char_array_>
        { 
            template< typename A >
            struct pts
            {
                typedef std::ptrdiff_t type;
            };
        };

        template<>
        struct range_difference_type_<char_ptr_>
        {
            template< typename S >
            struct pts
            {
                typedef std::ptrdiff_t type;
            };         
        };
        
        template<>
        struct range_difference_type_<const_char_ptr_>
        {
            template< typename S >
            struct pts
            {
                typedef std::ptrdiff_t type;
            };         
        };
        
        template<>
        struct range_difference_type_<wchar_t_ptr_>
        {
            template< typename S >
            struct pts
            {
                typedef std::ptrdiff_t type;
            };         
        };
        
        template<>
        struct range_difference_type_<const_wchar_t_ptr_>
        {
            template< typename S >
            struct pts
            {
                typedef std::ptrdiff_t type;
            };         
        };
        
    } 
    
    template< typename C >
    class range_difference
    {
        typedef HBOOST_RANGE_DEDUCED_TYPENAME range_detail::range<C>::type c_type;
    public:
        typedef HBOOST_RANGE_DEDUCED_TYPENAME range_detail::range_difference_type_<c_type>::HBOOST_NESTED_TEMPLATE pts<C>::type type; 
    };

}

#endif

