///////////////////////////////////////////////////////////////////////////////
// accumulator_base.hpp
//
//  Copyright 2005 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_ACCUMULATORS_FRAMEWORK_ACCUMULATORS_BASE_HPP_EAN_28_10_2005
#define HBOOST_ACCUMULATORS_FRAMEWORK_ACCUMULATORS_BASE_HPP_EAN_28_10_2005

#include <hboost/mpl/placeholders.hpp>
#include <hboost/mpl/joint_view.hpp>
#include <hboost/mpl/single_view.hpp>
#include <hboost/mpl/fold.hpp>
#include <hboost/mpl/contains.hpp>
#include <hboost/mpl/empty_sequence.hpp>
#include <hboost/accumulators/framework/accumulator_concept.hpp>

namespace hboost { namespace accumulators
{

namespace detail
{
    typedef void void_;
}

///////////////////////////////////////////////////////////////////////////////
// dont_care
//
struct dont_care
{
    template<typename Args>
    dont_care(Args const &)
    {
    }
};

///////////////////////////////////////////////////////////////////////////////
// accumulator_base
//
struct accumulator_base
{
    // hidden if defined in derived classes
    detail::void_ operator ()(dont_care)
    {
    }

    typedef mpl::false_ is_droppable;

    detail::void_ add_ref(dont_care)
    {
    }

    detail::void_ drop(dont_care)
    {
    }

    detail::void_ on_drop(dont_care)
    {
    }
};

}} // namespace hboost::accumulators

#endif
