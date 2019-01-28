/* boost random/detail/generator_bits.hpp header file
 *
 * Copyright Steven Watanabe 2011
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org for most recent version including documentation.
 *
 * $Id$
 *
 */

#ifndef HBOOST_RANDOM_DETAIL_GENERATOR_BITS_HPP
#define HBOOST_RANDOM_DETAIL_GENERATOR_BITS_HPP

#include <hboost/limits.hpp>

namespace hboost {
namespace random {
namespace detail {

// This is a temporary measure that retains backwards
// compatibility.
template<class URNG>
struct generator_bits {
    static std::size_t value() {
        return std::numeric_limits<typename URNG::result_type>::digits;
    }
};

} // namespace detail
} // namespace random
} // namespace hboost

#endif // HBOOST_RANDOM_DETAIL_GENERATOR_BITS_HPP