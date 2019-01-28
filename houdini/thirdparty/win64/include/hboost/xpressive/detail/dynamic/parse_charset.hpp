///////////////////////////////////////////////////////////////////////////////
// parse_charset.hpp
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_XPRESSIVE_DETAIL_DYNAMIC_PARSE_CHARSET_HPP_EAN_10_04_2005
#define HBOOST_XPRESSIVE_DETAIL_DYNAMIC_PARSE_CHARSET_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/config.hpp>
#include <hboost/integer.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/throw_exception.hpp>
#include <hboost/numeric/conversion/converter.hpp>
#include <hboost/xpressive/detail/detail_fwd.hpp>
#include <hboost/xpressive/detail/dynamic/parser_enum.hpp>
#include <hboost/xpressive/detail/utility/literals.hpp>
#include <hboost/xpressive/detail/utility/chset/chset.hpp>
#include <hboost/xpressive/regex_constants.hpp>

namespace hboost { namespace xpressive { namespace detail
{

enum escape_type
{
    escape_char
  , escape_mark
  , escape_class
};

///////////////////////////////////////////////////////////////////////////////
// escape_value
//
template<typename Char, typename Class>
struct escape_value
{
    Char ch_;
    int mark_nbr_;
    Class class_;
    escape_type type_;
};

///////////////////////////////////////////////////////////////////////////////
// char_overflow_handler
//
struct char_overflow_handler
{
    void operator ()(numeric::range_check_result result) const // throw(regex_error)
    {
        if(numeric::cInRange != result)
        {
            HBOOST_THROW_EXCEPTION(
                regex_error(
                    regex_constants::error_escape
                  , "character escape too large to fit in target character type"
                )
            );
        }
    }
};

///////////////////////////////////////////////////////////////////////////////
// parse_escape
//
template<typename FwdIter, typename CompilerTraits>
escape_value<typename iterator_value<FwdIter>::type, typename CompilerTraits::regex_traits::char_class_type>
parse_escape(FwdIter &begin, FwdIter end, CompilerTraits &tr)
{
    using namespace regex_constants;
    typedef typename iterator_value<FwdIter>::type char_type;
    typedef typename CompilerTraits::regex_traits regex_traits;
    typedef typename regex_traits::char_class_type char_class_type;

    // define an unsigned type the same size as char_type
    typedef typename hboost::uint_t<CHAR_BIT * sizeof(char_type)>::least uchar_t;
    HBOOST_MPL_ASSERT_RELATION(sizeof(uchar_t), ==, sizeof(char_type));
    typedef numeric::conversion_traits<uchar_t, int> converstion_traits;

    HBOOST_XPR_ENSURE_(begin != end, error_escape, "unexpected end of pattern found");
    numeric::converter<int, uchar_t, converstion_traits, char_overflow_handler> converter;
    escape_value<char_type,char_class_type> esc = { 0, 0, 0, escape_char };
    bool const icase = (0 != (regex_constants::icase_ & tr.flags()));
    regex_traits const &rxtraits = tr.traits();
    FwdIter tmp;

    esc.class_ = rxtraits.lookup_classname(begin, begin + 1, icase);
    if(0 != esc.class_)
    {
        esc.type_ = escape_class;
        return esc;
    }

    if(-1 != rxtraits.value(*begin, 8))
    {
        esc.ch_ = converter(toi(begin, end, rxtraits, 8, 0777));
        return esc;
    }

    switch(*begin)
    {
    // bell character
    case HBOOST_XPR_CHAR_(char_type, 'a'):
        esc.ch_ = HBOOST_XPR_CHAR_(char_type, '\a');
        ++begin;
        break;
    // escape character
    case HBOOST_XPR_CHAR_(char_type, 'e'):
        esc.ch_ = converter(27);
        ++begin;
        break;
    // control character
    case HBOOST_XPR_CHAR_(char_type, 'c'):
        HBOOST_XPR_ENSURE_(++begin != end, error_escape, "unexpected end of pattern found");
        HBOOST_XPR_ENSURE_
        (
            rxtraits.in_range(HBOOST_XPR_CHAR_(char_type, 'a'), HBOOST_XPR_CHAR_(char_type, 'z'), *begin)
         || rxtraits.in_range(HBOOST_XPR_CHAR_(char_type, 'A'), HBOOST_XPR_CHAR_(char_type, 'Z'), *begin)
          , error_escape
          , "invalid escape control letter; must be one of a-z or A-Z"
        );
        // Convert to character according to ECMA-262, section 15.10.2.10:
        esc.ch_ = converter(*begin % 32);
        ++begin;
        break;
    // formfeed character
    case HBOOST_XPR_CHAR_(char_type, 'f'):
        esc.ch_ = HBOOST_XPR_CHAR_(char_type, '\f');
        ++begin;
        break;
    // newline
    case HBOOST_XPR_CHAR_(char_type, 'n'):
        esc.ch_ = HBOOST_XPR_CHAR_(char_type, '\n');
        ++begin;
        break;
    // return
    case HBOOST_XPR_CHAR_(char_type, 'r'):
        esc.ch_ = HBOOST_XPR_CHAR_(char_type, '\r');
        ++begin;
        break;
    // horizontal tab
    case HBOOST_XPR_CHAR_(char_type, 't'):
        esc.ch_ = HBOOST_XPR_CHAR_(char_type, '\t');
        ++begin;
        break;
    // vertical tab
    case HBOOST_XPR_CHAR_(char_type, 'v'):
        esc.ch_ = HBOOST_XPR_CHAR_(char_type, '\v');
        ++begin;
        break;
    // hex escape sequence
    case HBOOST_XPR_CHAR_(char_type, 'x'):
        HBOOST_XPR_ENSURE_(++begin != end, error_escape, "unexpected end of pattern found");
        tmp = begin;
        esc.ch_ = converter(toi(begin, end, rxtraits, 16, 0xff));
        HBOOST_XPR_ENSURE_(2 == std::distance(tmp, begin), error_escape, "invalid hex escape : "
            "must be \\x HexDigit HexDigit");
        break;
    // Unicode escape sequence
    case HBOOST_XPR_CHAR_(char_type, 'u'):
        HBOOST_XPR_ENSURE_(++begin != end, error_escape, "unexpected end of pattern found");
        tmp = begin;
        esc.ch_ = converter(toi(begin, end, rxtraits, 16, 0xffff));
        HBOOST_XPR_ENSURE_(4 == std::distance(tmp, begin), error_escape, "invalid Unicode escape : "
            "must be \\u HexDigit HexDigit HexDigit HexDigit");
        break;
    // backslash
    case HBOOST_XPR_CHAR_(char_type, '\\'):
        //esc.ch_ = HBOOST_XPR_CHAR_(char_type, '\\');
        //++begin;
        //break;
    // all other escaped characters represent themselves
    default:
        esc.ch_ = *begin;
        ++begin;
        break;
    }

    return esc;
}

//////////////////////////////////////////////////////////////////////////
// parse_charset
//
template<typename FwdIter, typename RegexTraits, typename CompilerTraits>
inline void parse_charset
(
    FwdIter &begin
  , FwdIter end
  , compound_charset<RegexTraits> &chset
  , CompilerTraits &tr
)
{
    using namespace regex_constants;
    typedef typename RegexTraits::char_type char_type;
    typedef typename RegexTraits::char_class_type char_class_type;
    HBOOST_XPR_ENSURE_(begin != end, error_brack, "unexpected end of pattern found");
    RegexTraits const &rxtraits = tr.traits();
    bool const icase = (0 != (regex_constants::icase_ & tr.flags()));
    FwdIter iprev = FwdIter();
    escape_value<char_type, char_class_type> esc = {0, 0, 0, escape_char};
    bool invert = false;

    // check to see if we have an inverse charset
    if(begin != end && token_charset_invert == tr.get_charset_token(iprev = begin, end))
    {
        begin = iprev;
        invert = true;
    }

    // skip the end token if-and-only-if it is the first token in the charset
    if(begin != end && token_charset_end == tr.get_charset_token(iprev = begin, end))
    {
        for(; begin != iprev; ++begin)
        {
            chset.set_char(*begin, rxtraits, icase);
        }
    }

    compiler_token_type tok;
    char_type ch_prev = char_type(), ch_next = char_type();
    bool have_prev = false;

    HBOOST_XPR_ENSURE_(begin != end, error_brack, "unexpected end of pattern found");

    // remember the current position and grab the next token
    iprev = begin;
    tok = tr.get_charset_token(begin, end);
    do
    {
        HBOOST_XPR_ENSURE_(begin != end, error_brack, "unexpected end of pattern found");

        if(token_charset_hyphen == tok && have_prev)
        {
            // remember the current position
            FwdIter iprev2 = begin;
            have_prev = false;

            // ch_prev is lower bound of a range
            switch(tr.get_charset_token(begin, end))
            {
            case token_charset_hyphen:
            case token_charset_invert:
                begin = iprev2; // un-get these tokens and fall through
                HBOOST_FALLTHROUGH;
            case token_literal:
                ch_next = *begin++;
                HBOOST_XPR_ENSURE_(ch_prev <= ch_next, error_range, "invalid charset range");
                chset.set_range(ch_prev, ch_next, rxtraits, icase);
                continue;
            case token_charset_backspace:
                ch_next = char_type(8); // backspace
                HBOOST_XPR_ENSURE_(ch_prev <= ch_next, error_range, "invalid charset range");
                chset.set_range(ch_prev, ch_next, rxtraits, icase);
                continue;
            case token_escape:
                esc = parse_escape(begin, end, tr);
                if(escape_char == esc.type_)
                {
                    HBOOST_XPR_ENSURE_(ch_prev <= esc.ch_, error_range, "invalid charset range");
                    chset.set_range(ch_prev, esc.ch_, rxtraits, icase);
                    continue;
                }
                HBOOST_FALLTHROUGH;
            case token_charset_end:
            default:                // not a range.
                begin = iprev;      // backup to hyphen token
                chset.set_char(ch_prev, rxtraits, icase);
                chset.set_char(*begin++, rxtraits, icase);
                continue;
            }
        }

        if(have_prev)
        {
            chset.set_char(ch_prev, rxtraits, icase);
            have_prev = false;
        }

        switch(tok)
        {
        case token_charset_hyphen:
        case token_charset_invert:
        case token_charset_end:
        case token_posix_charset_end:
            begin = iprev; // un-get these tokens
            ch_prev = *begin++;
            have_prev = true;
            continue;

        case token_charset_backspace:
            ch_prev = char_type(8); // backspace
            have_prev = true;
            continue;

        case token_posix_charset_begin:
            {
                FwdIter tmp = begin, start = begin;
                bool invert = (token_charset_invert == tr.get_charset_token(tmp, end));
                if(invert)
                {
                    begin = start = tmp;
                }
                while(token_literal == (tok = tr.get_charset_token(begin, end)))
                {
                    tmp = ++begin;
                    HBOOST_XPR_ENSURE_(begin != end, error_brack, "unexpected end of pattern found");
                }
                if(token_posix_charset_end == tok)
                {
                    char_class_type chclass = rxtraits.lookup_classname(start, tmp, icase);
                    HBOOST_XPR_ENSURE_(0 != chclass, error_ctype, "unknown class name");
                    chset.set_class(chclass, invert);
                    continue;
                }
                begin = iprev; // un-get this token
                ch_prev = *begin++;
                have_prev = true;
            }
            continue;

        case token_escape:
            esc = parse_escape(begin, end, tr);
            if(escape_char == esc.type_)
            {
                ch_prev = esc.ch_;
                have_prev = true;
            }
            else if(escape_class == esc.type_)
            {
                char_class_type upper_ = lookup_classname(rxtraits, "upper");
                HBOOST_ASSERT(0 != upper_);
                chset.set_class(esc.class_, rxtraits.isctype(*begin++, upper_));
            }
            else
            {
                HBOOST_ASSERT(false);
            }
            continue;

        default:
            ch_prev = *begin++;
            have_prev = true;
            continue;
        }
    }
    while(HBOOST_XPR_ENSURE_((iprev = begin) != end, error_brack, "unexpected end of pattern found"),
          token_charset_end != (tok = tr.get_charset_token(begin, end)));

    if(have_prev)
    {
        chset.set_char(ch_prev, rxtraits, icase);
    }

    if(invert)
    {
        chset.inverse();
    }
}

}}} // namespace hboost::xpressive::detail

#endif
