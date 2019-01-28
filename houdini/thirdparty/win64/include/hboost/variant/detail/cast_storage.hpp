//-----------------------------------------------------------------------------
// boost variant/detail/cast_storage.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_DETAIL_CAST_STORAGE_HPP
#define HBOOST_VARIANT_DETAIL_CAST_STORAGE_HPP

#include "hboost/config.hpp"

namespace hboost {
namespace detail { namespace variant {

///////////////////////////////////////////////////////////////////////////////
// (detail) function template cast_storage
//
// Casts the given storage to the specified type, but with qualification.
//

template <typename T>
inline T& cast_storage(void* storage)
{
    return *static_cast<T*>(storage);
}

template <typename T>
inline const T& cast_storage(const void* storage)
{
    return *static_cast<const T*>(storage);
}

}} // namespace detail::variant
} // namespace hboost

#endif // HBOOST_VARIANT_DETAIL_CAST_STORAGE_HPP
