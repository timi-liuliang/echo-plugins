// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/concept_tags.hpp
/// \brief Bimap tags and concepts

#ifndef HBOOST_BIMAP_DETAIL_CONCEPT_TAGS_HPP
#define HBOOST_BIMAP_DETAIL_CONCEPT_TAGS_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/placeholders.hpp>
#include <hboost/mpl/bool.hpp>

namespace hboost {
namespace bimaps {
namespace detail {

/// \brief Tag of {SetType}_of definition classes
/**
The {SetType}_of classes are derived from this class so it is easy to construct
metafunctions. For example now is easy to create a is_set_type_of metafunction.
                                                                                **/

struct set_type_of_tag          {};

/// \brief Tag of {SetType}_of_relation defition classes

struct set_type_of_relation_tag {};

/// \brief Tag of {Side}_based identifiers

struct side_based_tag : set_type_of_relation_tag {};

} // namespace detail


/** \struct hboost::bimaps::left_based
    \brief Tag to indicate that the main view will be based on the left side.

This is convenient because the multi-index core will be more efficient.
If possible use this options or the right based one.

See also right_based.
                                                                            **/

/** \struct hboost::bimaps::right_based
    \brief Tag to indicate that the main view will be based on the right side.

This is convenient because the multi-index core will be more efficient.
If possible use this options or the right based one.

See also left_based.
                                                                            **/

#ifndef HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

struct left_based : ::hboost::bimaps::detail::side_based_tag
{
    // I run into troubles if I do not define bind for side based tags.
    // Maybe a more coherent way of binding the relation can be developped.
    template< class Relation > struct bind_to { typedef void type; };

    typedef mpl::bool_<true>  left_mutable_key;
    typedef mpl::bool_<true> right_mutable_key;
};

struct right_based : ::hboost::bimaps::detail::side_based_tag
{
    // I run into troubles if I do not define bind for side based tags.
    // Maybe a more coherent way of binding the relation can be developped.
    template< class Relation > struct bind_to { typedef void type; };

    typedef mpl::bool_<true>  left_mutable_key;
    typedef mpl::bool_<true> right_mutable_key;
};

#endif // HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

typedef mpl::_ _relation;

} // namespace bimaps
} // namespace hboost


#endif // HBOOST_BIMAP_DETAIL_CONCEPT_TAGS_HPP

