//
// detail/limits.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_LIMITS_HPP
#define HBOOST_ASIO_DETAIL_LIMITS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_HBOOST_LIMITS)
# include <hboost/limits.hpp>
#else // defined(HBOOST_ASIO_HAS_HBOOST_LIMITS)
# include <limits>
#endif // defined(HBOOST_ASIO_HAS_HBOOST_LIMITS)

#endif // HBOOST_ASIO_DETAIL_LIMITS_HPP
