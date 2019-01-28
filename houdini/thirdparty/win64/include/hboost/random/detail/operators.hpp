/* boost random/detail/operators.hpp header file
 *
 * Copyright Steven Watanabe 2010-2011
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org for most recent version including documentation.
 *
 * $Id$
 */

#ifndef HBOOST_RANDOM_DETAIL_OPERATORS_HPP
#define HBOOST_RANDOM_DETAIL_OPERATORS_HPP

#include <hboost/random/detail/config.hpp>
#include <hboost/detail/workaround.hpp>

#if HBOOST_WORKAROUND(HBOOST_MSVC, <= 1310)   \
    || HBOOST_WORKAROUND(__SUNPRO_CC, HBOOST_TESTED_AT(0x5100))

#define HBOOST_RANDOM_DETAIL_OSTREAM_OPERATOR(os, T, t)                  \
    template<class CharT, class Traits>                                 \
    friend std::basic_ostream<CharT,Traits>&                            \
    operator<<(std::basic_ostream<CharT,Traits>& os, const T& t) {      \
        t.print(os, t);                                                 \
        return os;                                                      \
    }                                                                   \
    template<class CharT, class Traits>                                 \
    static std::basic_ostream<CharT,Traits>&                            \
    print(std::basic_ostream<CharT,Traits>& os, const T& t)

#define HBOOST_RANDOM_DETAIL_ISTREAM_OPERATOR(is, T, t)                  \
    template<class CharT, class Traits>                                 \
    friend std::basic_istream<CharT,Traits>&                            \
    operator>>(std::basic_istream<CharT,Traits>& is, T& t) {            \
        t.read(is, t);                                                  \
        return is;                                                      \
    }                                                                   \
    template<class CharT, class Traits>                                 \
    static std::basic_istream<CharT,Traits>&                            \
    read(std::basic_istream<CharT,Traits>& is, T& t)

#endif

#if defined(__BORLANDC__)

#define HBOOST_RANDOM_DETAIL_EQUALITY_OPERATOR(T, lhs, rhs)              \
    bool operator==(const T& rhs) const                                 \
    { return T::is_equal(*this, rhs); }                                 \
    static bool is_equal(const T& lhs, const T& rhs)

#define HBOOST_RANDOM_DETAIL_INEQUALITY_OPERATOR(T)                      \
    bool operator!=(const T& rhs) const                                 \
    { return !T::is_equal(*this, rhs); }

#endif

#ifndef HBOOST_RANDOM_DETAIL_OSTREAM_OPERATOR
#define HBOOST_RANDOM_DETAIL_OSTREAM_OPERATOR(os, T, t)                  \
    template<class CharT, class Traits>                                 \
    friend std::basic_ostream<CharT,Traits>&                            \
    operator<<(std::basic_ostream<CharT,Traits>& os, const T& t)
#endif

#ifndef HBOOST_RANDOM_DETAIL_ISTREAM_OPERATOR
#define HBOOST_RANDOM_DETAIL_ISTREAM_OPERATOR(is, T, t)                  \
    template<class CharT, class Traits>                                 \
    friend std::basic_istream<CharT,Traits>&                            \
    operator>>(std::basic_istream<CharT,Traits>& is, T& t)
#endif

#ifndef HBOOST_RANDOM_DETAIL_EQUALITY_OPERATOR
#define HBOOST_RANDOM_DETAIL_EQUALITY_OPERATOR(T, lhs, rhs)              \
    friend bool operator==(const T& lhs, const T& rhs)
#endif

#ifndef HBOOST_RANDOM_DETAIL_INEQUALITY_OPERATOR
#define HBOOST_RANDOM_DETAIL_INEQUALITY_OPERATOR(T)                      \
    friend bool operator!=(const T& lhs, const T& rhs)                  \
    { return !(lhs == rhs); }
#endif

#endif