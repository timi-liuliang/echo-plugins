// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_SIZE_HPP
#define HBOOST_RANGE_SIZE_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/range/config.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/size_type.hpp>
#include <hboost/range/detail/has_member_size.hpp>
#include <hboost/assert.hpp>
#include <hboost/cstdint.hpp>
#include <hboost/utility.hpp>

namespace hboost
{
    namespace range_detail
    {

        template<class SinglePassRange>
        inline typename ::hboost::enable_if<
            has_member_size<SinglePassRange>,
            typename range_size<const SinglePassRange>::type
        >::type
        range_calculate_size(const SinglePassRange& rng)
        {
            return rng.size();
        }

        template<class SinglePassRange>
        inline typename disable_if<
            has_member_size<SinglePassRange>,
            typename range_size<const SinglePassRange>::type
        >::type
        range_calculate_size(const SinglePassRange& rng)
        {
            return std::distance(hboost::begin(rng), hboost::end(rng));
        }
    }

    template<class SinglePassRange>
    inline typename range_size<const SinglePassRange>::type
    size(const SinglePassRange& rng)
    {
// Very strange things happen on some compilers that have the range concept
// asserts disabled. This preprocessor condition is clearly redundant on a
// working compiler but is vital for at least some compilers such as clang 4.2
// but only on the Mac!
#if HBOOST_RANGE_ENABLE_CONCEPT_ASSERT == 1
        HBOOST_RANGE_CONCEPT_ASSERT((hboost::SinglePassRangeConcept<SinglePassRange>));
#endif

#if !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564)) && \
    !HBOOST_WORKAROUND(__GNUC__, < 3) \
    /**/
        using namespace range_detail;
#endif

        return range_calculate_size(rng);
    }

} // namespace 'boost'

#endif
