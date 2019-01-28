//
// detail/cstdint.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_CSTDINT_HPP
#define HBOOST_ASIO_DETAIL_CSTDINT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_CSTDINT)
# include <cstdint>
#else // defined(HBOOST_ASIO_HAS_CSTDINT)
# include <hboost/cstdint.hpp>
#endif // defined(HBOOST_ASIO_HAS_CSTDINT)

namespace hboost {
namespace asio {

#if defined(HBOOST_ASIO_HAS_CSTDINT)
using std::int16_t;
using std::uint16_t;
using std::int32_t;
using std::uint32_t;
using std::int64_t;
using std::uint64_t;
#else // defined(HBOOST_ASIO_HAS_CSTDINT)
using hboost::int16_t;
using hboost::uint16_t;
using hboost::int32_t;
using hboost::uint32_t;
using hboost::int64_t;
using hboost::uint64_t;
#endif // defined(HBOOST_ASIO_HAS_CSTDINT)

} // namespace asio
} // namespace hboost

#endif // HBOOST_ASIO_DETAIL_CSTDINT_HPP
