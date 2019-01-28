//  Copyright Neil Groves 2010. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_COUNTING_RANGE_HPP_INCLUDED
#define HBOOST_RANGE_COUNTING_RANGE_HPP_INCLUDED

#include <hboost/config.hpp>
#if HBOOST_MSVC >= 1400
#pragma warning(push)
#pragma warning(disable : 4244)
#endif

#include <hboost/range/iterator_range_core.hpp>
#include <hboost/range/value_type.hpp>
#include <hboost/range/iterator.hpp>
#include <hboost/iterator/counting_iterator.hpp>

namespace hboost
{
    template<class Value>
    inline iterator_range<counting_iterator<Value> >
    counting_range(Value first, Value last)
    {
        typedef counting_iterator<Value> counting_iterator_t;
        typedef iterator_range<counting_iterator_t> result_t;
        return result_t(counting_iterator_t(first),
                        counting_iterator_t(last));
    }

    template<class Range>
    inline iterator_range<
        counting_iterator<
            HBOOST_DEDUCED_TYPENAME range_iterator<const Range>::type
        >
    >
    counting_range(const Range& rng)
    {
        typedef counting_iterator<
            HBOOST_DEDUCED_TYPENAME range_iterator<const Range>::type
        > counting_iterator_t;

        typedef iterator_range<counting_iterator_t> result_t;

        return result_t(counting_iterator_t(hboost::begin(rng)),
                        counting_iterator_t(hboost::end(rng)));
    }

    template<class Range>
    inline iterator_range<
        counting_iterator<
            HBOOST_DEDUCED_TYPENAME range_iterator<Range>::type
        >
    >
    counting_range(Range& rng)
    {
        typedef counting_iterator<
            HBOOST_DEDUCED_TYPENAME range_iterator<Range>::type
        > counting_iterator_t;

        typedef iterator_range<counting_iterator_t> result_t;

        return result_t(counting_iterator_t(hboost::begin(rng)),
                        counting_iterator_t(hboost::end(rng)));
    }
} // namespace hboost

#if HBOOST_MSVC >= 1400
#pragma warning(pop)
#endif

#endif // include guard
