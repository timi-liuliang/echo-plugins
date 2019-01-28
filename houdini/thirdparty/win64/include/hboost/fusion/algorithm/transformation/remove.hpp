/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_REMOVE_07162005_0818)
#define HBOOST_FUSION_REMOVE_07162005_0818

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/view/filter_view/filter_view.hpp>
#include <hboost/mpl/not.hpp>
#include <hboost/type_traits/is_same.hpp>

namespace hboost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence, typename T>
        struct remove
        {
            typedef filter_view<Sequence, mpl::not_<is_same<mpl::_, T> > > type;
        };
    }

    template <typename T, typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::remove<Sequence const, T>::type
    remove(Sequence const& seq)
    {
        typedef typename result_of::remove<Sequence const, T>::type result_type;
        return result_type(seq);
    }
}}

#endif

