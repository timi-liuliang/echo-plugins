//
// Copyright (c) Antony Polukhin, 2013-2014.
//
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_TYPE_INDEX_CTTI_REGISTER_CLASS_HPP
#define HBOOST_TYPE_INDEX_CTTI_REGISTER_CLASS_HPP

/// \file ctti_register_class.hpp
/// \brief Contains HBOOST_TYPE_INDEX_REGISTER_CLASS macro implementation that uses hboost::typeindex::ctti_type_index.
/// Not intended for inclusion from user's code.

#include <hboost/type_index/ctti_type_index.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

namespace hboost { namespace typeindex { namespace detail {

template <class T>
inline const ctti_data& ctti_construct_typeid_ref(const T*) HBOOST_NOEXCEPT {
    return ctti_construct<T>();
}

}}} // namespace hboost::typeindex::detail

/// @cond
#define HBOOST_TYPE_INDEX_REGISTER_CLASS                                                                             \
    virtual const hboost::typeindex::detail::ctti_data& hboost_type_index_type_id_runtime_() const HBOOST_NOEXCEPT {   \
        return hboost::typeindex::detail::ctti_construct_typeid_ref(this);                                           \
    }                                                                                                               \
/**/
/// @endcond

#endif // HBOOST_TYPE_INDEX_CTTI_REGISTER_CLASS_HPP

