// Boost.Range library
//
//  Copyright Thorsten Ottosen 2006. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_DETAIL_AS_LITERAL_HPP
#define HBOOST_RANGE_DETAIL_AS_LITERAL_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/range/detail/detail_str.hpp>
#include <hboost/range/iterator_range.hpp>

namespace hboost
{
    template< class Range >
    inline iterator_range<HBOOST_DEDUCED_TYPENAME range_iterator<Range>::type> 
    as_literal( Range& r )
    {
        return ::hboost::make_iterator_range( ::hboost::range_detail::str_begin(r),
                                             ::hboost::range_detail::str_end(r) );
    }

}

#endif
