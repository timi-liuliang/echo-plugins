//  Copyright Neil Groves 2014. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_DETAIL_COMBINE_CXX11_HPP
#define HBOOST_RANGE_DETAIL_COMBINE_CXX11_HPP

#include <hboost/range/iterator_range_core.hpp>
#include <hboost/range/iterator.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/iterator/zip_iterator.hpp>

#include <fstream>

namespace hboost
{
    namespace range
    {

template<typename... Ranges>
auto combine(Ranges&&... rngs) ->
    combined_range<decltype(hboost::make_tuple(hboost::begin(rngs)...))>
{
    return combined_range<decltype(hboost::make_tuple(hboost::begin(rngs)...))>(
                hboost::make_tuple(hboost::begin(rngs)...),
                hboost::make_tuple(hboost::end(rngs)...));
}

    } // namespace range

using range::combine;

} // namespace hboost

#endif // include guard
