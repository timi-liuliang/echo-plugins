/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_CLEAR_10022005_1442)
#define HBOOST_FUSION_CLEAR_10022005_1442

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/vector_fwd.hpp>
#include <hboost/fusion/container/list/list_fwd.hpp>
#include <hboost/fusion/container/map/map_fwd.hpp>
#include <hboost/fusion/container/set/set_fwd.hpp>
#include <hboost/fusion/container/deque/deque_fwd.hpp>

namespace hboost { namespace fusion
{
    struct cons_tag;
    struct map_tag;
    struct set_tag;
    struct vector_tag;
    struct deque_tag;

    namespace detail
    {
        template <typename Tag>
        struct clear;

        template <>
        struct clear<cons_tag> : mpl::identity<list<> > {};

        template <>
        struct clear<map_tag> : mpl::identity<map<> > {};

        template <>
        struct clear<set_tag> : mpl::identity<set<> > {};

        template <>
        struct clear<vector_tag> : mpl::identity<vector<> > {};

        template <>
        struct clear<deque_tag> : mpl::identity<deque<> > {};
    }
}}

#endif
