//  cast_ptr.hpp  --------------------------------------------------------------//

//  Copyright 2015 Andrey Semashev

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef HBOOST_DETAIL_WINAPI_DETAIL_CAST_PTR_HPP
#define HBOOST_DETAIL_WINAPI_DETAIL_CAST_PTR_HPP

#include <hboost/detail/winapi/config.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

namespace hboost {
namespace detail {
namespace winapi {
namespace detail {

//! This class is used to automatically cast pointers to the type used in the current Windows SDK function declarations
class cast_ptr
{
private:
    const void* m_p;

public:
    explicit HBOOST_FORCEINLINE cast_ptr(const void* p) HBOOST_NOEXCEPT : m_p(p) {}
    template< typename T >
    HBOOST_FORCEINLINE operator T* () const HBOOST_NOEXCEPT { return (T*)m_p; }
};

}
}
}
}

#endif // HBOOST_DETAIL_WINAPI_DETAIL_CAST_PTR_HPP
