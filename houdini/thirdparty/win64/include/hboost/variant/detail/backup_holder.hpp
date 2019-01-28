//-----------------------------------------------------------------------------
// boost variant/detail/backup_holder.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_DETAIL_BACKUP_HOLDER_HPP
#define HBOOST_VARIANT_DETAIL_BACKUP_HOLDER_HPP

#include "hboost/config.hpp"
#include "hboost/assert.hpp"

namespace hboost {
namespace detail { namespace variant {

template <typename T>
class backup_holder
{
private: // representation

    T* backup_;

public: // structors

    ~backup_holder() HBOOST_NOEXCEPT
    {
        delete backup_;
    }

    explicit backup_holder(T* backup) HBOOST_NOEXCEPT
        : backup_(backup)
    {
    }

    backup_holder(const backup_holder&);

public: // modifiers

    backup_holder& operator=(const backup_holder& rhs)
    {
        *backup_ = rhs.get();
        return *this;
    }

    backup_holder& operator=(const T& rhs)
    {
        *backup_ = rhs;
        return *this;
    }

    void swap(backup_holder& rhs) HBOOST_NOEXCEPT
    {
        T* tmp = rhs.backup_;
        rhs.backup_ = this->backup_;
        this->backup_ = tmp;
    }

public: // queries

    T& get() HBOOST_NOEXCEPT
    {
        return *backup_;
    }

    const T& get() const HBOOST_NOEXCEPT
    {
        return *backup_;
    }

};

template <typename T>
backup_holder<T>::backup_holder(const backup_holder&)
    : backup_(0)
{
    // not intended for copy, but do not want to prohibit syntactically
    HBOOST_ASSERT(false);
}

template <typename T>
void swap(backup_holder<T>& lhs, backup_holder<T>& rhs) HBOOST_NOEXCEPT
{
    lhs.swap(rhs);
}

}} // namespace detail::variant
} // namespace hboost

#endif // HBOOST_VARIANT_DETAIL_BACKUP_HOLDER_HPP
