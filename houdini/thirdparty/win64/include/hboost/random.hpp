/* boost random.hpp header file
 *
 * Copyright Jens Maurer 2000-2001
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/random for documentation.
 *
 * $Id$
 *
 * Revision history
 *  2000-02-18  portability fixes (thanks to Beman Dawes)
 *  2000-02-21  shuffle_output, inversive_congruential_schrage,
 *              generator_iterator, uniform_smallint
 *  2000-02-23  generic modulus arithmetic helper, removed *_schrage classes,
 *              implemented Streamable and EqualityComparable concepts for 
 *              generators, added Bernoulli distribution and Box-Muller
 *              transform
 *  2000-03-01  cauchy, lognormal, triangle distributions; fixed 
 *              uniform_smallint; renamed gaussian to normal distribution
 *  2000-03-05  implemented iterator syntax for distribution functions
 *  2000-04-21  removed some optimizations for better BCC/MSVC compatibility
 *  2000-05-10  adapted to BCC and MSVC
 *  2000-06-13  incorporated review results
 *  2000-07-06  moved basic templates from namespace detail to random
 *  2000-09-23  warning removals and int64 fixes (Ed Brey)
 *  2000-09-24  added lagged_fibonacci generator (Matthias Troyer)
 *  2001-02-18  moved to individual header files
 */

#ifndef HBOOST_RANDOM_HPP
#define HBOOST_RANDOM_HPP

// generators
#include <hboost/random/additive_combine.hpp>
#include <hboost/random/discard_block.hpp>
#include <hboost/random/independent_bits.hpp>
#include <hboost/random/inversive_congruential.hpp>
#include <hboost/random/lagged_fibonacci.hpp>
#include <hboost/random/linear_congruential.hpp>
#include <hboost/random/linear_feedback_shift.hpp>
#include <hboost/random/mersenne_twister.hpp>
#include <hboost/random/ranlux.hpp>
#include <hboost/random/shuffle_order.hpp>
#include <hboost/random/shuffle_output.hpp>
#include <hboost/random/subtract_with_carry.hpp>
#include <hboost/random/taus88.hpp>
#include <hboost/random/xor_combine.hpp>

// misc
#include <hboost/random/generate_canonical.hpp>
#include <hboost/random/seed_seq.hpp>
#include <hboost/random/random_number_generator.hpp>
#include <hboost/random/variate_generator.hpp>

// distributions
#include <hboost/random/bernoulli_distribution.hpp>
#include <hboost/random/beta_distribution.hpp>
#include <hboost/random/binomial_distribution.hpp>
#include <hboost/random/cauchy_distribution.hpp>
#include <hboost/random/chi_squared_distribution.hpp>
#include <hboost/random/discrete_distribution.hpp>
#include <hboost/random/exponential_distribution.hpp>
#include <hboost/random/extreme_value_distribution.hpp>
#include <hboost/random/fisher_f_distribution.hpp>
#include <hboost/random/gamma_distribution.hpp>
#include <hboost/random/geometric_distribution.hpp>
#include <hboost/random/hyperexponential_distribution.hpp>
#include <hboost/random/laplace_distribution.hpp>
#include <hboost/random/lognormal_distribution.hpp>
#include <hboost/random/negative_binomial_distribution.hpp>
#include <hboost/random/non_central_chi_squared_distribution.hpp>
#include <hboost/random/normal_distribution.hpp>
#include <hboost/random/piecewise_constant_distribution.hpp>
#include <hboost/random/piecewise_linear_distribution.hpp>
#include <hboost/random/poisson_distribution.hpp>
#include <hboost/random/student_t_distribution.hpp>
#include <hboost/random/triangle_distribution.hpp>
#include <hboost/random/uniform_01.hpp>
#include <hboost/random/uniform_int.hpp>
#include <hboost/random/uniform_int_distribution.hpp>
#include <hboost/random/uniform_on_sphere.hpp>
#include <hboost/random/uniform_real.hpp>
#include <hboost/random/uniform_real_distribution.hpp>
#include <hboost/random/uniform_smallint.hpp>
#include <hboost/random/weibull_distribution.hpp>

#include <hboost/random/generate_canonical.hpp>

#endif // HBOOST_RANDOM_HPP
