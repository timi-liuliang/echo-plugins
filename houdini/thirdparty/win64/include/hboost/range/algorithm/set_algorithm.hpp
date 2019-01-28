//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_SET_ALGORITHM_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_SET_ALGORITHM_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function includes
///
/// range-based version of the includes std algorithm
///
/// \pre SinglePassRange1 is a model of the SinglePassRangeConcept
/// \pre SinglePassRange2 is a model of the SinglePassRangeConcept
/// \pre BinaryPredicate is a model of the BinaryPredicateConcept
template<class SinglePassRange1, class SinglePassRange2>
inline bool includes(const SinglePassRange1& rng1,
                     const SinglePassRange2& rng2)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange1> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange2> ));
    return std::includes(hboost::begin(rng1),hboost::end(rng1),
                         hboost::begin(rng2),hboost::end(rng2));
}

/// \overload
template<class SinglePassRange1, class SinglePassRange2,
         class BinaryPredicate>
inline bool includes(const SinglePassRange1& rng1,
                     const SinglePassRange2& rng2,
                     BinaryPredicate         pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange1> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange2> ));
    return std::includes(hboost::begin(rng1), hboost::end(rng1),
                         hboost::begin(rng2), hboost::end(rng2), pred);
}

/// \brief template function set_union
///
/// range-based version of the set_union std algorithm
///
/// \pre SinglePassRange1 is a model of the SinglePassRangeConcept
/// \pre SinglePassRange2 is a model of the SinglePassRangeConcept
/// \pre BinaryPredicate is a model of the BinaryPredicateConcept
template<class SinglePassRange1, class SinglePassRange2,
         class OutputIterator>
inline OutputIterator set_union(const SinglePassRange1& rng1,
                                const SinglePassRange2& rng2,
                                OutputIterator          out)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange1> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange2> ));
    return std::set_union(hboost::begin(rng1), hboost::end(rng1),
                          hboost::begin(rng2), hboost::end(rng2), out);
}

/// \overload
template<class SinglePassRange1, class SinglePassRange2,
         class OutputIterator, class BinaryPredicate>
inline OutputIterator set_union(const SinglePassRange1& rng1,
                                const SinglePassRange2& rng2,
                                OutputIterator          out,
                                BinaryPredicate         pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange1> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange2> ));
    return std::set_union(hboost::begin(rng1), hboost::end(rng1),
                          hboost::begin(rng2), hboost::end(rng2), out, pred);
}

/// \brief template function set_intersection
///
/// range-based version of the set_intersection std algorithm
///
/// \pre SinglePassRange1 is a model of the SinglePassRangeConcept
/// \pre SinglePassRange2 is a model of the SinglePassRangeConcept
/// \pre BinaryPredicate is a model of the BinaryPredicateConcept
template<class SinglePassRange1, class SinglePassRange2,
         class OutputIterator>
inline OutputIterator set_intersection(const SinglePassRange1& rng1,
                                       const SinglePassRange2& rng2,
                                       OutputIterator          out)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange1> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange2> ));
    return std::set_intersection(hboost::begin(rng1), hboost::end(rng1),
                                 hboost::begin(rng2), hboost::end(rng2), out);
}

/// \overload
template<class SinglePassRange1, class SinglePassRange2,
         class OutputIterator, class BinaryPredicate>
inline OutputIterator set_intersection(const SinglePassRange1& rng1,
                                       const SinglePassRange2& rng2,
                                       OutputIterator          out,
                                       BinaryPredicate         pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange1> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange2> ));
    return std::set_intersection(hboost::begin(rng1), hboost::end(rng1),
                                 hboost::begin(rng2), hboost::end(rng2),
                                 out, pred);
}

/// \brief template function set_difference
///
/// range-based version of the set_difference std algorithm
///
/// \pre SinglePassRange1 is a model of the SinglePassRangeConcept
/// \pre SinglePassRange2 is a model of the SinglePassRangeConcept
/// \pre BinaryPredicate is a model of the BinaryPredicateConcept
template<class SinglePassRange1, class SinglePassRange2,
         class OutputIterator>
inline OutputIterator set_difference(const SinglePassRange1& rng1,
                                     const SinglePassRange2& rng2,
                                     OutputIterator out)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange1> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange2> ));
    return std::set_difference(hboost::begin(rng1), hboost::end(rng1),
                               hboost::begin(rng2), hboost::end(rng2), out);
}

/// \overload
template<class SinglePassRange1, class SinglePassRange2,
         class OutputIterator, class BinaryPredicate>
inline OutputIterator set_difference(const SinglePassRange1& rng1,
                                     const SinglePassRange2& rng2,
                                     OutputIterator          out,
                                     BinaryPredicate         pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange1> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange2> ));
    return std::set_difference(
        hboost::begin(rng1), hboost::end(rng1),
        hboost::begin(rng2), hboost::end(rng2), out, pred);
}

/// \brief template function set_symmetric_difference
///
/// range-based version of the set_symmetric_difference std algorithm
///
/// \pre SinglePassRange1 is a model of the SinglePassRangeConcept
/// \pre SinglePassRange2 is a model of the SinglePassRangeConcept
/// \pre BinaryPredicate is a model of the BinaryPredicateConcept
template<class SinglePassRange1, class SinglePassRange2,
         class OutputIterator>
inline OutputIterator
set_symmetric_difference(const SinglePassRange1& rng1,
                         const SinglePassRange2& rng2,
                         OutputIterator          out)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange1> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange2> ));
    return std::set_symmetric_difference(hboost::begin(rng1), hboost::end(rng1),
                                         hboost::begin(rng2), hboost::end(rng2), out);
}

/// \overload
template<class SinglePassRange1, class SinglePassRange2,
         class OutputIterator, class BinaryPredicate>
inline OutputIterator
set_symmetric_difference(const SinglePassRange1& rng1,
                         const SinglePassRange2& rng2,
                         OutputIterator          out,
                         BinaryPredicate         pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange1> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange2> ));
    return std::set_symmetric_difference(
        hboost::begin(rng1), hboost::end(rng1),
        hboost::begin(rng2), hboost::end(rng2), out, pred);
}

    } // namespace range
    using range::includes;
    using range::set_union;
    using range::set_intersection;
    using range::set_difference;
    using range::set_symmetric_difference;
} // namespace hboost

#endif // include guard
