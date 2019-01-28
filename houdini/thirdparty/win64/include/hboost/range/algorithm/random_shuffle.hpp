//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_RANDOM_SHUFFLE_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_RANDOM_SHUFFLE_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function random_shuffle
///
/// range-based version of the random_shuffle std algorithm
///
/// \pre RandomAccessRange is a model of the RandomAccessRangeConcept
/// \pre Generator is a model of the UnaryFunctionConcept
template<class RandomAccessRange>
inline RandomAccessRange& random_shuffle(RandomAccessRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( RandomAccessRangeConcept<RandomAccessRange> ));
    std::random_shuffle(hboost::begin(rng), hboost::end(rng));
    return rng;
}

/// \overload
template<class RandomAccessRange>
inline const RandomAccessRange& random_shuffle(const RandomAccessRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( RandomAccessRangeConcept<const RandomAccessRange> ));
    std::random_shuffle(hboost::begin(rng), hboost::end(rng));
    return rng;
}

/// \overload
template<class RandomAccessRange, class Generator>
inline RandomAccessRange& random_shuffle(RandomAccessRange& rng, Generator& gen)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( RandomAccessRangeConcept<RandomAccessRange> ));
    std::random_shuffle(hboost::begin(rng), hboost::end(rng), gen);
    return rng;
}

/// \overload
template<class RandomAccessRange, class Generator>
inline const RandomAccessRange& random_shuffle(const RandomAccessRange& rng, Generator& gen)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( RandomAccessRangeConcept<const RandomAccessRange> ));
    std::random_shuffle(hboost::begin(rng), hboost::end(rng), gen);
    return rng;
}

    } // namespace range
    using range::random_shuffle;
} // namespace hboost

#endif // include guard