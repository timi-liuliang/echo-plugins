//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_UNIQUE_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_UNIQUE_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/range/detail/range_return.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function unique
///
/// range-based version of the unique std algorithm
///
/// \pre Rng meets the requirements for a Forward range
template< range_return_value re, class ForwardRange >
inline HBOOST_DEDUCED_TYPENAME range_return<ForwardRange,re>::type
unique( ForwardRange& rng )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    return range_return<ForwardRange,re>::
        pack( std::unique( hboost::begin(rng),
                           hboost::end(rng)), rng );
}

/// \overload
template< range_return_value re, class ForwardRange >
inline HBOOST_DEDUCED_TYPENAME range_return<const ForwardRange,re>::type
unique( const ForwardRange& rng )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return range_return<const ForwardRange,re>::
        pack( std::unique( hboost::begin(rng),
                           hboost::end(rng)), rng );
}
/// \overload
template< range_return_value re, class ForwardRange, class BinaryPredicate >
inline HBOOST_DEDUCED_TYPENAME range_return<ForwardRange,re>::type
unique( ForwardRange& rng, BinaryPredicate pred )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    return range_return<ForwardRange,re>::
        pack(std::unique(hboost::begin(rng), hboost::end(rng), pred),
             rng);
}
/// \overload
template< range_return_value re, class ForwardRange, class BinaryPredicate >
inline HBOOST_DEDUCED_TYPENAME range_return<const ForwardRange,re>::type
unique( const ForwardRange& rng, BinaryPredicate pred )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return range_return<const ForwardRange,re>::
        pack(std::unique(hboost::begin(rng), hboost::end(rng), pred),
             rng);
}

/// \overload
template< class ForwardRange >
inline HBOOST_DEDUCED_TYPENAME range_return<ForwardRange, return_begin_found>::type
unique( ForwardRange& rng )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    return ::hboost::range::unique<return_begin_found>(rng);
}
/// \overload
template< class ForwardRange >
inline HBOOST_DEDUCED_TYPENAME range_return<const ForwardRange, return_begin_found>::type
unique( const ForwardRange& rng )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return ::hboost::range::unique<return_begin_found>(rng);
}
/// \overload
template< class ForwardRange, class BinaryPredicate >
inline HBOOST_DEDUCED_TYPENAME range_return<ForwardRange, return_begin_found>::type
unique( ForwardRange& rng, BinaryPredicate pred )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    return ::hboost::range::unique<return_begin_found>(rng, pred);
}
/// \overload
template< class ForwardRange, class BinaryPredicate >
inline HBOOST_DEDUCED_TYPENAME range_return<const ForwardRange, return_begin_found>::type
unique( const ForwardRange& rng, BinaryPredicate pred )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return ::hboost::range::unique<return_begin_found>(rng, pred);
}

    } // namespace range
    using range::unique;
} // namespace hboost

#endif // include guard
