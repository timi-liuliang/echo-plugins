/*=============================================================================
    Copyright (c) 2012 Nathan Ridge

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_FUSION_SUPPORT_AS_CONST_HPP
#define HBOOST_FUSION_SUPPORT_AS_CONST_HPP

#include <hboost/config.hpp>
#include <hboost/fusion/support/config.hpp>

namespace hboost { namespace fusion { namespace extension
{
    // A customization point that allows certain wrappers around
    // Fusion sequence elements (e.g. adt_attribute_proxy) to be
    // unwrapped in contexts where the element only needs to be
    // read. The library wraps accesses to Fusion elements in
    // such contexts with calls to this function. Users can
    // specialize this function for their own wrappers.
    template <typename T>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline const T& as_const(const T& obj) HBOOST_NOEXCEPT
    {
        return obj;
    }

}}}

#endif
