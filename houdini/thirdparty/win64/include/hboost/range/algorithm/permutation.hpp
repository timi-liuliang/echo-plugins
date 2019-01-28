//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_PERMUTATION_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_PERMUTATION_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function next_permutation
///
/// range-based version of the next_permutation std algorithm
///
/// \pre BidirectionalRange is a model of the BidirectionalRangeConcept
/// \pre Compare is a model of the BinaryPredicateConcept
template<class BidirectionalRange>
inline bool next_permutation(BidirectionalRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<BidirectionalRange> ));
    return std::next_permutation(hboost::begin(rng), hboost::end(rng));
}

/// \overload
template<class BidirectionalRange>
inline bool next_permutation(const BidirectionalRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<const BidirectionalRange> ));
    return std::next_permutation(hboost::begin(rng), hboost::end(rng));
}

/// \overload
template<class BidirectionalRange, class Compare>
inline bool next_permutation(BidirectionalRange& rng, Compare comp_pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<BidirectionalRange> ));
    return std::next_permutation(hboost::begin(rng), hboost::end(rng),
                                 comp_pred);
}

/// \overload
template<class BidirectionalRange, class Compare>
inline bool next_permutation(const BidirectionalRange& rng,
                             Compare                   comp_pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<const BidirectionalRange> ));
    return std::next_permutation(hboost::begin(rng), hboost::end(rng),
                                 comp_pred);
}

/// \brief template function prev_permutation
///
/// range-based version of the prev_permutation std algorithm
///
/// \pre BidirectionalRange is a model of the BidirectionalRangeConcept
/// \pre Compare is a model of the BinaryPredicateConcept
template<class BidirectionalRange>
inline bool prev_permutation(BidirectionalRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<BidirectionalRange> ));
    return std::prev_permutation(hboost::begin(rng), hboost::end(rng));
}

/// \overload
template<class BidirectionalRange>
inline bool prev_permutation(const BidirectionalRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<const BidirectionalRange> ));
    return std::prev_permutation(hboost::begin(rng), hboost::end(rng));
}

/// \overload
template<class BidirectionalRange, class Compare>
inline bool prev_permutation(BidirectionalRange& rng, Compare comp_pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<BidirectionalRange> ));
    return std::prev_permutation(hboost::begin(rng), hboost::end(rng),
                                 comp_pred);
}

/// \overload
template<class BidirectionalRange, class Compare>
inline bool prev_permutation(const BidirectionalRange& rng,
                             Compare                   comp_pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<const BidirectionalRange> ));
    return std::prev_permutation(hboost::begin(rng), hboost::end(rng),
                                 comp_pred);
}

    } // namespace range
    using range::next_permutation;
    using range::prev_permutation;
} // namespace hboost

#endif // include guard