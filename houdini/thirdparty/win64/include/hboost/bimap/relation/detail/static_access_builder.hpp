// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


/// \file relation/detail/static_access_builder.hpp
/// \brief Define macros to help building metafunctions

#ifndef HBOOST_BIMAP_RELATION_DETAIL_STATIC_ACCESS_BUILDER_HPP
#define HBOOST_BIMAP_RELATION_DETAIL_STATIC_ACCESS_BUILDER_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/bimap/relation/support/is_tag_of_member_at.hpp>
#include <hboost/bimap/detail/debug/static_error.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/preprocessor/cat.hpp>



/******************************************************************************
              BIMAP SYMMETRIC STATIC ACCESS INTERFACE
*******************************************************************************

template< class Tag, class SYMETRIC_TYPE >
struct NAME
{
    -UNDEFINED BODY-;
};

******************************************************************************/


/*===========================================================================*/
#define HBOOST_BIMAP_SYMMETRIC_STATIC_ACCESS_BUILDER(                          \
                                                                              \
        NAME,                                                                 \
        SYMMETRIC_TYPE,                                                       \
        LEFT_BODY,                                                            \
        RIGHT_BODY                                                            \
    )                                                                         \
                                                                              \
    template                                                                  \
    <                                                                         \
        class Tag,                                                            \
        class SYMMETRIC_TYPE,                                                 \
        class Enable = void                                                   \
    >                                                                         \
    struct NAME                                                               \
    {                                                                         \
        HBOOST_BIMAP_STATIC_ERROR(                                             \
            HBOOST_PP_CAT(NAME,_FAILURE),                                      \
            (SYMMETRIC_TYPE,Tag)                                              \
        );                                                                    \
    };                                                                        \
                                                                              \
    template< class Tag, class SYMMETRIC_TYPE >                               \
    struct NAME                                                               \
    <                                                                         \
        Tag, SYMMETRIC_TYPE,                                                  \
        HBOOST_DEDUCED_TYPENAME enable_if                                      \
        <                                                                     \
            ::hboost::bimaps::relation::support::is_tag_of_member_at_left      \
            <                                                                 \
                Tag,                                                          \
                SYMMETRIC_TYPE                                                \
            >                                                                 \
                                                                              \
        >::type                                                               \
    >                                                                         \
    {                                                                         \
        LEFT_BODY;                                                            \
    };                                                                        \
                                                                              \
    template< class Tag, class SYMMETRIC_TYPE >                               \
    struct NAME                                                               \
    <                                                                         \
        Tag, SYMMETRIC_TYPE,                                                  \
        HBOOST_DEDUCED_TYPENAME enable_if                                      \
        <                                                                     \
            ::hboost::bimaps::relation::support::is_tag_of_member_at_right     \
            <                                                                 \
                Tag,                                                          \
                SYMMETRIC_TYPE                                                \
            >                                                                 \
                                                                              \
        >::type                                                               \
    >                                                                         \
    {                                                                         \
        RIGHT_BODY;                                                           \
    };
/*===========================================================================*/


#endif // HBOOST_BIMAP_RELATION_DETAIL_STATIC_ACCES_BUILDER_HPP


