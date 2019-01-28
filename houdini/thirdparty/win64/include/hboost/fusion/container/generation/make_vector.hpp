/*=============================================================================
    Copyright (c) 2014-2015 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_FUSION_MAKE_VECTOR_11112014_2252
#define HBOOST_FUSION_MAKE_VECTOR_11112014_2252

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/vector.hpp>

#if !defined(HBOOST_FUSION_HAS_VARIADIC_VECTOR)
# include <hboost/fusion/container/generation/detail/pp_make_vector.hpp>
#else

///////////////////////////////////////////////////////////////////////////////
// C++11 variadic interface
///////////////////////////////////////////////////////////////////////////////

#include <hboost/fusion/support/detail/as_fusion_element.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/type_traits/remove_const.hpp>
#include <utility>

namespace hboost { namespace fusion
{
    namespace result_of
    {
        template <typename ...T>
        struct make_vector
        {
            typedef vector<
                typename detail::as_fusion_element<
                    typename remove_const<
                        typename remove_reference<T>::type
                    >::type
                >::type...
            > type;
        };
    }

    template <typename ...T>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::make_vector<T...>::type
    make_vector(T&&... arg)
    {
        return typename result_of::make_vector<T...>::type(std::forward<T>(arg)...);
    }
 }}


#endif
#endif

