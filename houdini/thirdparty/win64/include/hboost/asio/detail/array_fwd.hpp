//
// detail/array_fwd.hpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_ARRAY_FWD_HPP
#define HBOOST_ASIO_DETAIL_ARRAY_FWD_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

namespace hboost {

template<class T, std::size_t N>
class array;

} // namespace hboost

// Standard library components can't be forward declared, so we'll have to
// include the array header. Fortunately, it's fairly lightweight and doesn't
// add significantly to the compile time.
#if defined(HBOOST_ASIO_HAS_STD_ARRAY)
# include <array>
#endif // defined(HBOOST_ASIO_HAS_STD_ARRAY)

#endif // HBOOST_ASIO_DETAIL_ARRAY_FWD_HPP
