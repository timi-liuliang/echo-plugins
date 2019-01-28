///////////////////////////////////////////////////////////////////////////////
// quant_style.hpp
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_XPRESSIVE_DETAIL_CORE_QUANT_STYLE_HPP_EAN_10_04_2005
#define HBOOST_XPRESSIVE_DETAIL_CORE_QUANT_STYLE_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/config.hpp>
#include <hboost/mpl/has_xxx.hpp>
#include <hboost/xpressive/detail/utility/width.hpp>
#include <hboost/xpressive/detail/detail_fwd.hpp>

namespace hboost { namespace xpressive { namespace detail
{

HBOOST_MPL_HAS_XXX_TRAIT_DEF(is_hboost_xpressive_xpression_)

///////////////////////////////////////////////////////////////////////////////
// is_xpr
//
template<typename Xpr>
struct is_xpr
  : has_is_hboost_xpressive_xpression_<Xpr>
{};

///////////////////////////////////////////////////////////////////////////////
// quant_enum
//
enum quant_enum
{
    quant_none,
    quant_fixed_width,
    quant_variable_width
};

///////////////////////////////////////////////////////////////////////////////
// quant_style
//
template<quant_enum QuantStyle, std::size_t Width = unknown_width::value, bool Pure = true>
struct quant_style
{
    typedef void is_hboost_xpressive_xpression_;

    // Which quantification strategy to use?
    HBOOST_STATIC_CONSTANT(int, quant = QuantStyle);

    // how many characters this matcher consumes
    HBOOST_STATIC_CONSTANT(std::size_t, width = Width);

    // whether this matcher has observable side-effects
    HBOOST_STATIC_CONSTANT(bool, pure = Pure);

    static detail::width get_width()
    {
        return width;
    }
};

#define HBOOST_XPR_QUANT_STYLE(Style, Width, Pure)                               \
    typedef void is_hboost_xpressive_xpression_;                                 \
    HBOOST_STATIC_CONSTANT(int, quant = Style);                                  \
    HBOOST_STATIC_CONSTANT(std::size_t, width = Width);                          \
    HBOOST_STATIC_CONSTANT(bool, pure = Pure);                                   \
    static detail::width get_width() { return width; }                          \
    /**/

//    // Replace transmogrify stupidity with rebindable matchers/placeholders
//#define HBOOST_XPR_IDENTITY_REBIND(TYPE)                                         \/
//    template<typename BidiIter, typename ICase, typename Traits>                \/
//    struct rebind                                                               \/
//    {                                                                           \/
//        typedef TYPE type;                                                      \/
//    };                                                                          \/
//    /**/

///////////////////////////////////////////////////////////////////////////////
// quant_style_none
//  this sub-expression cannot be quantified
typedef quant_style<quant_none> quant_style_none;

///////////////////////////////////////////////////////////////////////////////
// quant_style_fixed_unknown_width
//  this sub-expression is fixed width for the purpose of quantification, but
//  the width cannot be determined at compile time. An example would be the
//  string_matcher or the mark_matcher.
typedef quant_style<quant_fixed_width> quant_style_fixed_unknown_width;

///////////////////////////////////////////////////////////////////////////////
// quant_style_variable_width
//  this sub-expression can match a variable number of characters
typedef quant_style<quant_variable_width> quant_style_variable_width;

///////////////////////////////////////////////////////////////////////////////
// quant_style_fixed_width
//  for when the sub-expression has a fixed width that is known at compile time
template<std::size_t Width>
struct quant_style_fixed_width
  : quant_style<quant_fixed_width, Width>
{
};

///////////////////////////////////////////////////////////////////////////////
// quant_style_assertion
//  a zero-width assertion.
struct quant_style_assertion
  : quant_style<quant_none, 0>
{
};

///////////////////////////////////////////////////////////////////////////////
// quant_type
//
template<typename Matcher>
struct quant_type
  : mpl::int_<Matcher::quant>
{
};

}}} // namespace hboost::xpressive::detail

#endif
