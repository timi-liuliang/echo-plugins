// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/generate_relation_binder.hpp
/// \brief Define macros to help building the set type of definitions

#ifndef HBOOST_BIMAP_DETAIL_GENERATE_RELATION_BINDER_HPP
#define HBOOST_BIMAP_DETAIL_GENERATE_RELATION_BINDER_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/mpl/apply.hpp>

/*===========================================================================*/
#define HBOOST_BIMAP_GENERATE_RELATION_BINDER_0CP(                             \
                                                                              \
        SET_TYPE_OF                                                           \
    )                                                                         \
                                                                              \
    template< class Relation >                                                \
    struct bind_to                                                            \
    {                                                                         \
        typedef SET_TYPE_OF<Relation> type;                                   \
                                                                              \
    };
/*===========================================================================*/



/*===========================================================================*/
#define HBOOST_BIMAP_GENERATE_RELATION_BINDER_1CP(                             \
                                                                              \
        SET_TYPE_OF,                                                          \
        CP1                                                                   \
    )                                                                         \
                                                                              \
    template< class Relation >                                                \
    struct bind_to                                                            \
    {                                                                         \
        typedef SET_TYPE_OF                                                   \
        <                                                                     \
            Relation,                                                         \
            HBOOST_DEDUCED_TYPENAME mpl::apply<CP1,                            \
                HBOOST_DEDUCED_TYPENAME Relation::storage_base >::type         \
                                                                              \
        > type;                                                               \
                                                                              \
    };
/*===========================================================================*/



/*===========================================================================*/
#define HBOOST_BIMAP_GENERATE_RELATION_BINDER_2CP(                             \
                                                                              \
        SET_TYPE_OF,                                                          \
        CP1,                                                                  \
        CP2                                                                   \
    )                                                                         \
                                                                              \
    template< class Relation >                                                \
    struct bind_to                                                            \
    {                                                                         \
        typedef SET_TYPE_OF                                                   \
        <                                                                     \
            Relation,                                                         \
            HBOOST_DEDUCED_TYPENAME mpl::apply<CP1,                            \
                HBOOST_DEDUCED_TYPENAME Relation::storage_base >::type,        \
            HBOOST_DEDUCED_TYPENAME mpl::apply<CP2,                            \
                HBOOST_DEDUCED_TYPENAME Relation::storage_base >::type         \
                                                                              \
        > type;                                                               \
                                                                              \
    };
/*===========================================================================*/



#endif // HBOOST_BIMAP_DETAIL_GENERATE_RELATION_BINDER_HPP
