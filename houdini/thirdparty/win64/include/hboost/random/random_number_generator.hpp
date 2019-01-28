/* boost random/random_number_generator.hpp header file
 *
 * Copyright Jens Maurer 2000-2001
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org for most recent version including documentation.
 *
 * $Id$
 *
 * Revision history
 *  2001-02-18  moved to individual header files
 */

#ifndef HBOOST_RANDOM_RANDOM_NUMBER_GENERATOR_HPP
#define HBOOST_RANDOM_RANDOM_NUMBER_GENERATOR_HPP

#include <hboost/assert.hpp>
#include <hboost/random/uniform_int_distribution.hpp>

#include <hboost/random/detail/disable_warnings.hpp>

namespace hboost {
namespace random {

/**
 * Instantiations of class template random_number_generator model a
 * RandomNumberGenerator (std:25.2.11 [lib.alg.random.shuffle]). On
 * each invocation, it returns a uniformly distributed integer in
 * the range [0..n).
 *
 * The template parameter IntType shall denote some integer-like value type.
 */
template<class URNG, class IntType = long>
class random_number_generator
{
public:
    typedef URNG base_type;
    typedef IntType argument_type;
    typedef IntType result_type;
    /**
     * Constructs a random_number_generator functor with the given
     * \uniform_random_number_generator as the underlying source of
     * random numbers.
     */
    random_number_generator(base_type& rng) : _rng(rng) {}

    // compiler-generated copy ctor is fine
    // assignment is disallowed because there is a reference member

    /**
     * Returns a value in the range [0, n)
     */
    result_type operator()(argument_type n)
    {
        HBOOST_ASSERT(n > 0);
        return uniform_int_distribution<IntType>(0, n-1)(_rng);
    }

private:
    base_type& _rng;
};

} // namespace random

using random::random_number_generator;

} // namespace hboost

#include <hboost/random/detail/enable_warnings.hpp>

#endif // HBOOST_RANDOM_RANDOM_NUMBER_GENERATOR_HPP
