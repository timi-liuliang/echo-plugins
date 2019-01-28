/* boost random/detail/large_arithmetic.hpp header file
 *
 * Copyright Steven Watanabe 2011
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org for most recent version including documentation.
 *
 * $Id$
 */

#ifndef HBOOST_RANDOM_DETAIL_LARGE_ARITHMETIC_HPP
#define HBOOST_RANDOM_DETAIL_LARGE_ARITHMETIC_HPP

#include <hboost/cstdint.hpp>
#include <hboost/integer.hpp>
#include <hboost/limits.hpp>
#include <hboost/random/detail/integer_log2.hpp>

#include <hboost/random/detail/disable_warnings.hpp>

namespace hboost {
namespace random {
namespace detail {

struct div_t {
    hboost::uintmax_t quotient;
    hboost::uintmax_t remainder;
};

inline div_t muldivmod(hboost::uintmax_t a, hboost::uintmax_t b, hboost::uintmax_t m)
{
    const int bits =
        ::std::numeric_limits< ::hboost::uintmax_t>::digits / 2;
    const ::hboost::uintmax_t mask = (::hboost::uintmax_t(1) << bits) - 1;
    typedef ::hboost::uint_t<bits>::fast digit_t;

    int shift = std::numeric_limits< ::hboost::uintmax_t>::digits - 1
        - detail::integer_log2(m);

    a <<= shift;
    m <<= shift;

    digit_t product[4] = { 0, 0, 0, 0 };
    digit_t a_[2] = { digit_t(a & mask), digit_t((a >> bits) & mask) };
    digit_t b_[2] = { digit_t(b & mask), digit_t((b >> bits) & mask) };
    digit_t m_[2] = { digit_t(m & mask), digit_t((m >> bits) & mask) };

    // multiply a * b
    for(int i = 0; i < 2; ++i) {
        digit_t carry = 0;
        for(int j = 0; j < 2; ++j) {
            ::hboost::uint64_t temp = ::hboost::uintmax_t(a_[i]) * b_[j] +
                carry + product[i + j];
            product[i + j] = digit_t(temp & mask);
            carry = digit_t(temp >> bits);
        }
        if(carry != 0) {
            product[i + 2] += carry;
        }
    }

    digit_t quotient[2];

    if(m == 0) {
        div_t result = {
            ((::hboost::uintmax_t(product[3]) << bits) | product[2]),
            ((::hboost::uintmax_t(product[1]) << bits) | product[0]) >> shift,
        };
        return result;
    }

    // divide product / m
    for(int i = 3; i >= 2; --i) {
        ::hboost::uintmax_t temp =
            ::hboost::uintmax_t(product[i]) << bits | product[i - 1];

        digit_t q = digit_t((product[i] == m_[1]) ? mask : temp / m_[1]);

        ::hboost::uintmax_t rem =
            ((temp - ::hboost::uintmax_t(q) * m_[1]) << bits) + product[i - 2];

        ::hboost::uintmax_t diff = m_[0] * ::hboost::uintmax_t(q);

        int error = 0;
        if(diff > rem) {
            if(diff - rem > m) {
                error = 2;
            } else {
                error = 1;
            }
        }
        q -= error;
        rem = rem + error * m - diff;

        quotient[i - 2] = q;
        product[i] = 0;
        product[i-1] = static_cast<digit_t>((rem >> bits) & mask);
        product[i-2] = static_cast<digit_t>(rem & mask);
    }

    div_t result = {
        ((::hboost::uintmax_t(quotient[1]) << bits) | quotient[0]),
        ((::hboost::uintmax_t(product[1]) << bits) | product[0]) >> shift,
    };
    return result;
}

inline hboost::uintmax_t muldiv(hboost::uintmax_t a, hboost::uintmax_t b, hboost::uintmax_t m)
{ return detail::muldivmod(a, b, m).quotient; }

inline hboost::uintmax_t mulmod(hboost::uintmax_t a, hboost::uintmax_t b, hboost::uintmax_t m)
{ return detail::muldivmod(a, b, m).remainder; }

} // namespace detail
} // namespace random
} // namespace hboost

#include <hboost/random/detail/enable_warnings.hpp>

#endif // HBOOST_RANDOM_DETAIL_LARGE_ARITHMETIC_HPP
