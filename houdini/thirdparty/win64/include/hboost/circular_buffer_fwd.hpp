// Forward declaration of the circular buffer and its adaptor.

// Copyright (c) 2003-2008 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See www.boost.org/libs/circular_buffer for documentation.

#if !defined(HBOOST_CIRCULAR_BUFFER_FWD_HPP)
#define HBOOST_CIRCULAR_BUFFER_FWD_HPP

#if defined(_MSC_VER)
    #pragma once
#endif

#include <hboost/config.hpp>
#if !defined(HBOOST_NO_STD_ALLOCATOR)
    #include <memory>
#else
    #include <vector>
#endif

namespace hboost {

#if !defined(HBOOST_NO_STD_ALLOCATOR)
    #define HBOOST_CB_DEFAULT_ALLOCATOR(T) std::allocator<T>
#else
    #define HBOOST_CB_DEFAULT_ALLOCATOR(T) HBOOST_DEDUCED_TYPENAME std::vector<T>::allocator_type
#endif

template <class T, class Alloc = HBOOST_CB_DEFAULT_ALLOCATOR(T)>
class circular_buffer;

template <class T, class Alloc = HBOOST_CB_DEFAULT_ALLOCATOR(T)>
class circular_buffer_space_optimized;

#undef HBOOST_CB_DEFAULT_ALLOCATOR

} // namespace hboost

#endif // #if !defined(HBOOST_CIRCULAR_BUFFER_FWD_HPP)
