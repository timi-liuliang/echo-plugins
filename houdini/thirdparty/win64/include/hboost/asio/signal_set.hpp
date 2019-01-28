//
// signal_set.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_SIGNAL_SET_HPP
#define HBOOST_ASIO_SIGNAL_SET_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <hboost/asio/basic_signal_set.hpp>

namespace hboost {
namespace asio {

/// Typedef for the typical usage of a signal set.
typedef basic_signal_set<> signal_set;

} // namespace asio
} // namespace hboost

#endif // HBOOST_ASIO_SIGNAL_SET_HPP
