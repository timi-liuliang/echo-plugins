/*=============================================================================
    Copyright (c) 2014 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_FUSION_MAKE_TUPLE_14122014_0048
#define HBOOST_FUSION_MAKE_TUPLE_14122014_0048

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/tuple/tuple_fwd.hpp>

///////////////////////////////////////////////////////////////////////////////
// With no variadics, we will use the C++03 version
///////////////////////////////////////////////////////////////////////////////
#if !defined(HBOOST_FUSION_HAS_VARIADIC_TUPLE)
# include <hboost/fusion/tuple/detail/make_tuple.hpp>
#else

///////////////////////////////////////////////////////////////////////////////
// C++11 interface
///////////////////////////////////////////////////////////////////////////////
#include <hboost/fusion/support/detail/as_fusion_element.hpp>
#include <hboost/fusion/tuple/tuple.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/type_traits/remove_const.hpp>

namespace hboost { namespace fusion
{
    template <typename ...T>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline tuple<typename detail::as_fusion_element<
        typename remove_const<
            typename remove_reference<T>::type
        >::type
    >::type...>
    make_tuple(T&&... arg)
    {
        typedef tuple<typename detail::as_fusion_element<
            typename remove_const<
                typename remove_reference<T>::type
            >::type
        >::type...> result_type;
        return result_type(std::forward<T>(arg)...);
    }
}}

#endif
#endif

