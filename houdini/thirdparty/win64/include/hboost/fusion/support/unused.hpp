/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_SUPPORT_UNUSED_20070305_1038)
#define HBOOST_FUSION_SUPPORT_UNUSED_20070305_1038

#include <hboost/fusion/support/config.hpp>
#include <iosfwd>

#include <hboost/config.hpp>
#if defined(HBOOST_MSVC)
# pragma warning(push)
# pragma warning(disable: 4522) // multiple assignment operators specified warning
#endif

#define HBOOST_FUSION_UNUSED_HAS_IO

namespace hboost { namespace fusion
{
    struct unused_type
    {
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        unused_type() HBOOST_NOEXCEPT
        {
        }

        template <typename T>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        unused_type(T const&) HBOOST_NOEXCEPT
        {
        }

        template <typename T>
        HBOOST_FUSION_CONSTEXPR_THIS HBOOST_FUSION_GPU_ENABLED
        unused_type const&
        operator=(T const&) const HBOOST_NOEXCEPT
        {
            return *this;
        }

        template <typename T>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        unused_type&
        operator=(T const&) HBOOST_NOEXCEPT
        {
            return *this;
        }

        HBOOST_FUSION_CONSTEXPR_THIS HBOOST_FUSION_GPU_ENABLED
        unused_type const&
        operator=(unused_type const&) const HBOOST_NOEXCEPT
        {
            return *this;
        }

        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        unused_type&
        operator=(unused_type const&) HBOOST_NOEXCEPT
        {
            return *this;
        }
    };

    HBOOST_CONSTEXPR_OR_CONST unused_type unused = unused_type();

    namespace detail
    {
        struct unused_only
        {
            HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
            unused_only(unused_type const&) HBOOST_NOEXCEPT {}
        };
    }

    HBOOST_CONSTEXPR
    inline std::ostream& operator<<(std::ostream& out, detail::unused_only const&) HBOOST_NOEXCEPT
    {
        return out;
    }

    HBOOST_CONSTEXPR
    inline std::istream& operator>>(std::istream& in, unused_type&) HBOOST_NOEXCEPT
    {
        return in;
    }
}}

#if defined(HBOOST_MSVC)
# pragma warning(pop)
#endif

#endif
