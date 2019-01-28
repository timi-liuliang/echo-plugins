//  Copyright Neil Groves 2010. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_COMBINE_HPP
#define HBOOST_RANGE_COMBINE_HPP

#include <hboost/config.hpp>
#include <hboost/range/iterator_range_core.hpp>
#include <hboost/iterator/zip_iterator.hpp>

namespace hboost
{
    namespace range
    {

template<typename IterTuple>
class combined_range
        : public iterator_range<zip_iterator<IterTuple> >
{
    typedef iterator_range<zip_iterator<IterTuple> > base;
public:
    combined_range(IterTuple first, IterTuple last)
        : base(first, last)
    {
    }
};

    } // namespace range
} // namespace hboost

#if defined(HBOOST_NO_CXX11_AUTO_DECLARATIONS) || \
    defined(HBOOST_NO_CXX11_DECLTYPE) || \
    defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) || \
    defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)
#   include <hboost/range/detail/combine_cxx03.hpp>
#else
#   include <hboost/range/detail/combine_cxx11.hpp>
#endif

#endif
