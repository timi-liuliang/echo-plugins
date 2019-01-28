///////////////////////////////////////////////////////////////////////////////
// count.hpp
//
//  Copyright 2005 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_ACCUMULATORS_STATISTICS_COUNT_HPP_EAN_28_10_2005
#define HBOOST_ACCUMULATORS_STATISTICS_COUNT_HPP_EAN_28_10_2005

#include <hboost/mpl/always.hpp>
#include <hboost/accumulators/framework/accumulator_base.hpp>
#include <hboost/accumulators/framework/extractor.hpp>
#include <hboost/accumulators/framework/depends_on.hpp>
#include <hboost/accumulators/statistics_fwd.hpp>

namespace hboost { namespace accumulators
{

namespace impl
{

    ///////////////////////////////////////////////////////////////////////////////
    // count_impl
    struct count_impl
      : accumulator_base
    {
        // for hboost::result_of
        typedef std::size_t result_type;

        count_impl(dont_care)
          : cnt(0)
        {
        }

        void operator ()(dont_care)
        {
            ++this->cnt;
        }

        result_type result(dont_care) const
        {
            return this->cnt;
        }

    private:
        std::size_t cnt;
    };

} // namespace impl

///////////////////////////////////////////////////////////////////////////////
// tag::count
//
namespace tag
{
    struct count
      : depends_on<>
    {
        /// INTERNAL ONLY
        ///
        typedef mpl::always<accumulators::impl::count_impl> impl;
    };
}

///////////////////////////////////////////////////////////////////////////////
// extract::count
//
namespace extract
{
    extractor<tag::count> const count = {};

    HBOOST_ACCUMULATORS_IGNORE_GLOBAL(count)
}

using extract::count;

}} // namespace hboost::accumulators

#endif
