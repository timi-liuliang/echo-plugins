//
// detail/assert.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_ASSERT_HPP
#define HBOOST_ASIO_DETAIL_ASSERT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_HBOOST_ASSERT)
# include <hboost/assert.hpp>
#else // defined(HBOOST_ASIO_HAS_HBOOST_ASSERT)
# include <cassert>
#endif // defined(HBOOST_ASIO_HAS_HBOOST_ASSERT)

#if defined(HBOOST_ASIO_HAS_HBOOST_ASSERT)
# define HBOOST_ASIO_ASSERT(expr) HBOOST_ASSERT(expr)
#else // defined(HBOOST_ASIO_HAS_HBOOST_ASSERT)
# define HBOOST_ASIO_ASSERT(expr) assert(expr)
#endif // defined(HBOOST_ASIO_HAS_HBOOST_ASSERT)

#endif // HBOOST_ASIO_DETAIL_ASSERT_HPP
