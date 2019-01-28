///////////////////////////////////////////////////////////////////////////////
// literals.hpp
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_XPRESSIVE_DETAIL_UTILITY_LITERALS_HPP_EAN_10_04_2005
#define HBOOST_XPRESSIVE_DETAIL_UTILITY_LITERALS_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/config.hpp> // for HBOOST_STATIC_CONSTANT
#include <hboost/cstdint.hpp> // for HBOOST_STATIC_CONSTANT
#include <hboost/detail/workaround.hpp>

namespace hboost { namespace xpressive { namespace detail
{

///////////////////////////////////////////////////////////////////////////////
// char_literal
//
template<typename Char, hboost::intmax_t Ch, hboost::intmax_t Wch>
struct char_literal;

template<typename Char, hboost::intmax_t Ch>
struct char_literal<Char, Ch, Ch>
{
    HBOOST_STATIC_CONSTANT(hboost::intmax_t, value = Ch);
};

#ifndef HBOOST_NO_INCLASS_MEMBER_INITIALIZATION
template<typename Char, hboost::intmax_t Ch>
hboost::intmax_t const char_literal<Char, Ch, Ch>::value;
#endif

template<typename Ch>
struct string_literal;

template<>
struct string_literal<char>
{
    static HBOOST_CONSTEXPR char const *pick(char const *cstr, wchar_t const *)
    {
        return cstr;
    }

    static HBOOST_CONSTEXPR char pick(char ch, wchar_t)
    {
        return ch;
    }
};

template<>
struct string_literal<wchar_t>
{
    static HBOOST_CONSTEXPR wchar_t const *pick(char const *, wchar_t const *cstr)
    {
        return cstr;
    }

    static HBOOST_CONSTEXPR wchar_t pick(char, wchar_t ch)
    {
        return ch;
    }
};

#if HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3206))

# define HBOOST_XPR_CHAR_(Char, ch) ch
# define HBOOST_XPR_CSTR_(Char, st) hboost::xpressive::detail::string_literal<Char>::pick(st, L##st)

#else

# define HBOOST_XPR_CHAR_(Char, ch) hboost::xpressive::detail::char_literal<Char, ch, L##ch>::value
# define HBOOST_XPR_CSTR_(Char, st) hboost::xpressive::detail::string_literal<Char>::pick(st, L##st)

#endif

}}} // namespace hboost::xpressive::detail

#endif
