// Copyright Kevlin Henney, 2000-2005.
// Copyright Alexander Nasonov, 2006-2010.
// Copyright Antony Polukhin, 2011-2014.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// what:  lexical_cast custom keyword cast
// who:   contributed by Kevlin Henney,
//        enhanced with contributions from Terje Slettebo,
//        with additional fixes and suggestions from Gennaro Prota,
//        Beman Dawes, Dave Abrahams, Daryle Walker, Peter Dimov,
//        Alexander Nasonov, Antony Polukhin, Justin Viiret, Michael Hofmann,
//        Cheng Yang, Matthew Bradbury, David W. Birdsall, Pavel Korzh and other Boosters
// when:  November 2000, March 2003, June 2005, June 2006, March 2011 - 2014

#ifndef HBOOST_LEXICAL_CAST_DETAIL_IS_CHARACTER_HPP
#define HBOOST_LEXICAL_CAST_DETAIL_IS_CHARACTER_HPP

#include <hboost/config.hpp>
#ifdef HBOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <hboost/mpl/bool.hpp>
#include <hboost/type_traits/is_same.hpp>

namespace hboost {

    namespace detail // is_character<...>
    {
        // returns true, if T is one of the character types
        template < typename T >
        struct is_character
        {
            typedef HBOOST_DEDUCED_TYPENAME hboost::mpl::bool_<
                    hboost::is_same< T, char >::value ||
                    #if !defined(HBOOST_NO_STRINGSTREAM) && !defined(HBOOST_NO_STD_WSTRING)
                        hboost::is_same< T, wchar_t >::value ||
                    #endif
                    #ifndef HBOOST_NO_CXX11_CHAR16_T
                        hboost::is_same< T, char16_t >::value ||
                    #endif
                    #ifndef HBOOST_NO_CXX11_CHAR32_T
                        hboost::is_same< T, char32_t >::value ||
                    #endif
                   	hboost::is_same< T, unsigned char >::value ||
                   	hboost::is_same< T, signed char >::value
            > type;

            HBOOST_STATIC_CONSTANT(bool, value = (type::value) );
        };
    }
}

#endif // HBOOST_LEXICAL_CAST_DETAIL_IS_CHARACTER_HPP

