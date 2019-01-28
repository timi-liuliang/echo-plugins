//
// detail/shared_ptr.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_SHARED_PTR_HPP
#define HBOOST_ASIO_DETAIL_SHARED_PTR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_STD_SHARED_PTR)
# include <memory>
#else // defined(HBOOST_ASIO_HAS_STD_SHARED_PTR)
# include <hboost/shared_ptr.hpp>
#endif // defined(HBOOST_ASIO_HAS_STD_SHARED_PTR)

namespace hboost {
namespace asio {
namespace detail {

#if defined(HBOOST_ASIO_HAS_STD_SHARED_PTR)
using std::shared_ptr;
#else // defined(HBOOST_ASIO_HAS_STD_SHARED_PTR)
using hboost::shared_ptr;
#endif // defined(HBOOST_ASIO_HAS_STD_SHARED_PTR)

} // namespace detail
} // namespace asio
} // namespace hboost

#endif // HBOOST_ASIO_DETAIL_SHARED_PTR_HPP
