///////////////////////////////////////////////////////////////////////////////
// sum.hpp
//
//  Copyright 2005 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_ACCUMULATORS_STATISTICS_SUM_HPP_EAN_28_10_2005
#define HBOOST_ACCUMULATORS_STATISTICS_SUM_HPP_EAN_28_10_2005

#include <hboost/mpl/placeholders.hpp>
#include <hboost/accumulators/framework/accumulator_base.hpp>
#include <hboost/accumulators/framework/extractor.hpp>
#include <hboost/accumulators/numeric/functional.hpp>
#include <hboost/accumulators/framework/parameters/sample.hpp>
#include <hboost/accumulators/framework/parameters/weight.hpp>
#include <hboost/accumulators/framework/accumulators/external_accumulator.hpp>
#include <hboost/accumulators/framework/depends_on.hpp>
#include <hboost/accumulators/statistics_fwd.hpp>
#include <hboost/accumulators/statistics/count.hpp>

namespace hboost { namespace accumulators
{

namespace impl
{
    ///////////////////////////////////////////////////////////////////////////////
    // sum_impl
    template<typename Sample, typename Tag>
    struct sum_impl
      : accumulator_base
    {
        // for hboost::result_of
        typedef Sample result_type;

        template<typename Args>
        sum_impl(Args const &args)
          : sum(args[parameter::keyword<Tag>::get() | Sample()])
        {
        }

        template<typename Args>
        void operator ()(Args const &args)
        {
            // what about overflow?
            this->sum += args[parameter::keyword<Tag>::get()];
        }

        result_type result(dont_care) const
        {
            return this->sum;
        }

    private:

        Sample sum;
    };

} // namespace impl

///////////////////////////////////////////////////////////////////////////////
// tag::sum
// tag::sum_of_weights
// tag::sum_of_variates
//
namespace tag
{
    struct sum
      : depends_on<>
    {
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::sum_impl<mpl::_1, tag::sample> impl;
    };

    struct sum_of_weights
      : depends_on<>
    {
        typedef mpl::true_ is_weight_accumulator;
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::sum_impl<mpl::_2, tag::weight> impl;
    };

    template<typename VariateType, typename VariateTag>
    struct sum_of_variates
      : depends_on<>
    {
        /// INTERNAL ONLY
        ///
        typedef mpl::always<accumulators::impl::sum_impl<VariateType, VariateTag> > impl;
    };

    struct abstract_sum_of_variates
      : depends_on<>
    {
    };
}

///////////////////////////////////////////////////////////////////////////////
// extract::sum
// extract::sum_of_weights
// extract::sum_of_variates
//
namespace extract
{
    extractor<tag::sum> const sum = {};
    extractor<tag::sum_of_weights> const sum_of_weights = {};
    extractor<tag::abstract_sum_of_variates> const sum_of_variates = {};

    HBOOST_ACCUMULATORS_IGNORE_GLOBAL(sum)
    HBOOST_ACCUMULATORS_IGNORE_GLOBAL(sum_of_weights)
    HBOOST_ACCUMULATORS_IGNORE_GLOBAL(sum_of_variates)
}

using extract::sum;
using extract::sum_of_weights;
using extract::sum_of_variates;

// So that mean can be automatically substituted with
// weighted_mean when the weight parameter is non-void.
template<>
struct as_weighted_feature<tag::sum>
{
    typedef tag::weighted_sum type;
};

template<>
struct feature_of<tag::weighted_sum>
  : feature_of<tag::sum>
{};

template<typename VariateType, typename VariateTag>
struct feature_of<tag::sum_of_variates<VariateType, VariateTag> >
  : feature_of<tag::abstract_sum_of_variates>
{
};

}} // namespace hboost::accumulators

#endif