/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_ERASE_KEY_10022005_1851)
#define HBOOST_FUSION_ERASE_KEY_10022005_1851

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/algorithm/query/find.hpp>
#include <hboost/fusion/algorithm/transformation/erase.hpp>
#include <hboost/mpl/not.hpp>
#include <hboost/type_traits/is_same.hpp>

namespace hboost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence, typename Key>
        struct erase_key
          : erase<Sequence, typename find<Sequence, Key>::type>
        {};
    }

    template <typename Key, typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::erase_key<Sequence const, Key>::type
    erase_key(Sequence const& seq)
    {
        return erase(seq, find<Key>(seq));
    }
}}

#endif

