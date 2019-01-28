/*
(c) 2014 Glen Joseph Fernandes
<glenjofe -at- gmail.com>

Distributed under the Boost Software
License, Version 1.0.
http://boost.org/LICENSE_1_0.txt
*/
#ifndef HBOOST_ALIGN_DETAIL_IS_ALIGNMENT_HPP
#define HBOOST_ALIGN_DETAIL_IS_ALIGNMENT_HPP

#include <hboost/config.hpp>
#include <cstddef>

namespace hboost {
namespace alignment {
namespace detail {

HBOOST_CONSTEXPR inline bool is_alignment(std::size_t value)
    HBOOST_NOEXCEPT
{
    return (value > 0) && ((value & (value - 1)) == 0);
}

} /* .detail */
} /* .alignment */
} /* .boost */

#endif
