#ifndef HBOOST_BIND_PLACEHOLDERS_HPP_INCLUDED
#define HBOOST_BIND_PLACEHOLDERS_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  bind/placeholders.hpp - _N definitions
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//  Copyright 2015 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  See http://www.boost.org/libs/bind/bind.html for documentation.
//

#include <hboost/bind/arg.hpp>
#include <hboost/config.hpp>

namespace hboost
{

namespace placeholders
{

#if defined(__BORLANDC__) || defined(__GNUC__) && (__GNUC__ < 4)

inline hboost::arg<1> _1() { return hboost::arg<1>(); }
inline hboost::arg<2> _2() { return hboost::arg<2>(); }
inline hboost::arg<3> _3() { return hboost::arg<3>(); }
inline hboost::arg<4> _4() { return hboost::arg<4>(); }
inline hboost::arg<5> _5() { return hboost::arg<5>(); }
inline hboost::arg<6> _6() { return hboost::arg<6>(); }
inline hboost::arg<7> _7() { return hboost::arg<7>(); }
inline hboost::arg<8> _8() { return hboost::arg<8>(); }
inline hboost::arg<9> _9() { return hboost::arg<9>(); }

#else

HBOOST_STATIC_CONSTEXPR hboost::arg<1> _1;
HBOOST_STATIC_CONSTEXPR hboost::arg<2> _2;
HBOOST_STATIC_CONSTEXPR hboost::arg<3> _3;
HBOOST_STATIC_CONSTEXPR hboost::arg<4> _4;
HBOOST_STATIC_CONSTEXPR hboost::arg<5> _5;
HBOOST_STATIC_CONSTEXPR hboost::arg<6> _6;
HBOOST_STATIC_CONSTEXPR hboost::arg<7> _7;
HBOOST_STATIC_CONSTEXPR hboost::arg<8> _8;
HBOOST_STATIC_CONSTEXPR hboost::arg<9> _9;

#endif

} // namespace placeholders

} // namespace hboost

#endif // #ifndef HBOOST_BIND_PLACEHOLDERS_HPP_INCLUDED
