// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/generate_view_binder.hpp
/// \brief Define macros to help building the set type of definitions

#ifndef HBOOST_BIMAP_DETAIL_GENERATE_VIEW_BINDER_HPP
#define HBOOST_BIMAP_DETAIL_GENERATE_VIEW_BINDER_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/multi_index/tag.hpp>

/*===========================================================================*/
#define HBOOST_BIMAP_GENERATE_MAP_VIEW_BINDER(                                 \
                                                                              \
    MAP_VIEW_TYPE                                                             \
                                                                              \
)                                                                             \
                                                                              \
template< class Tag, class BimapType >                                        \
struct map_view_bind                                                          \
{                                                                             \
    typedef MAP_VIEW_TYPE                                                     \
    <                                                                         \
        Tag,                                                                  \
        BimapType                                                             \
                                                                              \
    > type;                                                                   \
};
/*===========================================================================*/


/*===========================================================================*/
#define HBOOST_BIMAP_GENERATE_SET_VIEW_BINDER(                                 \
                                                                              \
    SET_VIEW_TYPE                                                             \
                                                                              \
)                                                                             \
                                                                              \
template< class IndexType >                                                   \
struct set_view_bind                                                          \
{                                                                             \
    typedef SET_VIEW_TYPE<IndexType> type;                                    \
};
/*===========================================================================*/


#endif // HBOOST_BIMAP_DETAIL_GENERATE_VIEW_BINDER_HPP