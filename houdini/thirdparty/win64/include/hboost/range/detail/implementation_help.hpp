// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_DETAIL_IMPLEMENTATION_HELP_HPP
#define HBOOST_RANGE_DETAIL_IMPLEMENTATION_HELP_HPP

#include <hboost/range/config.hpp>
#include <hboost/range/detail/common.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <cstddef>
#include <string.h>

#ifndef HBOOST_NO_CWCHAR
#include <wchar.h>
#endif

namespace hboost
{
    namespace range_detail
    {
        template <typename T>
        inline void hboost_range_silence_warning( const T& ) { }

        /////////////////////////////////////////////////////////////////////
        // end() help
        /////////////////////////////////////////////////////////////////////

        inline const char* str_end( const char* s, const char* )
        {
            return s + strlen( s );
        }

#ifndef HBOOST_NO_CWCHAR
        inline const wchar_t* str_end( const wchar_t* s, const wchar_t* )
        {
            return s + wcslen( s );
        }
#else
        inline const wchar_t* str_end( const wchar_t* s, const wchar_t* )
        {
            if( s == 0 || s[0] == 0 )
                return s;
            while( *++s != 0 )
                ;
            return s;
        }
#endif

        template< class Char >
        inline Char* str_end( Char* s )
        {
            return const_cast<Char*>( str_end( s, s ) );
        }

        template< class T, std::size_t sz >
        inline T* array_end( T HBOOST_RANGE_ARRAY_REF()[sz] )
        {
            return hboost_range_array + sz;
        }

        template< class T, std::size_t sz >
        inline const T* array_end( const T HBOOST_RANGE_ARRAY_REF()[sz] )
        {
            return hboost_range_array + sz;
        }

        /////////////////////////////////////////////////////////////////////
        // size() help
        /////////////////////////////////////////////////////////////////////

        template< class Char >
        inline std::size_t str_size( const Char* const& s )
        {
            return str_end( s ) - s;
        }

        template< class T, std::size_t sz >
        inline std::size_t array_size( T HBOOST_RANGE_ARRAY_REF()[sz] )
        {
            hboost_range_silence_warning( hboost_range_array );
            return sz;
        }

        template< class T, std::size_t sz >
        inline std::size_t array_size( const T HBOOST_RANGE_ARRAY_REF()[sz] )
        {
            hboost_range_silence_warning( hboost_range_array );
            return sz;
        }

        inline bool is_same_address(const void* l, const void* r)
        {
            return l == r;
        }

        template<class T1, class T2>
        inline bool is_same_object(const T1& l, const T2& r)
        {
            return range_detail::is_same_address(&l, &r);
        }

    } // namespace 'range_detail'

} // namespace 'boost'


#endif
