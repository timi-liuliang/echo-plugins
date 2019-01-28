//
// detail/regex_fwd.hpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_REGEX_FWD_HPP
#define HBOOST_ASIO_DETAIL_REGEX_FWD_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#if defined(HBOOST_ASIO_HAS_HBOOST_REGEX)

#include <hboost/regex_fwd.hpp>
#include <hboost/regex/v4/match_flags.hpp>

namespace hboost {

template <class BidiIterator>
struct sub_match;

template <class BidiIterator, class Allocator>
class match_results;

} // namespace hboost

#endif // defined(HBOOST_ASIO_HAS_HBOOST_REGEX)

#endif // HBOOST_ASIO_DETAIL_REGEX_FWD_HPP
